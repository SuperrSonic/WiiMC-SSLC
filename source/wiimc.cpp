/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * wiimc.cpp
 ***************************************************************************/

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <unistd.h>
#include <dirent.h>
#include <wiiuse/wpad.h>
#include <sys/iosupport.h>
#include <di/di.h>
#include <fat.h>
#include <sdcard/wiisd_io.h>
#include <time.h>

#include "utils/ehcmodule_elf.h"
#include "utils/mload.h"
#include "utils/usb2storage.h"

#include "utils/FreeTypeGX.h"
#include "utils/gettext.h"
#include "utils/mem2_manager.h"
#include "video.h"
#include "networkop.h"
#include "menu.h"
#include "libwiigui/gui.h"
#include "input.h"
#include "filelist.h"
#include "fileop.h"
#include "filebrowser.h"
#include "musicplaylist.h"
#include "wiimc.h"
#include "settings.h"

extern char MPLAYER_DATADIR[512]; 
extern char MPLAYER_CONFDIR[512]; 
extern char MPLAYER_LIBDIR[512]; 
extern char MPLAYER_CSSDIR[512];

#include "mplayer/input/input.h"
#include "mplayer/osdep/gx_supp.h"

bool want3DS = false;
bool isReload = false;
u8 iosVal = 58;

static bool load_ehci_module()
{
	data_elf my_data_elf;
	
	if (mload_elf(ehcmodule_elf, &my_data_elf) < 0)
		return false;
	
	if (mload_run_thread(my_data_elf.start, my_data_elf.stack, my_data_elf.size_stack, my_data_elf.prio) < 0) {
	//	usleep(1000);
		usleep(100);
		if (mload_run_thread(my_data_elf.start, my_data_elf.stack, my_data_elf.size_stack, 0x47) < 0)
			return false;
	}
	
	return true;
}
/*
bool load_ehci_module()
{
	data_elf elf;
	memset(&elf, 0, sizeof(data_elf));

	if(mload_elf((void *) ehcmodule_elf, &elf) != 0)
		return false;

	if(mload_run_thread(elf.start, elf.stack, elf.size_stack, elf.prio) < 0)
		return false;
	
	usleep(5000);
	return true;
}
*/

extern "C" {
extern void __exception_setreload(int t);
extern char *network_useragent;
}

bool ExitRequested = false;
bool ShutdownRequested = false;
int subtitleFontFound = 0;
char appPath[1024] = { 0 };
char loadedFile[1024] = { 0 };
char loadedDevice[16] = { 0 };
char loadedFileDisplay[128] = { 0 };
static bool settingsSet = false;
bool AutobootExit = false;
bool isDynamic = false;
int sync_interlace = 0;
int use_lavf = 0;

// MPlayer threads
#define MPLAYER_STACKSIZE (512*1024)
#define CACHE_STACKSIZE (16*1024)
static lwp_t mthread = LWP_THREAD_NULL;
static lwp_t cthread = LWP_THREAD_NULL;
static u8 cachestack[CACHE_STACKSIZE] ATTRIBUTE_ALIGN (32);

#define EXIT_STACKSIZE (8*1024)
static lwp_t ethread = LWP_THREAD_NULL;
static u8 exitstack[EXIT_STACKSIZE] ATTRIBUTE_ALIGN (32);

static void SaveLogToSD();

/****************************************************************************
 * Shutdown / Reboot / Exit
 ***************************************************************************/

static u64 sleepStart = 0;

void SetSleepTimer()
{
	if(WiiSettings.sleepTimer == 0)
		sleepStart = 0;
	else
		sleepStart = gettime();
}

void ResetSleepTimer()
{
	sleepStart = 0;
}

void CheckSleepTimer()
{
	if(WiiSettings.sleepTimer == 0 || sleepStart == 0)
		return;

	if(diff_sec(sleepStart, gettime()) > (u32)(WiiSettings.sleepTimer*60))
	{
		ExitRequested = true;
		ShutdownRequested = true;
		controlledbygui = 2;
	}
}

void *exitthread(void *arg)
{
	sleep(6);
	StopGX();
	AUDIO_StopDMA();
	AUDIO_RegisterDMACallback(NULL);

	SaveLogToSD();

	if(ShutdownRequested || WiiSettings.exitAction == EXIT_POWEROFF)
		SYS_ResetSystem(SYS_POWEROFF, 0, 0);
	else if(WiiSettings.exitAction == EXIT_WIIMENU)
		SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);

	exit(0);
	return NULL;
}

void ActivateExitThread()
{
	if(ethread == LWP_THREAD_NULL)
		LWP_CreateThread (&ethread, exitthread, NULL, exitstack, EXIT_STACKSIZE, 40);
}

static void ShutdownCB()
{
	ExitRequested = true;
	//ShutdownRequested = true; // Power button is more responsive when a crash occurs
	ActivateExitThread();
}

