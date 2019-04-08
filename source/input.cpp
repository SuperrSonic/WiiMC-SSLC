/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * input.cpp
 * Wii/GameCube controller management
 ***************************************************************************/

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ogcsys.h>
#include <unistd.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>

#include "wiimc.h"
#include "menu.h"
#include "video.h"
#include "input.h"
#include "settings.h"
#include "libwiigui/gui.h"
#include "mplayer/input/input.h"
#include "mplayer/osdep/gx_supp.h"

#define RUMBLE_MAX				60000
#define RUMBLE_COOLOFF			10000000
#define VOL_DELAY				30000
#define VOLDISP_MAX				500000

#define RESIZE_INITIAL_DELAY	500000 // to allow more precise adjustment
#define RESIZE_DELAY			5000

static bool rumbleDisabled = false;
static int rumbleOn[4] = {0,0,0,0};
static u64 prev[4];
static u64 now[4];
static int osdLevel = 0;
static int volprev = 0, volnow = 0;
static int resizeprev = 0, resizeinitial = 0;

GuiTrigger userInput[4];

/****************************************************************************
 * UpdatePads
 *
 * Scans wpad
 ***************************************************************************/
void UpdatePads()
{
	WPAD_ReadPending(0, NULL); // only wiimote 1
	PAD_ScanPads();

	userInput[0].pad.btns_d = PAD_ButtonsDown(0);
	userInput[0].pad.btns_h = PAD_ButtonsHeld(0);
	userInput[0].pad.stickX = PAD_StickX(0);
	userInput[0].pad.stickY = PAD_StickY(0);
}

/****************************************************************************
 * SetupPads
 *
 * Sets up userInput triggers for use
 ***************************************************************************/
void SetupPads()
{
	PAD_Init();

	WPAD_SetIdleTimeout(60);

	// read wiimote accelerometer and IR data
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetVRes(WPAD_CHAN_0, screenwidth, screenheight);

	userInput[0].chan = 0;
	userInput[0].wpad = WPAD_Data(0);
}

/****************************************************************************
 * ShutoffRumble
 ***************************************************************************/

static void ShutoffRumble(int i, int cooloff)
{
	if(CONF_GetPadMotorMode() == 0)
		return;

	prev[i] = gettime() + cooloff;
	WPAD_Rumble(i, 0); // rumble off
	rumbleOn[i] = 0;
}

void ShutoffRumble()
{
	ShutoffRumble(0, RUMBLE_COOLOFF*3);
}

void DisableRumble()
{
	rumbleDisabled = true;
	ShutoffRumble();
}

void EnableRumble()
{
	rumbleDisabled = false;
}

void RequestRumble(int i)
{
	if(CONF_GetPadMotorMode() == 0 || !WiiSettings.rumble || rumbleDisabled || i < 0)
		return;

	now[i] = gettime();

	if(prev[i] > now[i])
		return;

	if(diff_usec(prev[i], now[i]) > RUMBLE_MAX)
	{
		rumbleOn[i] = 1;
		WPAD_Rumble(i, 1); // rumble on
		prev[i] = now[i];
	}
}

/****************************************************************************
 * DoRumble
 ***************************************************************************/

void DoRumble(int i)
{
	if(rumbleOn[i])
	{
		now[i] = gettime();

		if(diff_usec(prev[i], now[i]) > RUMBLE_MAX)
			ShutoffRumble(i, RUMBLE_COOLOFF);
	}
}

/****************************************************************************
 * MPlayerInput
 ***************************************************************************/

void MPlayerResize(float fZoomHorIncr, float fZoomVertIncr)
{
	WiiSettings.videoZoomHor += fZoomHorIncr;
	WiiSettings.videoZoomVert += fZoomVertIncr;
	GX_SetScreenPos(WiiSettings.videoXshift, WiiSettings.videoYshift, 
		CONF_GetAspectRatio() == CONF_ASPECT_4_3 ? WiiSettings.videoFull ? WiiSettings.videoZoomHor * 1.35
			: WiiSettings.videoZoomHor : WiiSettings.videoZoomHor, WiiSettings.videoZoomVert);
}

