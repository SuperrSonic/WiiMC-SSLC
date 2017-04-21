/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * musicplaylist.cpp
 * Music playlist
 ***************************************************************************/

#include <gccore.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "wiimc.h"
#include "filebrowser.h"
#include "menu.h"
#include "settings.h"
#include "fileop.h"
#include "utils/mem2_manager.h"
#include "utils/gettext.h"

extern "C" {
#include "mplayer/playtree.h"
}

static int *shuffleList = NULL;
static int shuffleIndex = -1;

BROWSERENTRY *MusicPlaylistGetNextShuffle()
{
	if(shuffleIndex == -1 || shuffleIndex >= browserMusic.numEntries)
	{
		// populate new list
		int i, n, t;
		int num = browserMusic.numEntries;

		mem2_free(shuffleList, MEM2_BROWSER);
		shuffleList = (int *)mem2_malloc(num*sizeof(int), MEM2_BROWSER);

		for(i=0; i < num; i++)
			shuffleList[i] = i;

		// shuffle the list
		for(i = 0; i < num-1; i++)
		{
			n = rand() / (RAND_MAX/(num-i) + 1);

			// swap
			t = shuffleList[i];
			shuffleList[i] = shuffleList[i+n];
			shuffleList[i+n] = t;
		}
		shuffleIndex = 0;
	}

	BROWSERENTRY *s = browserMusic.first;
	for(int i=0; i < shuffleList[shuffleIndex]; i++)
		s = s->next;
	shuffleIndex++;

	return s;
}

/****************************************************************************
 * ParsePlaylist
 * 
 * Loads the files inside the current playlist into the filebrowser
 ***************************************************************************/

int MusicPlaylistLoad()
{
	if(browserMusic.numEntries == 0)
	{
		InfoPrompt("Playlist is Empty", "Hover over items and push the + button to add them to your playlist.");
		return 0;
	}

	BrowserHistoryStore(browser.dir);
	ResetFiles();

	BROWSERENTRY *f_entry = AddEntryFiles();
	if(!f_entry) return 0;
	f_entry->file = mem2_strdup(BrowserHistoryRetrieve(), MEM2_BROWSER);
	if(!f_entry->file) // no mem
	{
		DeleteEntryFiles(f_entry);
		return 0;
	}
	f_entry->display = mem2_strdup("Exit Playlist", MEM2_BROWSER);
	if(!f_entry->display) // no mem
	{
		DeleteEntryFiles(f_entry);
		return 0;
	}
	f_entry->length = 0;
	f_entry->icon = ICON_FOLDER;

	char ext[7];
	GetExt(browser.dir, ext);

	if(IsPlaylistExt(ext) || strncmp(browser.dir, "http:", 5) == 0)
		f_entry->type = TYPE_PLAYLIST;
	else
		f_entry->type = TYPE_FOLDER;

	BROWSERENTRY *i = browserMusic.first;
	while(i)
	{
		f_entry = AddEntryFiles();
		if(!f_entry)
			break;

		f_entry->file = mem2_strdup(i->file, MEM2_BROWSER);
		if(!f_entry->file) // no mem
		{
			DeleteEntryFiles(f_entry);
			break;
		}
		if(i->display)
		{
			f_entry->display = mem2_strdup(i->display, MEM2_BROWSER);
			if(!f_entry->display) // no mem
			{
				DeleteEntryFiles(f_entry);
				break;
			}
		}
		f_entry->length = 0;
		f_entry->type = TYPE_FILE;
		f_entry->icon = ICON_FILE_CHECKED;
		i = i->next; 
	}
	browser.selIndex = browser.first;
	return browser.numEntries;
}

BROWSERENTRY * MusicPlaylistFindIndex(char * fullpath)
{
	BROWSERENTRY *i = browserMusic.first;

	while(i)
	{
		if(i->file && strcmp(fullpath, i->file) == 0)
			return i;
		i=i->next;
	}	
	return NULL;
}

bool MusicPlaylistFind(BROWSERENTRY *index)
{
	if(!index)
		return false;

	char fullpath[MAXPATHLEN];
	GetFullPath(index, fullpath);

	if(MusicPlaylistFindIndex(fullpath))
		return true;

	return false;
}

/****************************************************************************
 * EnqueueFile
 * 
 * Adds the specified file (with full path) to the playlist
 *  0 - not enqueued
 *  1 - enqueued
 * -1 - error
 ***************************************************************************/
