/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * menu.cpp
 * Menu flow routines - handles all menu logic
 ***************************************************************************/

#include <gccore.h>
#include <ogcsys.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <map>
#include <libexif/exif-data.h>
#include <wiiuse/wpad.h>
#include <di/di.h>
#include <iso9660.h>
#include <ogc/lwp_watchdog.h>
#include <ogc/machine/processor.h>

#include "libwiigui/gui.h"
#include "menu.h"
#include "wiimc.h"
#include "settings.h"
#include "fileop.h"
#include "input.h"
#include "networkop.h"
#include "musicplaylist.h"
#include "filebrowser.h"
#include "utils/gettext.h"
#include "utils/http.h"
#include "utils/playlog.h"
#include "filelist.h"

extern "C" {
#include "mplayer/stream/url.h"
}

#define THREAD_SLEEP 	200
#define GSTACK 			(16384)
#define GUITH_STACK 	(16384)

static u8 guistack[GSTACK] ATTRIBUTE_ALIGN (32);
static u8 progressstack[GUITH_STACK] ATTRIBUTE_ALIGN (32);
static u8 screensaverstack[GUITH_STACK] ATTRIBUTE_ALIGN (32);
static u8 creditsstack[GUITH_STACK] ATTRIBUTE_ALIGN (32);

extern char streamtitle[128]; // ICY data (http.c)
extern char streamurl[128]; // ICY data (http.c)
extern char streamname[128]; // ICY data (http.c)
extern int streamtitle_changed;
extern int streamurl_changed;
extern int streamname_changed;
extern int wii_error; // to display a specific error when loading fails

// frequently used objects

static GuiImageData *bg = NULL;
static GuiImageData *navDivider = NULL;
static GuiImageData *btnBottom = NULL;
static GuiImageData *btnBottomOver = NULL;
static GuiImageData *arrowRightSmall = NULL;
static GuiImageData *pointer[4] = { NULL, NULL, NULL, NULL };
static const u8 *pointerImg[4] = { NULL, NULL, NULL, NULL };
static const u8 *pointerGrabImg[4] = { NULL, NULL, NULL, NULL };
static GuiImageData *throbber = NULL;
static GuiImageData *progressLeft = NULL;
static GuiImageData *progressMid = NULL;
static GuiImageData *progressRight = NULL;
static GuiImageData *progressEmpty = NULL;
static GuiImageData *progressShortEmpty = NULL;
static GuiImageData *progressLongEmpty = NULL;
static GuiImageData *progressLine = NULL;
static GuiImageData *volumeTop = NULL;
static GuiImageData *volumeMid = NULL;
static GuiImageData *volumeBottom = NULL;
static GuiImageData *volumeEmpty = NULL;
static GuiImageData *volumeLine = NULL;
static GuiImage *disabled = NULL;
static GuiTrigger *trigA = NULL;
static GuiTrigger *trigHeldA = NULL;
static GuiTrigger *trigB = NULL;
static GuiTrigger *trigLeft = NULL;
static GuiTrigger *trigRight = NULL;
static GuiTrigger *trigUp = NULL;
static GuiTrigger *trigDown = NULL;
static GuiTrigger *trigMinus = NULL;
static GuiTrigger *trigPlus = NULL;

static GuiImage *videoImg = NULL;
static GuiButton *videosBtn = NULL;
static GuiButton *musicBtn = NULL;
static GuiButton *dvdBtn = NULL;
static GuiButton *onlineBtn = NULL;
static GuiButton *settingsBtn = NULL;

static GuiImage *videosBtnImg = NULL;
static GuiImage *videosBtnOnImg = NULL;
static GuiImage *musicBtnImg = NULL;
static GuiImage *musicBtnOnImg = NULL;
static GuiImage *dvdBtnImg = NULL;
static GuiImage *dvdBtnOnImg = NULL;
static GuiImage *onlineBtnImg = NULL;
static GuiImage *onlineBtnOnImg = NULL;
static GuiImage *settingsBtnImg = NULL;
static GuiImage *settingsBtnOnImg = NULL;

static GuiButton *logoBtn = NULL;
static GuiWindow *mainWindow = NULL;
static GuiWindow *menuWindow = NULL;
static GuiWindow *mplayerWindow = NULL;
static GuiText *settingText = NULL;
static GuiText *settingText2 = NULL;
static GuiText *nowPlaying = NULL;

// actionbar

static GuiText *statusText = NULL;

static GuiWindow *videobar = NULL;
static GuiWindow *audiobar = NULL;
static GuiWindow *audiobar2 = NULL;

static GuiImageData *actionbarLeft = NULL;
static GuiImageData *actionbarMid = NULL;
static GuiImageData *actionbarRight = NULL;
static GuiImageData *actionbarBackward = NULL;
static GuiImageData *actionbarPause = NULL;
static GuiImageData *actionbarPlay = NULL;
static GuiImageData *actionbarForward = NULL;
static GuiImageData *actionbarRotate = NULL;
static GuiImageData *actionbarSingle = NULL;
static GuiImageData *actionbarContinuous = NULL;
static GuiImageData *actionbarShuffle = NULL;
static GuiImageData *actionbarLoop = NULL;
static GuiImageData *actionbarPlaylist = NULL;
static GuiImageData *actionbarClose = NULL;
static GuiImageData *actionbarVolume = NULL;
static GuiImageData *actionbarThrough = NULL;

static GuiImage *videobarLeftImg = NULL;
static GuiImage *videobarMidImg = NULL;
static GuiImage *videobarRightImg = NULL;
static GuiImage *videobarProgressImg = NULL;
static GuiImage *videobarProgressLeftImg = NULL;
static GuiImage *videobarProgressMidImg = NULL;
static GuiImage *videobarProgressLineImg = NULL;
static GuiImage *videobarProgressRightImg = NULL;
static GuiImage *videobarVolumeImg = NULL;
static GuiImage *videobarVolumeLevelImg = NULL;
static GuiImage *videobarVolumeLevelTopImg = NULL;
static GuiImage *videobarVolumeLevelMidImg = NULL;
static GuiImage *videobarVolumeLevelLineImg = NULL;
static GuiImage *videobarVolumeLevelBottomImg = NULL;
static GuiImage *videobarBackwardImg = NULL;
static GuiImage *videobarPauseImg = NULL;
static GuiImage *videobarForwardImg = NULL;

static GuiTooltip *videobarVolumeTip = NULL;
static GuiTooltip *videobarBackwardTip = NULL;
static GuiTooltip *videobarPauseTip = NULL;
static GuiTooltip *videobarForwardTip = NULL;

static GuiButton *videobarProgressBtn = NULL;
static GuiButton *videobarVolumeBtn = NULL;
static GuiButton *videobarVolumeLevelBtn = NULL;
static GuiButton *videobarBackwardBtn = NULL;
static GuiButton *videobarPauseBtn = NULL;
static GuiButton *videobarForwardBtn = NULL;

static GuiText *videobarTime = NULL;
static GuiText *videobarDropped = NULL;

static GuiImage *audiobarLeftImg = NULL;
static GuiImage *audiobarMidImg = NULL;
static GuiImage *audiobarRightImg = NULL;
static GuiImage *audiobarProgressImg = NULL;
static GuiImage *audiobarProgressLeftImg = NULL;
static GuiImage *audiobarProgressMidImg = NULL;
static GuiImage *audiobarProgressLineImg = NULL;
static GuiImage *audiobarProgressRightImg = NULL;
static GuiImage *audiobarPlaylistImg = NULL;
static GuiImage *audiobarBackwardImg = NULL;
static GuiImage *audiobarPauseImg = NULL;
static GuiImage *audiobarForwardImg = NULL;
static GuiImage *audiobarModeImg = NULL;
static GuiImage *audiobarVolumeImg = NULL;
static GuiImage *audiobarVolumeLevelImg = NULL;
static GuiImage *audiobarVolumeLevelTopImg = NULL;
static GuiImage *audiobarVolumeLevelMidImg = NULL;
static GuiImage *audiobarVolumeLevelLineImg = NULL;
static GuiImage *audiobarVolumeLevelBottomImg = NULL;

static GuiTooltip *audiobarPlaylistTip = NULL;
static GuiTooltip *audiobarBackwardTip = NULL;
static GuiTooltip *audiobarPauseTip = NULL;
static GuiTooltip *audiobarForwardTip = NULL;
static GuiTooltip *audiobarModeTip = NULL;
static GuiTooltip *audiobarVolumeTip = NULL;

static GuiButton *audiobarProgressBtn = NULL;
static GuiButton *audiobarPlaylistBtn = NULL;
static GuiButton *audiobarBackwardBtn = NULL;
static GuiButton *audiobarPauseBtn = NULL;
static GuiButton *audiobarForwardBtn = NULL;
static GuiButton *audiobarModeBtn = NULL;
static GuiButton *audiobarVolumeBtn = NULL;
static GuiButton *audiobarVolumeLevelBtn = NULL;

static GuiText *audiobarNowPlaying[4] = { NULL, NULL, NULL, NULL };
static GuiButton *audiobarNowPlayingBtn = NULL;
bool nowPlayingSet = false;

static GuiImage *navDividerImg = NULL;
static GuiImage *bgImg = NULL;
static GuiTooltip *logoBtnTip = NULL;
static GuiImageData *logo = NULL;
static GuiImage *logoBtnImg = NULL;
static GuiImageData *navHighlight = NULL;
static GuiImageData *videos = NULL;
static GuiImageData *videosOver = NULL;
static GuiImageData *videosOn = NULL;
static GuiImageData *music = NULL;
static GuiImageData *musicOver = NULL;
static GuiImageData *musicOn = NULL;
static GuiImageData *dvd = NULL;
static GuiImageData *dvdOver = NULL;
static GuiImageData *dvdOn = NULL;
static GuiImageData *online = NULL;
static GuiImageData *onlineOver = NULL;
static GuiImageData *onlineOn = NULL;
static GuiImageData *settings = NULL;
static GuiImageData *settingsOver = NULL;
static GuiImageData *settingsOn = NULL;
static GuiTooltip *videosBtnTip = NULL;
static GuiImage *videosBtnOverImg = NULL;
static GuiImage *videosBtnHighlightImg = NULL;
static GuiTooltip *musicBtnTip = NULL;
static GuiImage *musicBtnOverImg = NULL;
static GuiImage *musicBtnHighlightImg = NULL;
static GuiTooltip *dvdBtnTip = NULL;
static GuiImage *dvdBtnOverImg = NULL;
static GuiImage *dvdBtnHighlightImg = NULL;
static GuiTooltip *onlineBtnTip = NULL;
static GuiImage *onlineBtnOverImg = NULL;
static GuiImage *onlineBtnHighlightImg = NULL;
static GuiTooltip *settingsBtnTip = NULL;
static GuiImage *settingsBtnOverImg = NULL;
static GuiImage *settingsBtnHighlightImg = NULL;

int menuCurrent = -1;
static int menuPrevious = MENU_BROWSE_VIDEOS;
static int menuUndo = MENU_BROWSE_VIDEOS;
static int netEditIndex = 0; // current index of FTP/SMB share being edited

// threads
static lwp_t guithread = LWP_THREAD_NULL;
static lwp_t progressthread = LWP_THREAD_NULL;
static lwp_t screensaverthread = LWP_THREAD_NULL;
static lwp_t creditsthread = LWP_THREAD_NULL;
static lwp_t thumbthread = LWP_THREAD_NULL;

static int progressThreadHalt = 0;
static int screensaverThreadHalt = 0;
static int creditsThreadHalt = 0;
static int thumbThreadHalt = 0;

static int guiHalt = 0;
static bool guiShutdown = true;
static int showProgress = 0;

static char progressTitle[100];
static char progressMsg[200];
static int progressDone = 0;
static int progressTotal = 0;

bool menuMode = 0; // 0 - normal GUI, 1 - GUI for MPlayer
static int slideshow = 0; // slideshow mode
static u64 ssTimer = 0;
static vu32* HW_VIDIM = (vu32*)0xCD80001C;

static void UpdateMenuImages(int oldBtn, int newBtn)
{	
	if(oldBtn > MENU_SETTINGS)
		oldBtn = MENU_SETTINGS;
	if(newBtn > MENU_SETTINGS)
		newBtn = MENU_SETTINGS;

	if(oldBtn == newBtn)
		return; // do nothing

	switch(oldBtn)
	{
		case MENU_BROWSE_VIDEOS:
			videosBtn->SetImage(videosBtnImg); break;
		case MENU_BROWSE_MUSIC:
			musicBtn->SetImage(musicBtnImg); break;
		case MENU_DVD:
			dvdBtn->SetImage(dvdBtnImg); break;
		case MENU_BROWSE_ONLINEMEDIA:
			onlineBtn->SetImage(onlineBtnImg); break;
		case MENU_SETTINGS:
			settingsBtn->SetImage(settingsBtnImg); break;
	}

	switch(newBtn)
	{
		case MENU_BROWSE_VIDEOS:
			videosBtn->SetImage(videosBtnOnImg); break;
		case MENU_BROWSE_MUSIC:
			musicBtn->SetImage(musicBtnOnImg); break;
		case MENU_DVD:
			dvdBtn->SetImage(dvdBtnOnImg); break;
		case MENU_BROWSE_ONLINEMEDIA:
			onlineBtn->SetImage(onlineBtnOnImg); break;
		case MENU_SETTINGS:
			settingsBtn->SetImage(settingsBtnOnImg); break;
	}
}

static void SaveFolder()
{
	if(WiiSettings.lockFolders)
		return;

	switch(browser.menu)
	{
		case MENU_BROWSE_VIDEOS:
			strcpy(WiiSettings.videosFolder, browser.dir);
			break;
		case MENU_BROWSE_MUSIC:
			strcpy(WiiSettings.musicFolder, browser.dir);
			break;
		case MENU_BROWSE_ONLINEMEDIA:
			strcpy(WiiSettings.onlinemediaFolder, browser.dir);
			break;
	}
}

static void ChangeMenuNoHistory(int menu)
{
	if(menu == menuCurrent)
		return;

	SaveFolder();
	UpdateMenuImages(menuCurrent, menu);
	menuCurrent = menu;
}

static void ChangeMenu(int menu)
{
	if(menu == menuCurrent)
		return;

	SaveFolder();
	menuUndo = menuPrevious;
	menuPrevious = menuCurrent;
	menuCurrent = menu;
	UpdateMenuImages(menuPrevious, menuCurrent);
}

void UndoChangeMenu()
{
	SaveFolder();
	UpdateMenuImages(menuCurrent, menuPrevious);
	menuCurrent = menuPrevious;
	menuPrevious = menuUndo;
}

static void ChangeMenu(void *ptr, int menu)
{
	GuiButton *b = (GuiButton *)ptr;
	if(b->GetState() == STATE_CLICKED)
	{
		ChangeMenu(menu);
		b->ResetState();
	}
}

static void ChangeMenuVideos(void *ptr) { ChangeMenu(ptr, MENU_BROWSE_VIDEOS); }
static void ChangeMenuMusic(void *ptr) { ChangeMenu(ptr, MENU_BROWSE_MUSIC); }
static void ChangeMenuDVD(void *ptr) { ChangeMenu(ptr, MENU_DVD); }
static void ChangeMenuOnline(void *ptr) { ChangeMenu(ptr, MENU_BROWSE_ONLINEMEDIA); }
static void ChangeMenuSettings(void *ptr) { ChangeMenu(ptr, MENU_SETTINGS); }

/****************************************************************************
 * ResumeGui
 *
 * Signals the GUI thread to start, and resumes the thread. This is called
 * after finishing the removal/insertion of new elements, and after initial
 * GUI setup.
 ***************************************************************************/
static void ResumeGui()
{
	if(guithread == LWP_THREAD_NULL || guiShutdown)
		return;

	guiHalt = 0;
	LWP_ResumeThread (guithread);
}

/****************************************************************************
 * SuspendGui
 *
 * Signals the GUI thread to stop, and waits for GUI thread to stop
 * This is necessary whenever removing/inserting new elements into the GUI.
 * This eliminates the possibility that the GUI is in the middle of accessing
 * an element that is being changed.
 ***************************************************************************/
static void SuspendGui()
{
	if(guithread == LWP_THREAD_NULL)
		return;

	guiHalt = 1;

	// wait for thread to finish
	while(!LWP_ThreadIsSuspended(guithread))
		usleep(THREAD_SLEEP);
}

static void *ScreensaverThread(void *arg)
{
	GuiWindow *oldWindow;

	GuiImageData logoLarge(logo_large_png);
	GuiImage logoLargeImg(&logoLarge);
	logoLargeImg.SetAlignment(ALIGN_LEFT, ALIGN_TOP);

	GuiWindow w(logoLargeImg.GetWidth(), logoLargeImg.GetHeight()+100);
	w.Append(&logoLargeImg);
	w.SetPosition(screenwidth/2-w.GetWidth()/2, screenheight/2-w.GetHeight()/2);

	int threadsleep;

	while(1)
	{
		LWP_SuspendThread (screensaverthread);

		if(screensaverThreadHalt == 2)
			break;

		oldWindow = mainWindow;

		SuspendGui();
		audiobar->Remove(audiobarNowPlayingBtn);

		for(int i=1; i < 4; i++)
			audiobarNowPlaying[i]->SetMaxWidth(logoLargeImg.GetWidth());

		w.Append(audiobarNowPlayingBtn);
		audiobarNowPlayingBtn->SetPosition(-8, logoLargeImg.GetHeight()+20);
		mainWindow = &w;
		ResumeGui();

		while(1)
		{
			threadsleep = 1000*1000*6; // 6 sec

			while(threadsleep > 0)
			{
				if(screensaverThreadHalt != 0)
					goto done;
				usleep(THREAD_SLEEP);
				threadsleep -= THREAD_SLEEP;
			}

			int x=0,y=0;

			while(x < 30 || x > (screenwidth-w.GetWidth()-30))
				x = (int)(((double)rand() / double(RAND_MAX + 1.0)) * screenwidth);
			while(y < 30 || y > (screenheight-w.GetHeight()-30))
				y = (int)(((double)rand() / double(RAND_MAX + 1.0)) * screenheight);

			w.SetPosition(x, y);
			
			if(WiiSettings.inactivityShutdown > 0 && 
				!(wiiAudioOnly() && !wiiIsPaused()) &&
				diff_sec(ssTimer, gettime()) > (u32)(WiiSettings.inactivityShutdown*3600))
			{
				ExitRequested = true;
				ShutdownRequested = true;
			}
		}
done:
		SuspendGui();
		w.Remove(audiobarNowPlayingBtn);

		for(int i=1; i < 4; i++)
			audiobarNowPlaying[i]->SetMaxWidth(screenwidth-460);

		audiobar->Append(audiobarNowPlayingBtn);
		audiobarNowPlayingBtn->SetPosition(0, 0);
		mainWindow = oldWindow;
		ResumeGui();
		if(screensaverThreadHalt == 2)
			break;
	}
	return NULL;
}

static void ResumeScreensaverThread()
{
	if(screensaverthread == LWP_THREAD_NULL || guiShutdown)
		return;

	screensaverThreadHalt = 0;
	LWP_ResumeThread(screensaverthread);
}

static bool videoPaused;

extern "C" void DoMPlayerGuiDraw()
{
	if(menuMode != 1)
		return;

	mainWindow->Draw();
	mainWindow->DrawTooltip();

	if(userInput[0].wpad->ir.valid)
		Menu_DrawImg(userInput[0].wpad->ir.x-48, userInput[0].wpad->ir.y-48,
			96, 96, pointer[0]->GetImage(), userInput[0].wpad->ir.angle, 1, 1, 255, GX_TF_RGBA8);

	DoRumble(0);
	mainWindow->Update(&userInput[0]);

	if(mainWindow->IsVisible() && wiiInDVDMenu())
	{
		mainWindow->SetVisible(false);
		mainWindow->SetState(STATE_DISABLED);
	}
	else if(!mainWindow->IsVisible() && !wiiInDVDMenu())
	{
		mainWindow->SetVisible(true);
		mainWindow->SetState(STATE_DEFAULT);
		HideVideoVolumeLevelBar();
		statusText->SetVisible(false);
	}

	if(videoPaused != wiiIsPaused())
	{
		videoPaused = !videoPaused;
		if(videoPaused)
		{
			videobarPauseImg->SetImage(actionbarPlay);
			videobarPauseTip->SetText("Play");
		}
		else
		{
			videobarPauseImg->SetImage(actionbarPause);
			videobarPauseTip->SetText("Pause");
		}
	}
}

/****************************************************************************
 * GuiThread
 *
 * Primary GUI thread to allow GUI to respond to state changes, and draws GUI
 ***************************************************************************/
static void *GuiThread (void *arg)
{
	int i;
	ssTimer = 0;

	while(1)
	{
		if(guiHalt == 1)
			LWP_SuspendThread(guithread);

		if(guiHalt == 2)
			break;

		UpdatePads();
		mainWindow->Draw();

		if (mainWindow->GetState() != STATE_DISABLED)
			mainWindow->DrawTooltip();

		if(userInput[0].wpad->ir.valid)
			Menu_DrawImg(userInput[0].wpad->ir.x-48, userInput[0].wpad->ir.y-48,
				96, 96, pointer[0]->GetImage(), userInput[0].wpad->ir.angle, 1, 1, 255, GX_TF_RGBA8);
		
		DoRumble(0);
		mainWindow->Update(&userInput[0]);

		if(mainWindow->GetState() != STATE_DISABLED)
		{
			if(userInput[0].wpad->btns_d & (WPAD_BUTTON_1 | WPAD_CLASSIC_BUTTON_X) || userInput[0].pad.btns_d & PAD_TRIGGER_R)
			{
				int newMenu = menuCurrent + 1;
				if(newMenu == MENU_DVD && WiiSettings.dvdDisabled)
					newMenu++;
				if(newMenu > MENU_SETTINGS)
					newMenu = MENU_BROWSE_VIDEOS;
				ChangeMenu(newMenu);
			}
			else if(userInput[0].wpad->btns_d & (WPAD_BUTTON_2 | WPAD_CLASSIC_BUTTON_Y) || userInput[0].pad.btns_d & PAD_TRIGGER_L)
			{
				int newMenu = menuCurrent - 1;
				if(newMenu == MENU_DVD && WiiSettings.dvdDisabled)
					newMenu--;
				if(newMenu < MENU_BROWSE_VIDEOS)
					newMenu = MENU_SETTINGS;
				ChangeMenu(newMenu);
			}
		}

		Menu_Render();

		if(userInput[0].wpad->data_present > 0 || (userInput[0].pad.btns_d > 0 &&
											userInput[0].pad.btns_d != 0x1000) ||
											userInput[0].pad.stickX < -36 ||
											userInput[0].pad.stickY < -36 ||
											userInput[0].pad.stickX > 36 ||
											userInput[0].pad.stickY > 36)
		{
			if(ssTimer != 0)
			{
				ssTimer = 0;
				screensaverThreadHalt = 1;
				if(WiiSettings.screenDim == 1)
					*HW_VIDIM &= ~1 << 7;
			}
		}
		else if(!slideshow)
		{
			if(ssTimer == 0)
				ssTimer = gettime();

			if(diff_sec(ssTimer, gettime()) > (u32)(WiiSettings.screensaverDelay-60))
				ResumeScreensaverThread();
			if(WiiSettings.screenDim == 1) {
				if(diff_sec(ssTimer, gettime()) > (u32)(WiiSettings.screensaverDelay-60+4)) {
					*HW_VIDIM |= 1 << 7; // Enable dimming
					//*HW_VIDIM |= 1 << 5; //luma = 2
					*HW_VIDIM |= 1 << 4;
					//*HW_VIDIM |= 1 << 3;
					//*HW_VIDIM |= 1 << 2; //chroma = 2
					*HW_VIDIM |= 1 << 1;
					//*HW_VIDIM |= 1 << 0;
				}
			}
		}

		CheckSleepTimer();

		if(userInput[0].wpad->btns_d & (WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME) || userInput[0].pad.btns_d & PAD_BUTTON_START)
			ExitRequested = true; // exit program

		if(ExitRequested)
		{
			for(i = 0; i <= 255; i += 15)
			{
				mainWindow->Draw();
				Menu_DrawRectangle(0,0,screenwidth,screenheight,(GXColor){0, 0, 0, i},1);
				Menu_Render();
			}
			// Turn off dimming on exit
			if(WiiSettings.screenDim == 1)
				*HW_VIDIM &= ~1 << 7;
			
			//Update message board time
			Playlog_Exit();
			
			guiShutdown = true;
			guiHalt = 1;
			ActivateExitThread();
			SaveFolder();
		}
		usleep(THREAD_SLEEP);
	}
	return NULL;
}

extern "C" void ShutdownGui()
{
	if(menuMode == 1) // prevent MPlayer from shutting down OSD
		return;

	guiShutdown = true;
	CancelAction();
	SuspendGui();

	while(menuMode != 1)
		usleep(100); // wait for GUI mode to switch
}

static void ResetText()
{
	LoadLanguage();

	videobar->ResetText();
	audiobar->ResetText();

	if(mainWindow)
		mainWindow->ResetText();
}

static int currentFont = FONT_DEFAULT;
static u8 *ext_font_ttf = NULL;