static void ResetCB()
{
	ExitRequested = true;
	ActivateExitThread();
}

/****************************************************************************
 * USB Gecko Debugging
 ***************************************************************************/

static bool gecko = false;
static mutex_t gecko_mutex = 0;

#define GECKO_BUFFER_SIZE 4096

static char *gecko_buf[2] = { NULL, NULL };
static char *gecko_buf_ptr = NULL;
static int gecko_buf_size = 0;

static void SaveLogToSD()
{
	char _file[128];
	FILE *fp;
	const DISC_INTERFACE* sd = &__io_wiisd;

	if(WiiSettings.debug != 1) return;

	sd->startup();
	
	if(!fatMount("sdlog",sd,0,2,64)) return;

	char s[50];
	struct tm tim;
	time_t now;
	now = time(NULL);
	tim = *(localtime(&now));
	strftime(s,49,"%Y%m%d_%k%M%S",&tim);

	sprintf(_file,"sdlog:/wiimc_log_%s.txt",s);
	fp=fopen(_file,"wb");
	
	if(fp)
	{
		fprintf(fp,"buffer1:\n");
			
		if(gecko_buf_ptr == gecko_buf[0])
			fwrite(gecko_buf[1], 1, GECKO_BUFFER_SIZE, fp);
		else
			fwrite(gecko_buf[0], 1, GECKO_BUFFER_SIZE, fp);

		fprintf(fp,"\n\n========================================================\nbuffer2:\n");
		fwrite(gecko_buf_ptr, 1, GECKO_BUFFER_SIZE, fp);
		fclose(fp);
	}
	fatUnmount("sdlog");
}

static ssize_t __out_write(struct _reent *r, int fd, const char *ptr, size_t len)
{
	if (!gecko || len == 0)
		return len;
	
	if(!ptr || len < 0)
		return -1;

	u32 level;
	LWP_MutexLock(gecko_mutex);
	level = IRQ_Disable();
	usb_sendbuffer(1, ptr, len);
	IRQ_Restore(level);

	if(WiiSettings.debug == 1)
	{
		if(len >= GECKO_BUFFER_SIZE) len = GECKO_BUFFER_SIZE - 1;
		
		if(gecko_buf_size + len >= GECKO_BUFFER_SIZE)
		{
			if(gecko_buf_ptr == gecko_buf[0]) gecko_buf_ptr = gecko_buf[1];
			else gecko_buf_ptr = gecko_buf[0];
			gecko_buf_size = 0;
			memset(gecko_buf_ptr, 0, GECKO_BUFFER_SIZE);
		}
		memcpy(gecko_buf+gecko_buf_size, ptr, len);
		gecko_buf_size+=len;
	}
	
	LWP_MutexUnlock(gecko_mutex);
	return len;
}

const devoptab_t gecko_out = {
	"stdout",	// device name
	0,			// size of file structure
	NULL,		// device open
	NULL,		// device close
	__out_write,// device write
	NULL,		// device read
	NULL,		// device seek
	NULL,		// device fstat
	NULL,		// device stat
	NULL,		// device link
	NULL,		// device unlink
	NULL,		// device chdir
	NULL,		// device rename
	NULL,		// device mkdir
	0,			// dirStateSize
	NULL,		// device diropen_r
	NULL,		// device dirreset_r
	NULL,		// device dirnext_r
	NULL,		// device dirclose_r
	NULL		// device statvfs_r
};

static void USBGeckoOutput()
{
	gecko = usb_isgeckoalive(1); // uncomment to enable USB Gecko output

	LWP_MutexInit(&gecko_mutex, false);

	gecko_buf[0] = (char*) calloc(GECKO_BUFFER_SIZE, 1);
	gecko_buf[1] = (char*) calloc(GECKO_BUFFER_SIZE, 1);
	gecko_buf_ptr = gecko_buf[0];

	devoptab_list[STD_OUT] = &gecko_out;
	devoptab_list[STD_ERR] = &gecko_out;
}

/****************************************************************************
 * IOS Check
 ***************************************************************************/
bool SupportedIOS(u32 ios)
{
	if(ios == 58 || ios == 61 || ios == 202 || ios == 249)
		return true;

	return false;
}