static int EnqueueFile(char * path)
{
	if(path == NULL || MusicPlaylistFindIndex(path))
		return 0;

	char ext[7];
	GetExt(path, ext);


	// check if this is a valid audio file
	if((!IsAudioExt(ext) && !IsPlaylistExt(ext)) || strcmp(ext, "plx") == 0)
		return 0;

	if(IsPlaylistExt(ext))
	{
		// use MPlayer to try parsing the file
		play_tree_t * list = parse_playlist_file(path);

		if(!list)
			return 0;

		play_tree_iter_t *pt_iter = pt_iter_create(&list, NULL);

		if(!pt_iter)
		{
			play_tree_free(list, 1);
			return 0;
		}

		char file[MAXPATHLEN];
		play_tree_t* i;

		for(i = pt_iter->tree; i != NULL; i = i->next)
		{
			if(!i->files || strlen(i->files[0]) >= MAXPATHLEN)
				continue;

			if(!FindDevice(i->files[0], NULL, NULL))
				continue;

			GetExt(i->files[0], ext);

			if(!IsAllowedExt(ext))
				continue;

			strcpy(file, i->files[0]);
			CleanupPath(file);

			if(file[0] == 0)
				continue;

			if(MusicPlaylistFindIndex(file))
				continue;

			BROWSERENTRY *m_entry = AddEntryMusic();
			if(!m_entry) // add failed
			{
				pt_iter_destroy(&pt_iter);
				play_tree_free(list, 1);
				return -1;
			}

			m_entry->file = mem2_strdup(file, MEM2_BROWSER);
			if(!m_entry->file) // no mem
			{
				pt_iter_destroy(&pt_iter);
				play_tree_free(list, 1);
				DeleteEntryMusic(m_entry);
				return -1;
			}

			// use parameter pt_prettyformat_title for display if it exists
			if(i->params) 
			{
				for (int n = 0; i->params[n].name != NULL; n++) 
				{
					if(strcasecmp(i->params[n].name,PLAY_TREE_PARAM_PRETTYFORMAT_TITLE) != 0)
						continue;
					if(i->params[n].value == NULL)
						break;
					m_entry->display = mem2_strdup(i->params[n].value, MEM2_BROWSER);
					if(!m_entry->display) // no mem
					{
						pt_iter_destroy(&pt_iter);
						play_tree_free(list, 1);
						DeleteEntryMusic(m_entry);
						return -1;
					}
					break;
				}
			}

			if(!m_entry->display)
			{
				char *start = strrchr(i->files[0],'/');

				// use part after last / for display name, if it's not already the end of the string
				if(start != NULL && start[1] != 0)
				{
					start++;
					m_entry->display = mem2_strdup(start, MEM2_BROWSER);
				}
				else
				{
					m_entry->display = mem2_strdup(i->files[0], MEM2_BROWSER);
				}
				if(!m_entry->display) // no mem
				{
					pt_iter_destroy(&pt_iter);
					play_tree_free(list, 1);
					DeleteEntryMusic(m_entry);
					return -1;
				}

				// hide the file's extension
				if(WiiSettings.hideExtensions)
					StripExt(m_entry->display);
			}
		}

		pt_iter_destroy(&pt_iter);
		play_tree_free(list, 1);
	}
	else
	{
		BROWSERENTRY *m_entry = AddEntryMusic();
		if(!m_entry) // add failed
			return -1;

		m_entry->file = mem2_strdup(path, MEM2_BROWSER);
		if(!m_entry->file) // no mem
		{
			DeleteEntryMusic(m_entry);
			return -1;
		}

		char *start = strrchr(path,'/');

		// use part after last / for display name, if it's not already the end of the string
		if(start != NULL && start[1] != 0)
		{
			start++;
			m_entry->display = mem2_strdup(start, MEM2_BROWSER);
		}
		else
		{
			m_entry->display = mem2_strdup(path, MEM2_BROWSER);
		}
		if(!m_entry->display) // no mem
		{
			DeleteEntryMusic(m_entry);
			return -1;
		}

		// hide the file's extension
		if(WiiSettings.hideExtensions)
			StripExt(m_entry->display);
	}

	shuffleIndex = -1; // reset shuffle
	return 1;
}

static int MusicSortStart;
static int MusicSortEnd;

static int MusicSortCallback(const void *f1, const void *f2)
{
	if ((((BROWSERENTRY *)f1)->pos < MusicSortStart) || (((BROWSERENTRY *)f1)->pos > MusicSortEnd)
		|| (((BROWSERENTRY *)f2)->pos < MusicSortStart) || (((BROWSERENTRY *)f2)->pos > MusicSortEnd))
	{
		return 0;
	}
	return stricmp(((BROWSERENTRY *)f1)->display, ((BROWSERENTRY *)f2)->display);
}

/****************************************************************************
 * EnqueueFolder
 * 
 * Adds all music files in the specified folder to the playlist
 ***************************************************************************/
