/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * networkop.cpp
 * Network and SMB support routines
 ****************************************************************************/

#include <network.h>
#include <unistd.h>
#include <ogc/lwp_watchdog.h>
#include <smb.h>
#include <mxml.h>

#include "wiimc.h"
#include "fileop.h"
#include "filebrowser.h"
#include "menu.h"
#include "networkop.h"
#include "settings.h"
#include "utils/ftp_devoptab.h"
#include "utils/http.h"
#include "utils/gettext.h"
#include "libwiigui/gui.h"

void ShowAction (const char *msg, UpdateCallback c);

static int netHalt = 0;
static bool networkInit = false;
static bool networkShareInit[MAX_SHARES] = { false, false, false, false, false };
static bool ftpInit[MAX_SHARES] = { false, false, false, false, false };
char wiiIP[16] = { 0 };

/****************************************************************************
 * InitializeNetwork
 * Initializes the Wii/GameCube network interface
 ***************************************************************************/

static lwp_t networkthread = LWP_THREAD_NULL;
static u8 netstack[32768] ATTRIBUTE_ALIGN (32);

static void * netcb (void *arg)
{
	s32 res=-1;
	int retry;
	int wait;
	static bool prevInit = false;

	while(netHalt != 2)
	{
		retry = 5;
		
		while (retry > 0 && netHalt != 2)
		{
			net_deinit();
			
			if(prevInit)
			{
				prevInit=false; // only call net_wc24cleanup once
				net_wc24cleanup(); // kill wc24
				usleep(10000);
			}

			res = net_init_async(NULL, NULL);

			if(res != 0)
			{
				sleep(1);
				retry--;
				continue;
			}

			res = net_get_status();
			wait = 500; // only wait 10 sec

			while (res == -EBUSY && wait > 0 && netHalt != 2)
			{
				usleep(200000);
				res = net_get_status();
				wait--;
			}

			if (res == 0)
			{
				struct in_addr hostip;
				hostip.s_addr = net_gethostip();
				
				if (hostip.s_addr)
				{
					strcpy(wiiIP, inet_ntoa(hostip));
					networkInit = true;
					prevInit = true;
					break;
				}
			}

			retry--;
			usleep(2000);
		}

		if(netHalt != 2)
		{
			LWP_SuspendThread(networkthread);
			usleep(100);
		}
	}
	return NULL;
}

/****************************************************************************
 * StartNetworkThread
 *
 * Signals the network thread to resume, or creates a new thread
 ***************************************************************************/
void StartNetworkThread()
{
	netHalt = 0;

	if(networkthread == LWP_THREAD_NULL)
		LWP_CreateThread(&networkthread, netcb, NULL, netstack, 32768, 40);
	else
		LWP_ResumeThread(networkthread);
}

/****************************************************************************
 * StopNetworkThread
 *
 * Signals the network thread to stop
 ***************************************************************************/
static void StopNetworkThread()
{
	if(networkthread == LWP_THREAD_NULL || !LWP_ThreadIsSuspended(networkthread))
		return;

	netHalt = 2;

	LWP_ResumeThread(networkthread);

	// wait for thread to finish
	LWP_JoinThread(networkthread, NULL);
	networkthread = LWP_THREAD_NULL;
}

extern "C"{
void CheckMplayerNetwork() //to use in cache2.c in mplayer
{
	if(net_gethostip()==0)
	{
		networkInit = false;
		StartNetworkThread();	
	}
}
}

static bool cancelNetworkInit = false;

static void networkInitCallback(void *ptr)
{
	GuiButton *b = (GuiButton *)ptr;
	
	if(b->GetState() == STATE_CLICKED)
	{
		b->ResetState();
		cancelNetworkInit = true;
	}
}

bool InitializeNetwork(bool silent)
{
	if(networkInit)
		return true;

	ShowAction("Initializing network...", networkInitCallback);
	cancelNetworkInit = false;

	while(!networkInit)
	{
		StartNetworkThread();

		while (!LWP_ThreadIsSuspended(networkthread) && !cancelNetworkInit)
			usleep(50 * 1000);

		StopNetworkThread();

		if(silent || cancelNetworkInit)
			break;
	}

	CancelAction();

	return networkInit;
}