void ChangeLanguage()
{
	char error[128] = {0};

	/*if(WiiSettings.language == LANG_JAPANESE ||
		WiiSettings.language == LANG_SIMP_CHINESE ||
		WiiSettings.language == LANG_TRAD_CHINESE ||
		WiiSettings.language == LANG_KOREAN ||
		WiiSettings.language == LANG_RUSSIAN || 
		WiiSettings.language == LANG_TAMIL || 
		WiiSettings.language == LANG_BULGARIAN)
	{
		char filepath[MAXPATHLEN];
		char httppath[MAXPATHLEN];
		char httpRoot[] = "http://wiimc.googlecode.com/svn/trunk/fonts";
		int newFont;

		switch(WiiSettings.language)
		{
			case LANG_SIMP_CHINESE:
				if(currentFont == FONT_SIMP_CHINESE) return;
				sprintf(filepath, "%s/zh_cn.ttf", appPath);
				sprintf(httppath, "%s/zh_cn.ttf", httpRoot);
				newFont = FONT_SIMP_CHINESE;
				break;
			case LANG_TRAD_CHINESE:
				if(currentFont == FONT_TRAD_CHINESE) return;
				sprintf(filepath, "%s/zh_cn.ttf", appPath);
				sprintf(httppath, "%s/zh_cn.ttf", httpRoot);
				newFont = FONT_TRAD_CHINESE;
				break;
			case LANG_KOREAN:
				if(currentFont == FONT_KOREAN) return;
				sprintf(filepath, "%s/ko.ttf", appPath);
				sprintf(httppath, "%s/ko.ttf", httpRoot);
				newFont = FONT_KOREAN;
				break;
			case LANG_JAPANESE:
				if(currentFont == FONT_JAPANESE) return;
				sprintf(filepath, "%s/jp.ttf", appPath);
				sprintf(httppath, "%s/jp.ttf", httpRoot);
				newFont = FONT_JAPANESE;
				break;
			case LANG_TAMIL:
				if(currentFont == FONT_TAMIL) return;
				sprintf(filepath, "%s/ta.ttf", appPath);
				sprintf(httppath, "%s/ta.ttf", httpRoot);
				newFont = FONT_TAMIL;
				break;
			case LANG_RUSSIAN:
			case LANG_BULGARIAN:
				if(currentFont == FONT_GENERIC) return;
				sprintf(filepath, "%s/gen.ttf", appPath);
				sprintf(httppath, "%s/gen.ttf", httpRoot);
				newFont = FONT_GENERIC;
				break;	
		}

		// try to load font
restart:
		FILE *file = fopen (filepath, "rb");

		if(file)
		{
			fseeko(file,0,SEEK_END);
			u32 loadSize = ftello(file);

			if(loadSize == 0)
			{
				if(remove(filepath) == 0)
				{
					goto restart;
				}
				else
				{
					ErrorPrompt("Error opening font file!");
					goto error;
				}
			}

			if(ext_font_ttf)
			{
				SuspendGui();
				mem2_free(ext_font_ttf, MEM2_EXTFONT);
				ext_font_ttf = NULL;
			}

			if(AddMem2Area(loadSize+1024,MEM2_EXTFONT))
			{
				ext_font_ttf = (u8 *)mem2_memalign(32, loadSize, MEM2_EXTFONT); // can be a problem we have to see how to manage it
				if(ext_font_ttf)
				{
					fseeko(file,0,SEEK_SET);
					fread (ext_font_ttf, 1, loadSize, file);
				}
			}
			fclose(file);

			if(ext_font_ttf)
			{
				SuspendGui();
				DeinitFreeType();

				currentFont = newFont;
				if(InitFreeType(ext_font_ttf, loadSize))
				{
					ResetText();
					ResumeGui();
					return;
				}
				else
				{
					sprintf(error, "Could not change language. The font file is corrupted!");
				}
			}
			else
			{
				sprintf(error, "Could not change language. Not enough memory!");
			}
		}
		else
		{
			bool installFont = WindowPrompt(
				"Font Required",
				gettext("A new font is required to display this language."),
				"Download font",
				"Cancel");

			if(installFont)
			{
				FILE *hfile = fopen (filepath, "wb");

				if (hfile > 0)
				{
					int res = http_request(httppath, hfile, NULL, 1024*1024*2, NOTSILENT);
					fclose (hfile);

					if(res > 0)
						goto restart;

					remove(filepath);
					ErrorPrompt("Error downloading font file!");
				}
				else
				{
					ErrorPrompt("Unable to save font file!");
				}
			}
		}
error:
		WiiSettings.language = LANG_ENGLISH;
	}*/
	
	if(WiiSettings.language == LANG_KOREAN)
	{
		char filepath[MAXPATHLEN];
		int newFont;

		switch(WiiSettings.language)
		{
			case LANG_KOREAN:
				if(currentFont == FONT_KOREAN) return;
				sprintf(filepath, "%s/ko.ttf", appPath);
				newFont = FONT_KOREAN;
				break;
		}

		// try to load font
restart:
		FILE *file = fopen (filepath, "rb");

		if(file)
		{
			fseeko(file,0,SEEK_END);
			u32 loadSize = ftello(file);

			if(loadSize == 0)
			{
				if(remove(filepath) == 0)
				{
					goto restart;
				}
				else
				{
					ErrorPrompt("Error opening font file!");
					goto error;
				}
			}

			if(ext_font_ttf)
			{
				SuspendGui();
				mem2_free(ext_font_ttf, MEM2_EXTFONT);
				ext_font_ttf = NULL;
			}

			if(AddMem2Area(loadSize+1024,MEM2_EXTFONT))
			{
				ext_font_ttf = (u8 *)mem2_memalign(32, loadSize, MEM2_EXTFONT); // can be a problem we have to see how to manage it
				if(ext_font_ttf)
				{
					fseeko(file,0,SEEK_SET);
					fread (ext_font_ttf, 1, loadSize, file);
				}
			}
			fclose(file);

			if(ext_font_ttf)
			{
				SuspendGui();
				DeinitFreeType();

				currentFont = newFont;
				if(InitFreeType(ext_font_ttf, loadSize))
				{
					ResetText();
					ResumeGui();
					return;
				}
				else
				{
					sprintf(error, "Could not change language. The font file is corrupted!");
				}
			}
			else
			{
				sprintf(error, "Could not change language. Not enough memory!");
			}
		}
error:
		WiiSettings.language = LANG_ENGLISH;
	}

	if(currentFont != FONT_DEFAULT)
	{
		SuspendGui();
		DeinitFreeType();
		if(ext_font_ttf) mem2_free(ext_font_ttf, MEM2_EXTFONT);
		ext_font_ttf = NULL;
		InitFreeType((u8*)font_ttf, font_ttf_size);
		currentFont = FONT_DEFAULT;
		ResetText();
		ResumeGui();

		if(error[0] != 0)
			ErrorPrompt(error);
	}
	else
	{
		SuspendGui();
		ResetText();
		ResumeGui();
	}
}

/****************************************************************************
 * WindowPrompt
 *
 * Displays a prompt window to user, with information, an error message, or
 * presenting a user with a choice
 ***************************************************************************/
int
WindowPrompt(const char *title, wchar_t *msg, const char *btn1Label, const char *btn2Label)
{
	if(!mainWindow || ExitRequested)
		return 0;

	int choice = -1;

	GuiWindow promptWindow(556,244);
	promptWindow.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	promptWindow.SetPosition(0, -10);
	GuiImageData btnOutline(button_png);
	GuiImageData btnOutlineOver(button_over_png);

	GuiImageData dialogBox(dialogue_box_png);
	GuiImage dialogBoxImg(&dialogBox);
	dialogBoxImg.SetAlpha(220);

	GuiText titleTxt(title, 28, (GXColor){255, 255, 255, 255});
	titleTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	titleTxt.SetPosition(0,18);
	GuiText msgTxt(NULL, 20, (GXColor){255, 255, 255, 255});
	msgTxt.SetWText(msg);
	msgTxt.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	msgTxt.SetPosition(0,-20);
	msgTxt.SetWrap(true, 430);

	GuiText btn1Txt(btn1Label, 20, (GXColor){255, 255, 255, 255});
	GuiImage btn1Img(&btnOutline);
	GuiImage btn1ImgOver(&btnOutlineOver);
	GuiButton btn1(btnOutline.GetWidth(), btnOutline.GetHeight());

	if(btn2Label)
	{
		btn1.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
		btn1.SetPosition(20, -40);
	}
	else
	{
		btn1.SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
		btn1.SetPosition(0, -40);
	}

	btn1.SetLabel(&btn1Txt);
	btn1.SetImage(&btn1Img);
	btn1.SetImageOver(&btn1ImgOver);
	btn1.SetTrigger(trigA);
	btn1.SetSelectable(false);
	btn1.SetEffectGrow();

	GuiText btn2Txt(btn2Label, 20, (GXColor){255, 255, 255, 255});
	GuiImage btn2Img(&btnOutline);
	GuiImage btn2ImgOver(&btnOutlineOver);
	GuiButton btn2(btnOutline.GetWidth(), btnOutline.GetHeight());
	btn2.SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	btn2.SetPosition(-20, -40);
	btn2.SetLabel(&btn2Txt);
	btn2.SetImage(&btn2Img);
	btn2.SetImageOver(&btn2ImgOver);
	btn2.SetTrigger(trigA);
	btn2.SetSelectable(false);
	btn2.SetEffectGrow();

	promptWindow.Append(&dialogBoxImg);
	promptWindow.Append(&titleTxt);
	promptWindow.Append(&msgTxt);
	promptWindow.Append(&btn1);

	if(btn2Label)
		promptWindow.Append(&btn2);

	promptWindow.SetEffect(EFFECT_FADE, 50);
	CancelAction();

	bool isDisabled = mainWindow->Find(disabled);

	SuspendGui();
	mainWindow->SetState(STATE_DISABLED);
	if(!isDisabled)
		mainWindow->Append(disabled);
	mainWindow->Append(&promptWindow);
	ResumeGui();

	while(choice == -1)
	{
		usleep(THREAD_SLEEP);

		if(btn1.GetState() == STATE_CLICKED || userInput[0].pad.btns_d == PAD_BUTTON_A)
			choice = 1;
		else if(btn2.GetState() == STATE_CLICKED || userInput[0].pad.btns_d == PAD_BUTTON_B)
			choice = 0;

		if(guiShutdown)
			choice = 0;
	}

	promptWindow.SetEffect(EFFECT_FADE, -50);
	while(promptWindow.GetEffect() > 0 && !guiShutdown) usleep(THREAD_SLEEP);
	SuspendGui();
	mainWindow->Remove(&promptWindow);
	if(!isDisabled)
		mainWindow->Remove(disabled);
	mainWindow->SetState(STATE_DEFAULT);
	ResumeGui();
	return choice;
}

int WindowPrompt(const char *title, const char *msg, const char *btn1Label, const char *btn2Label)
{
	wchar_t *tempmsg = charToWideChar(gettext(msg));
	int res = WindowPrompt(title, tempmsg, btn1Label, btn2Label);
	delete[] tempmsg;
	return res;
}

/****************************************************************************
 * ProgressWindow
 *
 * Opens a window, which displays progress to the user. Can either display a
 * progress bar showing % completion, or a throbber that only shows that an
 * action is in progress.
 ***************************************************************************/
static int progsleep = 0;
static UpdateCallback progressCancelCallback = NULL;

static void
ProgressWindow(char *title, char *msg)
{
	GuiWindow promptWindow(556,244);
	promptWindow.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	promptWindow.SetPosition(0, -10);
	GuiImageData btnOutline(button_png);
	GuiImageData btnOutlineOver(button_over_png);

	GuiImageData dialogBox(dialogue_box_png);
	GuiImage dialogBoxImg(&dialogBox);
	dialogBoxImg.SetAlpha(220);
	
	int baroffset = 556/2 - progressEmpty->GetWidth()/2;

	GuiImage progressEmptyImg(progressEmpty);
	progressEmptyImg.SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	progressEmptyImg.SetPosition(baroffset, 40);
	
	GuiImage progressLeftImg(progressLeft);
	progressLeftImg.SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	progressLeftImg.SetPosition(baroffset, 40);
	progressLeftImg.SetVisible(false);
	
	GuiImage progressMidImg(progressMid);
	progressMidImg.SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	progressMidImg.SetPosition(baroffset+8, 40);
	progressMidImg.SetTile(0);

	GuiImage progressLineImg(progressLine);
	progressLineImg.SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	progressLineImg.SetPosition(baroffset+8, 40);
	progressLineImg.SetVisible(false);

	GuiImage progressRightImg(progressRight);
	progressRightImg.SetAlignment(ALIGN_RIGHT, ALIGN_MIDDLE);
	progressRightImg.SetPosition(-baroffset, 40);
	progressRightImg.SetVisible(false);

	GuiImage throbberImg(throbber);
	throbberImg.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	throbberImg.SetPosition(0, 40);

	GuiText titleTxt(title, 28, (GXColor){255, 255, 255, 255});
	titleTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	titleTxt.SetPosition(0,18);
	GuiText msgTxt(msg, 20, (GXColor){255, 255, 255, 255});
	msgTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	msgTxt.SetPosition(0, 80);

	GuiText cancelBtnTxt("Cancel", 20, (GXColor){255, 255, 255, 255});
	GuiImage cancelBtnImg(&btnOutline);
	GuiImage cancelBtnImgOver(&btnOutlineOver);
	GuiButton cancelBtn(btnOutline.GetWidth(), btnOutline.GetHeight());

	cancelBtn.SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	cancelBtn.SetPosition(0, -20);
	cancelBtn.SetLabel(&cancelBtnTxt);
	cancelBtn.SetImage(&cancelBtnImg);
	cancelBtn.SetImageOver(&cancelBtnImgOver);
	cancelBtn.SetTrigger(trigA);
	cancelBtn.SetSelectable(false);
	cancelBtn.SetEffectGrow();
	cancelBtn.SetUpdateCallback(progressCancelCallback);
	
	promptWindow.Append(&dialogBoxImg);
	promptWindow.Append(&titleTxt);
	promptWindow.Append(&msgTxt);
	
	if(progressCancelCallback)
	{
		msgTxt.SetPosition(0, 60);
		throbberImg.SetPosition(0, 10);
		promptWindow.Append(&cancelBtn);
	}

	if(showProgress == 1)
	{
		promptWindow.Append(&progressEmptyImg);
		promptWindow.Append(&progressLeftImg);
		promptWindow.Append(&progressMidImg);
		promptWindow.Append(&progressLineImg);
		promptWindow.Append(&progressRightImg);
	}
	else
	{
		promptWindow.Append(&throbberImg);
	}
	
	// wait to see if progress flag changes soon
	progsleep = 400000;

	while(progsleep > 0)
	{
		if(!showProgress)
			break;
		usleep(THREAD_SLEEP);
		progsleep -= THREAD_SLEEP;
	}

	if(!showProgress || progressThreadHalt > 0)
		return;

	SuspendGui();
	int oldState = mainWindow->GetState();
	bool isDisabled = mainWindow->Find(disabled);
	mainWindow->SetState(STATE_DISABLED);
	if(!isDisabled)
		mainWindow->Append(disabled);
	mainWindow->Append(&promptWindow);
	ResumeGui();

	float angle = 0;
	u32 count = 0;
	float done = 0;
	int tile = 0;
	int maxtile = (progressEmpty->GetWidth()-16)/4;

	while(showProgress && progressThreadHalt == 0)
	{
		progsleep = 20000;

		while(progsleep > 0)
		{
			if(!showProgress)
				break;
			usleep(THREAD_SLEEP);
			progsleep -= THREAD_SLEEP;
		}

		if(showProgress == 1)
		{
			done = progressDone/(float)progressTotal;

			if(done > 0.02)
			{
				progressLeftImg.SetVisible(true);
				tile = (maxtile+2)*(done-0.02);
				if(tile > maxtile) tile = maxtile;
				progressMidImg.SetTile(tile);
				progressLineImg.SetPosition(baroffset+8 + tile*4, 40);
				progressLineImg.SetVisible(true);
			}
			
			if(tile == maxtile)
			{
				progressLineImg.SetVisible(false);
				progressRightImg.SetVisible(true);
			}
		}
		else if(showProgress == 2)
		{
			if(count % 5 == 0)
			{
				angle+=45.0f;
				if(angle >= 360.0f)
					angle = 0;
				throbberImg.SetAngle(angle);
			}
			++count;
		}
	}

	SuspendGui();
	mainWindow->Remove(&promptWindow);
	if(!isDisabled)
		mainWindow->Remove(disabled);
	mainWindow->SetState(oldState);
	ResumeGui();
}

static void *ProgressThread (void *arg)
{
	while(1)
	{
		if(progressThreadHalt == 1)
			LWP_SuspendThread (progressthread);
		if(progressThreadHalt == 2)
			return NULL;

		ProgressWindow(progressTitle, progressMsg);
		usleep(THREAD_SLEEP);
	}
	return NULL;
}

/****************************************************************************
 * CancelAction
 *
 * Signals the GUI progress window thread to halt, and waits for it to
 * finish. Prevents multiple progress window events from interfering /
 * overriding each other.
 ***************************************************************************/
void
CancelAction()
{
	progressThreadHalt = 1;
	showProgress = 0;
	progressCancelCallback = NULL;

	if(progressthread == LWP_THREAD_NULL)
		return;

	// wait for thread to finish
	while(!LWP_ThreadIsSuspended(progressthread))
		usleep(THREAD_SLEEP);
}

/****************************************************************************
 * ShowProgress
 *
 * Updates the variables used by the progress window for drawing a progress
 * bar. Also resumes the progress window thread if it is suspended.
 ***************************************************************************/
extern "C" {
void
ShowProgress (const char *msg, int done, int total)
{
	if(progressthread == LWP_THREAD_NULL || guiShutdown)
		return;

	if(!mainWindow || ExitRequested)
		return;

	if(total <= 0 || done < 0) // invalid values
		return;

	if(done > total) // this shouldn't happen
		done = total;

	if(showProgress != 1)
		CancelAction(); // wait for previous progress window to finish

	snprintf(progressMsg, 200, "%s", gettext(msg));
	sprintf(progressTitle, "Please Wait");
	progressThreadHalt = 0;
	showProgress = 1;
	progressTotal = total;
	progressDone = done;

	LWP_ResumeThread (progressthread);
}
}
/****************************************************************************
 * ShowAction
 *
 * Shows that an action is underway. Also resumes the progress window thread
 * if it is suspended.
 ***************************************************************************/
void
ShowAction (const char *msg, UpdateCallback cancelCallback)
{
	if(!mainWindow || ExitRequested)
		return;

	if(progressthread == LWP_THREAD_NULL || guiShutdown)
		return;

	if(showProgress != 2)
		CancelAction(); // wait for previous progress window to finish

	snprintf(progressMsg, 200, "%s", gettext(msg));
	sprintf(progressTitle, "Please Wait");
	progressThreadHalt = 0;
	showProgress = 2;
	progressDone = 0;
	progressTotal = 0;
	progressCancelCallback = cancelCallback;
		
	LWP_ResumeThread (progressthread);
}

void
ShowAction (const char *msg)
{
	ShowAction (msg, NULL);
}

void ErrorPrompt(const char *msg)
{
	WindowPrompt("Error", gettext(msg), "OK", NULL);
}

int ErrorPromptRetry(const char *msg)
{
	return WindowPrompt("Error", gettext(msg), "Retry", "Cancel");
}

void ErrorPrompt(wchar_t *msg)
{
	WindowPrompt("Error", msg, "OK", NULL);
}

int ErrorPromptRetry(wchar_t *msg)
{
	return WindowPrompt("Error", msg, "Retry", "Cancel");
}

void InfoPrompt(const char *msg)
{
	WindowPrompt("Information", gettext(msg), "OK", NULL);
}

void InfoPrompt(const char *title, const char *msg)
{
	WindowPrompt(title, gettext(msg), "OK", NULL);
}

void InfoPrompt(wchar_t *msg)
{
	WindowPrompt("Information", msg, "OK", NULL);
}

void InfoPrompt(const char *title, wchar_t *msg)
{
	WindowPrompt(title, msg, "OK", NULL);
}

/****************************************************************************
 * OnScreenKeyboard
 *
 * Opens an on-screen keyboard window, with the data entered being stored
 * into the specified variable.
 ***************************************************************************/
bool OnScreenKeyboard(char *var, u32 maxlen)
{
	int save = -1;

	GuiKeyboard keyboard(var, maxlen);

	GuiImageData btnOutline(button_png);
	GuiImageData btnOutlineOver(button_over_png);

	GuiText okBtnTxt("OK", 20, (GXColor){255, 255, 255, 255});
	GuiImage okBtnImg(&btnOutline);
	GuiImage okBtnImgOver(&btnOutlineOver);
	GuiButton okBtn(btnOutline.GetWidth(), btnOutline.GetHeight());

	okBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	okBtn.SetPosition(25, -25);

	okBtn.SetLabel(&okBtnTxt);
	okBtn.SetImage(&okBtnImg);
	okBtn.SetImageOver(&okBtnImgOver);
	okBtn.SetTrigger(trigA);
	okBtn.SetEffectGrow();

	GuiText cancelBtnTxt("Cancel", 20, (GXColor){255, 255, 255, 255});
	GuiImage cancelBtnImg(&btnOutline);
	GuiImage cancelBtnImgOver(&btnOutlineOver);
	GuiButton cancelBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	cancelBtn.SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	cancelBtn.SetPosition(-25, -25);
	cancelBtn.SetLabel(&cancelBtnTxt);
	cancelBtn.SetImage(&cancelBtnImg);
	cancelBtn.SetImageOver(&cancelBtnImgOver);
	cancelBtn.SetTrigger(trigA);
	cancelBtn.SetEffectGrow();

	keyboard.Append(&okBtn);
	keyboard.Append(&cancelBtn);

	SuspendGui();
	mainWindow->SetState(STATE_DISABLED);
	mainWindow->Append(disabled);
	mainWindow->Append(&keyboard);
	ResumeGui();

	while(save == -1)
	{
		usleep(THREAD_SLEEP);

		if(okBtn.GetState() == STATE_CLICKED)
			save = 1;
		else if(cancelBtn.GetState() == STATE_CLICKED)
			save = 0;
	}

	if(save)
		snprintf(var, maxlen+1, "%s", keyboard.kbtextstr);

	SuspendGui();
	mainWindow->Remove(&keyboard);
	mainWindow->Remove(disabled);
	mainWindow->SetState(STATE_DEFAULT);
	ResumeGui();

	return save;
}

bool OnScreenKeypad(char *var, u32 maxlen, bool enableNegative = false)
{
	int save = -1;

	GuiKeypad keypad(var, maxlen);
	
	if(enableNegative)
		keypad.EnableNegative();

	GuiImageData btnOutline(button_png);
	GuiImageData btnOutlineOver(button_over_png);

	GuiText okBtnTxt("OK", 20, (GXColor){255, 255, 255, 255});
	GuiImage okBtnImg(&btnOutline);
	GuiImage okBtnImgOver(&btnOutlineOver);
	GuiButton okBtn(btnOutline.GetWidth(), btnOutline.GetHeight());

	okBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	okBtn.SetPosition(25, -25);

	okBtn.SetLabel(&okBtnTxt);
	okBtn.SetImage(&okBtnImg);
	okBtn.SetImageOver(&okBtnImgOver);
	okBtn.SetTrigger(trigA);
	okBtn.SetEffectGrow();

	GuiText cancelBtnTxt("Cancel", 20, (GXColor){255, 255, 255, 255});
	GuiImage cancelBtnImg(&btnOutline);
	GuiImage cancelBtnImgOver(&btnOutlineOver);
	GuiButton cancelBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	cancelBtn.SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	cancelBtn.SetPosition(-25, -25);
	cancelBtn.SetLabel(&cancelBtnTxt);
	cancelBtn.SetImage(&cancelBtnImg);
	cancelBtn.SetImageOver(&cancelBtnImgOver);
	cancelBtn.SetTrigger(trigA);
	cancelBtn.SetEffectGrow();

	keypad.Append(&okBtn);
	keypad.Append(&cancelBtn);

	SuspendGui();
	mainWindow->SetState(STATE_DISABLED);
	mainWindow->Append(disabled);
	mainWindow->Append(&keypad);
	ResumeGui();

	while(save == -1)
	{
		usleep(THREAD_SLEEP);

		if(okBtn.GetState() == STATE_CLICKED)
			save = 1;
		else if(cancelBtn.GetState() == STATE_CLICKED)
			save = 0;
	}

	if(save)
		snprintf(var, maxlen+1, "%s", keypad.kptextstr);

	SuspendGui();
	mainWindow->Remove(&keypad);
	mainWindow->Remove(disabled);
	mainWindow->SetState(STATE_DEFAULT);
	ResumeGui();

	return save;
}

/****************************************************************************
 * SettingWindow
 *
 * Opens a new window, with the specified window element appended. Allows
 * for a customizable prompted setting.
 ***************************************************************************/
static int
SettingWindow(const char *title, GuiWindow *w)
{
	int save = -1;

	GuiWindow promptWindow(556,244);
	promptWindow.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	GuiImageData btnOutline(button_png);
	GuiImageData btnOutlineOver(button_over_png);

	GuiImageData dialogBox(dialogue_box_png);
	GuiImage dialogBoxImg(&dialogBox);
	dialogBoxImg.SetAlpha(220);

	GuiText titleTxt(title, 28, (GXColor){255, 255, 255, 255});
	titleTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	titleTxt.SetPosition(0,18);

	GuiText okBtnTxt("OK", 20, (GXColor){255, 255, 255, 255});
	GuiImage okBtnImg(&btnOutline);
	GuiImage okBtnImgOver(&btnOutlineOver);
	GuiButton okBtn(btnOutline.GetWidth(), btnOutline.GetHeight());

	okBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	okBtn.SetPosition(20, -25);

	okBtn.SetLabel(&okBtnTxt);
	okBtn.SetImage(&okBtnImg);
	okBtn.SetImageOver(&okBtnImgOver);
	okBtn.SetTrigger(trigA);
	okBtn.SetSelectable(false);
	okBtn.SetEffectGrow();

	GuiText cancelBtnTxt("Cancel", 20, (GXColor){255, 255, 255, 255});
	GuiImage cancelBtnImg(&btnOutline);
	GuiImage cancelBtnImgOver(&btnOutlineOver);
	GuiButton cancelBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	cancelBtn.SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	cancelBtn.SetPosition(-20, -25);
	cancelBtn.SetLabel(&cancelBtnTxt);
	cancelBtn.SetImage(&cancelBtnImg);
	cancelBtn.SetImageOver(&cancelBtnImgOver);
	cancelBtn.SetTrigger(trigA);
	cancelBtn.SetSelectable(false);
	cancelBtn.SetEffectGrow();

	promptWindow.Append(&dialogBoxImg);
	promptWindow.Append(&titleTxt);
	promptWindow.Append(&okBtn);
	promptWindow.Append(&cancelBtn);

	SuspendGui();
	mainWindow->SetState(STATE_DISABLED);
	mainWindow->Append(disabled);
	mainWindow->Append(&promptWindow);
	mainWindow->Append(w);
	ResumeGui();

	while(save == -1)
	{
		usleep(THREAD_SLEEP);

		if(okBtn.GetState() == STATE_CLICKED)
			save = 1;
		else if(cancelBtn.GetState() == STATE_CLICKED)
			save = 0;
	}
	SuspendGui();
	mainWindow->Remove(&promptWindow);
	mainWindow->Remove(w);
	mainWindow->Remove(disabled);
	mainWindow->SetState(STATE_DEFAULT);
	ResumeGui();
	return save;
}

/****************************************************************************
 * CreditsWindow
 * Display credits, legal copyright and licence
 *
 * THIS MUST NOT BE REMOVED OR DISABLED IN ANY DERIVATIVE WORK
 ***************************************************************************/

