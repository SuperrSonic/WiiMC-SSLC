/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * filebrowser.h
 *
 * Generic file routines - reading, writing, browsing
 ****************************************************************************/

#ifndef _FILEBROWSER_H_
#define _FILEBROWSER_H_

#include <unistd.h>
#include <gccore.h>

#define MAXJOLIET			255

enum
{
	TYPE_FILE,
	TYPE_FOLDER,
	TYPE_PLAYLIST,
	TYPE_SEARCH
};

typedef struct _bentry
{
	u64 length; // file length
	int type; // TYPE_FILE,	TYPE_FOLDER, TYPE_PLAYLIST, TYPE_SEARCH
	char *file; // full filename
	char *url; // address
	char *display; // name for browser display
	char *image;
	int icon; // icon to display
	int pos;
	_bentry *next;
	_bentry *prior;
} BROWSERENTRY;

typedef struct
{
	char dir[MAXPATHLEN]; // directory path
	int menu; // current menu area
	char lastdir[MAXPATHLEN]; // last browsed directory
	int numEntries; // number of entries in the browser
	BROWSERENTRY * selIndex; // currently selected index
	int pageIndex; // starting index of page display
	BROWSERENTRY *first;
	BROWSERENTRY *last;
} BROWSER;

enum
{
	ICON_NONE,
	ICON_FOLDER,
	ICON_FOLDER_CHECKED,
	ICON_FILE,
	ICON_FILE_CHECKED,
	ICON_CHECK,
	ICON_PLAY,
	ICON_SD,
	ICON_USB,
	ICON_DVD,
	ICON_SMB,
	ICON_FTP
};

extern BROWSER browser;
extern BROWSER browserSubs;
extern BROWSER browserVideos;
extern BROWSER browserMusic;
extern BROWSER browserOnlineMedia;

void BrowserInit(BROWSER *info);
void ResetFiles();
void ResetVideos();
void ResetMusic();
void ResetOnlineMedia();
BROWSERENTRY* AddEntryFiles();
BROWSERENTRY* AddEntrySubs();
BROWSERENTRY* AddEntryVideos();
BROWSERENTRY* AddEntryMusic();
BROWSERENTRY* AddEntryOnlineMedia();
bool DeleteEntryFiles(BROWSERENTRY *entry);
bool DeleteEntrySubs(BROWSERENTRY *entry);
bool DeleteEntryVideos(BROWSERENTRY *entry);
bool DeleteEntryMusic(BROWSERENTRY *entry);
bool DeleteEntryOnlineMedia(BROWSERENTRY *entry);

int EntryDistance(BROWSERENTRY * p1,BROWSERENTRY * p2);
int EntryPosition(BROWSER *info, BROWSERENTRY * i);
BROWSERENTRY * PositionToEntry(BROWSER *info, int pos);
void SortBrowser(BROWSER *_browser, int ( * comparator ) ( const void *, const void * ) );
void BrowserHistoryStore(char *path);
const char * BrowserHistoryRetrieve();
void BrowserHistoryDiscard();
void PopulateVideoPlaylist();
char *GetParentDir();
int BrowserChangeFolder(bool updateDir = true, bool waitParse = false);
int BrowseDevice();
void GetDisplay(BROWSERENTRY *entry, char *buf, int buflen);

#endif
