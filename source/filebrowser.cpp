/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * filebrowser.cpp
 *
 * Generic file routines - reading, writing, browsing
 ***************************************************************************/

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiiuse/wpad.h>
#include <sys/dir.h>
#include <string>
#include <vector>
using namespace std;

#include "filebrowser.h"
#include "wiimc.h"
#include "menu.h"
#include "fileop.h"
#include "networkop.h"
#include "settings.h"
#include "utils/mem2_manager.h"
#include "utils/gettext.h"

BROWSER browser;
BROWSER browserSubs;
BROWSER browserVideos;
BROWSER browserMusic;
BROWSER browserOnlineMedia;

static std::vector<std::string> browserHistory; // browser.dir history - for nested playlists

void BrowserInit(BROWSER *info)
{
	info->first=info->last=info->selIndex=NULL;
	info->numEntries=0;
	info->pageIndex=0;
	info->dir[0]='\0';
	info->lastdir[0]='\0';
}


/****************************************************************************
 * Reset()
 * Clears the browser memory
 ***************************************************************************/
static void Reset(BROWSER *info)
{
	BROWSERENTRY *i,*n;
	i=info->first;
	while(i!=NULL)
	{
		mem2_free(i->file, MEM2_BROWSER);
		mem2_free(i->url, MEM2_BROWSER);
		mem2_free(i->display, MEM2_BROWSER);
		mem2_free(i->image, MEM2_BROWSER);
		n=i->next;
		mem2_free(i, MEM2_BROWSER);
		i=n;
	}

	info->numEntries = 0;
	info->selIndex = NULL;
	info->pageIndex = 0;
	info->first = NULL;
	info->last = NULL;
}

void SortBrowser(BROWSER *_browser, int ( * comparator ) ( const void *, const void * ) )
{
	/* preform a bubble sort on the list */
	BROWSERENTRY *a = NULL;
	BROWSERENTRY *b = NULL; 
	BROWSERENTRY *e = NULL; 
	BROWSERENTRY *tmp = NULL; 
	BROWSERENTRY *tmp2 = NULL; 
	BROWSERENTRY *head = _browser->first;
	BROWSERENTRY *tail = _browser->last;
	int pos;
	if(!head) return;
	/*  tmp2 <-> a <-> b <-> tmp */
	while(e != head->next) 
	{
		a = head;
		b = a->next;
		while(a != e) 
		{
			if(comparator(a,b)>0)
			{	   
				pos=a->pos;
				tmp = b->next;
				tmp2 = a->prior;
				b->next = a;
				b->prior = tmp2;
				if(tmp2) tmp2->next = b;
				a->next = tmp;
				a->prior = b;
				if(tmp) tmp->prior = a;
				if(!tmp2) head = b;
				if(!tmp) tail = a;
				a->pos=b->pos;
				b->pos=pos;
			} 
			else 
			{
				a = a->next;
			}
			b = a->next;
			if(b == e) e = a;
		}
	}
	_browser->first = head;
	_browser->last = tail;
}

static void ResetSubs() { Reset(&browserSubs); }
void ResetFiles() {	ResetSubs(); Reset(&browser); }
void ResetVideos() { Reset(&browserVideos); }
void ResetMusic() { Reset(&browserMusic); }
void ResetOnlineMedia() { Reset(&browserOnlineMedia); }

static BROWSERENTRY *AddEntry(BROWSER *info)
{
	if(mem2_size(MEM2_BROWSER) < 8192)
		return NULL;

	BROWSERENTRY *last_entry, *new_entry;

	last_entry = info->last;
	new_entry = (BROWSERENTRY *)mem2_calloc(1, sizeof(BROWSERENTRY), MEM2_BROWSER);
	
	if(!new_entry)
		return NULL;

	if(last_entry==NULL)
		info->first = new_entry;
	else
		last_entry->next = new_entry;

	new_entry->prior = last_entry;
	info->last = new_entry;
	new_entry->pos=info->numEntries;
	info->numEntries++;
	return new_entry;
}

BROWSERENTRY * AddEntryFiles() { return AddEntry(&browser); }
BROWSERENTRY * AddEntrySubs() { return AddEntry(&browserSubs); }
BROWSERENTRY * AddEntryVideos() { return AddEntry(&browserVideos); }
BROWSERENTRY * AddEntryMusic() { return AddEntry(&browserMusic); }
BROWSERENTRY * AddEntryOnlineMedia() { return AddEntry(&browserOnlineMedia); }