static void CreditsWindow()
{
	int i = 0;
	int y = 76;

	GuiWindow *oldWindow = mainWindow;
	GuiWindow creditsWindow(screenwidth, screenheight);

	creditsWindow.Append(bgImg);

	GuiImage logoImg(logo);
	logoImg.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	logoImg.SetPosition(0, 30);
	
	creditsWindow.Append(&logoImg);

	GuiWindow alignWindow(0, screenheight);
	alignWindow.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	creditsWindow.Append(&alignWindow);

	int numEntries = 15;
	GuiText *txt[numEntries];

	wchar_t appVersion[20];
	swprintf(appVersion, 20, L"%s %s", gettext("Version"), APPVERSION);

	char iosVersion[30];
	sprintf(iosVersion, "IOS: %d", IOS_GetVersion());

	txt[i] = new GuiText(NULL, 16, (GXColor){255, 255, 255, 255});
	txt[i]->SetWText(appVersion);
	txt[i]->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	txt[i]->SetPosition((screenwidth/2)-45,30); i++;

	txt[i] = new GuiText(iosVersion, 16, (GXColor){255, 255, 255, 255});
	txt[i]->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	txt[i]->SetPosition((screenwidth/2)-45,56); i++;

	txt[i] = new GuiText(" ", 16, (GXColor){255, 255, 255, 255});
	txt[i]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	txt[i]->SetPosition(0,y); i++; y+=44;

	txt[i] = new GuiText("Programming", 20, (GXColor){160, 160, 160, 255});
	txt[i]->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	txt[i]->SetPosition(-15,y); i++;
	txt[i] = new GuiText("Daigo 'Tengensei'", 20, (GXColor){255, 255, 255, 255});
	txt[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	txt[i]->SetPosition(15,y); i++; y+=26;

	txt[i] = new GuiText("& menu design", 20, (GXColor){160, 160, 160, 255});
	txt[i]->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	txt[i]->SetPosition(-15,y); i++;
	txt[i] = new GuiText("Tantric & rodries", 20, (GXColor){255, 255, 255, 255});
	txt[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	txt[i]->SetPosition(15,y); i++; y+=26;
	
	txt[i] = new GuiText("Menu artwork", 20, (GXColor){160, 160, 160, 255});
	txt[i]->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	txt[i]->SetPosition(-15,y); i++;
	txt[i] = new GuiText("Sam Mularczyk", 20, (GXColor){255, 255, 255, 255});
	txt[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	txt[i]->SetPosition(15,y); i++; y+=26;
	
	txt[i] = new GuiText("Logo", 20, (GXColor){160, 160, 160, 255});
	txt[i]->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	txt[i]->SetPosition(-15,y); i++;
	txt[i] = new GuiText("Psyche & drmr", 20, (GXColor){255, 255, 255, 255});
	txt[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	txt[i]->SetPosition(15,y); i++; y+=44;

	txt[i] = new GuiText("Thanks to", 20, (GXColor){160, 160, 160, 255});
	txt[i]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	txt[i]->SetPosition(0,y); i++; y+=36;

	txt[i] = new GuiText("Yuji Naka", 16, (GXColor){255, 255, 255, 255});
	txt[i]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	txt[i]->SetPosition(0,y); i++; y+=20;

	txt[i] = new GuiText("Makoto Raiku", 16, (GXColor){255, 255, 255, 255});
	txt[i]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	txt[i]->SetPosition(0,y); i++; y+=44;

	txt[i] = new GuiText("This is a private build of WiiMC.", 14, (GXColor){160, 160, 160, 255});
	txt[i]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	txt[i]->SetPosition(0,y);
	txt[i]->SetWrap(true, 500);

	for(i=0; i < numEntries; i++)
		alignWindow.Append(txt[i]);

	GuiText backBtnTxt("Go back", 18, (GXColor){255, 255, 255, 255});
	backBtnTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnTxt.SetPosition(-6, 10);
	GuiImage backBtnImg(btnBottom);
	GuiImage backBtnImgOver(btnBottomOver);
	GuiImage backBtnArrow(arrowRightSmall);
	backBtnArrow.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnArrow.SetPosition(backBtnTxt.GetTextWidth()/2 + 6, 11);
	GuiButton backBtn(screenwidth, btnBottom->GetHeight());
	backBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	backBtn.SetPosition(0, 0);
	backBtn.SetLabel(&backBtnTxt);
	backBtn.SetImage(&backBtnImg);
	backBtn.SetImageOver(&backBtnImgOver);
	backBtn.SetIcon(&backBtnArrow);
	backBtn.SetTrigger(trigA);
	backBtn.SetTrigger(trigB);
	
	creditsWindow.Append(&backBtn);

	SuspendGui();
	mainWindow = &creditsWindow;
	ResumeGui();
	
	while(creditsThreadHalt == 0)
	{
		usleep(THREAD_SLEEP);
		
		if(backBtn.GetState() == STATE_CLICKED)
			break;
	}

	SuspendGui();
	mainWindow = oldWindow;
	ResumeGui();
	
	for(i=0; i < numEntries; i++)
		delete txt[i];
}

static void *CreditsThread(void *arg)
{
	while(1)
	{
		LWP_SuspendThread (creditsthread);

		if(creditsThreadHalt == 2)
			break;

		CreditsWindow();
		usleep(THREAD_SLEEP);
	}
	return NULL;
}

static void ResumeCreditsThread()
{
	if(creditsthread == LWP_THREAD_NULL || guiShutdown)
		return;

	creditsThreadHalt = 0;
	LWP_ResumeThread(creditsthread);
}

static void DisplayCredits(void *ptr)
{
	if(logoBtn->GetState() != STATE_CLICKED)
		return;

	logoBtn->ResetState();
	ResumeCreditsThread();
}

static void UpdateAudiobarModeBtn()
{
	switch(WiiSettings.playOrder)
	{
		case PLAY_SINGLE:
			audiobarModeImg->SetImage(actionbarSingle);
			audiobarModeTip->SetText("Single Play");
			break;
		case PLAY_CONTINUOUS:
			audiobarModeImg->SetImage(actionbarContinuous);
			audiobarModeTip->SetText("Continuous Play");
			break;
		case PLAY_SHUFFLE:
			audiobarModeImg->SetImage(actionbarShuffle);
			audiobarModeTip->SetText("Shuffle");
			break;
		case PLAY_LOOP:
			audiobarModeImg->SetImage(actionbarLoop);
			audiobarModeTip->SetText("Loop");
			break;
		case PLAY_THROUGH:
			audiobarModeImg->SetImage(actionbarThrough);
			audiobarModeTip->SetText("Through");
			break;
	}
}

void DisableVideoImg()
{
	if(!videoImg)
		return;

	videoImg->SetVisible(false);
}

void EnableVideoImg()
{
	if(!videoImg)
		return;

	videoImg->SetScaleY(screenheight/(float)vmode->efbHeight);
	videoImg->SetVisible(true);
}

bool VideoImgVisible()
{
	if(!videoImg)
		return false;

	return videoImg->IsVisible();
}

/****************************************************************************
 * MenuBrowse
 ***************************************************************************/

static GuiImage *thumbImg;
static BROWSERENTRY *thumbIndex = NULL;
static bool thumbLoad = false;

static void *ThumbThread (void *arg)
{
	GuiImageData *thumb = NULL;
	char *thumbBuffer = (char *)mem2_malloc(200*1024, MEM2_OTHER);
	thumbLoad = false;
	thumbThreadHalt = 0;

	while(!thumbThreadHalt)
	{
		if(thumbBuffer && thumbLoad)
		{
			thumbLoad = false;

			SuspendGui();
			thumbImg->SetVisible(false);
			thumbImg->SetImage(NULL);
			ResumeGui();

			if(thumb)
			{
				delete thumb;
				thumb = NULL;
			}

			if(thumbIndex)
			{
				BROWSERENTRY *loadIndex = thumbIndex;
				int read = 0;
				if(loadIndex->image && strncmp(loadIndex->image, "http:", 5) == 0)
					read = http_request(loadIndex->image, NULL, thumbBuffer, 200*1024, SILENT);
				else
					read = LoadFile(thumbBuffer, 200*1024, loadIndex->image, SILENT);
	
				if(read > 0 && loadIndex == thumbIndex) // file loaded and index has not changed
				{
					thumb = new GuiImageData((u8 *)thumbBuffer, read, GX_TF_RGBA8);

					if(thumb->GetImage())
					{
						SuspendGui();
						thumbImg->SetImage(thumb);
						thumbImg->SetScale(185, screenheight-100);
						thumbImg->SetVisible(true);
						ResumeGui();
					}
					else
					{
						delete thumb;
						thumb = NULL;
					}
				}
			}
		}
		usleep(THREAD_SLEEP);
	}
	if(thumb) delete thumb;
	if(thumbBuffer) mem2_free(thumbBuffer, MEM2_OTHER);
	return NULL;
}

static int LoadNewFile()
{
	if(!ChangeInterface(loadedFile, NOTSILENT))
		return 0;

	ResetVideos();
	DisableVideoImg();
	LoadMPlayerFile();

	audiobarNowPlaying[1]->SetText(NULL);
	audiobarNowPlaying[2]->SetText(NULL);
	audiobarNowPlaying[3]->SetText(NULL);

	// wait until MPlayer is ready to take control (or return control)
	while(!guiShutdown && controlledbygui != 1)
		usleep(THREAD_SLEEP);

	if(guiShutdown)
	{
		browserMusic.selIndex = NULL;
		PopulateVideoPlaylist();
		return 1; // playing a video
	}

	// failed or we are playing audio
	ResumeDeviceThread();

	if(!wiiAudioOnly())
	{
		browserMusic.selIndex = NULL;

		if(wii_error == 1)
			ErrorPrompt("Resolution exceeds maximum allowed (1024x720)!");
		else
			ErrorPrompt("Error loading file!");

		return 0;
	}

	// we are playing audio

	// clear any play icons
	BROWSERENTRY *i = browser.first;
	while(i)
	{
		if(i->icon == ICON_PLAY)
		{
			if(menuCurrent == MENU_BROWSE_ONLINEMEDIA)
				i->icon = ICON_NONE;
			else if(MusicPlaylistFind(i))
				i->icon = ICON_FILE_CHECKED;
			else
				i->icon = ICON_FILE;
		}
		i = i->next;
	}

	FindFile(); // try to find this file
	findLoadedFile = 2; // trigger browser to update

	wiiSetVolume(WiiSettings.volume);

	if(wiiIsPaused())
		wiiPause(); // unpause playback

	// update the audio bar
	if(wiiGetTimeLength() > 0)
	{
		audiobarProgressBtn->SetVisible(true);
		audiobarProgressBtn->SetState(STATE_DEFAULT);
		audiobarProgressMidImg->SetVisible(true);
	}
	else // this is a stream - hide progress bar
	{
		audiobarProgressBtn->SetVisible(false);
		audiobarProgressBtn->SetState(STATE_DISABLED);
		audiobarProgressLeftImg->SetVisible(false);
		audiobarProgressMidImg->SetVisible(false);
		audiobarProgressLineImg->SetVisible(false);
		audiobarProgressRightImg->SetVisible(false);
	}
	return 2; // playing audio
}

static void HideAudioVolumeLevelBar();

/*bool LoadYouTubeFile(char *url, char *newurl)
{
	if(!ChangeInterface(DEVICE_INTERNET,0,NOTSILENT))
		return false;

	char *buffer = (char *)mem2_malloc(128*1024, MEM2_OTHER);

	if(!buffer)
		return false;
		
	int size = http_request(url, NULL, buffer, (128*1024), SILENT);

	if(size <= 0)
	{
		mem2_free(buffer, MEM2_OTHER);
		return false;
	}
	
	buffer[size-1] = 0;
	char *str = strstr(buffer, "url_encoded_fmt_stream_map");

	if(str == NULL)
	{
		mem2_free(buffer, MEM2_OTHER);
		return false;
	}

	int fmt, chosenFormat = 0;
	char *urlc, *urlcod, *urlcend, *fmtc, *fmtcend;
	char format[5];
	
	// get start point
	urlc = str+30;
	
	// get end point
	char *strend = strstr(urlc,"\"");
	
	if(strend == NULL)
	{
		mem2_free(buffer, MEM2_OTHER);
		return false;
	}
	
	strend[0] = 0; //terminate the string
	
	// work through the string looking for required format
	while(chosenFormat != WiiSettings.youtubeFormat && urlc < strend-10)
	{
		//find section end ,
		//and set pointer to next section
		urlcend = strstr(urlc, ",");
	    char *nexturl = urlcend + 1;
		if(!urlcend) urlcend = strend; // last section
		
		snprintf(newurl,urlcend-urlc+1,"%s",urlc); //get section with 0

		// remove 3 levels of url codes
		url_unescape_string(newurl, newurl); // %252526 = %2526
		url_unescape_string(newurl, newurl); // %2526 = %26
		url_unescape_string(newurl, newurl); // %26 = &
		
		char *sig;
		int siglen;
		
		while (1)
		{
			// replace \u0026 with &
			sig = strstr(newurl,"u0026");
			if (!sig) break;
			siglen = strlen(newurl)-(sig+5-newurl)+1;
			if(siglen <= 0) break;
			memmove(sig,sig+5,siglen);
			memmove(sig-1,"&",1);
		}
		
		//get format code of this section
		fmtc = strstr(newurl, "itag="); // find itag= within section
		if(!fmtc) break;
		
		fmtcend = strstr(fmtc+5,"&");  // delimited by next & tag within url
		if(!fmtcend) break;
		
		snprintf(format, fmtcend-fmtc-5+1, "%s", fmtc+5);
		
		fmt = atoi(format);

		if((fmt == 5 || fmt == 18 || fmt == 35) && fmt <= WiiSettings.youtubeFormat && fmt > chosenFormat)
		{
			// section is decoded in newurl
			urlcod = strstr(newurl,"url="); //find start of url in urlc
			if(!urlcod) break;
			
			// put start of section back in urlc
			snprintf(urlc,urlcod-newurl+1,"%s",newurl); //includes & before url= 
			
			// shift end of section after url= to newline with & on end
			strcpy(newurl,urlcod+4);
			strcat(newurl,"&");
			
			// add start of section to end of section
			strcat(newurl,urlc);
			
			// expand signature   http://xxx&parm=xxx&sig=xxx&parm=xxx&0
			sig = strstr(newurl,"&sig=");  // = 19
			// get length of shift data including = and/0
			siglen = strlen(newurl)-(sig-newurl)-3; // =37-(19-0)-3= 15   =xxx&parm=xxx&0
			// shift signature data within newurl  
			memmove(sig+10,sig+4,siglen);   // make 6 spaces                 http://xxx&parm=xxx&sig......=xxx&parm=xxx&0
			memmove(sig,"&signature=",11);  // insert &signature= in newurl  http://xxx&parm=xxx&signature=xxx&parm=xxx&0
			
			// remove &type=
			sig = strstr(newurl,"&type=");    //http://xxx&type=xxx&signature=xxx&parm=xxx&0 =  10
			char *sigend = strstr(sig+6,"&"); // = 19
			siglen = strlen(newurl)-(sigend-newurl)+1; // get length of shift data including /0 = 43-(19-0)+1=25
			memmove(sig,sigend,siglen);     // shift data within newurl  http://xxx&type=xxx&signature=xxx&parm=xxx&0
			
			// remove &fallback_host=
			sig = strstr(newurl,"&fallback_host=");
			sigend = strstr(sig+15,"&");
			siglen = strlen(newurl)-(sigend-newurl)+1; // get length of shift data including /0
			memmove(sig,sigend,siglen);                // shift data within newurl

			// remove duplicate &itag=
			sig = strstr(newurl,"&itag=");
			sigend = strstr(sig+6,"&");
			siglen = strlen(newurl)-(sigend-newurl)+1; // get length of shift data including /0
			memmove(sig,sigend,siglen);                // shift data within newurl

			//remove last &    http://xxx&parm=xxx&parm=xxx&parm=xxx&0
			siglen = strlen(newurl);  //  =38
			newurl[siglen-1] = 0;  //  http://xxx&parm=xxx&sig=xxx&parm=xxx00

			chosenFormat = fmt;
		}
		urlc = nexturl; // do next section
	}
	
	mem2_free(buffer, MEM2_OTHER);

	if(chosenFormat > 0)
		return true;

	return false;
}*/

static GuiFileBrowser *fileBrowser = NULL;

void UpdateBrowser()
{
	if(!fileBrowser)
		return;
	
	fileBrowser->TriggerUpdate();
}

static void MenuBrowse(int menu)
{
	ShutoffRumble();
	ResetFiles();

	switch(menu)
	{
		case MENU_BROWSE_VIDEOS:
			strcpy(browser.dir, WiiSettings.videosFolder);
			break;
		case MENU_BROWSE_MUSIC:
			strcpy(browser.dir, WiiSettings.musicFolder);
			break;
		case MENU_BROWSE_ONLINEMEDIA:
			strcpy(browser.dir, WiiSettings.onlinemediaFolder);
			break;
		default:
			return;
	}

	browser.menu = menu;

	GuiTrigger trigPlus;
	trigPlus.SetButtonOnlyTrigger(-1, WPAD_BUTTON_PLUS | WPAD_CLASSIC_BUTTON_PLUS, PAD_BUTTON_Y);

	GuiTrigger trigMinus;
	trigMinus.SetButtonOnlyTrigger(-1, WPAD_BUTTON_MINUS | WPAD_CLASSIC_BUTTON_MINUS, PAD_BUTTON_X);

	GuiButton upOneLevelBtn(0,0);
	upOneLevelBtn.SetTrigger(trigB);
	upOneLevelBtn.SetSelectable(false);

	GuiText backBtnTxt("Resume", 18, (GXColor){255, 255, 255, 255});
	backBtnTxt.SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	backBtnTxt.SetPosition(-74, 10);
	GuiImage backBtnImg(btnBottom);
	GuiImage backBtnImgOver(btnBottomOver);
	GuiImage backBtnArrow(arrowRightSmall);
	backBtnArrow.SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	backBtnArrow.SetPosition(-54, 11);
	GuiButton backBtn(screenwidth, btnBottom->GetHeight());
	backBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	backBtn.SetPosition(0, 0);
	backBtn.SetLabel(&backBtnTxt);
	backBtn.SetImage(&backBtnImg);
	backBtn.SetImageOver(&backBtnImgOver);
	backBtn.SetIcon(&backBtnArrow);
	backBtn.SetTrigger(trigA);

	int play = -1;
	int pagesize = 11;
	char origname[1024]; // store original filename when performing searches

	if(VideoImgVisible() && menu != MENU_BROWSE_MUSIC)
		pagesize = 10;
	else if(menu == MENU_BROWSE_MUSIC || (menu == MENU_BROWSE_ONLINEMEDIA && wiiAudioOnly()))
		pagesize = 8;

	if(menu == MENU_BROWSE_ONLINEMEDIA)
	{
		fileBrowser = new GuiFileBrowser(screenwidth-200, pagesize);
		fileBrowser->SetRightCutoff();
		LWP_CreateThread (&thumbthread, ThumbThread, NULL, NULL, 0, 60);
	}
	else
	{
		fileBrowser = new GuiFileBrowser(screenwidth, pagesize);
	}

	fileBrowser->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	fileBrowser->SetPosition(0, 90);

	GuiButton playlistAddBtn(0, 0);
	playlistAddBtn.SetTrigger(&trigPlus);
	playlistAddBtn.SetSelectable(false);

	GuiButton playlistResetBtn(0, 0);
	playlistResetBtn.SetTrigger(&trigMinus);
	playlistResetBtn.SetSelectable(false);

	BROWSERENTRY *currentIndex = NULL;

	if(menu == MENU_BROWSE_ONLINEMEDIA)
	{
		if(!browserOnlineMedia.first)
		{
			// check if file exists
			struct stat buf;
			char filepath[1024];
			sprintf(filepath, "%s/onlinemedia.xml", appPath);

			if(stat(filepath, &buf) == 0)
				ErrorPrompt("Online media file is invalid.");
			else
				ErrorPrompt("Online media file not found.");

			if(menuPrevious == MENU_BROWSE_ONLINEMEDIA)
				menuPrevious = MENU_BROWSE_VIDEOS;
			UndoChangeMenu();
			goto done;
		}

		thumbImg = new GuiImage;
		thumbImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
		thumbImg->SetVisible(false);
		thumbImg->SetPosition(-30, 95);
		mainWindow->Append(thumbImg);
	}

	if(VideoImgVisible() && menu != MENU_BROWSE_MUSIC) // a video is loaded
	{
		if(!nowPlaying->IsVisible())
		{
			StripExt(loadedFileDisplay);
			nowPlaying->SetText(loadedFileDisplay);
			nowPlaying->SetVisible(true);
		}

		backBtn.SetLabel(nowPlaying, 1);
		mainWindow->Append(&backBtn);
	}
	ResumeGui();

	// populate initial directory listing

	// check if USB drive is not yet mounted - delay if not
	static bool firstLoad = true;
	if(firstLoad && strncmp(browser.dir, "usb", 3) == 0 && !ChangeInterface(browser.dir, false))
	{
		firstLoad = false; // only do this on initial application load
		
		// wait to see if USB mounts soon
		int progsleep = 400000;

		while(progsleep > 0 && !guiShutdown)
		{
			if(ChangeInterface(browser.dir, false))
				break;
			usleep(THREAD_SLEEP);
			progsleep -= THREAD_SLEEP;
		}

		// still not mounted - wait an additional 8 seconds
		if(!ChangeInterface(browser.dir, false))
		{
			mainWindow->Append(disabled);
			mainWindow->SetState(STATE_DISABLED);
			ShowAction("Loading...");
			u64 start = gettime();
			usleep(THREAD_SLEEP);

			while(diff_sec(start, gettime()) < 8 && !guiShutdown)
			{
				if(ChangeInterface(browser.dir, false))
					break;
				usleep(THREAD_SLEEP);
			}
		}
	}

	if(strncmp(browser.dir, "http:", 5) == 0)
	{
		mainWindow->Append(disabled);
		mainWindow->SetState(STATE_DISABLED);
		ShowAction("Loading...");
	}

	BrowserChangeFolder(false);

	if(mainWindow->Find(disabled))
	{
		CancelAction();
		mainWindow->Remove(disabled);
		mainWindow->SetState(STATE_DEFAULT);
	}

	if(findLoadedFile == 2)
	{
		findLoadedFile = 0;
		fileBrowser->TriggerUpdate();
	}

	SuspendGui();
	mainWindow->Append(fileBrowser);
	mainWindow->Append(&upOneLevelBtn);
	ResumeGui();

	if(menu == MENU_BROWSE_MUSIC || menu == MENU_BROWSE_ONLINEMEDIA)
	{
		SuspendGui();
		audiobar->SetState(STATE_DEFAULT);
		HideAudioVolumeLevelBar();

		if(menu == MENU_BROWSE_MUSIC) // add playlist functionality
		{
			audiobarPlaylistBtn->SetVisible(true);
			audiobarModeBtn->SetVisible(true);
			audiobarBackwardBtn->SetVisible(true);
			audiobarForwardBtn->SetVisible(true);
			audiobarPlaylistBtn->SetState(STATE_DEFAULT);
			audiobarModeBtn->SetState(STATE_DEFAULT);
			audiobarBackwardBtn->SetState(STATE_DEFAULT);
			audiobarForwardBtn->SetState(STATE_DEFAULT);
			mainWindow->Append(&playlistAddBtn);
			mainWindow->Append(&playlistResetBtn);
			UpdateAudiobarModeBtn();
			mainWindow->Append(audiobar);
		}
		else // hide playlist functionality for online media area
		{
			audiobarPlaylistBtn->SetVisible(false);
			audiobarModeBtn->SetVisible(false);
			audiobarBackwardBtn->SetVisible(false);
			audiobarForwardBtn->SetVisible(false);
			audiobarPlaylistBtn->SetState(STATE_DISABLED);
			audiobarModeBtn->SetState(STATE_DISABLED);
			audiobarBackwardBtn->SetState(STATE_DISABLED);
			audiobarForwardBtn->SetState(STATE_DISABLED);

			if(wiiAudioOnly())
				mainWindow->Append(audiobar);
		}
		
		if(wiiGetTimeLength() == 0)
		{
			audiobarProgressBtn->SetVisible(false);
			audiobarProgressBtn->SetState(STATE_DISABLED);
			audiobarProgressLeftImg->SetVisible(false);
			audiobarProgressMidImg->SetVisible(false);
		}
		ResumeGui();
	}

	while(menuCurrent == menu && !guiShutdown)
	{
		usleep(THREAD_SLEEP);

		if(findLoadedFile == 2)
		{
			findLoadedFile = 0;
			fileBrowser->TriggerUpdate();
		}

		// devices were inserted or removed - update the filebrowser!
		if(devicesChanged)
		{
			devicesChanged = false;

			// video is no longer loaded - remove back button
			if(pagesize == 10 && !VideoImgVisible())
			{
				pagesize = 11;
				SuspendGui();
				fileBrowser->ChangeSize(pagesize);
				mainWindow->Remove(&backBtn);
				ResumeGui();
			}

			if(!ChangeInterface(browser.dir, SILENT))
			{
				fileBrowser->SetState(STATE_DISABLED);
	
				if(!BrowserChangeFolder(false))
					goto done;
	
				fileBrowser->ResetState();
			}
		}

		// up one level
		if(upOneLevelBtn.GetState() == STATE_CLICKED)
		{
			upOneLevelBtn.ResetState();

			if(browser.selIndex && browser.selIndex->file && strncmp(browser.selIndex->file, "http:", 5) == 0)
			{
				mainWindow->Append(disabled);
				mainWindow->SetState(STATE_DISABLED);
				ShowAction("Loading...");
			}
			if(browser.dir[0] != 0)
			{
				fileBrowser->SetState(STATE_DISABLED);
				browser.selIndex = browser.first;
				if(!BrowserChangeFolder())
				{
					CancelAction();
					mainWindow->Remove(disabled);
					mainWindow->SetState(STATE_DEFAULT);
					goto done;
				}
			}
			fileBrowser->ResetState();
			CancelAction();
			mainWindow->Remove(disabled);
			mainWindow->SetState(STATE_DEFAULT);
		}

		// update file browser based on arrow buttons
		// request guiShutdown if A button pressed on a file
		for(int i=0; i<pagesize; i++)
		{
			if(fileBrowser->fileList[i]->GetState() == STATE_CLICKED)
			{
				fileBrowser->fileList[i]->ResetState();

				// check corresponding browser entry
				if(!browser.selIndex || !browser.selIndex->file || browser.selIndex->type == TYPE_FOLDER)
				{
					fileBrowser->SetState(STATE_DISABLED);

					if(!BrowserChangeFolder())
						goto done;

					fileBrowser->ResetState();
					continue;
				}

				if(browser.selIndex->type == TYPE_SEARCH)
				{
					char query[100] = {0};
					char escquery[100*3+1];
					OnScreenKeyboard(query, 100);

					if(query[0] == 0)
						continue;

					strcpy(origname, browser.selIndex->file); // save original URL
					url_escape_string(escquery, query); // escape the string for use in a URL

					// append query to search URL
					char tmp[1024];
					snprintf(tmp, 1024, "%s%s", browser.selIndex->file, escquery);
					mem2_free(browser.selIndex->file, MEM2_BROWSER);
					browser.selIndex->file = mem2_strdup(tmp, MEM2_BROWSER);
				}

				// this is a file
				char ext[7];
				GetExt(browser.selIndex->file, ext);
				int numItems = 0;

				/*if(strncmp(browser.selIndex->file, "http://www.youtube.", 19) == 0)
				{
					if(!mainWindow->Find(disabled))
						mainWindow->Append(disabled);
					mainWindow->SetState(STATE_DISABLED);
					ShowAction("Loading...");

					char url[MAXPATHLEN + 1];

					if(LoadYouTubeFile(browser.selIndex->file, url))
					{
						snprintf(loadedFileDisplay, 128, "%s", browser.selIndex->display);
						strcpy(loadedFile, url);
						int res = LoadNewFile();
						strcpy(loadedFile, browser.selIndex->file);
						CancelAction();

						if(res == 1) // loaded a video file
							goto done;
					}
					CancelAction();
					mainWindow->Remove(disabled);
					mainWindow->SetState(STATE_DEFAULT);
					ErrorPrompt("Error loading YouTube file!");
					continue;
				}*/

				// identified as a playlist, or file is an unrecognized audio or video extension or allowed protocol
				// parse as a playlist
				if(!IsAllowedExt(ext) && 
					(browser.selIndex->type == TYPE_PLAYLIST ||
					IsPlaylistExt(ext) || 
					!IsAllowedProtocol(browser.selIndex->file) || 
					(strncmp(browser.selIndex->file, "http:", 5) == 0 && !IsInternetStream(browser.selIndex->file)))
					)
				{
					if(strncmp(browser.selIndex->file, "http:", 5) == 0)
					{
						mainWindow->Append(disabled);
						mainWindow->SetState(STATE_DISABLED);
						ShowAction("Loading...");
					}

					fileBrowser->SetState(STATE_DISABLED);
					numItems = BrowserChangeFolder();

					if(numItems > 1)
					{
						char ext2[7];
						GetExt(browser.first->next->file, ext2);
						// let's load this one file
						if(numItems == 2 && IsPlaylistExt(ext) && !IsPlaylistExt(ext2)) 
						{
							sprintf(loadedFile, browser.first->next->file);
							GetDisplay(browser.first->next, loadedFileDisplay, 128);
							// go up one level
							browser.selIndex = 0;
							BrowserChangeFolder();
							fileBrowser->ResetState();
						}
						else
						{
							CancelAction();
							mainWindow->Remove(disabled);
							mainWindow->SetState(STATE_DEFAULT);
							fileBrowser->ResetState();
							continue;
						}
					}
					else if(browser.selIndex->type != TYPE_FILE)
					{
						CancelAction();
						mainWindow->Remove(disabled);
						mainWindow->SetState(STATE_DEFAULT);

						if(browser.selIndex->type == TYPE_SEARCH)
						{
							strcpy(browser.selIndex->file, origname); // restore original URL
							InfoPrompt("No Results Found", "Your search did not match any media files.");
						}
						fileBrowser->ResetState();
						continue;
					}
				}

				if(numItems == 0)
				{
					if(menuCurrent == MENU_BROWSE_VIDEOS) 
					{
						char aux[1024];
						GetFullPath(browser.selIndex, aux);
						if(strcmp(loadedFile, aux) == 0)
						{
							ResumeMPlayerFile();
							// wait until MPlayer is ready to take control (or return control)
							while(!guiShutdown && controlledbygui != 1)
								usleep(THREAD_SLEEP);

							goto done;
						}
					}

					GetFullPath(browser.selIndex, loadedFile);
					GetDisplay(browser.selIndex, loadedFileDisplay, 128);
				}

				browserMusic.selIndex = MusicPlaylistFindIndex(loadedFile);

				if(!mainWindow->Find(disabled))
					mainWindow->Append(disabled);
				mainWindow->SetState(STATE_DISABLED);

				ShowAction("Loading...");
				int res = LoadNewFile();
				CancelAction();

				if(res == 1) // loaded a video file
					goto done;

				mainWindow->Remove(disabled);
				mainWindow->SetState(STATE_DEFAULT);

				if(res == 2) // loaded an audio-only file
				{
					FindFile();

					// re-adjust for audio bar, if necessary
					if(pagesize != 8)
					{
						pagesize = 8;
						SuspendGui();
						fileBrowser->ChangeSize(pagesize);
						mainWindow->Remove(&backBtn);
						mainWindow->Append(audiobar);
						ResumeGui();
						break;
					}
				}
				else if(pagesize == 10 && !VideoImgVisible()) // video is no longer loaded
				{
					// remove back button
					pagesize = 11;
					SuspendGui();
					fileBrowser->ChangeSize(pagesize);
					mainWindow->Remove(&backBtn);
					ResumeGui();
					break;
				}
			}

			if(menu == MENU_BROWSE_ONLINEMEDIA && browser.selIndex != currentIndex)
			{
				currentIndex = browser.selIndex;
				thumbIndex = NULL;

				if(currentIndex && currentIndex->image)
				{
					char ext[7];
					GetExt(currentIndex->image, ext);
					if(IsImageExt(ext))	thumbIndex = browser.selIndex;
				}
				thumbLoad = true;
			}
		}

		if(backBtn.GetState() == STATE_CLICKED)
		{
			backBtn.ResetState();
			ResumeMPlayerFile(); // go back to MPlayer

			// wait until MPlayer is ready to take control (or return control)
			while(!guiShutdown && controlledbygui != 1)
				usleep(THREAD_SLEEP);
		}

		if(playlistAddBtn.GetState() == STATE_CLICKED)
		{
			playlistAddBtn.ResetState();
			BROWSERENTRY *addIndex = browser.selIndex;

			if(addIndex && addIndex != browser.first)
			{
				if(addIndex->icon == ICON_FILE_CHECKED || 
					addIndex->icon == ICON_FOLDER_CHECKED)
				{
					MusicPlaylistDequeue(addIndex);
				}
				else
				{
					ShowAction("Adding files to playlist...");
					MusicPlaylistEnqueue(addIndex);
					CancelAction();
				}
				fileBrowser->TriggerUpdate();
			}
		}

		if(playlistResetBtn.GetState() == STATE_CLICKED)
		{
			playlistResetBtn.ResetState();

			ResetMusic();

			if ((browser.dir[0] != 0)
			    && (strcmp(browser.first->display, "Exit Playlist") == 0))
			{
				fileBrowser->SetState(STATE_DISABLED);
				browser.selIndex = browser.first;
				if(!BrowserChangeFolder())
				{
					CancelAction();
					mainWindow->Remove(disabled);
					mainWindow->SetState(STATE_DEFAULT);
					goto done;
				}
			}
			fileBrowser->ResetState();
			CancelAction();
			mainWindow->Remove(disabled);
			mainWindow->SetState(STATE_DEFAULT);
		}

		if(!mainWindow->Find(audiobar))
			continue; // updating audio bar elements is not required

		if(audiobarPauseBtn->GetState() == STATE_CLICKED)
		{
			audiobarPauseBtn->ResetState();

			if(wiiAudioOnly())
			{
				wiiPause();
			}
			else if(browserMusic.numEntries > 0)
			{
				// start playlist
				if(!mainWindow->Find(disabled))
					mainWindow->Append(disabled);
				mainWindow->SetState(STATE_DISABLED);
				ShowAction("Loading...");

				FindNextFile(false);
				LoadNewFile();

				CancelAction();
				mainWindow->Remove(disabled);
				mainWindow->SetState(STATE_DEFAULT);
			}
		}

		if(!wiiIsPlaying() || wiiIsPaused() || !wiiAudioOnly())
		{
			if(play != 1)
			{
				audiobarPauseImg->SetImage(actionbarPlay);
				audiobarPauseTip->SetText("Play");
				play = 1;
			}
		}
		else
		{
			if(play != 0)
			{
				audiobarPauseImg->SetImage(actionbarPause);
				audiobarPauseTip->SetText("Pause");
				play = 0;
			}
		}

		if(wiiAudioOnly() || (browserMusic.numEntries > 0 && menu == MENU_BROWSE_MUSIC))
		{
			if(audiobarPauseBtn->GetAlpha() == 128 || audiobarPauseBtn->GetState() == STATE_DISABLED)
			{
				audiobarPauseBtn->SetState(STATE_DEFAULT);
				audiobarPauseBtn->SetAlpha(255);
			}
		}
		else
		{		
			if(audiobarPauseBtn->GetAlpha() == 255 || audiobarPauseBtn->GetState() != STATE_DISABLED)
			{
				audiobarPauseBtn->SetState(STATE_DISABLED);
				audiobarPauseBtn->SetAlpha(128);
			}
		}
		
		if(wiiAudioOnly())
		{
			if(wiiGetTimeLength() > 0)
			{
				if(audiobarProgressBtn->GetAlpha() == 128)
				{
					audiobarProgressBtn->SetState(STATE_DEFAULT);
					audiobarProgressBtn->SetAlpha(255);
				}
			}
			else
			{
				if(audiobarProgressBtn->GetState() != STATE_DISABLED)
				{
					audiobarProgressBtn->SetState(STATE_DISABLED);
					audiobarProgressBtn->SetAlpha(128);
					audiobarProgressLeftImg->SetVisible(false);
					audiobarProgressMidImg->SetTile(0);
					audiobarProgressLineImg->SetVisible(false);
					audiobarProgressRightImg->SetVisible(false);
				}
			}
		}
		else
		{
			if(audiobarProgressBtn->GetAlpha() == 255 || audiobarProgressBtn->GetState() != STATE_DISABLED)
			{
				audiobarProgressBtn->SetState(STATE_DISABLED);
				audiobarProgressBtn->SetAlpha(128);
				audiobarProgressLeftImg->SetVisible(false);
				audiobarProgressMidImg->SetTile(0);
				audiobarProgressLineImg->SetVisible(false);
				audiobarProgressRightImg->SetVisible(false);
			}
		}

		if(menu == MENU_BROWSE_ONLINEMEDIA)
			continue;

		if(audiobarPlaylistBtn->GetState() == STATE_CLICKED)
		{
			audiobarPlaylistBtn->ResetState();
			SuspendParseThread();
			MusicPlaylistLoad();
			FindFile();
			fileBrowser->TriggerUpdate();
		}

		if(audiobarBackwardBtn->GetState() == STATE_CLICKED)
		{
			audiobarBackwardBtn->ResetState();
			wiiSeekPos(0);
		}

		if(audiobarForwardBtn->GetState() == STATE_CLICKED)
		{
			audiobarForwardBtn->ResetState();

			if(browserMusic.numEntries > 0)
			{
				if(wiiAudioOnly())
				{
					StopMPlayerFile(); // end this song
					while(controlledbygui == 2) // wait for song to end
						usleep(THREAD_SLEEP);
					FindNextFile(true); // find next song
				}
				else
				{
					// start playlist
					if(!mainWindow->Find(disabled))
						mainWindow->Append(disabled);
					mainWindow->SetState(STATE_DISABLED);
					ShowAction("Loading...");

					FindNextFile(false);
					LoadNewFile();

					CancelAction();
					mainWindow->Remove(disabled);
					mainWindow->SetState(STATE_DEFAULT);
				}
			}
		}

		if(audiobarModeBtn->GetState() == STATE_CLICKED)
		{
			audiobarModeBtn->ResetState();
			WiiSettings.playOrder++;
			if(WiiSettings.playOrder > PLAY_THROUGH)
				WiiSettings.playOrder = PLAY_SINGLE;

			UpdateAudiobarModeBtn();
		}

		if(browserMusic.numEntries > 0)
		{
			if(browserMusic.numEntries == 1)
			{
				if(audiobarForwardBtn->GetAlpha() == 255 || audiobarForwardBtn->GetState() != STATE_DISABLED)
				{
					audiobarForwardBtn->SetState(STATE_DISABLED);
					audiobarForwardBtn->SetAlpha(128);
				}
			}
			else if(audiobarForwardBtn->GetAlpha() == 128)
			{
				audiobarForwardBtn->SetState(STATE_DEFAULT);
				audiobarForwardBtn->SetAlpha(255);
			}
		}
		else
		{
			if(audiobarForwardBtn->GetAlpha() == 255 || audiobarForwardBtn->GetState() != STATE_DISABLED)
			{
				audiobarForwardBtn->SetState(STATE_DISABLED);
				audiobarForwardBtn->SetAlpha(128);
			}
		}

		if(wiiAudioOnly())
		{
			audiobarNowPlayingBtn->SetState(STATE_DEFAULT);

			if(wiiGetTimeLength() > 0)
			{
				if(audiobarBackwardBtn->GetAlpha() == 128)
				{
					audiobarBackwardBtn->SetState(STATE_DEFAULT);
					audiobarBackwardBtn->SetAlpha(255);
				}
			}
			else
			{
				if(audiobarBackwardBtn->GetState() != STATE_DISABLED)
				{
					audiobarBackwardBtn->SetState(STATE_DISABLED);
					audiobarBackwardBtn->SetAlpha(128);
				}
			}
		}
		else
		{
			if(audiobarBackwardBtn->GetState() != STATE_DISABLED)
			{
				audiobarBackwardBtn->SetState(STATE_DISABLED);
				audiobarBackwardBtn->SetAlpha(128);
			}

			if(audiobarNowPlayingBtn->IsVisible())
				audiobarNowPlayingBtn->SetVisible(false);
		}
	}
done:
	SuspendParseThread(); // halt parsing
	SuspendGui();
	if(menu == MENU_BROWSE_ONLINEMEDIA)
	{
		thumbThreadHalt = 1;
		LWP_JoinThread(thumbthread, NULL);
		thumbthread = LWP_THREAD_NULL;
		mainWindow->Remove(thumbImg);
		delete thumbImg;
	}
	mainWindow->Remove(fileBrowser);
	mainWindow->Remove(&upOneLevelBtn);
	delete fileBrowser;
	fileBrowser = NULL;

	if(VideoImgVisible())
		mainWindow->Remove(&backBtn);

	if(menu == MENU_BROWSE_MUSIC || menu == MENU_BROWSE_ONLINEMEDIA)
		mainWindow->Remove(audiobar);

	if(menu == MENU_BROWSE_MUSIC) // remove playlist functionality
	{
		mainWindow->Remove(&playlistAddBtn);
		mainWindow->Remove(&playlistResetBtn);
	}

	if(guiShutdown)
		SaveFolder();
}

static void MenuDVD()
{
	ResumeGui();
	int device = -1;

	if(USBStorage_IsDVD())
	{
		if (ChangeInterface(DEVICE_DVD, -1, SILENT))
			device = DEVICE_DVD;
		else
			device = DEVICE_USB;
	}
	else if (ChangeInterface(DEVICE_DVD, -1, NOTSILENT))
	{
		device = DEVICE_DVD;
	}

	if (device == -1)
	{
		if(menuPrevious == MENU_DVD)
			menuPrevious = MENU_BROWSE_VIDEOS;
		UndoChangeMenu(); // go back to last menu
		return;
	}

	if(!wiiPlayingDVD())
	{
		if(WiiSettings.dvdMenu)
			sprintf(loadedFile, "dvdnav://");
		else
			sprintf(loadedFile, "dvd://");

		if (device == DEVICE_DVD)
			sprintf(loadedDevice, "/dev/di");
		else
			sprintf(loadedDevice, "/dev/usb");

		sprintf(loadedFileDisplay, "DVD");
		mainWindow->SetState(STATE_DISABLED);
		mainWindow->Append(disabled);
		ShowAction("Loading...");
		ResetVideos();
		DisableVideoImg();
		LoadMPlayerFile();

		// wait until MPlayer is ready to take or return control
		while(!guiShutdown && controlledbygui != 1)
			usleep(THREAD_SLEEP);

		CancelAction();
		SuspendGui();

		if(!guiShutdown) // load failed
		{
			if(menuPrevious == MENU_DVD)
				menuPrevious = MENU_BROWSE_VIDEOS;
			UndoChangeMenu(); // go back to last menu
			mainWindow->Remove(disabled);
			mainWindow->SetState(STATE_DEFAULT);
			ErrorPrompt("Unrecognized DVD format!");
		}
	}
	else
	{
		GuiText backBtnTxt("Resume", 18, (GXColor){255, 255, 255, 255});
		backBtnTxt.SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
		backBtnTxt.SetPosition(-74, 10);
		GuiImage backBtnImg(btnBottom);
		GuiImage backBtnImgOver(btnBottomOver);
		GuiImage backBtnArrow(arrowRightSmall);
		backBtnArrow.SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
		backBtnArrow.SetPosition(-54, 11);
		GuiButton backBtn(screenwidth, btnBottom->GetHeight());
		backBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
		backBtn.SetPosition(0, 0);
		backBtn.SetLabel(&backBtnTxt);
		backBtn.SetImage(&backBtnImg);
		backBtn.SetImageOver(&backBtnImgOver);
		backBtn.SetIcon(&backBtnArrow);
		backBtn.SetTrigger(trigA);

		SuspendGui();
		mainWindow->Append(&backBtn);
		ResumeGui();

		while(menuCurrent == MENU_DVD && !guiShutdown)
		{
			if(backBtn.GetState() == STATE_CLICKED)
			{
				backBtn.ResetState();
				ResumeMPlayerFile(); // go back to MPlayer

				// wait until MPlayer is ready to take control (or return control)
				while(!guiShutdown && controlledbygui != 1)
					usleep(THREAD_SLEEP);

				break;
			}
			usleep(THREAD_SLEEP);
		}
		CancelAction();
		SuspendGui();
		mainWindow->Remove(&backBtn);
	}
}

static void MenuSettingsGlobal()
{
	int ret;
	int i = 0;
	bool firstRun = true;
	OptionList options;

	sprintf(options.name[i++], "Language");
	sprintf(options.name[i++], "Volume");
	sprintf(options.name[i++], "File Extensions");
	sprintf(options.name[i++], "Exit Action");
	sprintf(options.name[i++], "Wiimote Rumble");
	sprintf(options.name[i++], "Sleep Timer");
	sprintf(options.name[i++], "Screensaver Delay");
	sprintf(options.name[i++], "Inactivity Shutdown");
	sprintf(options.name[i++], "Browser Folders");
	sprintf(options.name[i++], "Starting Area");
	sprintf(options.name[i++], "Screen Burn-in Reduction");
	sprintf(options.name[i++], "Double Strike");

	options.length = i;

	for(i=0; i < options.length; i++)
	{
		options.value[i][0] = 0;
		options.icon[i] = 0;
	}

	GuiText titleTxt("Settings - Global", 28, (GXColor){255, 255, 255, 255});
	titleTxt.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	titleTxt.SetPosition(49, 100);

	GuiText backBtnTxt("Go back", 18, (GXColor){255, 255, 255, 255});
	backBtnTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnTxt.SetPosition(-6, 10);
	GuiImage backBtnImg(btnBottom);
	GuiImage backBtnImgOver(btnBottomOver);
	GuiImage backBtnArrow(arrowRightSmall);
	backBtnArrow.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnArrow.SetPosition(backBtnTxt.GetTextWidth()/2 + 6, 11);
	GuiButton backBtn(screenwidth, btnBottom->GetHeight());
	backBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	backBtn.SetPosition(0, 0);
	backBtn.SetLabel(&backBtnTxt);
	backBtn.SetImage(&backBtnImg);
	backBtn.SetImageOver(&backBtnImgOver);
	backBtn.SetIcon(&backBtnArrow);
	backBtn.SetTrigger(trigA);
	backBtn.SetTrigger(trigB);

	GuiOptionBrowser optionBrowser(screenwidth, 7, &options);
	optionBrowser.SetPosition(0, 150);
	optionBrowser.SetAlignment(ALIGN_LEFT, ALIGN_TOP);

	SuspendGui();
	GuiWindow w(screenwidth, screenheight);
	w.Append(&backBtn);
	mainWindow->Append(&optionBrowser);
	mainWindow->Append(&w);
	mainWindow->Append(&titleTxt);
	ResumeGui();

	int ssDelay[] = { 120, 300, 600, 900, 1800, 3600 };
	int ssNum = sizeof(ssDelay) / sizeof(int);
	int ssDelayNum = 1;

	for(i=0; i < ssNum; i++)
		if(WiiSettings.screensaverDelay == ssDelay[i])
			ssDelayNum = i;

	while(menuCurrent == MENU_SETTINGS_GLOBAL && !guiShutdown)
	{
		usleep(THREAD_SLEEP);

		ret = optionBrowser.GetClickedOption();

		switch (ret)
		{
			case 0:
				WiiSettings.language++;

				if(WiiSettings.language >= LANG_LENGTH)
					WiiSettings.language = 0;
				break;
			case 1:
				WiiSettings.volume += 10;
				if(WiiSettings.volume > 100)
					WiiSettings.volume = 0;
				wiiSetVolume(WiiSettings.volume);
				break;
			case 2:
				WiiSettings.hideExtensions ^= 1;
				break;
			case 3:
				WiiSettings.exitAction++;
				if(WiiSettings.exitAction > EXIT_LOADER)
					WiiSettings.exitAction = 0;
				break;
			case 4:
				WiiSettings.rumble ^= 1;
				break;
			case 5:
				ResetSleepTimer();
				WiiSettings.sleepTimer += 30;
				if(WiiSettings.sleepTimer > 180)
					WiiSettings.sleepTimer = 0;
				break;
			case 6:
				ssDelayNum++;
				if(ssDelayNum == ssNum) ssDelayNum = 0;
				WiiSettings.screensaverDelay = ssDelay[ssDelayNum];
				break;
			case 7:
				WiiSettings.inactivityShutdown++;
				if(WiiSettings.inactivityShutdown > 8)
					WiiSettings.inactivityShutdown = 0;
				break;
			case 8:
				WiiSettings.lockFolders ^= 1;
				break;
			case 9:
				WiiSettings.startArea++;
				if(WiiSettings.startArea > MENU_BROWSE_ONLINEMEDIA)
					WiiSettings.startArea = MENU_BROWSE_VIDEOS;
				if(WiiSettings.startArea == MENU_DVD && WiiSettings.dvdDisabled)
					WiiSettings.startArea++;
				break;
			case 10:
				WiiSettings.screenDim ^= 1;
				break;
			case 11:
				WiiSettings.doubleStrike ^= 1;
				if(WiiSettings.doubleStrike == 1) {
					SetDoubleStrike();
					WiiSettings.videoDf = 0; // Not necessary but if enabled when 240p gets turned off it will do nothing so reset it
				} else {
					SetDoubleStrikeOff();
					WiiSettings.videoDf = 0;
				}
				if(WiiSettings.viWidth == 1)
						SetVIscale();
					else
						SetVIscaleback();
				break;
		}

		if(ret >= 0 || firstRun)
		{
			firstRun = false;

			switch(WiiSettings.language)
			{
				//case LANG_JAPANESE:				sprintf(options.value[0], "Japanese"); break;
				case LANG_ENGLISH:				sprintf(options.value[0], "English"); break;
				//case LANG_GERMAN:				sprintf(options.value[0], "Deutsch"); break;
				//case LANG_FRENCH:				sprintf(options.value[0], "Français"); break;
				case LANG_SPANISH:				sprintf(options.value[0], "Español"); break;
				/*case LANG_ITALIAN:				sprintf(options.value[0], "Italiano"); break;
				case LANG_DUTCH:				sprintf(options.value[0], "Dutch"); break;
				case LANG_SIMP_CHINESE:			sprintf(options.value[0], "Chinese (Simplified)"); break;
				case LANG_TRAD_CHINESE:			sprintf(options.value[0], "Chinese (Traditional)"); break;*/
				case LANG_KOREAN:				sprintf(options.value[0], "Korean"); break;
				/*case LANG_ROMANIAN:				sprintf(options.value[0], "Romanian"); break;
				case LANG_ESTONIAN:				sprintf(options.value[0], "Estonian"); break;
				case LANG_PORTUGUESE: 			sprintf(options.value[0], "Portuguese"); break;
				case LANG_BRAZILIAN_PORTUGUESE: sprintf(options.value[0], "Brazilian Portuguese"); break;
				case LANG_HUNGARIAN:			sprintf(options.value[0], "Hungarian"); break;
				case LANG_POLISH:				sprintf(options.value[0], "Polish"); break;
				case LANG_RUSSIAN:				sprintf(options.value[0], "Russian"); break;
				case LANG_SLOVAK:				sprintf(options.value[0], "Slovak"); break;
				case LANG_TAMIL:				sprintf(options.value[0], "Tamil"); break;
				case LANG_SWEDISH:				sprintf(options.value[0], "Swedish"); break;
				case LANG_DANISH:				sprintf(options.value[0], "Danish"); break;
				case LANG_BULGARIAN:			sprintf(options.value[0], "Bulgarian"); break;*/
			}
			
			sprintf (options.value[1], "%d%%", WiiSettings.volume);
			sprintf(options.value[2], "%s", WiiSettings.hideExtensions ? "Hide" : "Show");

			switch(WiiSettings.exitAction)
			{
				case EXIT_AUTO:		sprintf(options.value[3], "Auto"); break;
				case EXIT_WIIMENU:	sprintf(options.value[3], "Return to Wii Menu"); break;
				case EXIT_POWEROFF:	sprintf(options.value[3], "Power Off Wii"); break;
				case EXIT_LOADER:	sprintf(options.value[3], "Return to Loader"); break;
			}

			sprintf(options.value[4], "%s", WiiSettings.rumble ? "On" : "Off");

			if(WiiSettings.sleepTimer > 0)
				sprintf(options.value[5], "%d %s", WiiSettings.sleepTimer, gettext("min"));
			else
				sprintf(options.value[5], "Off");

			sprintf(options.value[6], "%d %s", (int)(WiiSettings.screensaverDelay/60), gettext("min"));
			
			if(WiiSettings.inactivityShutdown > 0)
				sprintf(options.value[7], "%d %s", WiiSettings.inactivityShutdown, gettext("hours"));
			else
				sprintf(options.value[7], "Off");
			
			sprintf(options.value[8], "%s", WiiSettings.lockFolders ? "Static" : "Use Last Browsed");
			
			switch(WiiSettings.startArea)
			{
				case MENU_BROWSE_VIDEOS: 		sprintf(options.value[9], "Videos"); break;
				case MENU_BROWSE_MUSIC: 		sprintf(options.value[9], "Music"); break;
				case MENU_DVD: 					sprintf(options.value[9], "DVD"); break;
				case MENU_BROWSE_ONLINEMEDIA: 	sprintf(options.value[9], "Online Media"); break;
			}
			
			sprintf(options.value[10], "%s", WiiSettings.screenDim ? "On" : "Off");
			sprintf(options.value[11], "%s", WiiSettings.doubleStrike ? "On" : "Off");

			optionBrowser.TriggerUpdate();
		}

		if(backBtn.GetState() == STATE_CLICKED)
		{
			ChangeMenuNoHistory(MENU_SETTINGS);
		}
	}
	ChangeLanguage();
	SuspendGui();
	SetSleepTimer();
	mainWindow->Remove(&optionBrowser);
	mainWindow->Remove(&w);
	mainWindow->Remove(&titleTxt);
}

static void ScreenZoomWindowUpdate(void *ptr, float h, float v)
{
	GuiButton *b = (GuiButton *)ptr;
	if(b->GetState() == STATE_CLICKED)
	{
		if (h!=0.0)
			WiiSettings.videoZoomHor = ((int)((WiiSettings.videoZoomHor+h)/fabs(h)+0.5))*fabs(h);
		if (v!=0.0)
			WiiSettings.videoZoomVert = ((int)((WiiSettings.videoZoomVert+v)/fabs(v)+0.5))*fabs(v);

		char zoom[10];
		sprintf(zoom, "%.2f%%", WiiSettings.videoZoomHor*100);
		settingText->SetText(zoom);
		sprintf(zoom, "%.2f%%", WiiSettings.videoZoomVert*100);
		settingText2->SetText(zoom);
		b->ResetState();

	}
}

static void ScreenZoomWindowLeftClick(void *ptr) { ScreenZoomWindowUpdate(ptr, -0.01, 0); }
static void ScreenZoomWindowRightClick(void *ptr) { ScreenZoomWindowUpdate(ptr, +0.01, 0); }
static void ScreenZoomWindowUpClick(void *ptr) { ScreenZoomWindowUpdate(ptr, 0, +0.01); }
static void ScreenZoomWindowDownClick(void *ptr) { ScreenZoomWindowUpdate(ptr, 0, -0.01); }

static void ScreenZoomWindow()
{
	GuiWindow *w = new GuiWindow(200,200);
	w->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	
	GuiImageData arrowLeft(arrow_left_png);
	GuiImage arrowLeftImg(&arrowLeft);
	GuiImageData arrowLeftOver(arrow_left_over_png);
	GuiImage arrowLeftOverImg(&arrowLeftOver);
	GuiButton arrowLeftBtn(arrowLeft.GetWidth(), arrowLeft.GetHeight());
	arrowLeftBtn.SetImage(&arrowLeftImg);
	arrowLeftBtn.SetImageOver(&arrowLeftOverImg);
	arrowLeftBtn.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	arrowLeftBtn.SetPosition(46, 0);
	arrowLeftBtn.SetTrigger(0, trigA);
	arrowLeftBtn.SetTrigger(1, trigLeft);
	arrowLeftBtn.SetSelectable(false);
	arrowLeftBtn.SetUpdateCallback(ScreenZoomWindowLeftClick);

	GuiImageData arrowRight(arrow_right_png);
	GuiImage arrowRightImg(&arrowRight);
	GuiImageData arrowRightOver(arrow_right_over_png);
	GuiImage arrowRightOverImg(&arrowRightOver);
	GuiButton arrowRightBtn(arrowRight.GetWidth(), arrowRight.GetHeight());
	arrowRightBtn.SetImage(&arrowRightImg);
	arrowRightBtn.SetImageOver(&arrowRightOverImg);
	arrowRightBtn.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	arrowRightBtn.SetPosition(136, 0);
	arrowRightBtn.SetTrigger(0, trigA);
	arrowRightBtn.SetTrigger(1, trigRight);
	arrowRightBtn.SetSelectable(false);
	arrowRightBtn.SetUpdateCallback(ScreenZoomWindowRightClick);

	GuiImageData arrowUp(arrow_up_png);
	GuiImage arrowUpImg(&arrowUp);
	GuiImageData arrowUpOver(arrow_up_over_png);
	GuiImage arrowUpOverImg(&arrowUpOver);
	GuiButton arrowUpBtn(arrowUp.GetWidth(), arrowUp.GetHeight());
	arrowUpBtn.SetImage(&arrowUpImg);
	arrowUpBtn.SetImageOver(&arrowUpOverImg);
	arrowUpBtn.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	arrowUpBtn.SetPosition(-76, -27);
	arrowUpBtn.SetTrigger(0, trigA);
	arrowUpBtn.SetTrigger(1, trigUp);
	arrowUpBtn.SetSelectable(false);
	arrowUpBtn.SetUpdateCallback(ScreenZoomWindowUpClick);

	GuiImageData arrowDown(arrow_down_png);
	GuiImage arrowDownImg(&arrowDown);
	GuiImageData arrowDownOver(arrow_down_over_png);
	GuiImage arrowDownOverImg(&arrowDownOver);
	GuiButton arrowDownBtn(arrowDown.GetWidth(), arrowDown.GetHeight());
	arrowDownBtn.SetImage(&arrowDownImg);
	arrowDownBtn.SetImageOver(&arrowDownOverImg);
	arrowDownBtn.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	arrowDownBtn.SetPosition(-76, 27);
	arrowDownBtn.SetTrigger(0, trigA);
	arrowDownBtn.SetTrigger(1, trigDown);
	arrowDownBtn.SetSelectable(false);
	arrowDownBtn.SetUpdateCallback(ScreenZoomWindowDownClick);

	GuiImageData screenPosition(screen_position_png);
	GuiImage screenPositionImg(&screenPosition);
	screenPositionImg.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	screenPositionImg.SetPosition(0, 0);

	settingText = new GuiText(NULL, 20, (GXColor){255, 255, 255, 255});
	settingText2 = new GuiText(NULL, 20, (GXColor){255, 255, 255, 255});
	char zoom[10];
	sprintf(zoom, "%.2f%%", WiiSettings.videoZoomHor*100);
	settingText->SetText(zoom);
	settingText->SetPosition(90, 0);
	sprintf(zoom, "%.2f%%", WiiSettings.videoZoomVert*100);
	settingText2->SetText(zoom);
	settingText2->SetPosition(-76, 0);

	float currentZoomHor = WiiSettings.videoZoomHor;
	float currentZoomVert = WiiSettings.videoZoomVert;

	w->Append(&arrowLeftBtn);
	w->Append(&arrowRightBtn);
	w->Append(&arrowUpBtn);
	w->Append(&arrowDownBtn);
	w->Append(&screenPositionImg);
	w->Append(settingText);
	w->Append(settingText2);

	if(!SettingWindow("Screen Zoom",w))
	{
		// undo changes
		WiiSettings.videoZoomHor = currentZoomHor;
		WiiSettings.videoZoomVert = currentZoomVert;
	}

	delete(w);
	delete(settingText);
	delete(settingText2);
}

static void ScreenPositionWindowUpdate(void *ptr, int x, int y)
{
	GuiButton *b = (GuiButton *)ptr;
	if(b->GetState() == STATE_CLICKED)
	{
		WiiSettings.videoXshift += x;
		WiiSettings.videoYshift += y;

		char shift[10];
		sprintf(shift, "%i, %i", WiiSettings.videoXshift, WiiSettings.videoYshift);
		settingText->SetText(shift);
		b->ResetState();
	}
}

static void ScreenPositionWindowLeftClick(void *ptr) { ScreenPositionWindowUpdate(ptr, -1, 0); }
static void ScreenPositionWindowRightClick(void *ptr) { ScreenPositionWindowUpdate(ptr, +1, 0); }
static void ScreenPositionWindowUpClick(void *ptr) { ScreenPositionWindowUpdate(ptr, 0, -1); }
static void ScreenPositionWindowDownClick(void *ptr) { ScreenPositionWindowUpdate(ptr, 0, +1); }

static void ScreenPositionWindow()
{
	GuiWindow *w = new GuiWindow(120,120);
	w->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	w->SetPosition(0, 0);

	GuiImageData arrowLeft(arrow_left_png);
	GuiImage arrowLeftImg(&arrowLeft);
	GuiImageData arrowLeftOver(arrow_left_over_png);
	GuiImage arrowLeftOverImg(&arrowLeftOver);
	GuiButton arrowLeftBtn(arrowLeft.GetWidth(), arrowLeft.GetHeight());
	arrowLeftBtn.SetImage(&arrowLeftImg);
	arrowLeftBtn.SetImageOver(&arrowLeftOverImg);
	arrowLeftBtn.SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	arrowLeftBtn.SetTrigger(0, trigA);
	arrowLeftBtn.SetTrigger(1, trigLeft);
	arrowLeftBtn.SetSelectable(false);
	arrowLeftBtn.SetUpdateCallback(ScreenPositionWindowLeftClick);

	GuiImageData arrowRight(arrow_right_png);
	GuiImage arrowRightImg(&arrowRight);
	GuiImageData arrowRightOver(arrow_right_over_png);
	GuiImage arrowRightOverImg(&arrowRightOver);
	GuiButton arrowRightBtn(arrowRight.GetWidth(), arrowRight.GetHeight());
	arrowRightBtn.SetImage(&arrowRightImg);
	arrowRightBtn.SetImageOver(&arrowRightOverImg);
	arrowRightBtn.SetAlignment(ALIGN_RIGHT, ALIGN_MIDDLE);
	arrowRightBtn.SetTrigger(0, trigA);
	arrowRightBtn.SetTrigger(1, trigRight);
	arrowRightBtn.SetSelectable(false);
	arrowRightBtn.SetUpdateCallback(ScreenPositionWindowRightClick);

	GuiImageData arrowUp(arrow_up_png);
	GuiImage arrowUpImg(&arrowUp);
	GuiImageData arrowUpOver(arrow_up_over_png);
	GuiImage arrowUpOverImg(&arrowUpOver);
	GuiButton arrowUpBtn(arrowUp.GetWidth(), arrowUp.GetHeight());
	arrowUpBtn.SetImage(&arrowUpImg);
	arrowUpBtn.SetImageOver(&arrowUpOverImg);
	arrowUpBtn.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	arrowUpBtn.SetTrigger(0, trigA);
	arrowUpBtn.SetTrigger(1, trigUp);
	arrowUpBtn.SetSelectable(false);
	arrowUpBtn.SetUpdateCallback(ScreenPositionWindowUpClick);

	GuiImageData arrowDown(arrow_down_png);
	GuiImage arrowDownImg(&arrowDown);
	GuiImageData arrowDownOver(arrow_down_over_png);
	GuiImage arrowDownOverImg(&arrowDownOver);
	GuiButton arrowDownBtn(arrowDown.GetWidth(), arrowDown.GetHeight());
	arrowDownBtn.SetImage(&arrowDownImg);
	arrowDownBtn.SetImageOver(&arrowDownOverImg);
	arrowDownBtn.SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	arrowDownBtn.SetTrigger(0, trigA);
	arrowDownBtn.SetTrigger(1, trigDown);
	arrowDownBtn.SetSelectable(false);
	arrowDownBtn.SetUpdateCallback(ScreenPositionWindowDownClick);

	GuiImageData screenPosition(screen_position_png);
	GuiImage screenPositionImg(&screenPosition);
	screenPositionImg.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);

	settingText = new GuiText(NULL, 20, (GXColor){0, 0, 0, 255});
	char shift[10];
	sprintf(shift, "%i, %i", WiiSettings.videoXshift, WiiSettings.videoYshift);
	settingText->SetText(shift);

	int currentX = WiiSettings.videoXshift;
	int currentY = WiiSettings.videoYshift;

	w->Append(&arrowLeftBtn);
	w->Append(&arrowRightBtn);
	w->Append(&arrowUpBtn);
	w->Append(&arrowDownBtn);
	w->Append(&screenPositionImg);
	w->Append(settingText);

	if(!SettingWindow("Screen Position",w))
	{
		WiiSettings.videoXshift = currentX; // undo changes
		WiiSettings.videoYshift = currentY;
	}

	delete(w);
	delete(settingText);
}

static void LanguageWindow(char *lang)
{
	GuiWindow promptWindow(556,352);
	promptWindow.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	GuiImageData btnOutline(button_png);
	GuiImageData btnOutlineOver(button_over_png);

	GuiImageData dialogBox(dialogue_box_large_png);
	GuiImage dialogBoxImg(&dialogBox);
	dialogBoxImg.SetAlpha(220);

	GuiText titleTxt(NULL, 28, (GXColor){255, 255, 255, 255});

	if(lang == WiiSettings.subtitleLanguage)
		titleTxt.SetText("Subtitle Language");
	else
		titleTxt.SetText("Audio Language");

	titleTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	titleTxt.SetPosition(0,18);

	GuiText okBtnTxt("OK", 20, (GXColor){255, 255, 255, 255});
	GuiImage okBtnImg(&btnOutline);
	GuiImage okBtnImgOver(&btnOutlineOver);
	GuiButton okBtn(btnOutline.GetWidth(), btnOutline.GetHeight());

	okBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	okBtn.SetPosition(20, -25);

	okBtn.SetLabel(&okBtnTxt);
	okBtn.SetImage(&okBtnImg);
	okBtn.SetImageOver(&okBtnImgOver);
	okBtn.SetTrigger(trigA);
	okBtn.SetSelectable(false);
	okBtn.SetEffectGrow();

	GuiText cancelBtnTxt("Cancel", 20, (GXColor){255, 255, 255, 255});
	GuiImage cancelBtnImg(&btnOutline);
	GuiImage cancelBtnImgOver(&btnOutlineOver);
	GuiButton cancelBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	cancelBtn.SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	cancelBtn.SetPosition(-20, -25);
	cancelBtn.SetLabel(&cancelBtnTxt);
	cancelBtn.SetImage(&cancelBtnImg);
	cancelBtn.SetImageOver(&cancelBtnImgOver);
	cancelBtn.SetTrigger(trigA);
	cancelBtn.SetSelectable(false);
	cancelBtn.SetEffectGrow();

	int i;
	OptionList options;
	sprintf(options.name[0], "Default");

	for(i=1; i < LANGUAGE_SIZE; i++)
		sprintf(options.name[i], "%s (%s)", languages[i].language, languages[i].abbrev);

	options.length = i;

	for(i=0; i < options.length; i++)
	{
		options.value[i][0] = 0;
		options.icon[i] = 0;
	}

	GuiOptionBrowser optionBrowser(544, 6, &options);
	optionBrowser.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	optionBrowser.SetPosition(6, 70);
	optionBrowser.TriggerUpdate();

	promptWindow.Append(&dialogBoxImg);
	promptWindow.Append(&titleTxt);
	promptWindow.Append(&optionBrowser);
	promptWindow.Append(&okBtn);
	promptWindow.Append(&cancelBtn);

	SuspendGui();
	mainWindow->SetState(STATE_DISABLED);
	mainWindow->Append(disabled);
	mainWindow->Append(&promptWindow);
	ResumeGui();

	int save = -1;

	while(save == -1)
	{
		usleep(THREAD_SLEEP);

		if(okBtn.GetState() == STATE_CLICKED)
			save = 1;
		else if(cancelBtn.GetState() == STATE_CLICKED)
			save = 0;
		else if(optionBrowser.GetClickedOption() >= 0)
			save = 1;
	}

	if(save && optionBrowser.GetSelectedOption() >= 0)
		strcpy(lang, languages[optionBrowser.GetSelectedOption()].abbrev);

	SuspendGui();
	mainWindow->Remove(&promptWindow);
	mainWindow->Remove(disabled);
	mainWindow->SetState(STATE_DEFAULT);
	ResumeGui();
}

static void MenuSettingsVideos()
{
	int ret;
	int i = 0;
	bool firstRun = true;
	OptionList options;
	
	sprintf(options.name[i++], "Screen Zoom");
	sprintf(options.name[i++], "Screen Position");
	sprintf(options.name[i++], "Frame Dropping");
	sprintf(options.name[i++], "Aspect Ratio");
	sprintf(options.name[i++], "Cache Fill");
	sprintf(options.name[i++], "Audio Language");
	sprintf(options.name[i++], "Audio Delay");
	sprintf(options.name[i++], "Auto-Resume");
	sprintf(options.name[i++], "Auto-Play Next Video");
	sprintf(options.name[i++], "Skip Backward");
	sprintf(options.name[i++], "Skip Forward");
	sprintf(options.name[i++], "Videos Files Folder");
    if (CONF_GetAspectRatio() == CONF_ASPECT_16_9)
       i += 1;
    else
        sprintf(options.name[i++], "Force Fullscreen in 4:3");
	sprintf(options.name[i++], "Volume Normalizer");
	sprintf(options.name[i++], "Deflicker");
	sprintf(options.name[i++], "Set VI Width to 720");
	sprintf(options.name[i++], "Skip Deblocking Filter");

	options.length = i;

	for(i=0; i < options.length; i++)
	{
		options.value[i][0] = 0;
		options.icon[i] = 0;
	}

	GuiText titleTxt("Settings - Videos", 28, (GXColor){255, 255, 255, 255});
	titleTxt.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	titleTxt.SetPosition(49, 100);

	GuiText backBtnTxt("Go back", 18, (GXColor){255, 255, 255, 255});
	backBtnTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnTxt.SetPosition(-6, 10);
	GuiImage backBtnImg(btnBottom);
	GuiImage backBtnImgOver(btnBottomOver);
	GuiImage backBtnArrow(arrowRightSmall);
	backBtnArrow.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnArrow.SetPosition(backBtnTxt.GetTextWidth()/2 + 6, 11);
	GuiButton backBtn(screenwidth, btnBottom->GetHeight());
	backBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	backBtn.SetPosition(0, 0);
	backBtn.SetLabel(&backBtnTxt);
	backBtn.SetImage(&backBtnImg);
	backBtn.SetImageOver(&backBtnImgOver);
	backBtn.SetIcon(&backBtnArrow);
	backBtn.SetTrigger(trigA);
	backBtn.SetTrigger(trigB);

	GuiOptionBrowser optionBrowser(screenwidth, 7, &options);
	optionBrowser.SetPosition(0, 150);
	optionBrowser.SetAlignment(ALIGN_LEFT, ALIGN_TOP);

	SuspendGui();
	GuiWindow w(screenwidth, screenheight);
	w.Append(&backBtn);
	mainWindow->Append(&optionBrowser);
	mainWindow->Append(&w);
	mainWindow->Append(&titleTxt);
	ResumeGui();
	
	int skip[] = { 5, 10, 15, 30, 60, 120, 300, 600, 1200 };
	int skipNum = sizeof(skip) / sizeof(int);
	int bwSkip = 0;
	int fwSkip = 2;

	for(i=0; i < skipNum; i++)
	{
		if(WiiSettings.skipBackward == skip[i])
			bwSkip = i;
		if(WiiSettings.skipForward == skip[i])
			fwSkip = i;
	}

	while(menuCurrent == MENU_SETTINGS_VIDEOS && !guiShutdown)
	{
		usleep(THREAD_SLEEP);

		ret = optionBrowser.GetClickedOption();

		switch (ret)
		{
			case 0:
				ScreenZoomWindow();
				break;
			case 1:
				ScreenPositionWindow();
				break;
			case 2:
				WiiSettings.frameDropping++;
				if (WiiSettings.frameDropping > 2)
					WiiSettings.frameDropping = 0;
				break;
			case 3:
				if(WiiSettings.aspectRatio > 2.34)
					WiiSettings.aspectRatio = -2;
				else if(WiiSettings.aspectRatio > 1.84)
					WiiSettings.aspectRatio = 2.35;
				else if(WiiSettings.aspectRatio > 1.77)
					WiiSettings.aspectRatio = 1.85;
				else if(WiiSettings.aspectRatio > 1.32)
					WiiSettings.aspectRatio = 1.7778;
				else if(WiiSettings.aspectRatio > -2.01)
					WiiSettings.aspectRatio = 1.3333;
				else
					WiiSettings.aspectRatio = -2;
				break;
			case 4:
				WiiSettings.cacheFill += 10;
				if (WiiSettings.cacheFill > 100)
					WiiSettings.cacheFill = 10;
				break;
			case 5:
				LanguageWindow(WiiSettings.audioLanguage);
				break;
			case 6:
				char delay[8];
				sprintf(delay, "%.2f", WiiSettings.audioDelay);
				if(OnScreenKeypad(delay, 7, true))
				{
					if(delay[0] == 0)
						WiiSettings.audioDelay = 0;
					else
						WiiSettings.audioDelay = atof(delay);
				}
				break;
			case 7:
				WiiSettings.autoResume ^= 1;
				break;
			case 8:
				WiiSettings.autoPlayNextVideo ^= 1;
				break;
			case 9:
				bwSkip++;
				if(bwSkip == skipNum) bwSkip = 0;
				WiiSettings.skipBackward = skip[bwSkip];
				break;
			case 10:
				fwSkip++;
				if(fwSkip == skipNum) fwSkip = 0;
				WiiSettings.skipForward = skip[fwSkip];
				break;
			case 11:
				OnScreenKeyboard(WiiSettings.videosFolder, MAXPATHLEN);
				CleanupPath(WiiSettings.videosFolder);
				break;
			case 12:
				WiiSettings.videoFull ^= 1;
				wiiSetVidFull();
				break;
			case 13:
				WiiSettings.audioNorm += 1;
				if (WiiSettings.audioNorm > 2)
					WiiSettings.audioNorm = 0;
				break;
			case 14:
				WiiSettings.videoDf ^= 1;
				if(WiiSettings.videoDf == 1)
					SetDf();
				else
					SetDfOff();
				break;
			case 15:
				WiiSettings.viWidth ^= 1;
				if(WiiSettings.viWidth == 1)
					SetVIscale();
				else
					SetVIscaleback();
				break;
			case 16:
				WiiSettings.skipLoop ^= 1;
				break;
		}

		if(ret >= 0 || firstRun)
		{
			firstRun = false;
			sprintf (options.value[0], "%.2f%%, %.2f%%", WiiSettings.videoZoomHor*100, WiiSettings.videoZoomVert*100);
			sprintf (options.value[1], "%d, %d", WiiSettings.videoXshift, WiiSettings.videoYshift);

			switch(WiiSettings.frameDropping)
			{
				case FRAMEDROPPING_AUTO:
					sprintf (options.value[2], "Auto"); break;
				case FRAMEDROPPING_ALWAYS:
					sprintf (options.value[2], "Always"); break;
				case FRAMEDROPPING_DISABLED:
					sprintf (options.value[2], "Disabled"); break;
			}

			if(WiiSettings.aspectRatio > 2.34)
				sprintf (options.value[3], "2.35:1");
			else if(WiiSettings.aspectRatio > 1.84)
				sprintf (options.value[3], "1.85:1");
			else if(WiiSettings.aspectRatio > 1.77)
				sprintf (options.value[3], "16:9");
			else if(WiiSettings.aspectRatio > 1.32)
				sprintf (options.value[3], "4:3");
			else
				sprintf (options.value[3], "Auto");

			sprintf (options.value[4], "%d%%", WiiSettings.cacheFill);
			strcpy(options.value[5], languages[GetLangIndex(WiiSettings.audioLanguage)].language);
			sprintf (options.value[6], "%.1f %s", WiiSettings.audioDelay, gettext("sec"));
			sprintf (options.value[7], "%s", WiiSettings.autoResume ? "On" : "Off");
			sprintf (options.value[8], "%s", WiiSettings.autoPlayNextVideo ? "On" : "Off");
			sprintf (options.value[9], "%d %s", WiiSettings.skipBackward, gettext("sec"));
			sprintf (options.value[10], "%d %s", WiiSettings.skipForward, gettext("sec"));
			snprintf(options.value[11], 60, "%s", WiiSettings.videosFolder);
			sprintf (options.value[12], "%s", WiiSettings.videoFull ? "On" : "Off");
			sprintf (options.value[13], "%d%", WiiSettings.audioNorm);
			sprintf (options.value[14], "%s", WiiSettings.videoDf ? "On" : "Off");
			sprintf (options.value[15], "%s", WiiSettings.viWidth ? "On" : "Off");
			sprintf (options.value[16], "%s", WiiSettings.skipLoop ? "On" : "Off");

			optionBrowser.TriggerUpdate();
		}

		if(backBtn.GetState() == STATE_CLICKED)
		{
			ChangeMenuNoHistory(MENU_SETTINGS);
		}
	}
	SuspendGui();
	mainWindow->Remove(&optionBrowser);
	mainWindow->Remove(&w);
	mainWindow->Remove(&titleTxt);
}

static void MenuSettingsMusic()
{
	int ret;
	int i = 0;
	bool firstRun = true;
	OptionList options;

	sprintf(options.name[i++], "Play Order");
	sprintf(options.name[i++], "Music Files Folder");

	options.length = i;
		
	for(i=0; i < options.length; i++)
	{
		options.value[i][0] = 0;
		options.icon[i] = 0;
	}

	GuiText titleTxt("Settings - Music", 28, (GXColor){255, 255, 255, 255});
	titleTxt.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	titleTxt.SetPosition(49, 100);

	GuiText backBtnTxt("Go back", 18, (GXColor){255, 255, 255, 255});
	backBtnTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnTxt.SetPosition(-6, 10);
	GuiImage backBtnImg(btnBottom);
	GuiImage backBtnImgOver(btnBottomOver);
	GuiImage backBtnArrow(arrowRightSmall);
	backBtnArrow.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnArrow.SetPosition(backBtnTxt.GetTextWidth()/2 + 6, 11);
	GuiButton backBtn(screenwidth, btnBottom->GetHeight());
	backBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	backBtn.SetPosition(0, 0);
	backBtn.SetLabel(&backBtnTxt);
	backBtn.SetImage(&backBtnImg);
	backBtn.SetImageOver(&backBtnImgOver);
	backBtn.SetIcon(&backBtnArrow);
	backBtn.SetTrigger(trigA);
	backBtn.SetTrigger(trigB);

	GuiOptionBrowser optionBrowser(screenwidth, 7, &options);
	optionBrowser.SetPosition(0, 150);
	optionBrowser.SetAlignment(ALIGN_LEFT, ALIGN_TOP);

	SuspendGui();
	GuiWindow w(screenwidth, screenheight);
	w.Append(&backBtn);
	mainWindow->Append(&optionBrowser);
	mainWindow->Append(&w);
	mainWindow->Append(&titleTxt);
	ResumeGui();

	while(menuCurrent == MENU_SETTINGS_MUSIC && !guiShutdown)
	{
		usleep(THREAD_SLEEP);

		ret = optionBrowser.GetClickedOption();

		switch (ret)
		{
			case 0:
				WiiSettings.playOrder++;
				if(WiiSettings.playOrder > PLAY_LOOP)
					WiiSettings.playOrder = 0;
				break;
			case 1:
				OnScreenKeyboard(WiiSettings.musicFolder, MAXPATHLEN);
				CleanupPath(WiiSettings.musicFolder);
				break;
		}

		if(ret >= 0 || firstRun)
		{
			firstRun = false;

			switch(WiiSettings.playOrder)
			{
				case PLAY_SINGLE:		sprintf(options.value[0], "Single"); break;
				case PLAY_CONTINUOUS:	sprintf(options.value[0], "Continuous"); break;
				case PLAY_SHUFFLE:		sprintf(options.value[0], "Shuffle"); break;
				case PLAY_LOOP:			sprintf(options.value[0], "Loop"); break;
				case PLAY_THROUGH:		sprintf(options.value[0], "Through"); break;
			}
			snprintf(options.value[1], 60, "%s", WiiSettings.musicFolder);

			optionBrowser.TriggerUpdate();
		}

		if(backBtn.GetState() == STATE_CLICKED)
		{
			ChangeMenuNoHistory(MENU_SETTINGS);
		}
	}
	SuspendGui();
	mainWindow->Remove(&optionBrowser);
	mainWindow->Remove(&w);
	mainWindow->Remove(&titleTxt);
}

static void MenuSettingsDVD()
{
	int ret;
	int i = 0;
	bool firstRun = true;
	OptionList options;

	sprintf(options.name[i++], "DVD Menu");
	sprintf(options.name[i++], "DVD Support");

	options.length = i;

	for(i=0; i < options.length; i++)
	{
		options.value[i][0] = 0;
		options.icon[i] = 0;
	}

	GuiText titleTxt("Settings - DVD", 28, (GXColor){255, 255, 255, 255});
	titleTxt.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	titleTxt.SetPosition(49, 100);

	GuiText backBtnTxt("Go back", 18, (GXColor){255, 255, 255, 255});
	backBtnTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnTxt.SetPosition(-6, 10);
	GuiImage backBtnImg(btnBottom);
	GuiImage backBtnImgOver(btnBottomOver);
	GuiImage backBtnArrow(arrowRightSmall);
	backBtnArrow.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnArrow.SetPosition(backBtnTxt.GetTextWidth()/2 + 6, 11);
	GuiButton backBtn(screenwidth, btnBottom->GetHeight());
	backBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	backBtn.SetPosition(0, 0);
	backBtn.SetLabel(&backBtnTxt);
	backBtn.SetImage(&backBtnImg);
	backBtn.SetImageOver(&backBtnImgOver);
	backBtn.SetIcon(&backBtnArrow);
	backBtn.SetTrigger(trigA);
	backBtn.SetTrigger(trigB);

	GuiOptionBrowser optionBrowser(screenwidth, 7, &options);
	optionBrowser.SetPosition(0, 150);
	optionBrowser.SetAlignment(ALIGN_LEFT, ALIGN_TOP);

	SuspendGui();
	GuiWindow w(screenwidth, screenheight);
	w.Append(&backBtn);
	mainWindow->Append(&optionBrowser);
	mainWindow->Append(&w);
	mainWindow->Append(&titleTxt);
	ResumeGui();

	while(menuCurrent == MENU_SETTINGS_DVD && !guiShutdown)
	{
		usleep(THREAD_SLEEP);

		ret = optionBrowser.GetClickedOption();

		switch (ret)
		{
			case 0:
				WiiSettings.dvdMenu ^= 1;
				break;
			case 1:
				WiiSettings.dvdDisabled ^= 1;
				break;
		}

		if(ret >= 0 || firstRun)
		{
			firstRun = false;
			sprintf(options.value[0], "%s", WiiSettings.dvdMenu ? "Show" : "Skip to Main Title");
			sprintf(options.value[1], "%s", WiiSettings.dvdDisabled ? "Disabled" : "Enabled");
			optionBrowser.TriggerUpdate();
		}

		if(backBtn.GetState() == STATE_CLICKED)
		{
			ChangeMenuNoHistory(MENU_SETTINGS);
		}
	}
	SuspendGui();
	mainWindow->Remove(&optionBrowser);
	mainWindow->Remove(&w);
	mainWindow->Remove(&titleTxt);

	if(WiiSettings.dvdDisabled)
	{
		dvdBtn->SetVisible(false);
		dvdBtn->SetState(STATE_DISABLED);
		onlineBtn->SetPosition(155, 30);
	}
	else
	{
		dvdBtn->SetVisible(true);
		dvdBtn->SetState(STATE_DEFAULT);
		onlineBtn->SetPosition(210, 30);
	}
}

static void MenuSettingsOnlineMedia()
{
	int ret;
	int i = 0;
	bool firstRun = true;
	OptionList options;

	sprintf(options.name[i++], "Cache Fill");
	//sprintf(options.name[i++], "YouTube Quality");
	sprintf(options.name[i++], "Online Media Folder");

	options.length = i;

	for(i=0; i < options.length; i++)
	{
		options.value[i][0] = 0;
		options.icon[i] = 0;
	}

	GuiText titleTxt("Settings - Online Media", 28, (GXColor){255, 255, 255, 255});
	titleTxt.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	titleTxt.SetPosition(49, 100);

	GuiText backBtnTxt("Go back", 18, (GXColor){255, 255, 255, 255});
	backBtnTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnTxt.SetPosition(-6, 10);
	GuiImage backBtnImg(btnBottom);
	GuiImage backBtnImgOver(btnBottomOver);
	GuiImage backBtnArrow(arrowRightSmall);
	backBtnArrow.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnArrow.SetPosition(backBtnTxt.GetTextWidth()/2 + 6, 11);
	GuiButton backBtn(screenwidth, btnBottom->GetHeight());
	backBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	backBtn.SetPosition(0, 0);
	backBtn.SetLabel(&backBtnTxt);
	backBtn.SetImage(&backBtnImg);
	backBtn.SetImageOver(&backBtnImgOver);
	backBtn.SetIcon(&backBtnArrow);
	backBtn.SetTrigger(trigA);
	backBtn.SetTrigger(trigB);

	GuiOptionBrowser optionBrowser(screenwidth, 7, &options);
	optionBrowser.SetPosition(0, 150);
	optionBrowser.SetAlignment(ALIGN_LEFT, ALIGN_TOP);

	SuspendGui();
	GuiWindow w(screenwidth, screenheight);
	w.Append(&backBtn);
	mainWindow->Append(&optionBrowser);
	mainWindow->Append(&w);
	mainWindow->Append(&titleTxt);
	ResumeGui();

	while(menuCurrent == MENU_SETTINGS_ONLINEMEDIA && !guiShutdown)
	{
		usleep(THREAD_SLEEP);

		ret = optionBrowser.GetClickedOption();

		switch (ret)
		{
			case 0:
				WiiSettings.onlineCacheFill += 5;

				if(WiiSettings.onlineCacheFill > 50)
					WiiSettings.onlineCacheFill = 5;
				break;
			/*case 1:
				if(WiiSettings.youtubeFormat == 5)
					WiiSettings.youtubeFormat = 43;
				else if(WiiSettings.youtubeFormat == 18)
					WiiSettings.youtubeFormat = 35;
				else
					WiiSettings.youtubeFormat = 5;
				break;*/
			case 1:
				OnScreenKeyboard(WiiSettings.onlinemediaFolder, MAXPATHLEN);
				if(!IsOnlineMediaPath(WiiSettings.onlinemediaFolder))
					CleanupPath(WiiSettings.onlinemediaFolder);
				break;
		}

		if(ret >= 0 || firstRun)
		{
			firstRun = false;

			sprintf (options.value[0], "%d%%", WiiSettings.onlineCacheFill);

			/*if(WiiSettings.youtubeFormat == 5)
				sprintf(options.value[1], "Low (400x240)");
			else if(WiiSettings.youtubeFormat == 43)
				sprintf(options.value[1], "Medium (480x360)");
			else
				sprintf(options.value[1], "High (854x480)");*/

			snprintf(options.value[1], 60, "%s", WiiSettings.onlinemediaFolder);
			optionBrowser.TriggerUpdate();
		}

		if(backBtn.GetState() == STATE_CLICKED)
		{
			ChangeMenuNoHistory(MENU_SETTINGS);
		}
	}
	SuspendGui();
	mainWindow->Remove(&optionBrowser);
	mainWindow->Remove(&w);
	mainWindow->Remove(&titleTxt);
}

static void MenuSettingsNetwork()
{
	int ret;
	int i = 0;
	OptionList options;
	
	// find all currently set SMB/FTP entries
	int map[MAX_OPTIONS];
	
	for(int j=0; j < MAX_SHARES; j++)
	{
		if(WiiSettings.smbConf[j].share[0] != 0)
		{
			options.icon[i] = ICON_SMB;
			options.value[i][0] = 0;
			map[i] = j;

			if(WiiSettings.smbConf[j].displayname[0] != 0)
				sprintf(options.name[i], "%s", WiiSettings.smbConf[j].displayname);
			else
				snprintf(options.name[i], 60, "%s", WiiSettings.smbConf[j].share);
			i++;
		}
	}
	for(int j=0; j < MAX_SHARES; j++)
	{
		if(WiiSettings.ftpConf[j].ip[0] != 0)
		{
			options.icon[i] = ICON_FTP;
			options.value[i][0] = 0;
			map[i] = j + MAX_SHARES;

			if(WiiSettings.ftpConf[j].displayname[0] != 0)
				sprintf(options.name[i], "%s", WiiSettings.ftpConf[j].displayname);
			else
				snprintf(options.name[i], 60, "%s@%s/%s", WiiSettings.ftpConf[j].user, WiiSettings.ftpConf[j].ip, WiiSettings.ftpConf[j].folder);
			i++;
		}
	}

	options.length = i;

	GuiText titleTxt("Settings - Network", 28, (GXColor){255, 255, 255, 255});
	titleTxt.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	titleTxt.SetPosition(49, 100);

	GuiImageData btnOutline(button_png);
	GuiImageData btnOutlineOver(button_over_png);
	GuiImageData iconSMB(icon_smb_png);
	GuiImageData iconFTP(icon_ftp_png);

	GuiText backBtnTxt("Go back", 18, (GXColor){255, 255, 255, 255});
	backBtnTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnTxt.SetPosition(-6, 10);
	GuiImage backBtnImg(btnBottom);
	GuiImage backBtnImgOver(btnBottomOver);
	GuiImage backBtnArrow(arrowRightSmall);
	backBtnArrow.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnArrow.SetPosition(backBtnTxt.GetTextWidth()/2 + 6, 11);
	GuiButton backBtn(screenwidth, btnBottom->GetHeight());
	backBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	backBtn.SetPosition(0, 0);
	backBtn.SetLabel(&backBtnTxt);
	backBtn.SetImage(&backBtnImg);
	backBtn.SetImageOver(&backBtnImgOver);
	backBtn.SetIcon(&backBtnArrow);
	backBtn.SetTrigger(trigA);
	backBtn.SetTrigger(trigB);

	GuiText addsmbBtnTxt("Add SMB Share", 20, (GXColor){255, 255, 255, 255});
	addsmbBtnTxt.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	GuiImage addsmbBtnImg(&btnOutline);
	GuiImage addsmbBtnImgOver(&btnOutlineOver);
	GuiButton addsmbBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	addsmbBtn.SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	addsmbBtn.SetPosition(-130, -90);
	addsmbBtn.SetLabel(&addsmbBtnTxt);
	addsmbBtn.SetImage(&addsmbBtnImg);
	addsmbBtn.SetImageOver(&addsmbBtnImgOver);
	addsmbBtn.SetTrigger(trigA);
	addsmbBtn.SetEffectGrow();
	
	GuiText addftpBtnTxt("Add FTP Site", 20, (GXColor){255, 255, 255, 255});
	addftpBtnTxt.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	GuiImage addftpBtnImg(&btnOutline);
	GuiImage addftpBtnImgOver(&btnOutlineOver);
	GuiButton addftpBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	addftpBtn.SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	addftpBtn.SetPosition(130, -90);
	addftpBtn.SetLabel(&addftpBtnTxt);
	addftpBtn.SetImage(&addftpBtnImg);
	addftpBtn.SetImageOver(&addftpBtnImgOver);
	addftpBtn.SetTrigger(trigA);
	addftpBtn.SetEffectGrow();

	GuiOptionBrowser optionBrowser(screenwidth, 6, &options);
	optionBrowser.SetPosition(0, 150);
	optionBrowser.SetCol1Position(30);
	optionBrowser.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	optionBrowser.TriggerUpdate();

	SuspendGui();
	GuiWindow w(screenwidth, screenheight);
	w.Append(&backBtn);
	w.Append(&addsmbBtn);
	w.Append(&addftpBtn);
	if(options.length > 0)
		mainWindow->Append(&optionBrowser);
	mainWindow->Append(&w);
	mainWindow->Append(&titleTxt);
	ResumeGui();

	while(menuCurrent == MENU_SETTINGS_NETWORK && !guiShutdown)
	{
		usleep(THREAD_SLEEP);

		ret = optionBrowser.GetClickedOption();

		if((ret >= 0 && map[ret] < MAX_SHARES) || addsmbBtn.GetState() == STATE_CLICKED)
		{
			if(ret >= 0) netEditIndex = map[ret];
			else netEditIndex = -1;
			ChangeMenuNoHistory(MENU_SETTINGS_NETWORK_SMB);
		}
		else if((ret >= 0 && map[ret] >= MAX_SHARES) || addftpBtn.GetState() == STATE_CLICKED)
		{
			if(ret >= 0) netEditIndex = map[ret] - MAX_SHARES;
			else netEditIndex = -1;
			ChangeMenuNoHistory(MENU_SETTINGS_NETWORK_FTP);
		}

		if(backBtn.GetState() == STATE_CLICKED)
		{
			ChangeMenuNoHistory(MENU_SETTINGS);
		}
	}
	SuspendGui();
	mainWindow->Remove(&optionBrowser);
	mainWindow->Remove(&w);
	mainWindow->Remove(&titleTxt);
}

static void MenuSettingsNetworkSMB()
{
	int ret;
	int i = 0;
	bool firstRun = true;
	OptionList options;
	wchar_t titleStr[100];
	char shareName[100];

	sprintf(options.name[i++], "Display Name");
	sprintf(options.name[i++], "Share IP");
	sprintf(options.name[i++], "Share Name");
	sprintf(options.name[i++], "Username");
	sprintf(options.name[i++], "Password");

	options.length = i;
	
	for(i=0; i < options.length; i++)
	{
		options.value[i][0] = 0;
		options.icon[i] = 0;
	}
	
	if(netEditIndex < 0)
		sprintf(shareName, "New Share");
	else if(WiiSettings.smbConf[netEditIndex].displayname[0] != 0)
		sprintf(shareName, "%s", WiiSettings.smbConf[netEditIndex].displayname);
	else
		sprintf(shareName, "%s", WiiSettings.smbConf[netEditIndex].share);

	swprintf(titleStr, 100, L"%s - %s", gettext("Settings - Network"), gettext(shareName));

	GuiText titleTxt(NULL, 28, (GXColor){255, 255, 255, 255});
	titleTxt.SetWText(titleStr);
	titleTxt.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	titleTxt.SetPosition(49, 100);

	GuiImageData btnOutline(button_png);
	GuiImageData btnOutlineOver(button_over_png);

	GuiText backBtnTxt("Go back", 18, (GXColor){255, 255, 255, 255});
	backBtnTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnTxt.SetPosition(-6, 10);
	GuiImage backBtnImg(btnBottom);
	GuiImage backBtnImgOver(btnBottomOver);
	GuiImage backBtnArrow(arrowRightSmall);
	backBtnArrow.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnArrow.SetPosition(backBtnTxt.GetTextWidth()/2 + 6, 11);
	GuiButton backBtn(screenwidth, btnBottom->GetHeight());
	backBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	backBtn.SetPosition(0, 0);
	backBtn.SetLabel(&backBtnTxt);
	backBtn.SetImage(&backBtnImg);
	backBtn.SetImageOver(&backBtnImgOver);
	backBtn.SetIcon(&backBtnArrow);
	backBtn.SetTrigger(trigA);
	backBtn.SetTrigger(trigB);
	
	GuiText deleteBtnTxt("Delete", 20, (GXColor){255, 255, 255, 255});
	GuiImage deleteBtnImg(&btnOutline);
	GuiImage deleteBtnImgOver(&btnOutlineOver);
	GuiButton deleteBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	deleteBtn.SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	deleteBtn.SetPosition(0, -90);
	deleteBtn.SetLabel(&deleteBtnTxt);
	deleteBtn.SetImage(&deleteBtnImg);
	deleteBtn.SetImageOver(&deleteBtnImgOver);
	deleteBtn.SetTrigger(trigA);
	deleteBtn.SetEffectGrow();

	GuiOptionBrowser optionBrowser(screenwidth, 6, &options);
	optionBrowser.SetPosition(0, 150);
	optionBrowser.SetAlignment(ALIGN_LEFT, ALIGN_TOP);

	SuspendGui();
	GuiWindow w(screenwidth, screenheight);
	w.Append(&backBtn);

	if(netEditIndex < 0)
	{
		// find a share to put the data into
		for(i=0; i < MAX_SHARES; i++)
		{
			if(WiiSettings.smbConf[i].share[0] == 0)
			{
				netEditIndex = i;
				break;
			}
		}
	}
	else
	{
		w.Append(&deleteBtn);
	}
	
	mainWindow->Append(&optionBrowser);
	mainWindow->Append(&w);
	mainWindow->Append(&titleTxt);
	ResumeGui();

	while(menuCurrent == MENU_SETTINGS_NETWORK_SMB && !guiShutdown)
	{
		usleep(THREAD_SLEEP);

		ret = optionBrowser.GetClickedOption();

		switch (ret)
		{
			case 0:
				OnScreenKeyboard(WiiSettings.smbConf[netEditIndex].displayname, 40);
				break;

			case 1:
				if(WiiSettings.smbConf[netEditIndex].ip[0] == 0)
				{
					// pre-populate IP
					if(wiiIP[0] != 0)
					{
						strcpy(WiiSettings.smbConf[netEditIndex].ip, wiiIP);
						char *dot = strrchr(WiiSettings.smbConf[netEditIndex].ip,'.');
						if (dot != NULL) *++dot = 0; // strip last octet
					}
				}

				OnScreenKeyboard(WiiSettings.smbConf[netEditIndex].ip, 80);
				break;

			case 2:
				OnScreenKeyboard(WiiSettings.smbConf[netEditIndex].share, 40);
				break;

			case 3:
				OnScreenKeyboard(WiiSettings.smbConf[netEditIndex].user, 20);
				break;

			case 4:
				char tmp[14] = { 0 };
				if(OnScreenKeyboard(tmp, 14))
					strcpy(WiiSettings.smbConf[netEditIndex].pwd, tmp);
				break;
		}

		if(ret >= 0 || firstRun)
		{
			firstRun = false;
			snprintf(options.value[0], 40, "%s", WiiSettings.smbConf[netEditIndex].displayname);
			snprintf(options.value[1], 40, "%s", WiiSettings.smbConf[netEditIndex].ip);
			snprintf(options.value[2], 40, "%s", WiiSettings.smbConf[netEditIndex].share);
			snprintf(options.value[3], 40, "%s", WiiSettings.smbConf[netEditIndex].user);
			sprintf(options.value[4], "********");
			optionBrowser.TriggerUpdate();
		}

		if(backBtn.GetState() == STATE_CLICKED)
		{
			ChangeMenuNoHistory(MENU_SETTINGS_NETWORK);
		}
		if(deleteBtn.GetState() == STATE_CLICKED)
		{
			deleteBtn.ResetState();
			if (WindowPrompt("Delete Share", gettext("Are you sure that you want to delete this share?"), "OK", "Cancel"))
			{
				WiiSettings.smbConf[netEditIndex].displayname[0] = 0;
				WiiSettings.smbConf[netEditIndex].ip[0] = 0;
				WiiSettings.smbConf[netEditIndex].share[0] = 0;
				WiiSettings.smbConf[netEditIndex].user[0] = 0;
				WiiSettings.smbConf[netEditIndex].pwd[0] = 0;
				ChangeMenuNoHistory(MENU_SETTINGS_NETWORK);
			}
		}
	}
	SuspendGui();
	mainWindow->Remove(&optionBrowser);
	mainWindow->Remove(&w);
	mainWindow->Remove(&titleTxt);
	CloseShare(netEditIndex+1);
}

static bool isnumeric(char *str)
{
	while(*str)
	{
		if(!isdigit(*str)) return false;
		str++;
	}
	return true;
}

static void MenuSettingsNetworkFTP()
{
	int ret;
	int i = 0;
	bool firstRun = true;
	OptionList options;
	wchar_t titleStr[100];
	char siteName[100];

	sprintf(options.name[i++], "Display Name");
	sprintf(options.name[i++], "IP");
	sprintf(options.name[i++], "Folder");
	sprintf(options.name[i++], "Username");
	sprintf(options.name[i++], "Password");
	sprintf(options.name[i++], "Port");
	sprintf(options.name[i++], "Mode");

	options.length = i;
		
	for(i=0; i < options.length; i++)
	{
		options.value[i][0] = 0;
		options.icon[i] = 0;
	}

	if(netEditIndex < 0)
		sprintf(siteName, "New Site");
	else if(WiiSettings.ftpConf[netEditIndex].displayname[0] != 0)
		sprintf(siteName, "%s", WiiSettings.ftpConf[netEditIndex].displayname);
	else
		snprintf(siteName, 100, "%s@%s:%d/%s", 
		WiiSettings.ftpConf[netEditIndex].user, 
		WiiSettings.ftpConf[netEditIndex].ip,
		WiiSettings.ftpConf[netEditIndex].port,
		WiiSettings.ftpConf[netEditIndex].folder);

	swprintf(titleStr, 100, L"%s - %s", gettext("Settings - Network"), gettext(siteName));

	GuiText titleTxt(NULL, 28, (GXColor){255, 255, 255, 255});
	titleTxt.SetWText(titleStr);
	titleTxt.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	titleTxt.SetPosition(49, 100);

	GuiImageData btnOutline(button_png);
	GuiImageData btnOutlineOver(button_over_png);

	GuiText backBtnTxt("Go back", 18, (GXColor){255, 255, 255, 255});
	backBtnTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnTxt.SetPosition(-6, 10);
	GuiImage backBtnImg(btnBottom);
	GuiImage backBtnImgOver(btnBottomOver);
	GuiImage backBtnArrow(arrowRightSmall);
	backBtnArrow.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnArrow.SetPosition(backBtnTxt.GetTextWidth()/2 + 6, 11);
	GuiButton backBtn(screenwidth, btnBottom->GetHeight());
	backBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	backBtn.SetPosition(0, 0);
	backBtn.SetLabel(&backBtnTxt);
	backBtn.SetImage(&backBtnImg);
	backBtn.SetImageOver(&backBtnImgOver);
	backBtn.SetIcon(&backBtnArrow);
	backBtn.SetTrigger(trigA);
	backBtn.SetTrigger(trigB);
	
	GuiText deleteBtnTxt("Delete", 20, (GXColor){255, 255, 255, 255});
	GuiImage deleteBtnImg(&btnOutline);
	GuiImage deleteBtnImgOver(&btnOutlineOver);
	GuiButton deleteBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	deleteBtn.SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	deleteBtn.SetPosition(0, -90);
	deleteBtn.SetLabel(&deleteBtnTxt);
	deleteBtn.SetImage(&deleteBtnImg);
	deleteBtn.SetImageOver(&deleteBtnImgOver);
	deleteBtn.SetTrigger(trigA);
	deleteBtn.SetEffectGrow();

	int size = 7;

	if(netEditIndex >= 0)
		size = 6;

	GuiOptionBrowser optionBrowser(screenwidth, size, &options);
	optionBrowser.SetPosition(0, 150);
	optionBrowser.SetAlignment(ALIGN_LEFT, ALIGN_TOP);

	SuspendGui();
	GuiWindow w(screenwidth, screenheight);
	w.Append(&backBtn);
	
	if(netEditIndex < 0)
	{
		// find a site to put the data into
		for(i=0; i < MAX_SHARES; i++)
		{
			if(WiiSettings.ftpConf[i].ip[0] == 0)
			{
				netEditIndex = i;
				break;
			}
		}
	}
	else
	{
		w.Append(&deleteBtn);
	}
	mainWindow->Append(&optionBrowser);
	mainWindow->Append(&w);
	mainWindow->Append(&titleTxt);
	ResumeGui();

	while(menuCurrent == MENU_SETTINGS_NETWORK_FTP && !guiShutdown)
	{
		usleep(THREAD_SLEEP);

		ret = optionBrowser.GetClickedOption();

		switch (ret)
		{
			case 0:
				OnScreenKeyboard(WiiSettings.ftpConf[netEditIndex].displayname, 40);
				break;
			case 1:
				OnScreenKeyboard(WiiSettings.ftpConf[netEditIndex].ip, 80);
				break;
			case 2:
				OnScreenKeyboard(WiiSettings.ftpConf[netEditIndex].folder, 80);
				break;
			case 3:
				OnScreenKeyboard(WiiSettings.ftpConf[netEditIndex].user, 20);
				break;
			case 4:
				char tmp[20];
				tmp[0] = 0;
				if(OnScreenKeyboard(tmp, 20))
					strcpy(WiiSettings.ftpConf[netEditIndex].pwd, tmp);
				break;
			case 5:
				char tmpPort[6];
				sprintf(tmpPort, "%d", WiiSettings.ftpConf[netEditIndex].port);
				if(OnScreenKeypad(tmpPort, 5))
				{
					if(tmpPort[0] == 0)
						ErrorPrompt("Port cannot be blank!");
					else if(!isnumeric(tmpPort))
						ErrorPrompt("Port is not a number!");
					else
					{
						int port = atoi(tmpPort);
						if(port < 4 || port > 49151)
							ErrorPrompt("Port is outside the allowed range (4-49151)!");
						else
							WiiSettings.ftpConf[netEditIndex].port = port;
					}
				}
				break;
			case 6:
				//WiiSettings.ftpConf[netEditIndex].passive ^= 1;
				break;
		}

		if(ret >= 0 || firstRun)
		{
			firstRun = false;
			snprintf(options.value[0], 40, "%s", WiiSettings.ftpConf[netEditIndex].displayname);
			snprintf(options.value[1], 40, "%s", WiiSettings.ftpConf[netEditIndex].ip);
			snprintf(options.value[2], 40, "%s", WiiSettings.ftpConf[netEditIndex].folder);
			snprintf(options.value[3], 40, "%s", WiiSettings.ftpConf[netEditIndex].user);
			sprintf(options.value[4], "********");
			sprintf(options.value[5], "%d", WiiSettings.ftpConf[netEditIndex].port);
			sprintf(options.value[6], "%s", WiiSettings.ftpConf[netEditIndex].passive ? "Passive" : "Active");
			optionBrowser.TriggerUpdate();
		}

		if(backBtn.GetState() == STATE_CLICKED)
		{
			ChangeMenuNoHistory(MENU_SETTINGS_NETWORK);
		}
		if(deleteBtn.GetState() == STATE_CLICKED)
		{
			deleteBtn.ResetState();
			if (WindowPrompt("Delete Site", gettext("Are you sure that you want to delete this site?"), "OK", "Cancel"))
			{
				WiiSettings.ftpConf[netEditIndex].displayname[0] = 0;
				WiiSettings.ftpConf[netEditIndex].ip[0] = 0;
				WiiSettings.ftpConf[netEditIndex].folder[0] = 0;
				WiiSettings.ftpConf[netEditIndex].user[0] = 0;
				WiiSettings.ftpConf[netEditIndex].pwd[0] = 0;
				WiiSettings.ftpConf[netEditIndex].port = 21;
				WiiSettings.ftpConf[netEditIndex].passive = 0;
				ChangeMenuNoHistory(MENU_SETTINGS_NETWORK);
			}
		}
	}
	SuspendGui();
	mainWindow->Remove(&optionBrowser);
	mainWindow->Remove(&w);
	mainWindow->Remove(&titleTxt);
	CloseFTP(netEditIndex+1);
}

static int GetCodepageIndex()
{
	if(WiiSettings.subtitleCodepage[0] == 0)
		return 0;

	for(int i=1; i < CODEPAGE_SIZE; i++)
		if(strcmp(WiiSettings.subtitleCodepage, codepages[i].cpname) == 0)
			return i;
	return 0;
}

static void CodepageWindow()
{
	GuiWindow promptWindow(556,352);
	promptWindow.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	GuiImageData btnOutline(button_png);
	GuiImageData btnOutlineOver(button_over_png);

	GuiImageData dialogBox(dialogue_box_large_png);
	GuiImage dialogBoxImg(&dialogBox);
	dialogBoxImg.SetAlpha(220);

	GuiText titleTxt("Subtitle Codepage", 28, (GXColor){255, 255, 255, 255});
	titleTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	titleTxt.SetPosition(0,18);

	GuiText okBtnTxt("OK", 20, (GXColor){255, 255, 255, 255});
	GuiImage okBtnImg(&btnOutline);
	GuiImage okBtnImgOver(&btnOutlineOver);
	GuiButton okBtn(btnOutline.GetWidth(), btnOutline.GetHeight());

	okBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	okBtn.SetPosition(20, -25);

	okBtn.SetLabel(&okBtnTxt);
	okBtn.SetImage(&okBtnImg);
	okBtn.SetImageOver(&okBtnImgOver);
	okBtn.SetTrigger(trigA);
	okBtn.SetSelectable(false);
	okBtn.SetEffectGrow();

	GuiText cancelBtnTxt("Cancel", 20, (GXColor){255, 255, 255, 255});
	GuiImage cancelBtnImg(&btnOutline);
	GuiImage cancelBtnImgOver(&btnOutlineOver);
	GuiButton cancelBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	cancelBtn.SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	cancelBtn.SetPosition(-20, -25);
	cancelBtn.SetLabel(&cancelBtnTxt);
	cancelBtn.SetImage(&cancelBtnImg);
	cancelBtn.SetImageOver(&cancelBtnImgOver);
	cancelBtn.SetTrigger(trigA);
	cancelBtn.SetSelectable(false);
	cancelBtn.SetEffectGrow();

	int i;
	OptionList options;
	sprintf(options.name[0], "Default");

	for(i=1; i < CODEPAGE_SIZE; i++)
		sprintf(options.name[i], "%s (%s)", codepages[i].cpname, codepages[i].language);

	options.length = i;

	for(i=0; i < options.length; i++)
	{
		options.value[i][0] = 0;
		options.icon[i] = 0;
	}

	GuiOptionBrowser optionBrowser(544, 6, &options);
	optionBrowser.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	optionBrowser.SetPosition(6, 70);
	optionBrowser.TriggerUpdate();

	promptWindow.Append(&dialogBoxImg);
	promptWindow.Append(&titleTxt);
	promptWindow.Append(&optionBrowser);
	promptWindow.Append(&okBtn);
	promptWindow.Append(&cancelBtn);

	SuspendGui();
	mainWindow->SetState(STATE_DISABLED);
	mainWindow->Append(disabled);
	mainWindow->Append(&promptWindow);
	ResumeGui();

	int save = -1;

	while(save == -1)
	{
		usleep(THREAD_SLEEP);

		if(okBtn.GetState() == STATE_CLICKED)
			save = 1;
		else if(cancelBtn.GetState() == STATE_CLICKED)
			save = 0;
		else if(optionBrowser.GetClickedOption() >= 0)
			save = 1;
	}

	if(save && optionBrowser.GetSelectedOption() >= 0)
		strcpy(WiiSettings.subtitleCodepage, codepages[optionBrowser.GetSelectedOption()].cpname);

	SuspendGui();
	mainWindow->Remove(&promptWindow);
	mainWindow->Remove(disabled);
	mainWindow->SetState(STATE_DEFAULT);
	ResumeGui();
}

static void MenuSettingsSubtitles()
{
	int ret;
	int i = 0;
	bool firstRun = true;
	OptionList options;

	sprintf(options.name[i++], "Visibility");
	sprintf(options.name[i++], "Monospaced");
	sprintf(options.name[i++], "Delay");
	sprintf(options.name[i++], "Language");
	sprintf(options.name[i++], "Codepage");
	sprintf(options.name[i++], "Color");
	sprintf(options.name[i++], "Size");

	options.length = i;
		
	for(i=0; i < options.length; i++)
	{
		options.value[i][0] = 0;
		options.icon[i] = 0;
	}

	GuiText titleTxt("Settings - Subtitles", 28, (GXColor){255, 255, 255, 255});
	titleTxt.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	titleTxt.SetPosition(49, 100);

	GuiText backBtnTxt("Go back", 18, (GXColor){255, 255, 255, 255});
	backBtnTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnTxt.SetPosition(-6, 10);
	GuiImage backBtnImg(btnBottom);
	GuiImage backBtnImgOver(btnBottomOver);
	GuiImage backBtnArrow(arrowRightSmall);
	backBtnArrow.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnArrow.SetPosition(backBtnTxt.GetTextWidth()/2 + 6, 11);
	GuiButton backBtn(screenwidth, btnBottom->GetHeight());
	backBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	backBtn.SetPosition(0, 0);
	backBtn.SetLabel(&backBtnTxt);
	backBtn.SetImage(&backBtnImg);
	backBtn.SetImageOver(&backBtnImgOver);
	backBtn.SetIcon(&backBtnArrow);
	backBtn.SetTrigger(trigA);
	backBtn.SetTrigger(trigB);

	GuiOptionBrowser optionBrowser(screenwidth, 7, &options);
	optionBrowser.SetPosition(0, 150);
	optionBrowser.SetAlignment(ALIGN_LEFT, ALIGN_TOP);

	SuspendGui();
	GuiWindow w(screenwidth, screenheight);
	w.Append(&backBtn);
	mainWindow->Append(&optionBrowser);
	mainWindow->Append(&w);
	mainWindow->Append(&titleTxt);
	ResumeGui();

	if(!subtitleFontFound)
		ErrorPrompt("Subtitle font file not found. Subtitles will not be visible.");

	while(menuCurrent == MENU_SETTINGS_SUBTITLES && !guiShutdown)
	{
		usleep(THREAD_SLEEP);

		ret = optionBrowser.GetClickedOption();

		switch (ret)
		{
			case 0:
				WiiSettings.subtitleVisibility ^= 1;
				break;
			case 1:
				WiiSettings.monofont ^= 1;
				if (WiiSettings.monofont)
					wiiUseAltFont();
				else
					wiiMainFont();
				break;
			case 2:
				char delay[8];
				sprintf(delay, "%.2f", WiiSettings.subtitleDelay);
				if(OnScreenKeypad(delay, 7, true))
				{
					if(delay[0] == 0)
					{
						WiiSettings.subtitleDelay = 0;
					}
					else
					{
						float delayValue = atof(delay);
						if(delayValue >= -60 && delayValue <= 60)
							WiiSettings.subtitleDelay = delayValue;
						else
							ErrorPrompt("Invalid subtitle delay.");
					}
				}
				break;
			case 3:
				LanguageWindow(WiiSettings.subtitleLanguage);
				break;
			case 4:
				CodepageWindow();
				break;
			case 5:
				if(strcmp(WiiSettings.subtitleColor, "FFFFFF00") == 0) // white
					sprintf(WiiSettings.subtitleColor, "00000000"); // black
				else if(strcmp(WiiSettings.subtitleColor, "00000000") == 0) // black
					sprintf(WiiSettings.subtitleColor, "FFFF0000"); // yellow
				else if(strcmp(WiiSettings.subtitleColor, "FFFF0000") == 0) // yellow
					sprintf(WiiSettings.subtitleColor, "FF000000"); // red
				else
					sprintf(WiiSettings.subtitleColor, "FFFFFF00"); // white
				break;
			case 6:
				WiiSettings.subtitleSize += 0.5;
				if(WiiSettings.subtitleSize > 5)
					WiiSettings.subtitleSize = 1;
				break;
		}

		if(ret >= 0 || firstRun)
		{
			firstRun = false;

			sprintf(options.value[0], "%s", WiiSettings.subtitleVisibility ? "On" : "Off");
			sprintf(options.value[1], "%s", WiiSettings.monofont ? "On" : "Off");
			sprintf(options.value[2], "%.2f %s", WiiSettings.subtitleDelay, gettext("sec"));
			strcpy(options.value[3], languages[GetLangIndex(WiiSettings.subtitleLanguage)].language);
			if(GetCodepageIndex() == 0)
				sprintf(options.value[4], "Default");
			else
				sprintf(options.value[4], "%s (%s)", codepages[GetCodepageIndex()].cpname, codepages[GetCodepageIndex()].language);

			if(strcmp(WiiSettings.subtitleColor, "FFFFFF00") == 0)
				sprintf(options.value[5], "White");
			else if(strcmp(WiiSettings.subtitleColor, "00000000") == 0)
				sprintf(options.value[5], "Black");
			else if(strcmp(WiiSettings.subtitleColor, "FFFF0000") == 0)
				sprintf(options.value[5], "Yellow");
			else
				sprintf(options.value[5], "Red");

			sprintf(options.value[6], "%.1f", WiiSettings.subtitleSize);

			optionBrowser.TriggerUpdate();
		}

		if(backBtn.GetState() == STATE_CLICKED)
		{
			ChangeMenuNoHistory(MENU_SETTINGS);
		}
	}
	SuspendGui();
	mainWindow->Remove(&optionBrowser);
	mainWindow->Remove(&w);
	mainWindow->Remove(&titleTxt);
}

/****************************************************************************
 * MenuSettings
 ***************************************************************************/
static void MenuSettings()
{
	int ret;
	int i = 0;
	
	OptionList options;

	sprintf(options.name[i++], "Global");
	sprintf(options.name[i++], "Videos");
	sprintf(options.name[i++], "Music");
	sprintf(options.name[i++], "DVD");
	sprintf(options.name[i++], "Online Media");
	sprintf(options.name[i++], "Network");
	sprintf(options.name[i++], "Subtitles");

	options.length = i;

	for(i=0; i < options.length; i++)
	{
		options.value[i][0] = 0;
		options.icon[i] = 0;
	}

	GuiText titleTxt("Settings", 28, (GXColor){255, 255, 255, 255});
	titleTxt.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	titleTxt.SetPosition(49, 100);

	GuiText backBtnTxt("Go back", 18, (GXColor){255, 255, 255, 255});
	backBtnTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnTxt.SetPosition(-6, 10);
	GuiImage backBtnImg(btnBottom);
	GuiImage backBtnImgOver(btnBottomOver);
	GuiImage backBtnArrow(arrowRightSmall);
	backBtnArrow.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtnArrow.SetPosition(backBtnTxt.GetTextWidth()/2 + 6, 11);
	GuiButton backBtn(screenwidth, btnBottom->GetHeight());
	backBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	backBtn.SetPosition(0, 0);
	backBtn.SetLabel(&backBtnTxt);
	backBtn.SetImage(&backBtnImg);
	backBtn.SetImageOver(&backBtnImgOver);
	backBtn.SetIcon(&backBtnArrow);
	backBtn.SetTrigger(trigA);
	backBtn.SetTrigger(trigB);

	GuiOptionBrowser optionBrowser(screenwidth, 8, &options);
	optionBrowser.SetPosition(0, 150);
	optionBrowser.TriggerUpdate();

	SuspendGui();
	mainWindow->Append(&optionBrowser);
	mainWindow->Append(&backBtn);
	mainWindow->Append(&titleTxt);
	ResumeGui();

	while(menuCurrent == MENU_SETTINGS && !guiShutdown)
	{
		usleep(THREAD_SLEEP);

		ret = optionBrowser.GetClickedOption();

		switch (ret)
		{
			case 0:
				ChangeMenuNoHistory(MENU_SETTINGS_GLOBAL);
				break;
			case 1:
				ChangeMenuNoHistory(MENU_SETTINGS_VIDEOS);
				break;
			case 2:
				ChangeMenuNoHistory(MENU_SETTINGS_MUSIC);
				break;
			case 3:
				ChangeMenuNoHistory(MENU_SETTINGS_DVD);
				break;
			case 4:
				ChangeMenuNoHistory(MENU_SETTINGS_ONLINEMEDIA);
				break;
			case 5:
				ChangeMenuNoHistory(MENU_SETTINGS_NETWORK);
				break;
			case 6:
				ChangeMenuNoHistory(MENU_SETTINGS_SUBTITLES);
				break;
		}

		if(backBtn.GetState() == STATE_CLICKED)
			ChangeMenu(menuPrevious);
	}
	SuspendGui();
	mainWindow->Remove(&optionBrowser);
	mainWindow->Remove(&backBtn);
	mainWindow->Remove(&titleTxt);
}

static void VideoProgressCallback(void *ptr)
{
	GuiButton *b = (GuiButton *)ptr;

	double total = wiiGetTimeLength();
	double done = wiiGetTimePos();
	double percent = 0;
	
	if(total > 0)
		percent = done/total;
	
	if(b->GetState() == STATE_CLICKED)
	{
		if(b->GetStateChan() >= 0)
		{
			percent = (userInput[b->GetStateChan()].wpad->ir.x - b->GetLeft())/560.0;
			if(percent > 1.0) percent = 1.0;
			else if(percent < 0) percent = 0;
			done = total*percent;
			ShutoffRumble();
			wiiSeekPos((int)done);
		}
		b->ResetState();
	}
	
	if(percent <= 0.02)
	{
		videobarProgressLeftImg->SetVisible(false);
		videobarProgressMidImg->SetTile(0);
		videobarProgressLineImg->SetVisible(false);
		videobarProgressRightImg->SetVisible(false);
	}
	else if(percent >= 0.99)
	{
		videobarProgressLeftImg->SetVisible(true);
		videobarProgressMidImg->SetTile(136);
		videobarProgressLineImg->SetVisible(false);
		videobarProgressRightImg->SetVisible(true);
	}
	else
	{
		int tile = 136*percent;
		videobarProgressLeftImg->SetVisible(true);
		videobarProgressMidImg->SetTile(tile);
		videobarProgressLineImg->SetPosition(8 + tile*4, 60);
		videobarProgressLineImg->SetVisible(true);
		videobarProgressRightImg->SetVisible(false);
	}
	char time[50] = { 0 };
	char frames[50] = { 0 };
	wiiGetTimeDisplay(time);
	wiiGetDroppedFrames(frames);

	if(time[0] == 0)
		videobarTime->SetText(NULL);
	else
		videobarTime->SetText(time);

	if(WiiSettings.debug == 2)
		videobarDropped->SetText(frames);
}

static void VideoVolumeLevelCallback(void *ptr)
{
	GuiButton *b = (GuiButton *)ptr;

	if(b->GetState() == STATE_CLICKED)
	{
		if(b->GetStateChan() >= 0)
		{
			WiiSettings.volume = 100 - 100*((userInput[b->GetStateChan()].wpad->ir.y-5 - b->GetTop())/140.0);

			if(WiiSettings.volume > 94)
				WiiSettings.volume = 100;
			else if(WiiSettings.volume < 8)
				WiiSettings.volume = 0;

			wiiSetVolume(WiiSettings.volume);
		}
		b->ResetState();
	}

	float percent = WiiSettings.volume/100.0;
	int tile = 32*percent;

	if(percent < 0.05)
	{
		videobarVolumeLevelBottomImg->SetVisible(false);
		videobarVolumeLevelLineImg->SetVisible(false);
		videobarVolumeLevelTopImg->SetVisible(false);
	}
	else if(percent > 0.95)
	{
		videobarVolumeLevelBottomImg->SetVisible(true);
		videobarVolumeLevelLineImg->SetVisible(false);
		videobarVolumeLevelTopImg->SetVisible(true);
		tile = 32;
	}
	else
	{
		videobarVolumeLevelBottomImg->SetVisible(true);
		videobarVolumeLevelLineImg->SetPosition(20, -90-tile*4);
		videobarVolumeLevelLineImg->SetVisible(true);
		videobarVolumeLevelTopImg->SetVisible(false);
	}
	videobarVolumeLevelMidImg->SetTileVertical(tile);
	videobarVolumeLevelMidImg->SetPosition(20, -90-tile*4);
}

bool VideoVolumeLevelBarVisible()
{
	return videobarVolumeLevelBtn->IsVisible();
}

void ShowVideoVolumeLevelBar()
{
	videobarVolumeLevelBtn->SetVisible(true);
	videobarVolumeLevelBtn->SetState(STATE_DEFAULT);
}

void HideVideoVolumeLevelBar()
{
	videobarVolumeLevelBtn->SetVisible(false);
	videobarVolumeLevelBtn->SetState(STATE_DISABLED);
	videobarVolumeLevelTopImg->SetVisible(false);
	videobarVolumeLevelMidImg->SetTileVertical(0);
	videobarVolumeLevelLineImg->SetVisible(false);
	videobarVolumeLevelBottomImg->SetVisible(false);
}

static void VideoVolumeCallback(void *ptr)
{
	GuiButton *b = (GuiButton *)ptr;
	if(b->GetState() == STATE_CLICKED)
	{
		b->ResetState();

		// show/hide volume level bar
		if(videobarVolumeLevelBtn->IsVisible())
			HideVideoVolumeLevelBar();
		else
			ShowVideoVolumeLevelBar();
	}
}

static void VideoBackwardCallback(void *ptr)
{
	GuiButton *b = (GuiButton *)ptr;
	if(b->GetState() == STATE_CLICKED)
	{
		b->ResetState();
		ShutoffRumble();
		wiiRewind();
	}
}

static void VideoPauseCallback(void *ptr)
{
	GuiButton *b = (GuiButton *)ptr;
	if(b->GetState() == STATE_CLICKED)
	{
		b->ResetState();
		ShutoffRumble();
		wiiPause();
	}
}

static void VideoForwardCallback(void *ptr)
{
	GuiButton *b = (GuiButton *)ptr;
	if(b->GetState() == STATE_CLICKED)
	{
		b->ResetState();
		ShutoffRumble();
		wiiFastForward();
	}
}

static void AudioProgressCallback(void *ptr)
{
	GuiButton *b = (GuiButton *)ptr;

	double total = wiiGetTimeLength();
	double done = wiiGetTimePos();
	double percent = 0;

	if(!wiiAudioOnly())
		total = 0; // values are from a loaded video - do not show!

	if(total > 0)
		percent = done/total;

	if(b->GetState() == STATE_CLICKED)
	{
		if(b->GetStateChan() >= 0)
		{
			percent = (userInput[b->GetStateChan()].wpad->ir.x - b->GetLeft())/360.0;
			if(percent > 1.0) percent = 1.0;
			else if(percent < 0) percent = 0;
			done = total*percent;
			wiiSeekPos((int)done);
		}
		b->ResetState();
	}

	if(percent <= 0.01)
	{
		audiobarProgressLeftImg->SetVisible(false);
		audiobarProgressMidImg->SetTile(0);
		audiobarProgressLineImg->SetVisible(false);
		audiobarProgressRightImg->SetVisible(false);
	}
	else if(percent <= 0.03)
	{
		audiobarProgressLeftImg->SetVisible(true);
		audiobarProgressMidImg->SetTile(0);
		audiobarProgressLineImg->SetPosition(8, 60);
		audiobarProgressLineImg->SetVisible(true);
		audiobarProgressRightImg->SetVisible(false);
	}
	else if(percent >= 0.98)
	{
		audiobarProgressLeftImg->SetVisible(true);
		audiobarProgressMidImg->SetTile(86);
		audiobarProgressLineImg->SetVisible(false);
		audiobarProgressRightImg->SetVisible(true);
	}
	else
	{
		audiobarProgressLeftImg->SetVisible(true);
		int tile = 86*percent;
		audiobarProgressMidImg->SetTile(tile);
		audiobarProgressLineImg->SetPosition(8 + tile*4, 60);
		audiobarProgressLineImg->SetVisible(true);
		audiobarProgressRightImg->SetVisible(false);
	}
}

static void AudioVolumeLevelCallback(void *ptr)
{
	GuiButton *b = (GuiButton *)ptr;

	if(b->GetState() == STATE_CLICKED)
	{
		if(b->GetStateChan() >= 0)
		{
			WiiSettings.volume = 100 - 100*((userInput[b->GetStateChan()].wpad->ir.y-5 - b->GetTop())/140.0);

			if(WiiSettings.volume > 94)
				WiiSettings.volume = 100;
			else if(WiiSettings.volume < 8)
				WiiSettings.volume = 0;

			wiiSetVolume(WiiSettings.volume);
		}
		b->ResetState();
	}

	float percent = WiiSettings.volume/100.0;
	int tile = 32*percent;

	if(percent < 0.05)
	{
		audiobarVolumeLevelBottomImg->SetVisible(false);
		audiobarVolumeLevelLineImg->SetVisible(false);
		audiobarVolumeLevelTopImg->SetVisible(false);
	}
	else if(percent > 0.95)
	{
		audiobarVolumeLevelBottomImg->SetVisible(true);
		audiobarVolumeLevelLineImg->SetVisible(false);
		audiobarVolumeLevelTopImg->SetVisible(true);
		tile = 32;
	}
	else
	{
		audiobarVolumeLevelBottomImg->SetVisible(true);
		audiobarVolumeLevelLineImg->SetPosition(320, -90-tile*4);
		audiobarVolumeLevelLineImg->SetVisible(true);
		audiobarVolumeLevelTopImg->SetVisible(false);
	}
	audiobarVolumeLevelMidImg->SetTileVertical(tile);
	audiobarVolumeLevelMidImg->SetPosition(320, -90-tile*4);
}

static void ShowAudioVolumeLevelBar()
{
	audiobarVolumeLevelBtn->SetVisible(true);
	audiobarVolumeLevelBtn->SetState(STATE_DEFAULT);
}

static void HideAudioVolumeLevelBar()
{
	audiobarVolumeLevelBtn->SetVisible(false);
	audiobarVolumeLevelBtn->SetState(STATE_DISABLED);
	audiobarVolumeLevelTopImg->SetVisible(false);
	audiobarVolumeLevelMidImg->SetTileVertical(0);
	audiobarVolumeLevelLineImg->SetVisible(false);
	audiobarVolumeLevelBottomImg->SetVisible(false);
}

static void AudioVolumeCallback(void *ptr)
{
	GuiButton *b = (GuiButton *)ptr;
	if(b->GetState() == STATE_CLICKED)
	{
		b->ResetState();

		// show/hide volume level bar
		if(audiobarVolumeLevelBtn->IsVisible())
			HideAudioVolumeLevelBar();
		else
			ShowAudioVolumeLevelBar();
	}
}

static void AudioNowPlayingCallback(void *ptr)
{
	if(nowPlayingSet)
		return;

	if(!wiiAudioOnly())
	{
		if(audiobarNowPlayingBtn->IsVisible())
			audiobarNowPlayingBtn->SetVisible(false);
		return;
	}
	else
	{
		if(!audiobarNowPlayingBtn->IsVisible())
			audiobarNowPlayingBtn->SetVisible(true);
	}

	double total = wiiGetTimeLength();

	// display ICY data
	if(total <= 0.01)
	{
		if(strncmp(loadedFile, "http:", 5) == 0)
		{
			if(streamtitle_changed)
			{
				if(streamtitle[0] != 0)
				{
					char *dash = strchr(streamtitle,'-');  
					if(dash != NULL)
					{
						char artist[128];
						snprintf(artist, dash-streamtitle+1, "%s", streamtitle);
						audiobarNowPlaying[1]->SetText(artist);
						++dash;
						while(dash[0] == ' ') ++dash;
						if(strlen(dash) == 0) dash = NULL;
						audiobarNowPlaying[2]->SetText(dash);
					}
					else
					{
						audiobarNowPlaying[1]->SetText(streamtitle);
						audiobarNowPlaying[2]->SetText(NULL);
					}
				}
				else
				{
					audiobarNowPlaying[1]->SetText("Internet Stream");
					audiobarNowPlaying[2]->SetText(NULL);
				}
				streamtitle_changed = 0;
			}
			if(streamname_changed)
			{
				if(streamname[0] != 0)
					audiobarNowPlaying[3]->SetText(streamname);
				else
					audiobarNowPlaying[3]->SetText(NULL);
				streamname_changed = 0;
			}
		}
		return;
	}

	if(!nowPlayingSet && total > 0)
	{
		nowPlayingSet = true;
		char *title = NULL;
		char *artist = NULL;
		char *album = NULL;
		char *year = NULL;
		int i;

		audiobarNowPlaying[2]->SetText(NULL);
		audiobarNowPlaying[3]->SetText(NULL);

		title=wiiGetMetaTitle();
		if(title != NULL)
		{
			for(i=strlen(title)-1; i >= 0; i--)	if(title[i] == ' ') title[i] = '\0'; else break;

			if(strlen(title) > 0)
				audiobarNowPlaying[1]->SetText(title);
		}
		if(!title || title[0] == 0)
		{
			char tmp[MAXJOLIET+1];
			char *start = strrchr(loadedFile,'/');
			if(start != NULL) // start up starting part of path
				strcpy(tmp, &start[1]);
			else
				strcpy(tmp, loadedFile);
			StripExt(tmp);
			audiobarNowPlaying[1]->SetText(tmp);
		}

		artist = wiiGetMetaArtist();
		if(artist != NULL)
		{
			for(i=strlen(artist)-1; i >= 0; i--) if(artist[i] == ' ') artist[i] = '\0'; else break;
			
			if(strlen(artist) > 0)
				audiobarNowPlaying[2]->SetText(artist);
		}

		album = wiiGetMetaAlbum();
		if(album != NULL)
		{
			for(i=strlen(album)-1; i >= 0; i--) if(album[i] == ' ') album[i] = '\0'; else break;
		}

		year = wiiGetMetaYear();
		if(year != NULL)
		{
			for(i=strlen(year)-1; i >= 0; i--) if(year[i] == ' ') year[i] = '\0'; else break;
		}

		if(album && strlen(album) > 0 && year && strlen(year) > 0)
		{
			char *txt=(char*)mem2_malloc(sizeof(char)*1024, MEM2_GUI);

			if(txt)
			{
				sprintf(txt, "%s (%s)", album, year);
				audiobarNowPlaying[3]->SetText(txt);
				mem2_free(txt, MEM2_GUI);
			}
		}
		else if(album && strlen(album) > 0)
		{
			audiobarNowPlaying[3]->SetText(album);
		}
		else if(year && strlen(year) > 0)
		{
			audiobarNowPlaying[3]->SetText(year);
		}

		if(year) free(year);
		if(album) free(album);
		if(artist) free(artist);
		if(title) free(title);
	}
}

static void SetupGui()
{	
	static bool guiSetup = false;
	
	if(guiSetup)
		return;

	// pointers

	pointerImg[0] = player1_point_png;
	pointerImg[1] = player2_point_png;
	pointerImg[2] = player3_point_png;
	pointerImg[3] = player4_point_png;
	pointerGrabImg[0] = grab_png;
	pointerGrabImg[1] = grab_png;
	pointerGrabImg[2] = grab_png;
	pointerGrabImg[3] = grab_png;
	pointer[0] = new GuiImageData(pointerImg[0]);
	pointer[1] = new GuiImageData(pointerImg[1]);
	pointer[2] = new GuiImageData(pointerImg[2]);
	pointer[3] = new GuiImageData(pointerImg[3]);

	// triggers

	trigA = new GuiTrigger;
	trigA->SetSimpleTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	trigHeldA = new GuiTrigger;
	trigHeldA->SetHeldTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	trigB = new GuiTrigger;
	trigB->SetButtonOnlyTrigger(-1, WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B, PAD_BUTTON_B);
	
	trigLeft = new GuiTrigger;
	trigLeft->SetButtonOnlyTrigger(-1, WPAD_BUTTON_LEFT | WPAD_CLASSIC_BUTTON_LEFT, PAD_BUTTON_LEFT);
		
	trigRight = new GuiTrigger;
	trigRight->SetButtonOnlyTrigger(-1, WPAD_BUTTON_RIGHT | WPAD_CLASSIC_BUTTON_RIGHT, PAD_BUTTON_RIGHT);
	
	trigUp = new GuiTrigger;
	trigUp->SetButtonOnlyTrigger(-1, WPAD_BUTTON_UP | WPAD_CLASSIC_BUTTON_UP, PAD_BUTTON_UP);

	trigDown = new GuiTrigger;
	trigDown->SetButtonOnlyTrigger(-1, WPAD_BUTTON_DOWN | WPAD_CLASSIC_BUTTON_DOWN, PAD_BUTTON_DOWN);

	trigPlus = new GuiTrigger;
	trigPlus->SetButtonOnlyTrigger(-1, WPAD_BUTTON_PLUS | WPAD_CLASSIC_BUTTON_PLUS, PAD_BUTTON_Y);

	trigMinus = new GuiTrigger;
	trigMinus->SetButtonOnlyTrigger(-1, WPAD_BUTTON_MINUS | WPAD_CLASSIC_BUTTON_MINUS, PAD_BUTTON_X);

	// images
	throbber = new GuiImageData(throbber_png);
	progressLeft = new GuiImageData(progressbar_left_png);
	progressMid = new GuiImageData(progressbar_mid_png);
	progressRight = new GuiImageData(progressbar_right_png);
	progressEmpty = new GuiImageData(progressbar_empty_png);
	progressShortEmpty = new GuiImageData(progressbar_short_empty_png);
	progressLongEmpty = new GuiImageData(progressbar_long_empty_png);
	progressLine = new GuiImageData(progressbar_line_png);
	volumeTop = new GuiImageData(volume_top_png);
	volumeMid = new GuiImageData(volume_mid_png);
	volumeBottom = new GuiImageData(volume_bottom_png);
	volumeEmpty = new GuiImageData(volume_empty_png);
	volumeLine = new GuiImageData(volume_line_png);
	
	bg = new GuiImageData(bg_jpg, GX_TF_RGBA8);
	bg->SetImage(bg_jpg, bg_jpg_size);
	navDivider = new GuiImageData(nav_divider_png);
	btnBottom = new GuiImageData(button_bottom_png);
	btnBottomOver = new GuiImageData(button_bottom_over_png);
	arrowRightSmall = new GuiImageData(arrow_right_small_png);
	disabled = new GuiImage(screenwidth,screenheight,(GXColor){0, 0, 0, 100});
	actionbarLeft = new GuiImageData(actionbar_left_png);	
	actionbarMid = new GuiImageData(actionbar_mid_png);
	actionbarRight = new GuiImageData(actionbar_right_png);
	actionbarBackward = new GuiImageData(actionbar_backward_png);
	actionbarPause = new GuiImageData(actionbar_pause_png);
	actionbarPlay = new GuiImageData(actionbar_play_png);
	actionbarForward = new GuiImageData(actionbar_forward_png);
	actionbarRotate = new  GuiImageData(actionbar_continuous_png);
	actionbarSingle = new GuiImageData(actionbar_single_png);
	actionbarContinuous = new GuiImageData(actionbar_continuous_png);
	actionbarShuffle = new GuiImageData(actionbar_shuffle_png);
	actionbarLoop = new GuiImageData(actionbar_loop_png);	
	actionbarClose = new GuiImageData(actionbar_close_png);
	actionbarPlaylist = new GuiImageData(actionbar_playlist_png);
	actionbarVolume = new GuiImageData(actionbar_volume_png);
	actionbarThrough = new GuiImageData(actionbar_through_png);

	// video bar

	videobarLeftImg = new GuiImage(actionbarLeft);
	videobarMidImg = new GuiImage(actionbarMid);
	videobarMidImg->SetPosition(40, 0);
	videobarMidImg->SetTile(24); // 20x24 = 480
	videobarRightImg = new GuiImage(actionbarRight);
	videobarRightImg->SetPosition(520, 0);

	videobarProgressImg = new GuiImage(progressLongEmpty);
	videobarProgressLeftImg = new GuiImage(progressLeft);
	videobarProgressLeftImg->SetPosition(0, 60);
	videobarProgressLeftImg->SetVisible(false);
	videobarProgressMidImg = new GuiImage(progressMid);
	videobarProgressMidImg->SetPosition(8, 60);
	videobarProgressMidImg->SetTile(0);
	videobarProgressLineImg = new GuiImage(progressLine);
	videobarProgressLineImg->SetPosition(8, 60);
	videobarProgressRightImg = new GuiImage(progressRight);
	videobarProgressRightImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	videobarProgressRightImg->SetPosition(0, 60);
	videobarProgressRightImg->SetVisible(false);
	videobarVolumeImg = new GuiImage(actionbarVolume);
	videobarVolumeImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	videobarBackwardImg = new GuiImage(actionbarBackward);
	videobarBackwardImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	videobarPauseImg = new GuiImage(actionbarPause);
	videobarPauseImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	videobarForwardImg = new GuiImage(actionbarForward);
	videobarForwardImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	
	videobarVolumeLevelImg = new GuiImage(volumeEmpty);
	videobarVolumeLevelImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	videobarVolumeLevelTopImg = new GuiImage(volumeTop);
	videobarVolumeLevelTopImg->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	videobarVolumeLevelTopImg->SetPosition(20, -220);
	videobarVolumeLevelTopImg->SetVisible(false);
	videobarVolumeLevelMidImg = new GuiImage(volumeMid);
	videobarVolumeLevelMidImg->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	videobarVolumeLevelMidImg->SetTileVertical(0);
	videobarVolumeLevelLineImg = new GuiImage(volumeLine);
	videobarVolumeLevelLineImg->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	videobarVolumeLevelLineImg->SetPosition(20, 0);
	videobarVolumeLevelLineImg->SetVisible(false);
	videobarVolumeLevelBottomImg = new GuiImage(volumeBottom);
	videobarVolumeLevelBottomImg->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	videobarVolumeLevelBottomImg->SetPosition(20, -90);
	videobarVolumeLevelBottomImg->SetVisible(false);
	
	videobarVolumeTip = new GuiTooltip("Volume");
	videobarBackwardTip = new GuiTooltip("Skip Backward");
	videobarPauseTip = new GuiTooltip("Pause");
	videobarForwardTip = new GuiTooltip("Skip Forward");
	
	videobarProgressBtn = new GuiButton(videobarProgressImg->GetWidth(), videobarProgressImg->GetHeight());
	videobarProgressBtn->SetImage(videobarProgressImg);
	videobarProgressBtn->SetPosition(0, 60);
	videobarProgressBtn->SetTrigger(trigA);
	videobarProgressBtn->SetSelectable(false);
	videobarProgressBtn->SetUpdateCallback(VideoProgressCallback);
	
	videobarVolumeLevelBtn = new GuiButton(videobarVolumeLevelImg->GetWidth(), videobarVolumeLevelImg->GetHeight());
	videobarVolumeLevelBtn->SetImage(videobarVolumeLevelImg);
	videobarVolumeLevelBtn->SetPosition(20, -90);
	videobarVolumeLevelBtn->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	videobarVolumeLevelBtn->SetTrigger(trigA);
	videobarVolumeLevelBtn->SetSelectable(false);
	videobarVolumeLevelBtn->SetVisible(false);
	videobarVolumeLevelBtn->SetState(STATE_DISABLED);
	videobarVolumeLevelBtn->SetUpdateCallback(VideoVolumeLevelCallback);

	videobarVolumeBtn = new GuiButton(40, 40);
	videobarVolumeBtn->SetPosition(10, 4);
	videobarVolumeBtn->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	videobarVolumeBtn->SetImage(videobarVolumeImg);
	videobarVolumeBtn->SetTooltip(videobarVolumeTip);
	videobarVolumeBtn->SetTrigger(trigA);
	videobarVolumeBtn->SetSelectable(false);
	videobarVolumeBtn->SetUpdateCallback(VideoVolumeCallback);
	videobarVolumeBtn->SetEffectGrow();

	videobarBackwardBtn = new GuiButton(40, 40);
	videobarBackwardBtn->SetPosition(-60, 4);
	videobarBackwardBtn->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	videobarBackwardBtn->SetImage(videobarBackwardImg);
	videobarBackwardBtn->SetTooltip(videobarBackwardTip);
	videobarBackwardBtn->SetTrigger(trigA);
	videobarBackwardBtn->SetSelectable(false);
	videobarBackwardBtn->SetUpdateCallback(VideoBackwardCallback);
	videobarBackwardBtn->SetEffectGrow();

	videobarPauseBtn = new GuiButton(40, 40);
	videobarPauseBtn->SetPosition(0, 4);
	videobarPauseBtn->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	videobarPauseBtn->SetImage(videobarPauseImg);
	videobarPauseBtn->SetTooltip(videobarPauseTip);
	videobarPauseBtn->SetTrigger(trigA);
	videobarPauseBtn->SetSelectable(false);
	videobarPauseBtn->SetUpdateCallback(VideoPauseCallback);
	videobarPauseBtn->SetEffectGrow();
	
	videobarForwardBtn = new GuiButton(40, 40);
	videobarForwardBtn->SetPosition(60, 4);
	videobarForwardBtn->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	videobarForwardBtn->SetImage(videobarForwardImg);
	videobarForwardBtn->SetTooltip(videobarForwardTip);
	videobarForwardBtn->SetTrigger(trigA);
	videobarForwardBtn->SetSelectable(false);
	videobarForwardBtn->SetUpdateCallback(VideoForwardCallback);
	videobarForwardBtn->SetEffectGrow();
	
	videobarTime = new GuiText(NULL, 16, (GXColor){255, 255, 255, 255});
	videobarTime->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	videobarTime->SetPosition(-20, 14);
	
	videobarDropped = new GuiText(NULL, 16, (GXColor){255, 255, 255, 255});
	videobarDropped->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	videobarDropped->SetPosition(50, 14);

	videobar = new GuiWindow(560, 80);

	videobar->Append(videobarLeftImg);
	videobar->Append(videobarMidImg);
	videobar->Append(videobarRightImg);
	videobar->Append(videobarProgressBtn);
	videobar->Append(videobarProgressLeftImg);
	videobar->Append(videobarProgressMidImg);
	videobar->Append(videobarProgressLineImg);
	videobar->Append(videobarProgressRightImg);
	videobar->Append(videobarVolumeBtn);
	videobar->Append(videobarVolumeLevelBtn);
	videobar->Append(videobarVolumeLevelTopImg);
	videobar->Append(videobarVolumeLevelMidImg);
	videobar->Append(videobarVolumeLevelLineImg);
	videobar->Append(videobarVolumeLevelBottomImg);
	videobar->Append(videobarBackwardBtn);
	videobar->Append(videobarPauseBtn);
	videobar->Append(videobarForwardBtn);
	videobar->Append(videobarTime);
	videobar->Append(videobarDropped);

	videobar->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	videobar->SetPosition(0, -30);
	
	// audio bar

	audiobarLeftImg = new GuiImage(actionbarLeft);
	audiobarMidImg = new GuiImage(actionbarMid);
	audiobarMidImg->SetPosition(40, 0);
	audiobarMidImg->SetTile(14); // 20x14 = 280
	audiobarRightImg = new GuiImage(actionbarRight);
	audiobarRightImg->SetPosition(320, 0);

	audiobarProgressImg = new GuiImage(progressEmpty);
	audiobarProgressLeftImg = new GuiImage(progressLeft);
	audiobarProgressLeftImg->SetPosition(0, 60);
	audiobarProgressLeftImg->SetVisible(false);
	audiobarProgressMidImg = new GuiImage(progressMid);
	audiobarProgressMidImg->SetPosition(8, 60);
	audiobarProgressMidImg->SetTile(0);
	audiobarProgressLineImg = new GuiImage(progressLine);
	audiobarProgressLineImg->SetPosition(12, 60);
	audiobarProgressLineImg->SetVisible(false);
	audiobarProgressRightImg = new GuiImage(progressRight);
	audiobarProgressRightImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	audiobarProgressRightImg->SetPosition(0, 60);
	audiobarProgressRightImg->SetVisible(false);
	audiobarPlaylistImg = new GuiImage(actionbarPlaylist);
	audiobarPlaylistImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	audiobarBackwardImg = new GuiImage(actionbarBackward);
	audiobarBackwardImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	audiobarPauseImg = new GuiImage(actionbarPlay);
	audiobarPauseImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	audiobarForwardImg = new GuiImage(actionbarForward);
	audiobarForwardImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	audiobarModeImg = new GuiImage;
	audiobarModeImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	audiobarVolumeImg = new GuiImage(actionbarVolume);
	audiobarVolumeImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	audiobarVolumeLevelImg = new GuiImage(volumeEmpty);
	audiobarVolumeLevelImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	audiobarVolumeLevelTopImg = new GuiImage(volumeTop);
	audiobarVolumeLevelTopImg->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	audiobarVolumeLevelTopImg->SetPosition(320, -220);
	audiobarVolumeLevelTopImg->SetVisible(false);
	audiobarVolumeLevelMidImg = new GuiImage(volumeMid);
	audiobarVolumeLevelMidImg->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	audiobarVolumeLevelMidImg->SetTileVertical(0);
	audiobarVolumeLevelLineImg = new GuiImage(volumeLine);
	audiobarVolumeLevelLineImg->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	audiobarVolumeLevelLineImg->SetPosition(320, 0);
	audiobarVolumeLevelLineImg->SetVisible(false);
	audiobarVolumeLevelBottomImg = new GuiImage(volumeBottom);
	audiobarVolumeLevelBottomImg->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	audiobarVolumeLevelBottomImg->SetPosition(320, -90);
	audiobarVolumeLevelBottomImg->SetVisible(false);

	audiobarPlaylistTip = new GuiTooltip("Playlist");
	audiobarBackwardTip = new GuiTooltip("Restart");
	audiobarPauseTip = new GuiTooltip("Play");
	audiobarForwardTip = new GuiTooltip("Next");
	audiobarModeTip = new GuiTooltip("Single Play");
	audiobarVolumeTip = new GuiTooltip("Volume");

	UpdateAudiobarModeBtn();

	audiobarProgressBtn = new GuiButton(audiobarProgressImg->GetWidth(), audiobarProgressImg->GetHeight());
	audiobarProgressBtn->SetImage(audiobarProgressImg);
	audiobarProgressBtn->SetPosition(0, 60);
	audiobarProgressBtn->SetTrigger(trigA);
	audiobarProgressBtn->SetSelectable(false);
	audiobarProgressBtn->SetState(STATE_DISABLED);
	audiobarProgressBtn->SetAlpha(128);
	audiobarProgressBtn->SetUpdateCallback(AudioProgressCallback);
	
	audiobarPlaylistBtn = new GuiButton(40, 40);
	audiobarPlaylistBtn->SetPosition(10, 4);
	audiobarPlaylistBtn->SetImage(audiobarPlaylistImg);
	audiobarPlaylistBtn->SetTooltip(audiobarPlaylistTip);
	audiobarPlaylistBtn->SetTrigger(trigA);
	audiobarPlaylistBtn->SetEffectGrow();

	audiobarBackwardBtn = new GuiButton(40, 40);
	audiobarBackwardBtn->SetPosition(70, 4);
	audiobarBackwardBtn->SetImage(audiobarBackwardImg);
	audiobarBackwardBtn->SetTooltip(audiobarBackwardTip);
	audiobarBackwardBtn->SetTrigger(trigA);
	audiobarBackwardBtn->SetEffectGrow();
	audiobarBackwardBtn->SetState(STATE_DISABLED);
	audiobarBackwardBtn->SetAlpha(128);

	audiobarPauseBtn = new GuiButton(40, 40);
	audiobarPauseBtn->SetPosition(130, 4);
	audiobarPauseBtn->SetImage(audiobarPauseImg);
	audiobarPauseBtn->SetTooltip(audiobarPauseTip);
	audiobarPauseBtn->SetTrigger(trigA);
	audiobarPauseBtn->SetEffectGrow();
	audiobarPauseBtn->SetState(STATE_DISABLED);
	audiobarPauseBtn->SetAlpha(128);

	audiobarForwardBtn = new GuiButton(40, 40);
	audiobarForwardBtn->SetPosition(190, 4);
	audiobarForwardBtn->SetImage(audiobarForwardImg);
	audiobarForwardBtn->SetTooltip(audiobarForwardTip);
	audiobarForwardBtn->SetTrigger(trigA);
	audiobarForwardBtn->SetEffectGrow();
	audiobarForwardBtn->SetState(STATE_DISABLED);
	audiobarForwardBtn->SetAlpha(128);

	audiobarModeBtn = new GuiButton(40, 40);
	audiobarModeBtn->SetPosition(250, 4);
	audiobarModeBtn->SetImage(audiobarModeImg);
	audiobarModeBtn->SetTooltip(audiobarModeTip);
	audiobarModeBtn->SetTrigger(trigA);
	audiobarModeBtn->SetEffectGrow();
	
	audiobarVolumeLevelBtn = new GuiButton(audiobarVolumeLevelImg->GetWidth(), audiobarVolumeLevelImg->GetHeight());
	audiobarVolumeLevelBtn->SetImage(audiobarVolumeLevelImg);
	audiobarVolumeLevelBtn->SetPosition(320, -90);
	audiobarVolumeLevelBtn->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	audiobarVolumeLevelBtn->SetTrigger(trigA);
	audiobarVolumeLevelBtn->SetSelectable(false);
	audiobarVolumeLevelBtn->SetVisible(false);
	audiobarVolumeLevelBtn->SetState(STATE_DISABLED);
	audiobarVolumeLevelBtn->SetUpdateCallback(AudioVolumeLevelCallback);

	audiobarVolumeBtn = new GuiButton(40, 40);
	audiobarVolumeBtn->SetPosition(310, 4);
	audiobarVolumeBtn->SetImage(audiobarVolumeImg);
	audiobarVolumeBtn->SetTooltip(audiobarVolumeTip);
	audiobarVolumeBtn->SetTrigger(trigA);
	audiobarVolumeBtn->SetUpdateCallback(AudioVolumeCallback);
	audiobarVolumeBtn->SetEffectGrow();
	
	audiobarNowPlaying[0] = new GuiText("now playing", 16, (GXColor){160, 160, 160, 255});
	audiobarNowPlaying[0]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	audiobarNowPlaying[0]->SetPosition(10, 0);
	
	for(int i=1; i < 4; i++)
	{
		audiobarNowPlaying[i] = new GuiText(NULL, 16, (GXColor){255, 255, 255, 255});
		audiobarNowPlaying[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
		audiobarNowPlaying[i]->SetPosition(10, 20*i);
		audiobarNowPlaying[i]->SetMaxWidth(screenwidth-460);
	}

	audiobarNowPlayingBtn = new GuiButton(screenwidth-460, 80);
	audiobarNowPlayingBtn->SetLabel(audiobarNowPlaying[0], 0);
	audiobarNowPlayingBtn->SetLabel(audiobarNowPlaying[1], 1);
	audiobarNowPlayingBtn->SetLabel(audiobarNowPlaying[2], 2);
	audiobarNowPlayingBtn->SetLabel(audiobarNowPlaying[3], 3);
	audiobarNowPlayingBtn->SetSelectable(false);
	audiobarNowPlayingBtn->SetClickable(false);
	audiobarNowPlayingBtn->SetUpdateCallback(AudioNowPlayingCallback);
	audiobarNowPlayingBtn->SetVisible(false);

	audiobar2 = new GuiWindow(360, 80);
	audiobar2->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);

	audiobar2->Append(audiobarLeftImg);
	audiobar2->Append(audiobarMidImg);
	audiobar2->Append(audiobarRightImg);
	audiobar2->Append(audiobarProgressBtn);
	audiobar2->Append(audiobarProgressLeftImg);
	audiobar2->Append(audiobarProgressMidImg);
	audiobar2->Append(audiobarProgressLineImg);
	audiobar2->Append(audiobarProgressRightImg);
	audiobar2->Append(audiobarPlaylistBtn);
	audiobar2->Append(audiobarBackwardBtn);
	audiobar2->Append(audiobarPauseBtn);
	audiobar2->Append(audiobarForwardBtn);
	audiobar2->Append(audiobarModeBtn);
	audiobar2->Append(audiobarVolumeBtn);
	audiobar2->Append(audiobarVolumeLevelBtn);
	audiobar2->Append(audiobarVolumeLevelTopImg);
	audiobar2->Append(audiobarVolumeLevelMidImg);
	audiobar2->Append(audiobarVolumeLevelLineImg);
	audiobar2->Append(audiobarVolumeLevelBottomImg);

	audiobar = new GuiWindow(screenwidth-90, 80);
	audiobar->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	audiobar->SetPosition(0, -30);
	audiobar->Append(audiobar2);

	audiobar->Append(audiobarNowPlayingBtn);

	statusText = new GuiText(NULL, 24, (GXColor){255, 255, 255, 255});
	statusText->SetVisible(false);

	// setup menu window
	menuWindow = new GuiWindow(screenwidth, screenheight);

	videoImg = new GuiImage();
	videoImg->SetImage(videoScreenshot, vmode->fbWidth, vmode->viHeight);
	videoImg->SetScaleX(screenwidth/(float)vmode->fbWidth);
	videoImg->SetVisible(false);
	menuWindow->Append(videoImg);

	bgImg = new GuiImage(bg);
	bgImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	bgImg->SetAlpha(200);
	
	navDividerImg = new GuiImage (navDivider);
	navDividerImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	navDividerImg->SetPosition(0, 85);

	menuWindow->Append(bgImg);
	menuWindow->Append(navDividerImg);

	logoBtnTip = new GuiTooltip ("Credits");
	logo = new GuiImageData (logo_png);
	logoBtnImg = new GuiImage (logo);
	logoBtn = new GuiButton(logo->GetWidth(), logo->GetHeight());
	logoBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	logoBtn->SetPosition(-47, 40);
	logoBtn->SetImage(logoBtnImg);
	logoBtn->SetTrigger(trigA);
	logoBtn->SetSelectable(false);
	logoBtn->SetTooltip(logoBtnTip);
	logoBtn->SetUpdateCallback(DisplayCredits);
	menuWindow->Append(logoBtn);

	navHighlight = new GuiImageData (nav_highlight_png);
	videos = new GuiImageData (nav_videos_png);
	videosOver = new GuiImageData (nav_videos_over_png);
	videosOn = new GuiImageData (nav_videos_on_png);
	music = new GuiImageData (nav_music_png);
	musicOver = new GuiImageData (nav_music_over_png);
	musicOn = new GuiImageData (nav_music_on_png);
	dvd = new GuiImageData (nav_dvd_png);
	dvdOver = new GuiImageData (nav_dvd_over_png);
	dvdOn = new GuiImageData (nav_dvd_on_png);
	online = new GuiImageData (nav_onlinemedia_png);
	onlineOver = new GuiImageData (nav_onlinemedia_over_png);
	onlineOn = new GuiImageData (nav_onlinemedia_on_png);
	settings = new GuiImageData (nav_settings_png);
	settingsOver = new GuiImageData (nav_settings_over_png);
	settingsOn = new GuiImageData (nav_settings_on_png);

	videosBtnTip = new GuiTooltip ("Videos");
	videosBtnImg = new GuiImage(videos);
	videosBtnOnImg = new GuiImage(videosOn);
	videosBtnOverImg = new GuiImage (videosOver);
	videosBtnHighlightImg = new GuiImage (navHighlight);
	videosBtnHighlightImg->SetPosition(-20, 30);
	videosBtnHighlightImg->SetAlpha(128);
	videosBtn = new GuiButton(videosBtnImg->GetWidth(), videosBtnImg->GetHeight());
	videosBtn->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	videosBtn->SetPosition(45, 30);
	videosBtn->SetTooltip(videosBtnTip);
	videosBtn->SetImage(videosBtnImg);
	videosBtn->SetImageOver(videosBtnOverImg);
	videosBtn->SetIconOver(videosBtnHighlightImg);
	videosBtn->SetTrigger(trigA);
	videosBtn->SetSelectable(false);
	videosBtn->SetEffectGrow();
	videosBtn->SetUpdateCallback(ChangeMenuVideos);

	musicBtnTip = new GuiTooltip ("Music");
	musicBtnImg = new GuiImage(music);
	musicBtnOnImg = new GuiImage(musicOn);
	musicBtnOverImg = new GuiImage (musicOver);
	musicBtnHighlightImg = new GuiImage (navHighlight);
	musicBtnHighlightImg->SetPosition(-20, 30);
	musicBtnHighlightImg->SetAlpha(128);
	musicBtn = new GuiButton(musicBtnImg->GetWidth(), musicBtnImg->GetHeight());
	musicBtn->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	musicBtn->SetPosition(100, 30);
	musicBtn->SetTooltip(musicBtnTip);
	musicBtn->SetImage(musicBtnImg);
	musicBtn->SetImageOver(musicBtnOverImg);
	musicBtn->SetIconOver(musicBtnHighlightImg);
	musicBtn->SetTrigger(trigA);
	musicBtn->SetSelectable(false);
	musicBtn->SetEffectGrow();
	musicBtn->SetUpdateCallback(ChangeMenuMusic);

	dvdBtnTip = new GuiTooltip ("DVD");
	dvdBtnImg = new GuiImage(dvd);
	dvdBtnOnImg = new GuiImage(dvdOn);
	dvdBtnOverImg = new GuiImage (dvdOver);
	dvdBtnHighlightImg = new GuiImage (navHighlight);
	dvdBtnHighlightImg->SetPosition(-20, 30);
	dvdBtnHighlightImg->SetAlpha(128);
	dvdBtn = new GuiButton(dvdBtnImg->GetWidth(), dvdBtnImg->GetHeight());
	dvdBtn->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	dvdBtn->SetPosition(155, 30);
	dvdBtn->SetTooltip(dvdBtnTip);
	dvdBtn->SetImage(dvdBtnImg);
	dvdBtn->SetImageOver(dvdBtnOverImg);
	dvdBtn->SetIconOver(dvdBtnHighlightImg);
	dvdBtn->SetTrigger(trigA);
	dvdBtn->SetSelectable(false);
	dvdBtn->SetEffectGrow();
	dvdBtn->SetUpdateCallback(ChangeMenuDVD);

	onlineBtnTip = new GuiTooltip ("Online Media");
	onlineBtnImg = new GuiImage(online);
	onlineBtnOnImg = new GuiImage(onlineOn);
	onlineBtnOverImg = new GuiImage (onlineOver);
	onlineBtnHighlightImg = new GuiImage (navHighlight);
	onlineBtnHighlightImg->SetPosition(-20, 30);
	onlineBtnHighlightImg->SetAlpha(128);
	onlineBtn = new GuiButton(onlineBtnImg->GetWidth(), onlineBtnImg->GetHeight());
	onlineBtn->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	onlineBtn->SetPosition(210, 30);
	onlineBtn->SetTooltip(onlineBtnTip);
	onlineBtn->SetImage(onlineBtnImg);
	onlineBtn->SetImageOver(onlineBtnOverImg);
	onlineBtn->SetIconOver(onlineBtnHighlightImg);
	onlineBtn->SetTrigger(trigA);
	onlineBtn->SetSelectable(false);
	onlineBtn->SetEffectGrow();
	onlineBtn->SetUpdateCallback(ChangeMenuOnline);

	settingsBtnTip = new GuiTooltip ("Settings");
	settingsBtnImg = new GuiImage(settings);
	settingsBtnOnImg = new GuiImage(settingsOn);
	settingsBtnOverImg = new GuiImage (settingsOver);
	settingsBtnHighlightImg = new GuiImage (navHighlight);
	settingsBtnHighlightImg->SetPosition(-20, 30);
	settingsBtnHighlightImg->SetAlpha(128);
	settingsBtn = new GuiButton(settingsBtnImg->GetWidth(), settingsBtnImg->GetHeight());
	settingsBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	settingsBtn->SetPosition(-215, 30);
	settingsBtn->SetImage(settingsBtnImg);
	settingsBtn->SetImageOver(settingsBtnOverImg);
	settingsBtn->SetIconOver(settingsBtnHighlightImg);
	settingsBtn->SetTooltip(settingsBtnTip);
	settingsBtn->SetTrigger(trigA);
	settingsBtn->SetSelectable(false);
	settingsBtn->SetEffectGrow();
	settingsBtn->SetUpdateCallback(ChangeMenuSettings);

	menuWindow->Append(videosBtn);
	menuWindow->Append(musicBtn);
	menuWindow->Append(dvdBtn);
	menuWindow->Append(onlineBtn);
	menuWindow->Append(settingsBtn);

	nowPlaying = new GuiText(NULL, 18, (GXColor){255, 255, 255, 255});
	nowPlaying->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	nowPlaying->SetPosition(45, 10);
	nowPlaying->SetMaxWidth(screenwidth-180);
	nowPlaying->SetVisible(false);

	// setup MPlayer window
	mplayerWindow = new GuiWindow(screenwidth, screenheight);
	mplayerWindow->Append(videobar);
	mplayerWindow->Append(statusText);

	guiSetup = true;
}

static void StartGuiThreads()
{
	showProgress = 0;
	guiHalt = 1;
	progressThreadHalt = 1;
	screensaverThreadHalt = 1;
	creditsThreadHalt = 1;

	LWP_CreateThread (&guithread, GuiThread, NULL, guistack, GSTACK, 60);
	LWP_CreateThread (&progressthread, ProgressThread, NULL, progressstack, GUITH_STACK, 60);
	LWP_CreateThread (&screensaverthread, ScreensaverThread, NULL, screensaverstack, GUITH_STACK, 60);
	LWP_CreateThread (&creditsthread, CreditsThread, NULL, creditsstack, GUITH_STACK, 60);
}

static void StopGuiThreads()
{
	showProgress = 0;
	progressThreadHalt = 2;

	if(progressthread != LWP_THREAD_NULL)
	{
		if(LWP_ThreadIsSuspended(progressthread))
			LWP_ResumeThread (progressthread);
		
		// wait for thread to finish
		LWP_JoinThread(progressthread, NULL);
		progressthread = LWP_THREAD_NULL;
	}

	screensaverThreadHalt = 2;

	if(screensaverthread != LWP_THREAD_NULL)
	{
		while(!LWP_ThreadIsSuspended(screensaverthread))
			usleep(THREAD_SLEEP);

		LWP_ResumeThread (screensaverthread);

		// wait for thread to finish
		LWP_JoinThread(screensaverthread, NULL);
		screensaverthread = LWP_THREAD_NULL;
	}

	creditsThreadHalt = 2;

	if(creditsthread != LWP_THREAD_NULL)
	{
		while(!LWP_ThreadIsSuspended(creditsthread))
			usleep(THREAD_SLEEP);

		LWP_ResumeThread (creditsthread);

		// wait for thread to finish
		LWP_JoinThread(creditsthread, NULL);
		creditsthread = LWP_THREAD_NULL;
	}

	guiHalt = 2;

	if(guithread != LWP_THREAD_NULL)
	{
		if(LWP_ThreadIsSuspended(guithread))
			LWP_ResumeThread (guithread);
		
		// wait for thread to finish
		LWP_JoinThread(guithread, NULL);
		guithread = LWP_THREAD_NULL;
	}
}

/****************************************************************************
 * Menu
 ***************************************************************************/

void WiiMenu()
{
	static bool firstboot = true;
	menuMode = 0; // switch to normal GUI mode
	guiShutdown = false;
	FrameTimer = 1;

	SetupGui(); // only once

	// Blacken the screen if we autoboot a file
	if(firstboot)
	{
		if(loadedFile[0] != 0)
		{
			VIDEO_SetBlack(TRUE);
		}
	}

	mainWindow = menuWindow;
	mainWindow->Remove(disabled);
	mainWindow->SetState(STATE_DEFAULT);

	selectLoadedFile = videoImg->IsVisible();

	StartGuiThreads();
	EnableRumble();

	usleep(2000);
	// Load settings (only happens once)
	if(!LoadSettings())
	{
		if(!SaveSettings(NOTSILENT))
		{
			ExitRequested = true;
			return;
		}
	}

	if(menuCurrent == -1)
	{
		menuCurrent = WiiSettings.startArea;
		menuPrevious = WiiSettings.startArea;
		menuUndo = WiiSettings.startArea;
	}
	
	UpdateMenuImages(-1, menuCurrent);

	if(WiiSettings.dvdDisabled)
	{
		dvdBtn->SetVisible(false);
		dvdBtn->SetState(STATE_DISABLED);
		onlineBtn->SetPosition(155, 30);
	}

	// Init MPlayer path and vars (only happens once)
	if(!InitMPlayer())
	{
		ExitRequested = true;
		return;
	}
	
	// Try to autoboot the file sent by the plugin
	if(firstboot)
	{
		firstboot = false;

		if(loadedFile[0] != 0)
		{
			guiShutdown = true;
			sleep(2); // USB mount delay

			LoadNewFile();

			SuspendParseThread();
			SuspendGui();
		}
		VIDEO_SetBlack(FALSE);
	}
	
	usleep(500);
	ResumeGui();

	ResumeDeviceThread();
	ResumeParseThread();

	usleep(500);

	static bool checkIOS = true;

	if(checkIOS)
	{
		u32 ios = IOS_GetVersion();

		if(!SupportedIOS(ios))
			ErrorPrompt("The current IOS is unsupported. Functionality and/or stability may be adversely affected.");
		//else if(!SaneIOS(ios))
			//ErrorPrompt("The current IOS has been altered (fake-signed). Functionality and/or stability may be adversely affected.");
	}

	checkIOS = false;

	while(!guiShutdown)
	{
		switch (menuCurrent)
		{
			case MENU_BROWSE_VIDEOS:
			case MENU_BROWSE_MUSIC:
			case MENU_BROWSE_ONLINEMEDIA:				
				MenuBrowse(menuCurrent);
				break;
			case MENU_DVD:
				MenuDVD();
				break;
			case MENU_SETTINGS:
				MenuSettings();
				break;
			case MENU_SETTINGS_GLOBAL:
				MenuSettingsGlobal();
				break;
			case MENU_SETTINGS_VIDEOS:
				MenuSettingsVideos();
				break;
			case MENU_SETTINGS_MUSIC:
				MenuSettingsMusic();
				break;
			case MENU_SETTINGS_ONLINEMEDIA:
				MenuSettingsOnlineMedia();
				break;
			case MENU_SETTINGS_DVD:
				MenuSettingsDVD();
				break;
			case MENU_SETTINGS_NETWORK:
				MenuSettingsNetwork();
				break;
			case MENU_SETTINGS_NETWORK_SMB:
				MenuSettingsNetworkSMB();
				break;
			case MENU_SETTINGS_NETWORK_FTP:
				MenuSettingsNetworkFTP();
				break;
			case MENU_SETTINGS_SUBTITLES:
				MenuSettingsSubtitles();
				break;
			default: // unrecognized menu
				MenuBrowse(MENU_BROWSE_VIDEOS);
				break;
		}

		// leaving settings area
		if(menuPrevious >= MENU_SETTINGS && menuCurrent < MENU_SETTINGS)
		{
			ResumeGui();
			if(WiiSettings.saveExit == 0)
				SaveSettings(NOTSILENT);
			SuspendGui();
		}

		usleep(THREAD_SLEEP);
	}

	StopGuiThreads();
	DisableRumble();
	mainWindow = NULL;
	nowPlaying->SetVisible(false);
	StopDeviceThread();
	StopParseThread();

	if(ExitRequested)
		SaveSettings(SILENT);
}

bool BufferingStatusSet()
{
	return statusText->IsVisible();
}

void SetBufferingStatus(int s)
{
	if(s == 0)
	{
		statusText->SetVisible(false);
		return;
	}

	statusText->SetVisible(true);
	wchar_t txt[50];
	swprintf(txt, 50, L"%s (%02d%%)", gettext("Buffering"), s);
	statusText->SetWText(txt);
}

/****************************************************************************
 * MPlayer Menu
 ***************************************************************************/
void MPlayerMenu()
{
	guiShutdown = false;
	mainWindow = mplayerWindow;

	mainWindow->SetVisible(false);
	mainWindow->SetState(STATE_DISABLED);

	HideVideoVolumeLevelBar();
	
	if(strncmp(loadedFile, "http:", 5) == 0)
	{
		videobar->Remove(videobarBackwardBtn);
		videobar->Remove(videobarForwardBtn);
	}
	else
	{
		videobar->Append(videobarBackwardBtn);
		videobar->Append(videobarForwardBtn);
	}
	
	menuMode = 1; // switch to MPlayer GUI mode
	EnableRumble();

	videobarPauseImg->SetImage(actionbarPause);
	videobarPauseTip->SetText("Pause");

	while(controlledbygui == 0)
	{
		CheckSleepTimer();
		usleep(500000);

		if(isInserted[DEVICE_DVD] && dvdLastUsed && wiiIsPaused() && 
			diff_sec(dvdLastUsed, gettime()) > 60)
		{
			DI_StopMotor();
			dvdLastUsed = 0;
			ISO9660_Unmount("dvd:");
		}
	}

	DisableRumble();
	mainWindow = NULL;
}
