/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * settings.h
 * Settings save/load to XML file
 ***************************************************************************/

#include "networkop.h"

enum {
	PLAY_SINGLE,
	PLAY_CONTINUOUS,
	PLAY_SHUFFLE,
	PLAY_LOOP,
	PLAY_THROUGH
};

enum {
	ART_NONE = 0,
	ART_TOP,
	ART_SIDE,
	ART_FULL,
	ART_FULL_ALT
};

enum {
	YGG_NONE = 0, // no site art
	YGG_TUNEIN, // site thumbs as backup
	YGG_THUMB, // just thumbs
	YGG_THUMB_LARGE, // just thumbs larger, usually missing
	YGG_HI
};


enum {
	ANISON_TUNEIN_ONLY = 0,
	ANISON_TUNEIN_ANISON = 1, // 200
	ANISON_POSTER = 150, //150
	ANISON_JUST_ANISON = 200, //200
	ANISON_POSTER_HI = 300 //300
};

enum {
	LANG_JAPANESE = 0,
	LANG_ENGLISH,
	LANG_GERMAN,
	LANG_FRENCH,
	LANG_SPANISH,
	LANG_ITALIAN,
	LANG_DUTCH,
	LANG_SIMP_CHINESE,
	LANG_TRAD_CHINESE,
	LANG_KOREAN,
	/*LANG_ROMANIAN,
	LANG_ESTONIAN,
	LANG_BRAZILIAN_PORTUGUESE,
	LANG_HUNGARIAN,
	LANG_POLISH,
	LANG_RUSSIAN,
	LANG_PORTUGUESE,
	LANG_SLOVAK,
	LANG_TAMIL,
	LANG_SWEDISH,
	LANG_DANISH,
	LANG_BULGARIAN,*/
	LANG_LENGTH
};

enum {
	FONT_DEFAULT,
	FONT_GENERIC,
	FONT_SIMP_CHINESE,
	FONT_TRAD_CHINESE,
	FONT_JAPANESE,
	FONT_KOREAN,
	FONT_TAMIL
};

typedef struct _cp {
	const char *cpname;
	const char *language;
} CP;

typedef struct _lang {
	const char *language;
	const char *abbrev;
	const char *abbrev2;
} LANG;

//#define LANGUAGE_SIZE 139
#define LANGUAGE_SIZE 9
#define CODEPAGE_SIZE 28
#define MAX_SHARES 9

extern LANG languages[LANGUAGE_SIZE];
extern CP codepages[CODEPAGE_SIZE];

enum {
	EXIT_AUTO,
	EXIT_WIIMENU,
	EXIT_POWEROFF,
	EXIT_LOADER
};

enum {
	FRAMEDROPPING_DISABLED,
	FRAMEDROPPING_AUTO,
	FRAMEDROPPING_ALWAYS
};

enum {
	AUTOPLAY_OFF,
	AUTOPLAY_ON, // Through
	AUTOPLAY_SHUFFLE,
	AUTOPLAY_LOOP,
	AUTOPLAY_CONTINUOUS
};

enum {
	SUBTITLE_ALIGN_TOP,
	SUBTITLE_ALIGN_CENTER,
	SUBTITLE_ALIGN_BOTTOM
};