bool SaneIOS(u32 ios)
{
	bool res = false;
	u32 num_titles=0;
	u32 tmd_size;
	
	if(ios > 200)
		return false;

	if (ES_GetNumTitles(&num_titles) < 0)
		return false;

	if(num_titles < 1) 
		return false;

	u64 *titles = (u64 *)memalign(32, num_titles * sizeof(u64) + 32);
	
	if(!titles)
		return false;

	if (ES_GetTitles(titles, num_titles) < 0)
	{
		free(titles);
		return false;
	}
	
	u32 *tmdbuffer = (u32 *)memalign(32, MAX_SIGNED_TMD_SIZE);

	if(!tmdbuffer)
	{
		free(titles);
		return false;
	}

	for(u32 n=0; n < num_titles; n++)
	{
		if((titles[n] & 0xFFFFFFFF) != ios) 
			continue;

		if (ES_GetStoredTMDSize(titles[n], &tmd_size) < 0)
			break;

		if (tmd_size > 4096)
			break;

		if(ES_GetStoredTMD(titles[n], (signed_blob *)tmdbuffer, tmd_size) < 0)
			break;

		if (tmdbuffer[1] || tmdbuffer[2])
		{
			res = true;
			break;
		}
	}
	free(tmdbuffer);
    free(titles);
	return res;
}

/* Pick a random video on the current list */
static int *shuffleList_vid = NULL;
static int shuffleIndex_vid = -1;
//extern int find_prob;

BROWSERENTRY *VideoPlaylistGetNextShuffle()
{
	if(shuffleIndex_vid == -1 || shuffleIndex_vid >= browserVideos.numEntries)
	{
		// populate new list
		int i, n, t;
		int num = browserVideos.numEntries;

		mem2_free(shuffleList_vid, MEM2_BROWSER);
		shuffleList_vid = (int *)mem2_malloc(num*sizeof(int), MEM2_BROWSER);

		for(i=0; i < num; i++)
			shuffleList_vid[i] = i;

		// shuffle the list
		for(i = 0; i < num-1; i++)
		{
			n = rand() / (RAND_MAX/(num-i) + 1);

			// swap
			t = shuffleList_vid[i];
			shuffleList_vid[i] = shuffleList_vid[i+n];
			shuffleList_vid[i+n] = t;
		}
		shuffleIndex_vid = 0;
	}

	BROWSERENTRY *s = browserVideos.first;
	for(int i=0; i < shuffleList_vid[shuffleIndex_vid]; i++)
		s = s->next;
	shuffleIndex_vid++;

	return s;
}
/****************************************************************************
 * MPlayer interface
 ***************************************************************************/
extern "C" bool FindNextFile(bool load)
{
	nowPlayingSet = false;

	if(menuMode == 0 && menuCurrent == MENU_BROWSE_MUSIC)
	{
		// go and highlight the file
		selectLoadedFile = true;
		// clear any play icons
		BROWSERENTRY *i = browser.first;
		while(i)
		{
			if(i->icon == ICON_PLAY)
			{
				if(MusicPlaylistFind(i))
					i->icon = ICON_FILE_CHECKED;
				else
					i->icon = ICON_FILE;
			}
			i = i->next;
		}
		findLoadedFile = 2; // trigger file browser update
	}

	if(controlledbygui == 2) // file termination requested, do not load another file
		return false;

	if(controlledbygui == 0) // playing a video
	{
		if(!load)
			return false;

		if(!WiiSettings.autoPlayNextVideo || (!browserVideos.selIndex && WiiSettings.autoPlayNextVideo <= AUTOPLAY_ON))
		{
			loadedFile[0] = 0;
			loadedFileDisplay[0] = 0;
			ResetVideos();
			// Exit after loading autobooted file
			if(AutobootExit) {
				VIDEO_SetBlack (TRUE);
				ExitRequested = true;
			}
			// IF THERE IS ONLY ONE VIDEO IT WILL END UP GOING HERE AND SHUFFLE WILL FAIL!
			// Shuffle, Loop, and Continuous are included, but only Shuffle needs this.
			return false;
		}
		else
		{
			// Continuous: If the last video is over, start from the first video
			if(WiiSettings.autoPlayNextVideo == AUTOPLAY_CONTINUOUS && browserVideos.selIndex == NULL)
				browserVideos.selIndex = browserVideos.first;

			if(WiiSettings.autoPlayNextVideo != AUTOPLAY_SHUFFLE)
				strcpy(loadedFile, browserVideos.selIndex->file);
			browserVideos.selIndex = WiiSettings.autoPlayNextVideo == AUTOPLAY_SHUFFLE ?
						VideoPlaylistGetNextShuffle() : browserVideos.selIndex->next;

			// Lower this to get the second video shuffled, instead of the third
			if(WiiSettings.autoPlayNextVideo == AUTOPLAY_SHUFFLE)
				strcpy(loadedFile, browserVideos.selIndex->file);

			char *start = strrchr(loadedFile,'/');
			char ext[7];
			GetExt(loadedFile, ext);

			if(WiiSettings.skipLoop == 1 || strcasecmp(ext, "dash") == 0)
				wiiDash();
			else
				wiiElse();

			// THP doesn't support seek, yet the demuxer enables it
			if(strcasecmp(ext, "thp") == 0)
				wiiTHP();
			
			if(strcasecmp(ext, "sfd") == 0) {
				use_lavf = 1;
				wiiSFD();
			} else if(use_lavf == 1) {
				use_lavf = 0;
				wiiSFD();
			}
			
			//if(WiiSettings.nativeLoops > 0)
				wiiSpecialLoops(WiiSettings.nativeLoops);

			// use part after last / for display name, if it's not already the end of the string
			if(start != NULL && start[1] != 0)
			{
				start++;
				snprintf(loadedFileDisplay, 127, "%s", start);
			}
			else
			{
				snprintf(loadedFileDisplay, 127, "%s", loadedFile);
			}
		}
	}
	else
	{
		if(browserMusic.numEntries == 0 || (WiiSettings.playOrder == PLAY_SINGLE && browserMusic.selIndex != NULL))
		{
			browserMusic.selIndex = NULL;
			return false;
		}
		if(WiiSettings.playOrder == PLAY_THROUGH)
		{
			if(browserMusic.selIndex == NULL)
				return false;

			browserMusic.selIndex = browserMusic.selIndex->next;

			if(browserMusic.selIndex == NULL)
				return false;
		}
		else if(browserMusic.selIndex == NULL)
		{
			browserMusic.selIndex = browserMusic.first;
		}
		else if(WiiSettings.playOrder == PLAY_CONTINUOUS)
		{
			browserMusic.selIndex = browserMusic.selIndex->next;

			if(browserMusic.selIndex == NULL)
				browserMusic.selIndex = browserMusic.first;
		}
		else if(WiiSettings.playOrder == PLAY_SHUFFLE)
		{
			browserMusic.selIndex = MusicPlaylistGetNextShuffle();
		}
		sprintf(loadedFile, "%s", browserMusic.selIndex->file);
	}

	if(load)
	{
		// wait for directory parsing to finish (to find subtitles)
		// Needs testing
		while(WiiSettings.subtitleVisibility && !ParseDone())
			usleep(100);
		
	    char *partitionlabel = GetPartitionLabel(loadedFile);
		
		wiiLoadFile(loadedFile, partitionlabel);
	}

	if(controlledbygui == 1)
		FindFile(); // try to find this file

	return true;
}