bool DeleteEntry(BROWSER *info, BROWSERENTRY *entry)
{
	BROWSERENTRY *i,*p,*n;
	if(!entry) return false;
	i = info->first;
	while(i)
	{
		if (i == entry)
		{
			p=i->prior;
			n=i->next;
			if(!p) //first
				info->first = n;
			else 
				p->next=n;
			if(!n) //last	
				info->last =p;
			else
				n->prior=p;
			
			mem2_free(i->file, MEM2_BROWSER);
			mem2_free(i->url, MEM2_BROWSER);
			mem2_free(i->display, MEM2_BROWSER);
			mem2_free(i->image, MEM2_BROWSER);
			mem2_free(i, MEM2_BROWSER);
				
			return true;
		}
		i=i->next;
	}	
	return false;
}

bool DeleteEntryFiles(BROWSERENTRY *entry) { return DeleteEntry(&browser, entry); }
bool DeleteEntrySubs(BROWSERENTRY *entry) { return DeleteEntry(&browserSubs, entry); }
bool DeleteEntryVideos(BROWSERENTRY *entry) { return DeleteEntry(&browserVideos, entry); }
bool DeleteEntryMusic(BROWSERENTRY *entry) { return DeleteEntry(&browserMusic, entry); }
bool DeleteEntryOnlineMedia(BROWSERENTRY *entry) { return DeleteEntry(&browserOnlineMedia, entry); }

int EntryDistance(BROWSERENTRY * p1,BROWSERENTRY * p2)
{
	int pos;
	if(!p1 || !p2) return 0;
	pos= p1->pos - p2->pos;
	if(pos<0) return -pos;
	return pos;
}

int EntryPosition(BROWSER *info, BROWSERENTRY * i)
{
	if(!i)
		return -1;
	return i->pos;
}

BROWSERENTRY * PositionToEntry(BROWSER *info, int pos)
{
	BROWSERENTRY *entry;
	if(!info) return NULL;
	entry=info->first;
	for(int j=0;entry && j<pos;j++)
	{
		entry = entry->next;
	}
	return entry;
}

static BROWSERENTRY *sub_list;

extern "C" {

void BrowserGetSubInit()
{
	sub_list=browserSubs.first;
}

char * BrowserGetSub(char *filename)
{
	if(!sub_list || !sub_list->file)
		return NULL;

	strcpy(filename,sub_list->file);
	sub_list=sub_list->next;

	return filename;
}

int BrowserFindSub(char *path)
{
	if(!path || path[0] == 0)
		return 0;

	BROWSERENTRY *i;
	i = browserSubs.first;
	while(i)
	{
		if (strcasecmp(i->file, path) == 0)
			return 1;
		i=i->next;
	}	

	return 0;
}

}

void BrowserHistoryStore(char *path)
{
	string newStr(path);
	browserHistory.push_back(newStr);
}

const char * BrowserHistoryRetrieve()
{
	if(browserHistory.empty())
		return "";

	return browserHistory.back().c_str();
}

void BrowserHistoryDiscard()
{
	if(browserHistory.empty())
		return;

	browserHistory.pop_back();
}

void BrowserHistoryClear()
{
	if(browserHistory.empty())
		return;

	browserHistory.clear();
}

void PopulateVideoPlaylist()
{
	if(!WiiSettings.autoPlayNextVideo)
		return;

	char tmp[MAXPATHLEN];

	BROWSERENTRY *i;
	i=browser.first;
	while(i!=NULL)
	{
		if(!i->file || i->type != TYPE_FILE || strncmp(i->file, "http://www.youtube.com", 22) == 0)
		{
			i=i->next;
			continue;
		}

		BROWSERENTRY *entry = AddEntryVideos();
		if(!entry)
			break;

		GetFullPath(i, tmp);
		entry->file = mem2_strdup(tmp, MEM2_BROWSER);
		if(!entry->file) // no mem
		{
			DeleteEntryVideos(entry);
			break;
		}	
		if(browser.selIndex == i) browserVideos.selIndex = entry;
		i=i->next;
	}
	if(browserVideos.selIndex) browserVideos.selIndex = browserVideos.selIndex->next;
}

/****************************************************************************
 * UpdateDirName()
 * Update curent directory name for file browser
 ***************************************************************************/