struct SWiiSettings {
	// Global
	int 	hideExtensions;
	int 	language;
	int		volume;
	int		exitAction;
	int		rumble;
	int		sleepTimer;
	int		screensaverDelay;
	int		inactivityShutdown;
	int		lockFolders;
	int		startArea;
	int		debug;
	int 	artwork;
	char 	artworkFolder[MAXPATHLEN];
	int 	bannerLimit;
	char 	bannerFolder[MAXPATHLEN];
	int 	jpegQuality;
	int 	night;
	int		screenDim;
	int 	doubleStrike;
	int 	libass;
	int 	saveExit;
	int 	numThumb;
	char* 	yearNum;
	char* 	descTxt;
	int 	force576p;
	int 	tiledRender;
	char 	theme[10];
	// Videos
	float	videoZoomHor; // horizontal zoom amount
	float	videoZoomVert; // vertical zoom amount
	int 	videoXshift;
	int 	videoYshift;
	int 	frameDropping; // FRAMEDROPPING_DISABLED, FRAMEDROPPING_AUTO, FRAMEDROPPING_ALWAYS
	float 	aspectRatio;
	int 	cacheFill;
	char	audioLanguage[3];
	float 	audioDelay; // in secs
	int 	autoResume;
	int 	autoPlayNextVideo;
	int		skipBackward;
	int		skipForward;
	char 	videosFolder[MAXPATHLEN];
	int 	videoFull;
	int 	audioNorm;
	int		videoDf;
	int		viWidth;
	int		skipLoop;
	int		interlaceHandle;
	//int		duplicateFrame;
	//int 	videoDelay;
	// Music
	int 	playOrder; // PLAY_SINGLE, PLAY_CONTINUOUS, PLAY_SHUFFLE, PLAY_LOOP, PLAY_THROUGH
	int 	screensaverArt;
	int 	nativeLoops; //adx/brstm/ogg
	char	musicFolder[MAXPATHLEN];
	// DVD
	int		dvdMenu;
	int		dvdDisabled;
	// Online Media
	int 	onlineCacheFill;
	char	onlinemediaFolder[MAXPATHLEN];
	int 	yggdrasilQuality;
	int 	anisonfmQuality;
	int 	onlineBanners;
	// Network
	SMBSettings smbConf[MAX_SHARES];
	FTPSettings ftpConf[MAX_SHARES];
	// Subtitles
	int		subtitleVisibility; // 0 - off, 1 - on
	float 	subtitleDelay; // in secs
	char	subtitleLanguage[3];
	char	subtitleCodepage[13];
	char	subtitleColor[9];
	float	subtitleSize;
	int		borderstyle;
	float	outline;
	float	shadow;
	int 	bold;
	int 	monofont;
};

int GetLangIndex(char *lang);
void DefaultSettings ();
bool SaveSettings (bool silent);
bool LoadSettings ();

void LoadThumbsFile(char * filepath);
void LoadThumbsFileHTTP(char * filepath);

extern struct SWiiSettings WiiSettings;

const char validVideoExtensions[][7] =
{
	"3gp", "asf", "avi", "bik", "dash", "divx", "dpg", "dv", "dvr-ms", "evo", "f4v", 
	"flc", "fli", "flv", "ifo", "iso", "ivf", "m1v", "m2t", "m2ts", "m2v", 
	"m4v", "mkv", "mov", "mp4", "mp4v", "mpe", "mpeg", "mpg", "mqv", "nsv", 
	"nuv", "ogm", "qt", "rm", "rmvb", "sfd", "swf", "thp", "trp", "ts", "vdr", "vfw",
	"viv", "vob", "vro", "webm", "wmv", "y4m", ""
};

const char validAudioExtensions[][6] = 
{
	"aac", "ac3", "adx", "aif", "aiff", "amr", "ape", "apl", "asf", "bcstm", "bfstm", "brstm",
	"flac", "m4a", "m4b", "m4p", "mka", "mp2", "mp3", "mpc", "oga", "ogg", "opus", "ra", "ram", "rm",
	"shn", "wav", "wma", "wv", ""
};

const char validImageExtensions[][5] = 
{
	"bmp", "gif", "jpeg", "jpg", "png", ""
};

const char validPlaylistExtensions[][5] =
{
	"asx", "m3u", "pls", "plx", "ram", "smil", ""
};

const char validSubtitleExtensions[][6] = 
{
	"utf", "utf8", "utf-8", "sub", "srt", "smi", "rt", "txt", "ssa", "aqt", 
	"jss", "js", "ass", ""
};

const char validInternetProtocols[][5] =
{
	"http", "mms", ""
};