static void *
mplayerthread (void *arg)
{
	mplayer_main();
	return NULL;
}

extern "C" {
extern void *mplayercachethread(void *arg);

void SuspendCacheThread()
{
	LWP_SuspendThread(cthread);
}
void ResumeCacheThread()
{
	LWP_ResumeThread(cthread);
}
bool CacheThreadSuspended()
{
	if(LWP_ThreadIsSuspended(cthread))
		return true;
	return false;
}
}
/*
void show_mem()
{
	printf("m1(%.4f) m2(%.4f)\n",
								((float)((char*)SYS_GetArena1Hi()-(char*)SYS_GetArena1Lo()))/0x100000,
								 ((float)((char*)SYS_GetArena2Hi()-(char*)SYS_GetArena2Lo()))/0x100000);
}*/

u8 *font_mem = NULL;
u8 *mono_mem = NULL;
unsigned font_mem_size = 0;
unsigned mono_mem_size = 0;
int have_mono = 0;

bool InitMPlayer()
{
	u8 *mplayerstack;

	static bool init = false;
	if(init) return true;

	if(appPath[0] == 0)
	{
		InfoPrompt("Unable to Initialize MPlayer", "Unable to find a valid working path");
		return false;
	}

	if(chdir(appPath) != 0)
	{
		wchar_t msg[512];
		swprintf(msg, 512, L"%s %s", gettext("Unable to change path to"), appPath);
		InfoPrompt("Unable to Initialize MPlayer", msg);
		return false;
	}

	// check if subtitle font file exists
	struct stat st;
	char filepath[1024];
	char monopath[1024];
	sprintf(filepath, "%s/subfont.ttf", appPath);
	sprintf(monopath, "%s/monospace.ttf", appPath);

	if(stat(filepath, &st) == 0)
		subtitleFontFound = 1;
	if(stat(monopath, &st) == 0)
		have_mono = 1;
	
	// Memory font
	if(subtitleFontFound) {
		FILE *fp;
		fp = fopen(filepath, "rb");
		
		fseeko(fp,0,SEEK_END);
		font_mem_size = ftello(fp);
		font_mem = (u8 *)mem2_memalign(32, font_mem_size, MEM2_OTHER);
		
		fseeko(fp,0,SEEK_SET);
		fread (font_mem, 1, font_mem_size, fp);
		fclose(fp);
	}
	if(have_mono) {
		FILE *fp;
		fp = fopen(monopath, "rb");
		
		fseeko(fp,0,SEEK_END);
		mono_mem_size = ftello(fp);
		mono_mem = (u8 *)mem2_memalign(32, mono_mem_size, MEM2_OTHER);
		
		fseeko(fp,0,SEEK_SET);
		fread (mono_mem, 1, mono_mem_size, fp);
		fclose(fp);
	}
	
	//TEST, MEM WASTE MEM1
/*	u8 *binar_mem = NULL;
	u32 binar_mem_size = 0;
	FILE *fp;
		fp = fopen("sd1:/apps/WiiMC/dat.bin", "rb");
		
		fseeko(fp,0,SEEK_END);
		binar_mem_size = ftello(fp);
		binar_mem = (u8 *)memalign(32, binar_mem_size);
		
		fseeko(fp,0,SEEK_SET);
		fread (binar_mem, 1, binar_mem_size, fp);
		fclose(fp);
	//END */

	sprintf(MPLAYER_DATADIR,"%s",appPath);
	sprintf(MPLAYER_CONFDIR,"%s",appPath);
	sprintf(MPLAYER_LIBDIR,"%s",appPath);
	sprintf(MPLAYER_CSSDIR,"%s/css",appPath);
	DIR *dir = opendir(MPLAYER_CSSDIR);

	if(!dir && mkdir(MPLAYER_CSSDIR, 0777) != 0)
		sprintf(MPLAYER_CSSDIR, "off");
	else
		closedir(dir);

	setenv("HOME", MPLAYER_DATADIR, 1);
	setenv("DVDCSS_CACHE", MPLAYER_CSSDIR, 1);
	setenv("DVDCSS_METHOD", "key", 1);
	setenv("DVDCSS_VERBOSE", "0", 1);
	setenv("DVDREAD_VERBOSE", "0", 1);
	setenv("DVDCSS_RAW_DEVICE", "/dev/di", 1);
	
	char agent[30];
	sprintf(agent, "%s/%s (IOS%d)", APPNAME, APPVERSION, IOS_GetVersion());
	network_useragent = mem2_strdup(agent, MEM2_OTHER);

	// create mplayer thread
	mplayerstack=(u8*)(memalign(32,MPLAYER_STACKSIZE*sizeof(u8)));
	memset(mplayerstack,0,MPLAYER_STACKSIZE*sizeof(u8));
	LWP_CreateThread (&mthread, mplayerthread, NULL, mplayerstack, MPLAYER_STACKSIZE, 68);

	init = true;
	return true;
}