static bool EnqueueFolder(char * path, int silent)
{
	char *filepath;
	struct stat filestat;
	struct dirent *entry;
	DIR *dir = opendir(path);

	if(dir == NULL)
	{
		if(!silent)
		{
			wchar_t msg[512];
			swprintf(msg, 512, L"%s %s", gettext("Error opening"), path);
			ErrorPrompt(msg);
		}
		return false;
	}
	
	filepath = (char*)mem2_malloc((MAXPATHLEN+1)*sizeof(char), MEM2_BROWSER);
	if(!filepath) return false;

	while ((entry=readdir(dir))!=NULL)
	{
		if(entry->d_name[0] == '.')
			continue;

		if(strlen(path)+strlen(entry->d_name)>MAXPATHLEN) continue;
		snprintf(filepath, MAXPATHLEN, "%s/%s", path, entry->d_name);

#ifdef _DIRENT_HAVE_D_TYPE
		if(entry->d_type==DT_DIR)
			filestat.st_mode = S_IFDIR;
		else
			filestat.st_mode = S_IFREG;
#else
		if(stat(filepath, &filestat) < 0)
			continue;
#endif
		if(S_ISDIR(filestat.st_mode))
		{
			if(!EnqueueFolder(filepath, SILENT))
				break;
		}
	}

	// rewinddir seems to be broken
	closedir(dir);
	dir = opendir(path);

	int start = browserMusic.numEntries;
	while ((entry=readdir(dir))!=NULL)
	{
		if(entry->d_name[0] == '.')
			continue;

		if(strlen(path)+strlen(entry->d_name)>MAXPATHLEN) continue;
		snprintf(filepath, MAXPATHLEN, "%s/%s", path, entry->d_name);

#ifdef _DIRENT_HAVE_D_TYPE
		if(entry->d_type==DT_DIR)
			filestat.st_mode = S_IFDIR;
		else
			filestat.st_mode = S_IFREG;
#else
		if(stat(filepath, &filestat) < 0)
			continue;
#endif
		if(S_ISREG(filestat.st_mode))
		{
			if(EnqueueFile(filepath) < 0)
				break;
		}
	}
	if(browserMusic.numEntries-start > 1)
	{
		MusicSortStart = start;
		MusicSortEnd = browserMusic.numEntries - 1;
		SortBrowser(&browserMusic, MusicSortCallback);
	}

	mem2_free(filepath, MEM2_BROWSER);
	closedir(dir);

	return true;
}

bool MusicPlaylistEnqueue(BROWSERENTRY *index)
{
	char fullpath[MAXPATHLEN+1];
	GetFullPath(index, fullpath);
	
	if(index->type == TYPE_FOLDER)
		index->icon = ICON_FOLDER_CHECKED;
	else
		index->icon = ICON_FILE_CHECKED;
	
	if(index->type == TYPE_FOLDER)
		return EnqueueFolder(fullpath, NOTSILENT);

	if(EnqueueFile(fullpath) == 1)
		return true;

	return false;
}

static void Remove(BROWSERENTRY *i)
{
	if(!i)
		return;

	BROWSERENTRY *n = i->next;
	BROWSERENTRY *p = i->prior;

	if(browserMusic.first == i) browserMusic.first = n;
	if(browserMusic.last == i) browserMusic.last = p;

	if(n) n->prior = p;
	if(p) p->next = n;

	if(browserMusic.selIndex == i) 
		browserMusic.selIndex = NULL;

	shuffleIndex = -1; // reset shuffle

	if(browserMusic.numEntries > 0)
		browserMusic.numEntries--;

	mem2_free(i->file, MEM2_BROWSER);
	mem2_free(i->url, MEM2_BROWSER);
	mem2_free(i->display, MEM2_BROWSER);
	mem2_free(i->image, MEM2_BROWSER);
	mem2_free(i, MEM2_BROWSER);
}

void MusicPlaylistDequeue(BROWSERENTRY *index)
{
	char fullpath[MAXPATHLEN];
	GetFullPath(index, fullpath);

	int len = strlen(fullpath);

	if(index->type == TYPE_FOLDER)
		index->icon = ICON_FOLDER;
	else
		index->icon = ICON_FILE;

	if(index->type == TYPE_PLAYLIST)
	{
		// use MPlayer to try parsing the file
		play_tree_t * list = parse_playlist_file(fullpath);

		if(!list)
			return;

		play_tree_iter_t *pt_iter = pt_iter_create(&list, NULL);

		if(!pt_iter)
		{
			play_tree_free(list, 1);
			return;
		}

		char file[MAXPATHLEN];
		char ext[7];
		play_tree_t* i;

		for(i = pt_iter->tree; i != NULL; i = i->next)
		{
			if(!i->files || strlen(i->files[0]) >= MAXPATHLEN)
				continue;

			if(!FindDevice(i->files[0], NULL, NULL))
				continue;

			GetExt(i->files[0], ext);

			if(!IsAllowedExt(ext))
				continue;

			strcpy(file, i->files[0]);
			CleanupPath(file);

			if(file[0] == 0)
				continue;

			BROWSERENTRY *m_entry = browserMusic.first;
			while(m_entry)
			{
				if(strcmp(file, m_entry->file) == 0)
				{
					Remove(m_entry);
					break;
				}
				m_entry = m_entry->next; 
			}
		}
		pt_iter_destroy(&pt_iter);
		play_tree_free(list, 1);
		return;
	}

	BROWSERENTRY *m_entry = browserMusic.first;
	BROWSERENTRY *aux;
	while(m_entry)
	{
		if(m_entry->file && strncmp(fullpath, m_entry->file, len) == 0)
		{
			aux = m_entry;
			m_entry = m_entry->next; 
			Remove(aux);			
		}
		else 
			m_entry = m_entry->next; 
	}
}