void CloseShare(int num)
{
	char devName[10];
	sprintf(devName, "smb%d", num);

	if(networkShareInit[num-1])
		smbClose(devName);
	networkShareInit[num-1] = false;
}

/****************************************************************************
 * Mount SMB Share
 ****************************************************************************/

bool
ConnectShare (int num, bool silent)
{
	if(!InitializeNetwork(silent))
		return false;

	char mountpoint[10];
	sprintf(mountpoint, "smb%d", num);
	int retry = 1;
	int chkS = (strlen(WiiSettings.smbConf[num-1].share) > 0) ? 0:1;
	int chkI = (strlen(WiiSettings.smbConf[num-1].ip) > 0) ? 0:1;

	if(networkShareInit[num-1])
		return true;

	// check that all parameters have been set
	if(chkS + chkI > 0)
	{
		if(!silent)
		{
			char msg[50];
			wchar_t msg2[100];
			if(chkS + chkI > 1) // more than one thing is wrong
				sprintf(msg, "Check settings file.");
			else if(chkS)
				sprintf(msg, "Share name is blank.");
			else if(chkI)
				sprintf(msg, "Share IP is blank.");

			swprintf(msg2, 100, L"%s - %s", gettext("Invalid network share settings"), gettext(msg));
			ErrorPrompt(msg2);
		}
		return false;
	}

	while(retry)
	{
		if(!silent)
			ShowAction ("Connecting to network share...");
		
		if(smbInitDevice(mountpoint, WiiSettings.smbConf[num-1].user, WiiSettings.smbConf[num-1].pwd,
					WiiSettings.smbConf[num-1].share, WiiSettings.smbConf[num-1].ip))
			networkShareInit[num-1] = true;

		if(networkShareInit[num-1] || silent)
			break;

		retry = ErrorPromptRetry("Failed to connect to network share.");
		if(retry) InitializeNetwork(silent);
	}

	if(!silent)
		CancelAction();

	return networkShareInit[num-1];
}

void ReconnectShare(int num, bool silent)
{
	CloseShare(num);
	ConnectShare(num, silent);
}

void CloseFTP(int num)
{
	char devName[10];
	sprintf(devName, "ftp%d", num);

	if(ftpInit[num-1])
		ftpClose(devName);
	ftpInit[num-1] = false;
}

/****************************************************************************
 * Mount FTP site
 ****************************************************************************/

bool
ConnectFTP (int num, bool silent)
{
	if(!InitializeNetwork(silent))
		return false;

	char mountpoint[10];
	sprintf(mountpoint, "ftp%d", num);

	int chkI = (strlen(WiiSettings.ftpConf[num-1].ip) > 0) ? 0:1;
	int chkU = (strlen(WiiSettings.ftpConf[num-1].user) > 0) ? 0:1;
	int chkP = (strlen(WiiSettings.ftpConf[num-1].pwd) > 0) ? 0:1;

	// check that all parameters have been set
	if(chkI + chkU + chkP > 0)
	{
		if(!silent)
		{
			char msg[50];
			wchar_t msg2[100];
			if(chkI + chkU + chkP > 1) // more than one thing is wrong
				sprintf(msg, "Check settings file.");
			else if(chkI)
				sprintf(msg, "IP is blank.");
			else if(chkU)
				sprintf(msg, "Username is blank.");
			else if(chkP)
				sprintf(msg, "Password is blank.");

			swprintf(msg2, 100, L"%s - %s", gettext("Invalid FTP site settings"), gettext(msg));
			ErrorPrompt(msg2);
		}
		return false;
	}

	if(!ftpInit[num-1])
	{
		if(!silent)
			ShowAction ("Connecting to FTP site...");

		if(ftpInitDevice(mountpoint, WiiSettings.ftpConf[num-1].user, 
			WiiSettings.ftpConf[num-1].pwd, WiiSettings.ftpConf[num-1].folder, 
			WiiSettings.ftpConf[num-1].ip, WiiSettings.ftpConf[num-1].port, WiiSettings.ftpConf[num-1].passive))
		{
			ftpInit[num-1] = true;
		}
		if(!silent)
			CancelAction();
	}

	if(!ftpInit[num-1] && !silent)
		ErrorPrompt("Failed to connect to FTP site.");

	return ftpInit[num-1];
}