void LoadMPlayerFile()
{
	SuspendDeviceThread();

	if(!WiiSettings.subtitleVisibility)
		SuspendParseThread();

	settingsSet = false;
	nowPlayingSet = false;
	controlledbygui = 2; // signal any previous file to end
	sync_interlace = 0; // reset interlace-handling flag

	// wait for previous file to end
	while(controlledbygui == 2)
		usleep(100);

	char *partitionlabel;
	char ext[7];
	GetExt(loadedFile, ext);

	wiiSetDVDDevice(NULL);

	if(strlen(ext) > 0 && 
		(strcasecmp(ext, "iso") == 0 || strcasecmp(ext, "ifo") == 0))
	{
		//handled in MPEG demuxer
	/*	if(WiiSettings.dvdSyncType == 1)
			sync_interlace = 1;
		else if(WiiSettings.dvdSyncType == 2)
			sync_interlace = 2; */
		if(strcasecmp(ext, "ifo") == 0)
		{
			char *end = strrchr(loadedFile, '/');
			*end = 0; // strip filename
		}
		wiiSetDVDDevice(loadedFile);
		if(WiiSettings.dvdMenu == 0)
			sprintf(loadedFile, "dvdnav://");
		else
			sprintf(loadedFile, "dvdnav://%d", WiiSettings.dvdMenu);
		partitionlabel = NULL;
	}
/*	else if(strlen(ext) > 0 && (strcasecmp(ext, "iso") == 0 || strcasecmp(ext, "ifo") == 0))
	{
		// After title selecting, this code has become useless.
		if(WiiSettings.dvdSyncType == 1)
			sync_interlace = 1;
		else if(WiiSettings.dvdSyncType == 2)
			sync_interlace = 2;
		partitionlabel = GetPartitionLabel(loadedFile);
	} */
	else
	{
		if (strncmp(loadedFile, "dvd://", 6) == 0 || strncmp(loadedFile, "dvdnav://", 9) == 0) {
			//this is now set automatically in the MPEG demuxer
		/*	if(WiiSettings.dvdSyncType == 1)
				sync_interlace = 1;
			else if(WiiSettings.dvdSyncType == 2)
				sync_interlace = 2;*/
			
		    wiiSetDVDDevice(loadedDevice);
		}
		partitionlabel = GetPartitionLabel(loadedFile);
	}

	if(WiiSettings.skipLoop == 1 || strcasecmp(ext, "dash") == 0)
		wiiDash();
	else
		wiiElse();

	// THP doesn't support seek, yet the demuxer enables it
	if(strcasecmp(ext, "thp") == 0)
		wiiTHP();
	
	if(strcasecmp(ext, "sfd") == 0) {
		use_lavf = 1;
		wiiSFD();
	} else if(use_lavf == 1) {
		use_lavf = 0;
		wiiSFD();
	}
	
	//if(WiiSettings.nativeLoops > 0)
		wiiSpecialLoops(WiiSettings.nativeLoops);

	// wait for directory parsing to finish (to find subtitles)	
	while(WiiSettings.subtitleVisibility && !ParseDone())
		usleep(100);
	
	// set new file to load
	wiiLoadFile(loadedFile, partitionlabel);

	while(controlledbygui != 0)
		usleep(100);
}

