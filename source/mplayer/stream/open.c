/*
 * This file is part of MPlayer.
 *
 * MPlayer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * MPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MPlayer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "config.h"
#include "mp_msg.h"
#include "help_mp.h"

#ifdef __FreeBSD__
#include <sys/cdrio.h>
#endif

#include "m_option.h"
#include "stream.h"
#include "libmpdemux/demuxer.h"


/// We keep these 2 for the gui atm, but they will be removed.
char* cdrom_device=NULL;
int dvd_chapter=1;
int dvd_last_chapter=0;
char* dvd_device=NULL;
int dvd_title=0;
char *bluray_device=NULL;

// Open a new stream  (stdin/file/vcd/url)

#ifdef GEKKO
static void CleanFileName(const char *file)  //clean (//) exacmple smb1://film.avi -> smb1:/film.avi
{
	char buf[1024];
	if(strncmp(file, "smb", 3)==0 || strncmp(file, "sd", 2)==0 || strncmp(file, "usb", 3)==0)
	{
		int i=0,j=0;
		while(file[i]!='\0')
		{
			buf[j]=file[i];
			j++;
			if(file[i]=='/')
				while(file[i]=='/' && file[i]!='\0')i++;
			else
				i++;
		}
		buf[j]='\0';
		strcpy((char*)file,buf);
	}	
}
#endif

stream_t* open_stream(const char* filename,char** options, int* file_format){
  int dummy = DEMUXER_TYPE_UNKNOWN;
  if (!file_format) file_format = &dummy;
  // Check if playlist or unknown
  if (*file_format != DEMUXER_TYPE_PLAYLIST){
    *file_format=DEMUXER_TYPE_UNKNOWN;
  }

if(!filename) {
   mp_msg(MSGT_OPEN,MSGL_ERR,"NULL filename, report this bug\n");
   return NULL;
}

//============ Open STDIN or plain FILE ============
#ifdef GEKKO
  CleanFileName(filename);
#endif  

  return open_stream_full(filename,STREAM_READ,options,file_format);
}
