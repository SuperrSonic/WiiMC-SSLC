/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * musicplaylist.h
 *
 * Music playlist
 ****************************************************************************/

#ifndef _MUSICPLAYLIST_H_
#define _MUSICPLAYLIST_H_

BROWSERENTRY * MusicPlaylistGetNextShuffle();
int MusicPlaylistLoad();
BROWSERENTRY * MusicPlaylistFindIndex(char *path);
bool MusicPlaylistFind(BROWSERENTRY *index);
bool MusicPlaylistEnqueue(BROWSERENTRY *index);
void MusicPlaylistDequeue(BROWSERENTRY *index);

#endif