void ResumeMPlayerFile()
{
	DisableRumble();
	SuspendDeviceThread();
	SuspendParseThread();
	settingsSet = false;
	nowPlayingSet = false;
	controlledbygui = 0;
}

void StopMPlayerFile()
{
	controlledbygui = 2; // signal any previous file to end
}

void wiiSetVolNorm()
{
	if (WiiSettings.audioNorm == 1)
		wiiSetVolNorm1();
	else if (WiiSettings.audioNorm == 2)
		wiiSetVolNorm2();
}

void wiiSetVidFull()
{
	if (WiiSettings.videoFull == 1)
		wiiSetFullScreen();
	else
		wiiSetScreenNorm();

	GX_SetScreenPos(WiiSettings.videoXshift, WiiSettings.videoYshift, 
		CONF_GetAspectRatio() == CONF_ASPECT_4_3 ? WiiSettings.videoFull ? WiiSettings.videoZoomHor * 1.335
			: WiiSettings.videoZoomHor : WiiSettings.videoZoomHor, WiiSettings.videoZoomVert);
}

void wiiSetDf()
{
	if (WiiSettings.videoDf == 1)
		SetDf();
}

void wiiSetVIscale()
{
	if (WiiSettings.viWidth == 1)
		SetVIscale();
}

void wiiSetDoubleStrike()
{
	if (WiiSettings.doubleStrike == 1) {
		SetDoubleStrike();
		WiiSettings.videoDf = 0;
		if(WiiSettings.viWidth == 1)
			SetVIscale();
		else
			SetVIscaleback();
	}
}

void wiiSet576p()
{
	if (WiiSettings.force576p == 1) {
		Set576p();
		//WiiSettings.videoDf = 0;
		if(WiiSettings.viWidth == 1)
			SetVIscale();
		else
			SetVIscaleback();
	}
}

void wiiSetAssOff()
{
	if (WiiSettings.libass == 0)
		wiiAssOff();
}

void wiiBStyleOverride()
{
	if (WiiSettings.borderstyle > 0)
		wiiForceStyle(WiiSettings.borderstyle);
}

void wiiOutlineOverride()
{
	if (WiiSettings.outline > -1)
		wiiForceOutline(WiiSettings.outline);
}

void wiiShadowOverride()
{
	if (WiiSettings.shadow > -1)
		wiiForceShadow(WiiSettings.shadow);
}

void wiiBoldFont()
{
	if (WiiSettings.bold == 1)
		wiiForceBold();
}

void wiiExtraFont()
{
	if (WiiSettings.monofont == 1)
		wiiUseAltFont();
}