void MPlayerInput()
{
	bool ir = false;
	bool inDVDMenu = wiiInDVDMenu();
	static bool volumeUpdated = false;

	if(userInput[0].wpad->ir.valid)
		ir = true;

	u16 down = PAD_ButtonsDown(0);
	u16 held = PAD_ButtonsHeld(0);

	if(userInput[0].wpad->btns_d & WPAD_BUTTON_1 || down & PAD_BUTTON_B) {
		osdLevel ^= 1;
	} else if(AutobootExit && (userInput[0].wpad->btns_d & WPAD_BUTTON_HOME || down & PAD_BUTTON_START)) {
		VIDEO_SetBlack (TRUE);
		ExitRequested = true;
	} else if(ExitRequested || userInput[0].wpad->btns_d & WPAD_BUTTON_HOME || down & PAD_BUTTON_START) {
		if(wiiIsPaused())
			wiiPause();
		wiiGotoGui();
	}

	if(!inDVDMenu)
	{
		if(userInput[0].wpad->btns_d & WPAD_BUTTON_A || down & PAD_BUTTON_A)
		{
			// Hack to allow people to unpause while the OSD GUI is visible by
			// pointing above the button bar and pressing A. We also need to be outside
			// the boundaries of the volume bar area, when it is visible
			int x = userInput[0].wpad->ir.x;
			int y = userInput[0].wpad->ir.y;

			int xoffset = 20;

			if(screenwidth == 768)
				xoffset = 80;

			if(!drawGui || (y < 360 && 
				(!VideoVolumeLevelBarVisible() || !(x > xoffset && x < xoffset+100 && y > 180))))
			{
				wiiPause();
			}
		}
		else if(userInput[0].wpad->btns_h & WPAD_BUTTON_PLUS || held & PAD_BUTTON_Y)
		{
			volnow = gettime();
	
			if(diff_usec(volprev, volnow) > VOL_DELAY)
			{
				volprev = volnow;
				WiiSettings.volume++;
				if(WiiSettings.volume > 100) WiiSettings.volume = 100;
				wiiSetVolume(WiiSettings.volume);
				volumeUpdated = true;
				ShowVideoVolumeLevelBar();
			}
		}
		else if(userInput[0].wpad->btns_h & WPAD_BUTTON_MINUS || held & PAD_BUTTON_X)
		{
			volnow = gettime();
	
			if(diff_usec(volprev, volnow) > VOL_DELAY)
			{
				volprev = volnow;
				WiiSettings.volume--;
				if(WiiSettings.volume < 0) WiiSettings.volume = 0;
				wiiSetVolume(WiiSettings.volume);
				volumeUpdated = true;
				ShowVideoVolumeLevelBar();
			}
		}
		else if (userInput[0].wpad->btns_h & WPAD_BUTTON_B || held & PAD_TRIGGER_Z)
		{
			unsigned int delay = (resizeinitial == 1) ? RESIZE_INITIAL_DELAY : RESIZE_DELAY;
			int resizenow = gettime();

			if(userInput[0].wpad->btns_h & WPAD_BUTTON_RIGHT || held & PAD_BUTTON_RIGHT)
			{
				if(diff_usec(resizeprev, resizenow) > delay)
				{
					resizeinitial++;
					resizeprev = resizenow;
					MPlayerResize(+0.003F, 0.00F);
				}
			}
			else if(userInput[0].wpad->btns_h & WPAD_BUTTON_LEFT || held & PAD_BUTTON_LEFT)
			{
				if(diff_usec(resizeprev, resizenow) > delay)
				{
					resizeinitial++;
					resizeprev = resizenow;
					MPlayerResize(-0.003F, 0.00F);
				}
			}
			else if(userInput[0].wpad->btns_h & WPAD_BUTTON_UP || held & PAD_BUTTON_UP)
			{
				if(diff_usec(resizeprev, resizenow) > delay)
				{
					resizeinitial++;
					resizeprev = resizenow;
					MPlayerResize(0.00F, +0.003F);
				}
			}
			else if(userInput[0].wpad->btns_h & WPAD_BUTTON_DOWN || held & PAD_BUTTON_DOWN)
			{
				if(diff_usec(resizeprev, resizenow) > delay)
				{
					resizeinitial++;
					resizeprev = resizenow;
					MPlayerResize(0.00F, -0.003F);
				}
			}

			if(userInput[0].wpad->btns_d & (WPAD_BUTTON_RIGHT | WPAD_BUTTON_LEFT |
											WPAD_BUTTON_UP | WPAD_BUTTON_DOWN))
			{
				resizeinitial = 0;
			}
		}
		else if(userInput[0].wpad->btns_d & WPAD_BUTTON_RIGHT || down & PAD_BUTTON_RIGHT)
		{
			wiiFastForward();
		}
		else if(userInput[0].wpad->btns_d & WPAD_BUTTON_LEFT || down & PAD_BUTTON_LEFT)
		{
			wiiRewind();
		}
		else if(userInput[0].wpad->btns_d & WPAD_BUTTON_UP || down & PAD_BUTTON_UP)
		{
			if(!wiiIsPaused())
				wiiSetProperty(MP_CMD_SUB_SELECT, 0);
		}
		else if(userInput[0].wpad->btns_d & WPAD_BUTTON_DOWN || down & PAD_BUTTON_DOWN)
		{
			if(!wiiIsPaused()) {
				wiiSetProperty(MP_CMD_SWITCH_AUDIO, 0);
				/* When changing audio tracks there is a small de-sync */
				/* So we seek to re-sync */
				wiiSync();
			}
		}
		else if(down & PAD_TRIGGER_R)
		{
			if(!wiiIsPaused()) {
				wiiSetProperty(MP_CMD_SWITCH_ANGLE, 0);
			}
		}
		else if(userInput[0].wpad->btns_d & WPAD_BUTTON_2 || down & PAD_TRIGGER_L)
		{
			wiiDVDNav(MP_CMD_DVDNAV_MENU);
		}
	}
	else
	{
		if(userInput[0].wpad->ir.valid)
			wiiUpdatePointer((int)userInput[0].wpad->ir.x, (int)userInput[0].wpad->ir.y);

		if(userInput[0].wpad->btns_d & WPAD_BUTTON_A || down & PAD_BUTTON_A)
		{
			if(userInput[0].wpad->ir.valid)
				wiiDVDNav(MP_CMD_DVDNAV_MOUSECLICK);
			else
				wiiDVDNav(MP_CMD_DVDNAV_SELECT);
		}
		else if (userInput[0].wpad->btns_h & WPAD_BUTTON_B || held & PAD_TRIGGER_Z)
		{
			unsigned int delay = (resizeinitial == 1) ? RESIZE_INITIAL_DELAY : RESIZE_DELAY;
			int resizenow = gettime();

			if(userInput[0].wpad->btns_h & WPAD_BUTTON_RIGHT || held & PAD_BUTTON_RIGHT)
			{
				if(diff_usec(resizeprev, resizenow) > delay)
				{
					resizeinitial++;
					resizeprev = resizenow;
					MPlayerResize(+0.003F, 0.00F);
				}
			}
			else if(userInput[0].wpad->btns_h & WPAD_BUTTON_LEFT || held & PAD_BUTTON_LEFT)
			{
				if(diff_usec(resizeprev, resizenow) > delay)
				{
					resizeinitial++;
					resizeprev = resizenow;
					MPlayerResize(-0.003F, 0.00F);
				}
			}
			else if(userInput[0].wpad->btns_h & WPAD_BUTTON_UP || held & PAD_BUTTON_UP)
			{
				if(diff_usec(resizeprev, resizenow) > delay)
				{
					resizeinitial++;
					resizeprev = resizenow;
					MPlayerResize(0.00F, +0.003F);
				}
			}
			else if(userInput[0].wpad->btns_h & WPAD_BUTTON_DOWN || held & PAD_BUTTON_DOWN)
			{
				if(diff_usec(resizeprev, resizenow) > delay)
				{
					resizeinitial++;
					resizeprev = resizenow;
					MPlayerResize(0.00F, -0.003F);
				}
			}

			if(userInput[0].wpad->btns_d & (WPAD_BUTTON_RIGHT | WPAD_BUTTON_LEFT |
			                                WPAD_BUTTON_UP | WPAD_BUTTON_DOWN))
			{
				resizeinitial = 0;
			}
		}
		else if(userInput[0].wpad->btns_d & WPAD_BUTTON_UP || down & PAD_BUTTON_UP)
		{
			wiiDVDNav(MP_CMD_DVDNAV_UP);
		}
		else if(userInput[0].wpad->btns_d & WPAD_BUTTON_DOWN || down & PAD_BUTTON_DOWN)
		{
			wiiDVDNav(MP_CMD_DVDNAV_DOWN);
		}
		else if(userInput[0].wpad->btns_d & WPAD_BUTTON_RIGHT || down & PAD_BUTTON_RIGHT)
		{
			wiiDVDNav(MP_CMD_DVDNAV_RIGHT);
		}
		else if(userInput[0].wpad->btns_d & WPAD_BUTTON_LEFT || down & PAD_BUTTON_LEFT)
		{
			wiiDVDNav(MP_CMD_DVDNAV_LEFT);
		}
	}

	if(volumeUpdated)
	{
		volnow = gettime();

		if(volnow > volprev && diff_usec(volprev, volnow) > VOLDISP_MAX)
			volumeUpdated = false;
		else
			ir = true; // trigger display
	}

	if(ir || BufferingStatusSet() || osdLevel)
	{
		drawGui = true;
	}
	else if(drawGui)
	{
		drawGui = false;
		HideVideoVolumeLevelBar();
		ShutoffRumble();
	}
}