static int UpdateDirName()
{
	if(!browser.first || !browser.selIndex)
		return 1;

	// we are in a playlist, and have nowhere to go back to but the device listing
	if(browser.selIndex == browser.first && (!browser.selIndex->file || strlen(browser.selIndex->file) == 0))
	{
		browser.dir[0] = 0;
		BrowserHistoryClear();
		return 1;
	}

	// exiting a playlist - return to specified location
	if(browser.selIndex == browser.first && browser.selIndex->file[0] != '.' && browser.dir[0] != 0)
	{
		strcpy(browser.lastdir, browser.dir);
		strcpy(browser.dir, browser.selIndex->file);
		BrowserHistoryDiscard();
		return 1;
	}
	
	// something went wrong, let's reset everything
	if(!browser.selIndex->file)
	{
		browser.dir[0] = 0;
		BrowserHistoryClear();
		return 1;
	}

	// entering a playlist - this is handled when the playlist is parsed
	if(browser.selIndex->type == TYPE_PLAYLIST || 
		strncmp(browser.selIndex->file, "http:", 5) == 0)
	{
		BrowserHistoryStore(browser.dir);
		GetFullPath(browser.selIndex, browser.dir);
		return 1;
	}

	// current directory doesn't change
	if (strcmp(browser.selIndex->file,".") == 0)
	{
		return 0;
	}
	// go up to parent directory
	else if (strcmp(browser.selIndex->file,"..") == 0)
	{
		// already at the top level
		if(IsDeviceRoot(browser.dir))
		{
			strcpy(browser.lastdir, browser.dir);
			browser.lastdir[strlen(browser.lastdir)-1] = 0; // strip trailing slash
			browser.dir[0] = 0; // remove device - we are going to the device listing screen
		}
		else
		{
			// determine last subdirectory namelength
			int size = strlen(browser.dir);
			browser.dir[--size] = 0; // remove trailing slash
			char *test = strrchr(browser.dir,'/');

			if(test == NULL)
			{
				strcpy(browser.lastdir, browser.dir);
				browser.dir[0] = 0;
			}
			else
			{
				strcpy(browser.lastdir, ++test); // record last folder
				*test = 0; // remove last subdirectory name
			}
		}
		return 1;
	}

	if(menuCurrent != MENU_BROWSE_ONLINEMEDIA && browser.dir[0] == 0)
	{
		// try to switch to device
		if(!ChangeInterface(browser.selIndex->file, NOTSILENT))
			return -1;
	}

	// Open directory

	// test new directory namelength
	if ((strlen(browser.dir)+1+strlen(browser.selIndex->file)) < MAXPATHLEN)
	{
		if(browser.selIndex->type != TYPE_PLAYLIST)
		{
			// update current directory name
			sprintf(browser.dir, "%s%s/",browser.dir, browser.selIndex->file);
		}
		return 1;
	}
	else
	{
		ErrorPrompt("Directory name is too long!");
		return -1;
	}
}

static char parentdir[1024];

char *GetParentDir()
{
	int size = strlen(browser.dir);
	strcpy(parentdir, browser.dir);
	parentdir[--size] = 0;
	char *test = strrchr(parentdir,'/');

	if(test == NULL)
		return (char *)"";

	*test = 0; // remove last subdirectory name
	return parentdir;
}

/****************************************************************************
 * BrowserChangeFolder
 *
 * Update current directory and set new entry list if directory has changed
 ***************************************************************************/