extern "C" {
void SetMPlayerSettings()
{
	static float aspectRatio=-2;
	if(settingsSet)
		return;

	settingsSet = true;

	char ext[7];
	GetExt(loadedFile, ext);

	wiiSetVidFull();
	wiiSetAutoResume(WiiSettings.autoResume);
	wiiSetVolume(WiiSettings.volume);
	wiiSetSeekBackward(WiiSettings.skipBackward);
	wiiSetSeekForward(WiiSettings.skipForward);
	//wiiSetVideoDelay(WiiSettings.videoDelay);
	wiiSetAssOff();
	wiiBStyleOverride();
	wiiOutlineOverride();
	wiiShadowOverride();
	wiiBoldFont();
	wiiExtraFont();
	wiiSetCacheFill(WiiSettings.cacheFill);
	wiiSetVolNorm();
	wiiSetOnlineCacheFill(WiiSettings.onlineCacheFill);

	if(WiiSettings.autoPlayNextVideo == AUTOPLAY_LOOP) // Loop video setting from mplayer
		wiiSetLoopOn();

	if(strncmp(loadedFile, "dvd", 3) == 0) // always use framedropping for DVD
		wiiSetProperty(MP_CMD_FRAMEDROPPING, FRAMEDROPPING_AUTO);
	else
		wiiSetProperty(MP_CMD_FRAMEDROPPING, WiiSettings.frameDropping);

	if(aspectRatio!=WiiSettings.aspectRatio)
	{
		aspectRatio=WiiSettings.aspectRatio;
		wiiSetProperty(MP_CMD_SWITCH_RATIO, WiiSettings.aspectRatio);
	}

	wiiSetProperty(MP_CMD_AUDIO_DELAY, WiiSettings.audioDelay);

	/* Now using internal font as backup. */
	//if(!subtitleFontFound)
		//wiiSetProperty(MP_CMD_SUB_VISIBILITY, 0);
	//else
		wiiSetProperty(MP_CMD_SUB_VISIBILITY, WiiSettings.subtitleVisibility);

	wiiSetProperty(MP_CMD_SUB_DELAY, WiiSettings.subtitleDelay);
	
	wiiSetCodepage(WiiSettings.subtitleCodepage);

	char audioLang[14] = { 0 };
	char subtitleLang[14] = { 0 };

	if(WiiSettings.audioLanguage[0] != 0)
		sprintf(audioLang, "%s,%s,en,eng", 
			WiiSettings.audioLanguage, languages[GetLangIndex(WiiSettings.audioLanguage)].abbrev2);
	if(WiiSettings.subtitleLanguage[0] != 0)
		sprintf(subtitleLang, "%s,%s,en,eng", 
			WiiSettings.subtitleLanguage, languages[GetLangIndex(WiiSettings.subtitleLanguage)].abbrev2);

	wiiSetAudioLanguage(audioLang);
	wiiSetSubtitleLanguage(subtitleLang);
	wiiSetSubtitleColor(WiiSettings.subtitleColor);
	wiiSetSubtitleSize(0.6+(WiiSettings.subtitleSize-1)*0.6); // 1.0-5.0 --> 0.6-3.0
}
}

#include <ogc/machine/processor.h>
#define CheckAHBPROT()	(read32(0x0D800064) == 0xFFFFFFFF)
#define MEM2_PROT		0x0D8B420A
#define ES_MODULE_START	((u16 *)0x939F0000)
#define ES_MODULE_END	(ES_MODULE_START + 0x4000)
#define ES_HACK_OFFSET	4

void cIOS_access(void)
{
	/* I just need this for the screen dimmer. */
	if (CheckAHBPROT())
	{

		static const u16 ticket_check[] = {
			0x685B,			// ldr  r3, [r3, #4] ; Get TMD pointer
			0x22EC, 0x0052,	// movs r2, 0x1D8	; Set offset of access rights field in TMD
			0x189B,			// adds r3, r3, r2   ; Add offset to TMD pointer
			0x681B,			// ldr  r3, [r3]	 ; Load access rights. We'll hack it with full access rights!!!
			0x4698,			// mov  r8, r3	   ; Store it for the DVD video bitcheck later
			0x07DB			// lsls r3, r3, 0x1F ; check AHBPROT bit
		};

		/* Disable MEM 2 protection. */
		write16(MEM2_PROT, 2);

		for (u16 *patchme = ES_MODULE_START; patchme < ES_MODULE_END; patchme++)
		{
			if (!memcmp(patchme, ticket_check, sizeof(ticket_check)))
			{
				/* Apply patch */
				patchme[ES_HACK_OFFSET] = 0x23FF; // li r3, 0xFF ; Set full access rights

				/* Flush cache */
				DCFlushRange(patchme+ES_HACK_OFFSET, 2);
				break;
			}
		}
	}
}

bool use32kHz = false;
bool useDumbRP = true;

void setOption(char* key, char* valuePointer){
	bool isString = valuePointer[0] == '"';
	//char value = 0;
	
	if(isString) {
		char* p = valuePointer++;
		while(*++p != '"');
		*p = 0;
	} //else
		//value = atoi(valuePointer);
	
	unsigned int i = 0;
	for(i=0; i<5; i++){
		if(!strcmp("path", key)) {
			sprintf(loadedFile, valuePointer);
			AutobootExit = true;
		} else if(!strcmp("theme", key)) {
			sprintf(WiiSettings.theme, "%s", valuePointer);
			if(strcmp(WiiSettings.theme, "dynamic") == 0)
				isDynamic = true;
			//printf("give me info: %s", valuePointer);
		} else if(!strcmp("ios", key)) {
			isReload = true;
			iosVal = atoi(valuePointer);
			//IOS_ReloadIOS(atoi(valuePointer));
		} else if(!strcmp("32khz", key)) {
			if(atoi(valuePointer) == 1)
				use32kHz = true;
		} else if(!strcmp("dumbRestorePoints", key)) {
			if(atoi(valuePointer) == 0)
				useDumbRP = false;
		} else if(!strcmp("3ds", key)) {
			if(atoi(valuePointer) == 1)
				want3DS = true;
		}
		//break;
	}
}

