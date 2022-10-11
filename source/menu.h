/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * menu.h
 * Menu flow routines - handles all menu logic
 ***************************************************************************/

#ifndef _MENU_H_
#define _MENU_H_

#include <ogcsys.h>

void WiiMenu();
void MPlayerMenu();
void DisableMainWindow();
void ErrorPrompt(const char * msg);
int ErrorPromptRetry(const char * msg);
void ErrorPrompt(wchar_t * msg);
int ErrorPromptRetry(wchar_t * msg);
void InfoPrompt(const char * msg);
void InfoPrompt(const char *title, const char *msg);
void InfoPrompt(wchar_t * msg);
void InfoPrompt(const char *title, wchar_t *msg);
bool OnScreenKeyboard(char * var, u32 maxlen);
void ShowAction (const char *msg);
void CancelAction();
int WindowPrompt(const char *title, const char *msg, const char *btn1Label, const char *btn2Label);
void ChangeLanguage();
void ChangeTheme();
bool VideoVolumeLevelBarVisible();
void ShowVideoVolumeLevelBar();
void HideVideoVolumeLevelBar();
void UndoChangeMenu();
void UpdateBrowser();

void EnableVideoImg();
bool VideoImgVisible();
bool WiiMenuSuspended();

#ifdef __cplusplus
extern "C" {
#endif
void	DisableVideoImg();
void DoMPlayerGuiDraw();
void ShowProgress (const char *msg, int done, int total);
void SetBufferingStatus(int s);
bool BufferingStatusSet();
#ifdef __cplusplus
}
#endif


extern int menuCurrent;
extern bool menuMode;
extern bool nowPlayingSet;

enum
{
	MENU_BROWSE_VIDEOS,
	MENU_BROWSE_MUSIC,
	MENU_DVD,
	MENU_BROWSE_ONLINEMEDIA,
	MENU_SETTINGS,
	MENU_SETTINGS_GLOBAL,
	MENU_SETTINGS_VIDEOS,
	MENU_SETTINGS_MUSIC,
	MENU_SETTINGS_DVD,
	MENU_SETTINGS_ONLINEMEDIA,
	MENU_SETTINGS_NETWORK,
	MENU_SETTINGS_NETWORK_SMB,
	MENU_SETTINGS_NETWORK_FTP,
	MENU_SETTINGS_SUBTITLES
};

#endif