int BrowserChangeFolder(bool updateDir, bool waitParse)
{
	if(updateDir && !UpdateDirName())
		return -1;

	if(!(menuCurrent == MENU_BROWSE_ONLINEMEDIA && IsOnlineMediaPath(browser.dir)))
		CleanupPath(browser.dir);

	SuspendParseThread(); // halt parsing

	bool isPlaylist = false;

	if(browser.dir[0] != 0)
	{
		bool mounted = ChangeInterface(browser.dir, NOTSILENT);
		if(mounted)
		{
			char ext[7];
			GetExt(browser.dir, ext);
			isPlaylist = IsPlaylistExt(ext);
		}
		else if(menuCurrent != MENU_BROWSE_ONLINEMEDIA)
		{
			browser.dir[0] = 0;
		}
	}

	if(isPlaylist || (strlen(browser.dir) > 10 && strncmp(browser.dir,"http:", 5) == 0))
	{
		int res = ParsePlaylistFile();

		if(res > 0)
		{
			UpdateBrowser();
			return browser.numEntries;
		}

		// parsing failed - setup last browser dir
		strcpy(browser.dir, BrowserHistoryRetrieve());
		BrowserHistoryDiscard();

		if(browser.numEntries > 0) // parsing failed, but we held onto the last listing
		{
			UpdateBrowser();
			return res; // so we can return without any more work required
		}
	}

	ResetFiles();

	if(menuCurrent == MENU_BROWSE_ONLINEMEDIA)
		return ParseOnlineMedia();

	if(browser.dir[0] != 0)
		ParseDirectory(waitParse);

	if(browser.numEntries > 0)
	{
		UpdateBrowser();
		return browser.numEntries;
	}

	browser.dir[0] = 0;

	int i;
	char tmp[200];

	if(isInserted[DEVICE_SD])
	{
		for(i=0; i < MAX_DEVICES; i++)
		{
			if(part[DEVICE_SD][i].type > 0)
			{
				BROWSERENTRY *entry;
				entry=AddEntryFiles();
				
				sprintf(tmp, "%s:", part[DEVICE_SD][i].mount);
				entry->file = mem2_strdup(tmp, MEM2_BROWSER);

				if(strlen(part[DEVICE_SD][i].name) > 0)
					sprintf(tmp, "%s - %s", gettext("SD"), part[DEVICE_SD][i].name);
				else if(i == 0 && part[DEVICE_SD][1].type == 0) // only one SD partition
					sprintf(tmp, "SD Card");
				else
					sprintf(tmp, "%s (%d)", gettext("SD Card"), i+1);

				entry->display = mem2_strdup(tmp, MEM2_BROWSER);
				entry->length = 0;
				entry->type = TYPE_FOLDER; // flag this as a dir
				entry->icon = ICON_SD;
			}
		}
	}

	if(isInserted[DEVICE_USB])
	{
		for(i=0; i < MAX_DEVICES; i++)
		{
			if(part[DEVICE_USB][i].type > 0)
			{
				BROWSERENTRY *entry;
				entry=AddEntryFiles();

				sprintf(tmp, "%s:", part[DEVICE_USB][i].mount);
				entry->file = mem2_strdup(tmp, MEM2_BROWSER);

				if(strlen(part[DEVICE_USB][i].name) > 0)
					sprintf(tmp, "%s - %s", gettext("USB"), part[DEVICE_USB][i].name);
				else if(i == 0 && part[DEVICE_USB][1].type == 0) // only one USB partition
					sprintf(tmp, "USB Mass Storage");
				else
					sprintf(tmp, "%s (%d)", gettext("USB Mass Storage"), i+1);

				entry->display = mem2_strdup(tmp, MEM2_BROWSER);
				entry->length = 0;
				entry->type = TYPE_FOLDER; // flag this as a dir
				entry->icon = ICON_USB;
			}
		}
	}

	if(!WiiSettings.dvdDisabled && isInserted[DEVICE_DVD])
	{		
		BROWSERENTRY *entry;
		entry=AddEntryFiles();
		
		entry->file = mem2_strdup("dvd:", MEM2_BROWSER);
		entry->display = mem2_strdup("Data DVD", MEM2_BROWSER);
		entry->length = 0;
		entry->type = TYPE_FOLDER;
		entry->icon = ICON_DVD;
	}

	for(i=0; i < MAX_SHARES; i++)
	{
		if(WiiSettings.smbConf[i].share[0] != 0)
		{
			BROWSERENTRY *entry;
			entry=AddEntryFiles();

			sprintf(tmp, "smb%d:", i+1);
			entry->file = mem2_strdup(tmp, MEM2_BROWSER);
			
			if(WiiSettings.smbConf[i].displayname[0] != 0)
				sprintf(tmp, "%s", WiiSettings.smbConf[i].displayname);
			else
				sprintf(tmp, "%s", WiiSettings.smbConf[i].share);
			
			entry->display = mem2_strdup(tmp, MEM2_BROWSER);
			entry->length = 0;
			entry->type = TYPE_FOLDER; // flag this as a dir
			entry->icon = ICON_SMB;
		}
	}

	for(i=0; i < MAX_SHARES; i++)
	{
		if(WiiSettings.ftpConf[i].ip[0] != 0)
		{
			BROWSERENTRY *entry;
			entry=AddEntryFiles();

			sprintf(tmp, "ftp%d:", i+1);
			entry->file = mem2_strdup(tmp, MEM2_BROWSER);
			
			if(WiiSettings.ftpConf[i].displayname[0] != 0)
				sprintf(tmp, "%s", WiiSettings.ftpConf[i].displayname);
			else
				sprintf(tmp, "%s@%s/%s", WiiSettings.ftpConf[i].user, WiiSettings.ftpConf[i].ip, WiiSettings.ftpConf[i].folder);
			
			entry->display = mem2_strdup(tmp, MEM2_BROWSER);
			entry->length = 0;
			entry->type = TYPE_FOLDER; // flag this as a dir
			entry->icon = ICON_FTP;
		}
	}
	if(browser.lastdir[0] != 0)
		FindDirectory(); // try to find and select the last directory

	UpdateBrowser();
	return browser.numEntries;
}

void GetDisplay(BROWSERENTRY *entry, char *buf, int buflen)
{
	buf[0] = 0;

	if(entry->display)
	{
		snprintf(buf, buflen, "%s", entry->display);
		return;
	}
	snprintf(buf, buflen, "%s", entry->file);

	if(WiiSettings.hideExtensions)
		StripExt(buf);
}