void handleConfigPair(char* kv){
	char* vs = kv;
	while(*vs != ' ' && *vs != '\t' && *vs != ':' && *vs != '=')
			++vs;
	*(vs++) = 0;
	while(*vs == ' ' || *vs == '\t' || *vs == ':' || *vs == '=')
			++vs;

	setOption(kv, vs);
}

void handleIOSreload(void)
{
	if(!isReload)
		return;
	
	// module loading needed to be outside the arg loop to detect hdd.
	bool ehci = false;
	
	// Regain access, this allows burn-in reduction dimmer to work.
	cIOS_access();
	IOS_ReloadIOS(iosVal);
	
	if(IOS_GetVersion() == 202) {
		if(mload_init())
			ehci = load_ehci_module();
		USB2Enable(ehci);
	}
}

/****************************************************************************
 * Main
 ***************************************************************************/
extern "C" { 
	s32 __STM_Close();
	s32 __STM_Init();
	//u32 __di_check_ahbprot(void);
}
int main(int argc, char *argv[])
{
	L2Enhance();
	USBGeckoOutput(); // don't disable - we need the stdout/stderr devoptab!
	AUDIO_Init(NULL);
	DSP_Init();
	AUDIO_StopDMA();
	InitVideo();

	// Wii Power/Reset buttons
	__STM_Close();
	__STM_Init();
	__STM_Close();
	__STM_Init();
	SYS_SetPowerCallback(ShutdownCB);
	SYS_SetResetCallback(ResetCB);
	
	__exception_setreload(8);
	
	/*if(__di_check_ahbprot() != 1) */

	//if(argc > 2) {
//		bool ehci = false;
//		IOS_ReloadIOS(202);
		//mload_init();
		//if(mload_init() >= 0 && load_ehci_module())
//		if(mload_init())
//			ehci = load_ehci_module();
			//USB2Enable(true);
//		USB2Enable(ehci);
	//}

	// path sent by the plugin's argument
	if(argc > 1 && (argv[1][0] == 'u' || argv[1][0] == 's' || argv[1][0] == 'h'))
	{
		sprintf(loadedFile, argv[1]);
		AutobootExit = true;
		//VIDEO_SetBlack(FALSE);
	} else {
		int i;
		for(i=1; i<argc; ++i){
			handleConfigPair(argv[i]);
		}
	}
	
	// My usb drive fails to mount if I use 202, only on Wii boot
	handleIOSreload();

	DI_Init();
	WPAD_Init();
	USBStorage_Initialize(); // to set aside MEM2 area

	FindAppPath(); // Initialize SD and USB devices and look for apps/wiimc
	
	StartNetworkThread(); //to set net heap aside MEM2 area
	usleep(100); //force network thread execution

	u32 size = ( (1024*MAX_HEIGHT)+(WIDTH_MULT*MAX_HEIGHT) + (1024*(MAX_HEIGHT/2)*2) ) + // textures
                (vmode->fbWidth * vmode->efbHeight * 4) + //videoScreenshot                     
                (32*1024); // padding	
	AddMem2Area (size, MEM2_VIDEO); 
	AddMem2Area (2*1024*1024, MEM2_BROWSER);
	AddMem2Area (7.5*1024*1024, MEM2_GUI);
	AddMem2Area (5*1024*1024, MEM2_OTHER); // vars + ttf
	AddMem2Area (.125*1024*1024, MEM2_DESC); // desc test

	GX_AllocTextureMemory();

	BrowserInit(&browser);
	BrowserInit(&browserSubs);
	BrowserInit(&browserVideos);
	BrowserInit(&browserMusic);
	BrowserInit(&browserOnlineMedia);

	InitVideo2();
	SetupPads();
	WPAD_SetPowerButtonCallback((WPADShutdownCallback)ShutdownCB);

	DefaultSettings(); // set defaults
	srand (time (0)); // random seed

	if(!InitFreeType((u8*)font_ttf, font_ttf_size)) // Initialize font system
		return 0;

	// mplayer cache thread
	memset(cachestack,0,CACHE_STACKSIZE*sizeof(u8));
	LWP_CreateThread(&cthread, mplayercachethread, NULL, cachestack, CACHE_STACKSIZE, 70);

	usleep(200);

	// create GUI thread
 	while(1)
	{
		ResetVideo_Menu();
 		WiiMenu();

 		if(ExitRequested)
 			break;
 
		MPlayerMenu();
	}
 	
 	// application exiting
	SuspendDeviceThread();
	SuspendParseThread();
	StopGX();
	UnmountAllDevices();

	AUDIO_StopDMA();
	AUDIO_RegisterDMACallback(NULL);

	SaveLogToSD();

	if(ShutdownRequested || WiiSettings.exitAction == EXIT_POWEROFF)
		SYS_ResetSystem(SYS_POWEROFF, 0, 0);
	else if(WiiSettings.exitAction == EXIT_WIIMENU)
		SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
}
