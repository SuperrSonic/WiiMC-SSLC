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

#include "config.h"
#include "mp_msg.h"
#include "help_mp.h"

#include <stdlib.h>
#include <stdio.h>
#include "stream/stream.h"
#include "aviprint.h"
#include "demuxer.h"
#include "stheader.h"
#include "genres.h"
#include "mp3_hdr.h"
#include "demux_audio.h"

#include "libavutil/intreadwrite.h"

#include <string.h>

#define MP3 1
#define WAV 2
#define fLaC 3


#define HDR_SIZE 4

typedef struct da_priv {
  int frmt;
  double next_pts;
} da_priv_t;

//! rather arbitrary value for maximum length of wav-format headers
#define MAX_WAVHDR_LEN (1 * 1024 * 1024)

//! how many valid frames in a row we need before accepting as valid MP3
#define MIN_MP3_HDRS 12

//! Used to describe a potential (chain of) MP3 headers we found
typedef struct mp3_hdr {
  off_t frame_pos; // start of first frame in this "chain" of headers
  off_t next_frame_pos; // here we expect the next header with same parameters
  int mp3_chans;
  int mp3_freq;
  int mpa_spf;
  int mpa_layer;
  int mpa_br;
  int cons_hdrs; // if this reaches MIN_MP3_HDRS we accept as MP3 file
  struct mp3_hdr *next;
} mp3_hdr_t;

int hr_mp3_seek = 0;

/**
 * \brief free a list of MP3 header descriptions
 * \param list pointer to the head-of-list pointer
 */
static void free_mp3_hdrs(mp3_hdr_t **list) {
  mp3_hdr_t *tmp;
  while (*list) {
    tmp = (*list)->next;
    free(*list);
    *list = tmp;
  }
}

/**
 * \brief add another potential MP3 header to our list
 * If it fits into an existing chain this one is expanded otherwise
 * a new one is created.
 * All entries that expected a MP3 header before the current position
 * are discarded.
 * The list is expected to be and will be kept sorted by next_frame_pos
 * and when those are equal by frame_pos.
 * \param list pointer to the head-of-list pointer
 * \param st_pos stream position where the described header starts
 * \param mp3_chans number of channels as specified by the header (*)
 * \param mp3_freq sampling frequency as specified by the header (*)
 * \param mpa_spf frame size as specified by the header
 * \param mpa_layer layer type ("version") as specified by the header (*)
 * \param mpa_br bitrate as specified by the header
 * \param mp3_flen length of the frame as specified by the header
 * \return If non-null the current file is accepted as MP3 and the
 * mp3_hdr struct describing the valid chain is returned. Must be
 * freed independent of the list.
 *
 * parameters marked by (*) must be the same for all headers in the same chain
 */
static mp3_hdr_t *add_mp3_hdr(mp3_hdr_t **list, off_t st_pos,
                               int mp3_chans, int mp3_freq, int mpa_spf,
                               int mpa_layer, int mpa_br, int mp3_flen) {
  mp3_hdr_t *tmp;
  int in_list = 0;
  while (*list && (*list)->next_frame_pos <= st_pos) {
    if (((*list)->next_frame_pos < st_pos) || ((*list)->mp3_chans != mp3_chans)
         || ((*list)->mp3_freq != mp3_freq) || ((*list)->mpa_layer != mpa_layer) ) {
      // wasn't valid!
      tmp = (*list)->next;
      free(*list);
      *list = tmp;
    } else {
      (*list)->cons_hdrs++;
      (*list)->next_frame_pos = st_pos + mp3_flen;
      (*list)->mpa_spf = mpa_spf;
      (*list)->mpa_br = mpa_br;
      if ((*list)->cons_hdrs >= MIN_MP3_HDRS) {
        // copy the valid entry, so that the list can be easily freed
        tmp = malloc(sizeof(mp3_hdr_t));
        memcpy(tmp, *list, sizeof(mp3_hdr_t));
        tmp->next = NULL;
        return tmp;
      }
      in_list = 1;
      list = &((*list)->next);
    }
  }
  if (!in_list) { // does not belong into an existing chain, insert
    // find right position to insert to keep sorting
    while (*list && (*list)->next_frame_pos <= st_pos + mp3_flen)
      list = &((*list)->next);
    tmp = malloc(sizeof(mp3_hdr_t));
    tmp->frame_pos = st_pos;
    tmp->next_frame_pos = st_pos + mp3_flen;
    tmp->mp3_chans = mp3_chans;
    tmp->mp3_freq = mp3_freq;
    tmp->mpa_spf = mpa_spf;
    tmp->mpa_layer = mpa_layer;
    tmp->mpa_br = mpa_br;
    tmp->cons_hdrs = 1;
    tmp->next = *list;
    *list = tmp;
  }
  return NULL;
}

#if 1 /* this code is a mess, clean it up before reenabling */
#define FLAC_SIGNATURE_SIZE 4
#define FLAC_STREAMINFO_SIZE 34
#define FLAC_SEEKPOINT_SIZE 18

enum {
  FLAC_STREAMINFO = 0,
  FLAC_PADDING,
  FLAC_APPLICATION,
  FLAC_SEEKTABLE,
  FLAC_VORBIS_COMMENT,
  FLAC_CUESHEET
} flac_preamble_t;

static void
get_flac_metadata (demuxer_t* demuxer)
{
  uint8_t preamble[4];
  unsigned int blk_len;
  stream_t *s = demuxer->stream;

  /* file is qualified; skip over the signature bytes in the stream */
  stream_seek (s, 4);

  /* loop through the metadata blocks; use a do-while construct since there
   * will always be 1 metadata block */
  do {
    int r;

    r = stream_read (s, (char *) preamble, FLAC_SIGNATURE_SIZE);
    if (r != FLAC_SIGNATURE_SIZE)
      return;

    blk_len = AV_RB24(preamble + 1);

    switch (preamble[0] & 0x7F)
    {
    case FLAC_VORBIS_COMMENT:
    {
      /* For a description of the format please have a look at */
      /* http://www.xiph.org/vorbis/doc/v-comment.html */

      uint32_t length, comment_list_len;
      char comments[blk_len];
      uint8_t *ptr = comments;
      char *comment;
      int cn;
      char c;

      if (stream_read (s, comments, blk_len) == blk_len)
      {
        length = AV_RL32(ptr);
        ptr += 4 + length;

        comment_list_len = AV_RL32(ptr);
        ptr += 4;

        cn = 0;
        for (; cn < comment_list_len; cn++)
        {
          length = AV_RL32(ptr);
          ptr += 4;

          comment = ptr;
		  // This breaks Artist from displaying (>=)
        //  if (&comment[length] < comments || &comment[length] >= &comments[blk_len])
            //return;

          c = comment[length];
          comment[length] = 0;

          if (!strncasecmp ("TITLE=", comment, 6) && (length - 6 > 0))
            demux_info_add (demuxer, "Title", comment + 6);
          else if (!strncasecmp ("ARTIST=", comment, 7) && (length - 7 > 0))
            demux_info_add (demuxer, "Artist", comment + 7);
          else if (!strncasecmp ("ALBUM=", comment, 6) && (length - 6 > 0))
            demux_info_add (demuxer, "Album", comment + 6);
          else if (!strncasecmp ("DATE=", comment, 5) && (length - 5 > 0))
            demux_info_add (demuxer, "Year", comment + 5);
       /*   else if (!strncasecmp ("GENRE=", comment, 6) && (length - 6 > 0))
            demux_info_add (demuxer, "Genre", comment + 6);
          else if (!strncasecmp ("Comment=", comment, 8) && (length - 8 > 0))
            demux_info_add (demuxer, "Comment", comment + 8);
          else if (!strncasecmp ("TRACKNUMBER=", comment, 12)
                   && (length - 12 > 0))
          {
            char buf[31];
            buf[30] = '\0';
            sprintf (buf, "%d", atoi (comment + 12));
            demux_info_add(demuxer, "Track", buf);
          } */
          comment[length] = c;

          ptr += length;
        }
      }
      break;
    }

    case FLAC_STREAMINFO:
    case FLAC_PADDING:
    case FLAC_APPLICATION:
    case FLAC_SEEKTABLE:
    case FLAC_CUESHEET:
    default:
      /* 6-127 are presently reserved */
      stream_skip (s, blk_len);
      break;
    }
  } while ((preamble[0] & 0x80) == 0);
}
#endif

/**
 * @brief Determine the number of frames of a file encoded with
 *        variable bitrate mode (VBR).
 *
 * @param s stream to be read
 * @param off offset in stream to start reading from
 *
 * @return 0 (error or no variable bitrate mode) or number of frames
 */
static unsigned int mp3_vbr_frames(stream_t *s, off_t off) {
  static const int xing_offset[2][2] = {{32, 17}, {17, 9}};
  unsigned int data;
  unsigned char hdr[4];
  int framesize, chans, spf, layer;

  if ((s->flags & MP_STREAM_SEEK) == MP_STREAM_SEEK) {

    if (!stream_seek(s, off)) return 0;

    data = stream_read_dword(s);
    hdr[0] = data >> 24;
    hdr[1] = data >> 16;
    hdr[2] = data >> 8;
    hdr[3] = data;

    if (!mp_check_mp3_header(data)) return 0;

    framesize = mp_get_mp3_header(hdr, &chans, NULL, &spf, &layer, NULL);

    if (framesize == -1 || layer != 3) return 0;

    /* Xing / Info (at variable position: 32, 17 or 9 bytes after header) */

    if (!stream_skip(s, xing_offset[spf < 1152][chans == 1])) return 0;

    data = stream_read_dword(s);

    if (data == MKBETAG('X','i','n','g') || data == MKBETAG('I','n','f','o')) {
      data = stream_read_dword(s);

      if (data & 0x1)                   // frames field is present
        return stream_read_dword(s);    // frames
    }

    /* VBRI (at fixed position: 32 bytes after header) */

    if (!stream_seek(s, off + 4 + 32)) return 0;

    data = stream_read_dword(s);

    if (data == MKBETAG('V','B','R','I')) {
      data = stream_read_word(s);

      if (data == 1) {                       // check version
        if (!stream_skip(s, 8)) return 0;    // skip delay, quality and bytes
        return stream_read_dword(s);         // frames
      }
    }
  }

  return 0;
}

/**
 * @brief Determine the total size of an ID3v2 tag.
 *
 * @param maj_ver major version of the ID3v2 tag
 * @param s stream to be read, assumed to be positioned at revision byte
 *
 * @return 0 (error or malformed tag) or tag size
 */
static unsigned int id3v2_tag_size(uint8_t maj_ver, stream_t *s) {
  unsigned int header_footer_size;
  unsigned int size;
  int i;

  if(stream_read_char(s) == 0xff)
    return 0;
  header_footer_size = ((stream_read_char(s) & 0x10) && maj_ver >= 4) ? 20 : 10;

  size = 0;
  for(i = 0; i < 4; i++) {
    uint8_t data = stream_read_char(s);
    if (data & 0x80)
      return 0;
    size = size << 7 | data;
  }

  return header_footer_size + size;
}

#include "../../utils/mem2_manager.h"
u8 *pos_pic = NULL;
extern int embedded_pic;
//extern int wiim_inf;
//extern bool thumbLoad;
/*
wchar_t* charToWChar(char* cArray, int len) {
    char wideChar[2];
    wchar_t wideCharW;
    wchar_t *wArray = (wchar_t *) malloc(sizeof(wchar_t) * len / 2);
    int counter = 0;

    for (int j = 2; j < len; j+=2){
        wideChar[1] = cArray[j]; wideChar[0] = cArray[j + 1];

        wideCharW = (uint16_t)((uint8_t)wideChar[1] << 8 | (uint8_t)wideChar[0]);
        wArray[counter] = wideCharW;
        counter++;
    }
    wArray[counter] = '\0';
    return wArray;
} */
//From asfheader.c
static char* get_ucs2strII(const uint16_t* inbuf, uint16_t inlen)
{
  char* outbuf = calloc(inlen, 2);
  char* q;
  int i;

  if (!outbuf) {
    return NULL;
  }
  q = outbuf;
  for (i = 0; i < inlen / 2; i++) {
    uint8_t tmp;
    PUT_UTF8(AV_RL16(&inbuf[i]), tmp, *q++ = tmp;)
  }
  return outbuf;
}

static int demux_audio_open(demuxer_t* demuxer) {
  stream_t *s;
  sh_audio_t* sh_audio;
  uint8_t hdr[HDR_SIZE];
  int frmt = 0, n = 0, step;
  off_t st_pos = 0, next_frame_pos = 0;
  // mp3_hdrs list is sorted first by next_frame_pos and then by frame_pos
  mp3_hdr_t *mp3_hdrs = NULL, *mp3_found = NULL;
  da_priv_t* priv;
  double duration;
  int found_WAVE = 0;
  int found_ID3 = 0;
  unsigned int loop_limit = 32;
  bool skip_id3v1 = false;

  s = demuxer->stream;

  stream_read(s, hdr, HDR_SIZE);
  while(n < 30000 && !s->eof) {
    int mp3_freq, mp3_chans, mp3_flen, mpa_layer, mpa_spf, mpa_br;
    st_pos = stream_tell(s) - HDR_SIZE;
    step = 1;

    if( hdr[0] == 'R' && hdr[1] == 'I' && hdr[2] == 'F' && hdr[3] == 'F' ) {
      stream_skip(s,4);
      if(s->eof)
	break;
      stream_read(s,hdr,4);
      if(s->eof)
	break;
      if(hdr[0] != 'W' || hdr[1] != 'A' || hdr[2] != 'V'  || hdr[3] != 'E' )
	stream_skip(s,-8);
      else
      // We found wav header. Now we can have 'fmt ' or a mp3 header
      // empty the buffer
	step = 4;
    } else if( hdr[0] == 'I' && hdr[1] == 'D' && hdr[2] == '3' && hdr[3] >= 2 && hdr[3] != 0xff) {

	  //ID3v2.4, v2.3, v2.2
	  if(hdr[3] == 4)
		 found_ID3 = 2;
      else if(hdr[3] > 2)
         found_ID3 = 1;
	  else if(hdr[3] == 2)
		 found_ID3 = -1;

      unsigned int len = id3v2_tag_size(hdr[3], s);
	  loop_limit = len;
      if(len > 0)
        stream_skip(s,len-10);
      step = 4;
    } else if( found_WAVE && hdr[0] == 'f' && hdr[1] == 'm' && hdr[2] == 't' && hdr[3] == ' ' ) {
      frmt = WAV;
      break;
    } else if((mp3_flen = mp_get_mp3_header(hdr, &mp3_chans, &mp3_freq,
                                &mpa_spf, &mpa_layer, &mpa_br)) > 0) {
      mp3_found = add_mp3_hdr(&mp3_hdrs, st_pos, mp3_chans, mp3_freq,
                              mpa_spf, mpa_layer, mpa_br, mp3_flen);
      if (mp3_found) {
        frmt = MP3;
        break;
      }
    } else if( hdr[0] == 'f' && hdr[1] == 'L' && hdr[2] == 'a' && hdr[3] == 'C' ) {
      found_ID3 = 1;
	  loop_limit = id3v2_tag_size(3, s); // Works
      frmt = fLaC;
      if (!mp3_hdrs || mp3_hdrs->cons_hdrs < 3)
        break;
    }
    found_WAVE = hdr[0] == 'W' && hdr[1] == 'A' && hdr[2] == 'V' && hdr[3] == 'E';
    // Add here some other audio format detection
    if(step < HDR_SIZE)
      memmove(hdr,&hdr[step],HDR_SIZE-step);
    stream_read(s, &hdr[HDR_SIZE - step], step);
    n++;
  }

  free_mp3_hdrs(&mp3_hdrs);

  if(!frmt)
    return 0;

  sh_audio = new_sh_audio(demuxer,0, NULL);

  switch(frmt) {
  case MP3:
    sh_audio->format = (mp3_found->mpa_layer < 3 ? 0x50 : 0x55);
    demuxer->movi_start = mp3_found->frame_pos;
    demuxer->movi_end = s->end_pos;
    next_frame_pos = mp3_found->next_frame_pos;
    sh_audio->audio.dwSampleSize= 0;
    sh_audio->audio.dwScale = mp3_found->mpa_spf;
    sh_audio->audio.dwRate = mp3_found->mp3_freq;
    sh_audio->wf = malloc(sizeof(*sh_audio->wf));
    sh_audio->wf->wFormatTag = sh_audio->format;
    sh_audio->wf->nChannels = mp3_found->mp3_chans;
    sh_audio->wf->nSamplesPerSec = mp3_found->mp3_freq;
    sh_audio->wf->nAvgBytesPerSec = mp3_found->mpa_br * (1000 / 8);
    sh_audio->wf->nBlockAlign = mp3_found->mpa_spf;
    sh_audio->wf->wBitsPerSample = 16;
    sh_audio->wf->cbSize = 0;
    duration = (double) mp3_vbr_frames(s, demuxer->movi_start) * mp3_found->mpa_spf / mp3_found->mp3_freq;
    free(mp3_found);
    mp3_found = NULL;
    if(demuxer->movi_end && (s->flags & MP_STREAM_SEEK) == MP_STREAM_SEEK) {
      if(demuxer->movi_end >= 128) {
#if 0
	//Parial support for Enhanced ID3v1, used for getting longer text easily.
      stream_seek(s,demuxer->movi_end-188); //switch to 227?
      stream_read(s,hdr,4);
      if(!memcmp(hdr,"TAG+",4)) { // Works but is not up to spec.
	char buf[61];
	//uint8_t g;
          demuxer->movi_end -= 188;
	stream_read(s,buf,60);
	buf[60] = '\0';
	demux_info_add(demuxer,"Title",buf);
	stream_read(s,buf,60);
	buf[60] = '\0';
	demux_info_add(demuxer,"Artist",buf);
	stream_read(s,buf,60);
	buf[60] = '\0';
	demux_info_add(demuxer,"Album",buf);
	stream_read(s,buf,4);
	buf[4] = '\0';
	demux_info_add(demuxer,"Year",buf);
	stream_read(s,buf,30);
	/*buf[30] = '\0';
	demux_info_add(demuxer,"Comment",buf);
	if(buf[28] == 0 && buf[29] != 0) {
	  uint8_t trk = (uint8_t)buf[29];
	  sprintf(buf,"%d",trk);
	  demux_info_add(demuxer,"Track",buf);
	}
	g = stream_read_char(s); */
	//demux_info_add(demuxer,"Genre",genres[g]);
      }
#endif
#if 1
//wiim_inf = 2;

		uint8_t cur_bytes[4];
		uint8_t cur_b_desc[1];
		uint8_t apic_size[4];
		uint8_t apic_mime[4];
		uint8_t apic_desc[1]; //determine if there's a desc.
		u8 apic_datstep = 0;
		int val_apic_size = 0;
		int j = 0;
		if(found_ID3 > 0) {
#if 1
			//If a title, artist, or album is found, skip checking for ID3v1
			//title
			for(int i=4; i < loop_limit; i++) {
				stream_seek(s, i);
				stream_read(s, cur_bytes, 4);
			if(!memcmp(cur_bytes,"TIT2",4)) {
				//encoding byte, 00 is ascii, 01 is u16
				stream_seek(s, i+10);
				
				if(stream_read_char(s) == 0) {
				uint8_t size_of_TIT2[4];
				stream_seek(s, i+4);
				stream_read(s, size_of_TIT2, 4);
				char buf[(size_of_TIT2[0] << 24) + (size_of_TIT2[1] << 16) + (size_of_TIT2[2] << 8) + size_of_TIT2[3]-1];
				stream_seek(s, i+11);
				stream_read(s,buf,sizeof(buf));
				buf[sizeof(buf)] = '\0';
				demux_info_add(demuxer,"Title",buf);
				skip_id3v1 = true;
				break;
				}
				else { //utf-16 string
				uint8_t size_of_TIT2[4];
				stream_seek(s, i+4);
				stream_read(s, size_of_TIT2, 4);
				char buf[(size_of_TIT2[0] << 24) + (size_of_TIT2[1] << 16) + (size_of_TIT2[2] << 8) + size_of_TIT2[3]-2];
				stream_seek(s, i+13);
				stream_read(s,buf,sizeof(buf));
				buf[sizeof(buf)] = '\0';
				//buf[2] = '\0'; // first 0xFEXX
				
				//convert buf to utf8
				uint16_t* b = (uint16_t*) malloc(sizeof(buf) * sizeof(uint16_t));
				memcpy(b, buf, sizeof(buf));
			/*	for(int j=0;j < sizeof(buf);++j) {
					if(buf[j] == 0)
						buf[j] = buf[j+1];
				} */
				//wchar_t wc = charToWChar(buf, sizeof(buf));
			//	wcstombs(buf, charToWChar(buf, sizeof(buf)), sizeof(buf));

				//wcstombs(buf, wc, sizeof(buf));
	//			ConvertUTF16toUTF8 (
//	b[0], b[sizeof(buf)],
//	buf[0], buf[sizeof(buf)], 0);
				
				demux_info_add(demuxer,"Title",get_ucs2strII(b, sizeof(buf)));
				free(b);
				skip_id3v1 = true;
				break;
				}
			}
		}
		//artist
		for(int i=4; i < loop_limit; i++) {
				stream_seek(s, i);
				stream_read(s, cur_bytes, 4);
			if(!memcmp(cur_bytes,"TPE1",4)) {
				//encoding byte, 00 is ascii, 01 is u16
				stream_seek(s, i+10);
				
				if(stream_read_char(s) == 0) {
				uint8_t size_of_TIT2[4];
				stream_seek(s, i+4);
				stream_read(s, size_of_TIT2, 4);
				char buf[(size_of_TIT2[0] << 24) + (size_of_TIT2[1] << 16) + (size_of_TIT2[2] << 8) + size_of_TIT2[3]-1];
				stream_seek(s, i+11);
				stream_read(s,buf,sizeof(buf));
				buf[sizeof(buf)] = '\0';
				demux_info_add(demuxer,"Artist",buf);
				skip_id3v1 = true;
				break;
				}
				else {
				uint8_t size_of_TIT2[4];
				stream_seek(s, i+4);
				stream_read(s, size_of_TIT2, 4);
				char buf[(size_of_TIT2[0] << 24) + (size_of_TIT2[1] << 16) + (size_of_TIT2[2] << 8) + size_of_TIT2[3]-2];
				stream_seek(s, i+13);
				stream_read(s,buf,sizeof(buf));
				buf[sizeof(buf)] = '\0';
				uint16_t* b = (uint16_t*) malloc(sizeof(buf) * sizeof(uint16_t));
				memcpy(b, buf, sizeof(buf));
				
				demux_info_add(demuxer,"Artist",get_ucs2strII(b, sizeof(buf)));
				free(b);
				skip_id3v1 = true;
				break;
			}
			}
		}
		//album
		for(int i=4; i < loop_limit; i++) {
				stream_seek(s, i);
				stream_read(s, cur_bytes, 4);
			if(!memcmp(cur_bytes,"TALB",4)) {
				//encoding byte, 00 is ascii, 01 is u16
				stream_seek(s, i+10);
				if(stream_read_char(s) == 0) {
				uint8_t size_of_TIT2[4];
				stream_seek(s, i+4);
				stream_read(s, size_of_TIT2, 4);
				char buf[(size_of_TIT2[0] << 24) + (size_of_TIT2[1] << 16) + (size_of_TIT2[2] << 8) + size_of_TIT2[3]-1];
				stream_seek(s, i+11);
				stream_read(s,buf,sizeof(buf));
				buf[sizeof(buf)] = '\0';
				demux_info_add(demuxer,"Album",buf);
				skip_id3v1 = true;
				break;
				}
				else {
					uint8_t size_of_TIT2[4];
				stream_seek(s, i+4);
				stream_read(s, size_of_TIT2, 4);
				char buf[(size_of_TIT2[0] << 24) + (size_of_TIT2[1] << 16) + (size_of_TIT2[2] << 8) + size_of_TIT2[3]-2];
				stream_seek(s, i+13);
				stream_read(s,buf,sizeof(buf));
				buf[sizeof(buf)] = '\0';
				uint16_t* b = (uint16_t*) malloc(sizeof(buf) * sizeof(uint16_t));
				memcpy(b, buf, sizeof(buf));
				demux_info_add(demuxer,"Album",get_ucs2strII(b, sizeof(buf)));
				free(b);
				skip_id3v1 = true;
				break;
				}
			}
		}
		//year, TDRC should only be read in v2.4
		for(int i=4; i < loop_limit; i++) {
				stream_seek(s, i);
				stream_read(s, cur_bytes, 4);
			if((found_ID3 == 2 && !memcmp(cur_bytes,"TDRC",4)) || (found_ID3 == 1 && !memcmp(cur_bytes,"TYER",4))) {
				//encoding byte, 00 is ascii, 01 is u16
				stream_seek(s, i+10);
				if(stream_read_char(s) == 0) {
				uint8_t size_of_TIT2[4];
				stream_seek(s, i+4);
				stream_read(s, size_of_TIT2, 4);
				char buf[(size_of_TIT2[0] << 24) + (size_of_TIT2[1] << 16) + (size_of_TIT2[2] << 8) + size_of_TIT2[3]-1];
				stream_seek(s, i+11);
				stream_read(s,buf,sizeof(buf));
				buf[sizeof(buf)] = '\0';
				demux_info_add(demuxer,"Year",buf);
				break;
				}
				else {
					uint8_t size_of_TIT2[4];
				stream_seek(s, i+4);
				stream_read(s, size_of_TIT2, 4);
				char buf[(size_of_TIT2[0] << 24) + (size_of_TIT2[1] << 16) + (size_of_TIT2[2] << 8) + size_of_TIT2[3]-2];
				stream_seek(s, i+13);
				stream_read(s,buf,sizeof(buf));
				buf[sizeof(buf)] = '\0';
				uint16_t* b = (uint16_t*) malloc(sizeof(buf) * sizeof(uint16_t));
				memcpy(b, buf, sizeof(buf));
				demux_info_add(demuxer,"Year",get_ucs2strII(b, sizeof(buf)));
				free(b);
				break;
				}
			}
		}
#endif

		for(int i=32; i < loop_limit; i++) { // start at pos 32 should speed up search
			stream_seek(s, i);
			stream_read(s, cur_bytes, 4);
			if(cur_bytes[0] == 'A' && cur_bytes[1] == 'P' && cur_bytes[2] == 'I' && cur_bytes[3] == 'C') {
				stream_seek(s, i+4); // Get apic size
				stream_read(s,apic_size,4);
				//Goto pic
				val_apic_size = (apic_size[0] << 24) + (apic_size[1] << 16) + (apic_size[2] << 8) + apic_size[3];
				if(val_apic_size > 1.5*1024*1024)
					val_apic_size = 1.5*1024*1024;
					//break;
			//	wiim_inf = val_apic_size; // For debug
				stream_seek(s, i+17); // Get mime - jpeg, jpg, png
				stream_read(s, apic_mime, 4);
				if(apic_mime[3] == 'g') // jpeg mimetype
					apic_datstep = 24;
				else
					apic_datstep = 23; // jpg/png mimetype
				
				//handle description
				stream_seek(s, apic_datstep == 24 ? i+23 : i+22); // Get desc if NULL
				stream_read(s, apic_desc, 1);
				if(apic_desc[0] != '\0') {
					for(j=0;j<0xFF;j++) {
						stream_seek(s, apic_datstep == 24 ? i+23+j : i+22+j);
						stream_read(s, cur_b_desc, 1);
						if(cur_b_desc[0] == '\0')
							break;
					}
				}
				//wiim_inf = apic_datstep+j;
				
				pos_pic = (u8 *)mem2_memalign(32, 1.5*1024*1024, MEM2_OTHER);
				stream_seek(s, i+apic_datstep+j); // Get pic data
				stream_read(s, pos_pic, val_apic_size);
				//enable cover art in gui
			//	thumbLoad = true; // enable in audio callback to create a delay
				embedded_pic = 1;
				break;
			}
		}
		} else if(found_ID3 == -1) {
			//original version, rare.
			//title
			for(int i=4; i < loop_limit; i++) {
				stream_seek(s, i);
				stream_read(s, cur_bytes, 4);
			if(!memcmp(cur_bytes,"TT2",3)) {
				uint8_t size_of_TIT2[1];
				stream_seek(s, i+5);
				stream_read(s, size_of_TIT2, 1);
				char buf[size_of_TIT2[0]];
				stream_seek(s, i+7);
				stream_read(s,buf,sizeof(buf));
			//	buf[sizeof(buf)] = '\0';
				demux_info_add(demuxer,"Title",buf);
				skip_id3v1 = true;
				break;
				}
			}
			//artist
			for(int i=4; i < loop_limit; i++) {
				stream_seek(s, i);
				stream_read(s, cur_bytes, 4);
			if(!memcmp(cur_bytes,"TP1",3)) {
				uint8_t size_of_TIT2[1];
				stream_seek(s, i+5);
				stream_read(s, size_of_TIT2, 1);
				char buf[size_of_TIT2[0]];
				stream_seek(s, i+7);
				stream_read(s,buf,sizeof(buf));
			//	buf[sizeof(buf)] = '\0';
				demux_info_add(demuxer,"Artist",buf);
				skip_id3v1 = true;
				break;
				}
			}
			//album
			for(int i=4; i < loop_limit; i++) {
				stream_seek(s, i);
				stream_read(s, cur_bytes, 4);
			if(!memcmp(cur_bytes,"TAL",3)) {
				uint8_t size_of_TIT2[1];
				stream_seek(s, i+5);
				stream_read(s, size_of_TIT2, 1);
				char buf[size_of_TIT2[0]];
				stream_seek(s, i+7);
				stream_read(s,buf,sizeof(buf));
			//	buf[sizeof(buf)] = '\0';
				demux_info_add(demuxer,"Album",buf);
				skip_id3v1 = true;
				break;
				}
			}
			//year
			for(int i=4; i < loop_limit; i++) {
				stream_seek(s, i);
				stream_read(s, cur_bytes, 4);
			if(!memcmp(cur_bytes,"TYE",3)) {
				uint8_t size_of_TIT2[1];
				stream_seek(s, i+5);
				stream_read(s, size_of_TIT2, 1);
				char buf[size_of_TIT2[0]];
				stream_seek(s, i+7);
				stream_read(s,buf,sizeof(buf));
			//	buf[sizeof(buf)] = '\0';
				demux_info_add(demuxer,"Year",buf);
				skip_id3v1 = true;
				break;
				}
			}
			//pic
			for(int i=32; i < loop_limit; i++) { // start at pos 32 should speed up search
			stream_seek(s, i);
			stream_read(s, cur_bytes, 4);
			if(cur_bytes[0] == '\0' && cur_bytes[1] == 'P' && cur_bytes[2] == 'I' && cur_bytes[3] == 'C') {
				stream_seek(s, i+4); // Get apic size
				stream_read(s,apic_size,4);
				//Goto pic
				val_apic_size = (apic_size[0] << 24) + (apic_size[1] << 16) + (apic_size[2] << 8) + apic_size[3];
				if(val_apic_size > 1.5*1024*1024)
					val_apic_size = 1.5*1024*1024;
				stream_seek(s, i+8); // Get mime - jpeg, jpg, png
				stream_read(s, apic_mime, 4);
				if(apic_mime[2] == 'G') // JPG/PNG mimetype
					apic_datstep = 0xD;
				else
					apic_datstep = 0xE; // JPEG mimetype, not sure if it exists.
				
				pos_pic = (u8 *)mem2_memalign(32, 1.5*1024*1024, MEM2_OTHER);
				stream_seek(s, i+apic_datstep); // Get pic data
				stream_read(s, pos_pic, val_apic_size);
				embedded_pic = 1;
				break;
			}
			}
			
		}

		//works, now to automate
	//	pos_pic = (u8 *)mem2_memalign(32, 200*1024, MEM2_OTHER);
	//	stream_seek(s, 0x7F);
	//	stream_read(s,pos_pic,0xBFB0);
#endif

	  stream_seek(s,demuxer->movi_end-128);
      stream_read(s,hdr,3);
	  if(!memcmp(hdr,"TAG",3) && !skip_id3v1) {
	char buf[31];
	uint8_t g;
          demuxer->movi_end -= 128;
	stream_read(s,buf,30);
	buf[30] = '\0';
	demux_info_add(demuxer,"Title",buf);
	stream_read(s,buf,30);
	buf[30] = '\0';
	demux_info_add(demuxer,"Artist",buf);
	stream_read(s,buf,30);
	buf[30] = '\0';
	demux_info_add(demuxer,"Album",buf);
	stream_read(s,buf,4);
	buf[4] = '\0';
	demux_info_add(demuxer,"Year",buf);
	stream_read(s,buf,30);
	buf[30] = '\0';
	demux_info_add(demuxer,"Comment",buf);
	if(buf[28] == 0 && buf[29] != 0) {
	  uint8_t trk = (uint8_t)buf[29];
	  sprintf(buf,"%d",trk);
	  demux_info_add(demuxer,"Track",buf);
	}
	g = stream_read_char(s);
	demux_info_add(demuxer,"Genre",genres[g]);
      }
      }
      if(demuxer->movi_end >= 10) {
      stream_seek(s,demuxer->movi_end-10);
      stream_read(s,hdr,4);
      if(!memcmp(hdr,"3DI",3) && hdr[3] >= 4 && hdr[3] != 0xff) {
        unsigned int len = id3v2_tag_size(hdr[3], s);
        if(len > 0) {
          if(len > demuxer->movi_end - demuxer->movi_start) {
            mp_msg(MSGT_DEMUX,MSGL_WARN,MSGTR_MPDEMUX_AUDIO_BadID3v2TagSize,len);
            len = FFMIN(10,demuxer->movi_end-demuxer->movi_start);
          } else {
            stream_seek(s,demuxer->movi_end-len);
            stream_read(s,hdr,4);
            if(memcmp(hdr,"ID3",3) || hdr[3] < 4 || hdr[3] == 0xff || id3v2_tag_size(hdr[3], s) != len) {
              mp_msg(MSGT_DEMUX,MSGL_WARN,MSGTR_MPDEMUX_AUDIO_DamagedAppendedID3v2Tag);
              len = FFMIN(10,demuxer->movi_end-demuxer->movi_start);
            }
          }
          demuxer->movi_end -= len;
        }
      }
    }
    }
    if (duration && demuxer->movi_end && demuxer->movi_end > demuxer->movi_start) sh_audio->wf->nAvgBytesPerSec = (demuxer->movi_end - demuxer->movi_start) / duration;
    sh_audio->i_bps = sh_audio->wf->nAvgBytesPerSec;
    break;
  case WAV: {
    unsigned int chunk_type;
    unsigned int chunk_size;
    WAVEFORMATEX* w;
    int l;
    l = stream_read_dword_le(s);
    if(l < 16) {
      mp_msg(MSGT_DEMUX,MSGL_ERR,"[demux_audio] Bad wav header length: too short (%d)!!!\n",l);
      l = 16;
    }
    if(l > MAX_WAVHDR_LEN) {
      mp_msg(MSGT_DEMUX,MSGL_ERR,"[demux_audio] Bad wav header length: too long (%d)!!!\n",l);
      l = 16;
    }
    sh_audio->wf = w = malloc(l > sizeof(*w) ? l : sizeof(*w));
    w->wFormatTag = sh_audio->format = stream_read_word_le(s);
    w->nChannels = sh_audio->channels = stream_read_word_le(s);
    w->nSamplesPerSec = sh_audio->samplerate = stream_read_dword_le(s);
    w->nAvgBytesPerSec = stream_read_dword_le(s);
    w->nBlockAlign = stream_read_word_le(s);
    w->wBitsPerSample = stream_read_word_le(s);
    sh_audio->samplesize = (w->wBitsPerSample + 7) / 8;
    w->cbSize = 0;
    sh_audio->i_bps = sh_audio->wf->nAvgBytesPerSec;
    l -= 16;
    if (l >= 2) {
      w->cbSize = stream_read_word_le(s);
      l -= 2;
      if (l < w->cbSize) {
        mp_msg(MSGT_DEMUX,MSGL_ERR,"[demux_audio] truncated extradata (%d < %d)\n",
               l,w->cbSize);
        w->cbSize = l;
      }
      stream_read(s,(char*)(w + 1),w->cbSize);
      l -= w->cbSize;
      if (w->wFormatTag == 0xfffe && w->cbSize >= 22)
          sh_audio->format = av_le2ne16(((WAVEFORMATEXTENSIBLE *)w)->SubFormat);
    }

    if( mp_msg_test(MSGT_DEMUX,MSGL_V) ) print_wave_header(w, MSGL_V);
    if(l)
      stream_skip(s,l);
    do
    {
      chunk_type = stream_read_fourcc(demuxer->stream);
      chunk_size = stream_read_dword_le(demuxer->stream);
      if (chunk_type != mmioFOURCC('d', 'a', 't', 'a'))
        stream_skip(demuxer->stream, chunk_size);
    } while (!s->eof && chunk_type != mmioFOURCC('d', 'a', 't', 'a'));
    demuxer->movi_start = stream_tell(s);
    demuxer->movi_end = chunk_size ? demuxer->movi_start + chunk_size : s->end_pos;
//    printf("wav: %X .. %X\n",(int)demuxer->movi_start,(int)demuxer->movi_end);
    // Check if it contains dts audio
    if((w->wFormatTag == 0x01) && (w->nChannels == 2) && (w->nSamplesPerSec == 44100)) {
	unsigned char buf[16384]; // vlc uses 16384*4 (4 dts frames)
	unsigned int i;
	memset(buf, 0, sizeof(buf));
	stream_read(s, buf, sizeof(buf));
	for (i = 0; i < sizeof(buf) - 5; i += 2) {
	    // DTS, 14 bit, LE
	    if((buf[i] == 0xff) && (buf[i+1] == 0x1f) && (buf[i+2] == 0x00) &&
	       (buf[i+3] == 0xe8) && ((buf[i+4] & 0xfe) == 0xf0) && (buf[i+5] == 0x07)) {
		sh_audio->format = 0x2001;
		mp_msg(MSGT_DEMUX,MSGL_V,"[demux_audio] DTS audio in wav, 14 bit, LE\n");
		break;
	    }
	    // DTS, 14 bit, BE
	    if((buf[i] == 0x1f) && (buf[i+1] == 0xff) && (buf[i+2] == 0xe8) &&
	       (buf[i+3] == 0x00) && (buf[i+4] == 0x07) && ((buf[i+5] & 0xfe) == 0xf0)) {
		sh_audio->format = 0x2001;
		mp_msg(MSGT_DEMUX,MSGL_V,"[demux_audio] DTS audio in wav, 14 bit, BE\n");
		break;
	    }
	    // DTS, 16 bit, BE
	    if((buf[i] == 0x7f) && (buf[i+1] == 0xfe) && (buf[i+2] == 0x80) &&
	       (buf[i+3] == 0x01)) {
		sh_audio->format = 0x2001;
		mp_msg(MSGT_DEMUX,MSGL_V,"[demux_audio] DTS audio in wav, 16 bit, BE\n");
		break;
	    }
	    // DTS, 16 bit, LE
	    if((buf[i] == 0xfe) && (buf[i+1] == 0x7f) && (buf[i+2] == 0x01) &&
	       (buf[i+3] == 0x80)) {
		sh_audio->format = 0x2001;
		mp_msg(MSGT_DEMUX,MSGL_V,"[demux_audio] DTS audio in wav, 16 bit, LE\n");
		break;
	    }
	}
	if (sh_audio->format == 0x2001) {
	    sh_audio->needs_parsing = 1;
	    mp_msg(MSGT_DEMUX,MSGL_DBG2,"[demux_audio] DTS sync offset = %u\n", i);
        }

    }
    stream_seek(s,demuxer->movi_start);
  } break;
  case fLaC:
	    sh_audio->format = mmioFOURCC('f', 'L', 'a', 'C');
	    demuxer->movi_start = stream_tell(s) - 4;
	    demuxer->movi_end = s->end_pos;
	    if (demuxer->movi_end > demuxer->movi_start) {
	      // try to find out approx. bitrate
	      int64_t size = demuxer->movi_end - demuxer->movi_start;
	      int64_t num_samples;
	      int32_t srate;
	      stream_skip(s, 14);
	      srate = stream_read_int24(s) >> 4;
	      num_samples  = stream_read_int24(s) << 16;
	      num_samples |= stream_read_word(s);
	      if (num_samples && srate)
	        sh_audio->i_bps = size * srate / num_samples;
	    }
	    if (sh_audio->i_bps < 1) // guess value to prevent crash
	      sh_audio->i_bps = 64 * 1024;
	    sh_audio->needs_parsing = 1;
	    get_flac_metadata (demuxer);
		
		uint8_t cur_bytes[4];
		uint8_t apic_size[4];
		uint8_t apic_mime[4];
		uint8_t apic_desc[4];
		//u8 apic_datstep = 0;
		int len_mimetype = 0;
		int len_desc = 0;
		int val_apic_size = 0;
		if(found_ID3) {
		for(int i=32;i < loop_limit;i++) { // start at pos 32 should speed up search
			stream_seek(s, i);
			stream_read(s, cur_bytes, 4);
			if(cur_bytes[0] == 'a' && cur_bytes[1] == 'g' && cur_bytes[2] == 'e' && cur_bytes[3] == '/') {
			/*	stream_seek(s, i+4); // Get mime - jpeg, jpg, png
				stream_read(s, apic_mime, 4);
				if(apic_mime[3] == 'g') { // jpeg mimetype
					apic_datstep = 0x20;
					stream_seek(s, i+0x1C); // Get apic size
					stream_read(s,apic_size,4);
				} else {
					apic_datstep = 0x1F; // jpg/png mimetype
					stream_seek(s, i+0x1B); // Get apic size
					stream_read(s,apic_size,4);
				}*/
				stream_seek(s, (i-0xA)+4); //mimetype length
				stream_read(s, apic_mime, 4);
				len_mimetype = (apic_mime[0] << 24) + (apic_mime[1] << 16) + (apic_mime[2] << 8) + apic_mime[3];
				
				stream_seek(s, (i-0xA)+8+len_mimetype); //desc length
				stream_read(s, apic_desc, 4);
				len_desc = (apic_desc[0] << 24) + (apic_desc[1] << 16) + (apic_desc[2] << 8) + apic_desc[3];
				
				stream_seek(s, (i-0xA)+8+len_mimetype+len_desc+4+4+4+4+4);
				stream_read(s, apic_size, 4);
				
				//Goto pic
				val_apic_size = (apic_size[0] << 24) + (apic_size[1] << 16) + (apic_size[2] << 8) + apic_size[3];
				if(val_apic_size > 1.5*1024*1024)
					break;
				//wiim_inf = val_apic_size; // For debug
				
				pos_pic = (u8 *)mem2_memalign(32, 1.5*1024*1024, MEM2_OTHER);
				stream_seek(s, (i-0xA)+8+len_mimetype+len_desc+24); // Get pic data
				stream_read(s, pos_pic, val_apic_size);
				//enable cover art in gui
				embedded_pic = 1;
				break;
			}
		}
		}
		
	    break;
  }

  priv = malloc(sizeof(da_priv_t));
  priv->frmt = frmt;
  priv->next_pts = 0;
  demuxer->priv = priv;
  demuxer->audio->id = 0;
  demuxer->audio->sh = sh_audio;
  sh_audio->ds = demuxer->audio;
  sh_audio->samplerate = sh_audio->audio.dwRate;

  if(stream_tell(s) != demuxer->movi_start)
  {
    mp_msg(MSGT_DEMUX, MSGL_V, "demux_audio: seeking from 0x%X to start pos 0x%X\n",
            (int)stream_tell(s), (int)demuxer->movi_start);
    stream_seek(s,demuxer->movi_start);
    if (stream_tell(s) != demuxer->movi_start) {
      mp_msg(MSGT_DEMUX, MSGL_V, "demux_audio: seeking failed, now at 0x%X!\n",
              (int)stream_tell(s));
      if (next_frame_pos) {
        mp_msg(MSGT_DEMUX, MSGL_V, "demux_audio: seeking to 0x%X instead\n",
                (int)next_frame_pos);
        stream_seek(s, next_frame_pos);
      }
    }
  }

  mp_msg(MSGT_DEMUX,MSGL_V,"demux_audio: audio data 0x%X - 0x%X  \n",(int)demuxer->movi_start,(int)demuxer->movi_end);

  return DEMUXER_TYPE_AUDIO;
}


static int demux_audio_fill_buffer(demuxer_t *demux, demux_stream_t *ds) {
  int l;
  demux_packet_t* dp;
  sh_audio_t* sh_audio = ds->sh;
  da_priv_t* priv = demux->priv;
  double this_pts = priv->next_pts;
  stream_t* s = demux->stream;

  if(s->eof)
    return 0;

  switch(priv->frmt) {
  case MP3 :
    while(1) {
      uint8_t hdr[4];
      stream_read(s,hdr,4);
      if (s->eof)
        return 0;
      l = mp_decode_mp3_header(hdr);
      if(l < 0) {
	if (demux->movi_end && stream_tell(s) >= demux->movi_end)
	  return 0; // might be ID3 tag, i.e. EOF
	stream_skip(s,-3);
      } else {
	dp = new_demux_packet(l);
	memcpy(dp->buffer,hdr,4);
	if (stream_read(s,dp->buffer + 4,l-4) != l-4)
	{
	  free_demux_packet(dp);
	  return 0;
	}
	priv->next_pts += sh_audio->audio.dwScale/(double)sh_audio->samplerate;
	break;
      }
    } break;
  case WAV : {
    unsigned align = sh_audio->wf->nBlockAlign;
    l = sh_audio->wf->nAvgBytesPerSec;
    if (l <= 0) l = 65536;
    if (demux->movi_end && l > demux->movi_end - stream_tell(s)) {
      // do not read beyond end, there might be junk after data chunk
      l = demux->movi_end - stream_tell(s);
      if (l <= 0) return 0;
    }
    if (align)
      l = (l + align - 1) / align * align;
    dp = new_demux_packet(l);
    l = stream_read(s,dp->buffer,l);
    priv->next_pts += l/(double)sh_audio->i_bps;
    break;
  }
  case fLaC: {
    l = 65535;
    dp = new_demux_packet(l);
    l = stream_read(s,dp->buffer,l);
    //priv->next_pts = MP_NOPTS_VALUE;
    priv->next_pts += l/(double)sh_audio->i_bps;
    break;
  }
  default:
    mp_msg(MSGT_DEMUXER,MSGL_WARN,MSGTR_MPDEMUX_AUDIO_UnknownFormat,priv->frmt);
    return 0;
  }

  resize_demux_packet(dp, l);
  dp->pts = this_pts;
  ds_add_packet(ds, dp);
  return 1;
}

static void high_res_mp3_seek(demuxer_t *demuxer,float time) {
  uint8_t hdr[4];
  int len,nf;
  da_priv_t* priv = demuxer->priv;
  sh_audio_t* sh = (sh_audio_t*)demuxer->audio->sh;

  nf = time*sh->samplerate/sh->audio.dwScale;
  while(nf > 0) {
    stream_read(demuxer->stream,hdr,4);
    len = mp_decode_mp3_header(hdr);
    if(len < 0) {
      stream_skip(demuxer->stream,-3);
      continue;
    }
    stream_skip(demuxer->stream,len-4);
    priv->next_pts += sh->audio.dwScale/(double)sh->samplerate;
    nf--;
  }
}

static void demux_audio_seek(demuxer_t *demuxer,float rel_seek_secs,float audio_delay,int flags){
  sh_audio_t* sh_audio;
  stream_t* s;
  int64_t base,pos;
  float len;
  da_priv_t* priv;

  if(!(sh_audio = demuxer->audio->sh))
    return;
  s = demuxer->stream;
  priv = demuxer->priv;

  if(priv->frmt == MP3 && hr_mp3_seek && !(flags & SEEK_FACTOR)) {
    len = (flags & SEEK_ABSOLUTE) ? rel_seek_secs - priv->next_pts : rel_seek_secs;
    if(len < 0) {
      stream_seek(s,demuxer->movi_start);
      len = priv->next_pts + len;
      priv->next_pts = 0;
    }
    if(len > 0)
      high_res_mp3_seek(demuxer,len);
    return;
  }

  base = flags&SEEK_ABSOLUTE ? demuxer->movi_start : stream_tell(s);
  if(flags&SEEK_FACTOR)
    pos = base + ((demuxer->movi_end - demuxer->movi_start)*rel_seek_secs);
  else
    pos = base + (rel_seek_secs*sh_audio->i_bps);

  if(demuxer->movi_end && pos >= demuxer->movi_end) {
     pos = demuxer->movi_end;
  } else if(pos < demuxer->movi_start)
    pos = demuxer->movi_start;

  priv->next_pts = (pos-demuxer->movi_start)/(double)sh_audio->i_bps;

  switch(priv->frmt) {
  case WAV:
    pos -= (pos - demuxer->movi_start) %
            (sh_audio->wf->nBlockAlign ? sh_audio->wf->nBlockAlign :
             (sh_audio->channels * sh_audio->samplesize));
    break;
  }

  stream_seek(s,pos);
}

static void demux_close_audio(demuxer_t* demuxer) {
  da_priv_t* priv = demuxer->priv;

  free(priv);
}

static int demux_audio_control(demuxer_t *demuxer,int cmd, void *arg){
    sh_audio_t *sh_audio=demuxer->audio->sh;
    int audio_length = sh_audio->i_bps && demuxer->movi_end > demuxer->movi_start ?
                       (demuxer->movi_end - demuxer->movi_start) / sh_audio->i_bps : 0;
    da_priv_t* priv = demuxer->priv;

    switch(cmd) {
	case DEMUXER_CTRL_GET_TIME_LENGTH:
	    if (audio_length<=0) return DEMUXER_CTRL_DONTKNOW;
	    *((double *)arg)=(double)audio_length;
	    return DEMUXER_CTRL_GUESS;

	case DEMUXER_CTRL_GET_PERCENT_POS:
	    if (audio_length<=0)
    		return DEMUXER_CTRL_DONTKNOW;
    	    *((int *)arg)=(int)( (priv->next_pts*100)  / audio_length);
	    return DEMUXER_CTRL_OK;

	default:
	    return DEMUXER_CTRL_NOTIMPL;
    }
}


const demuxer_desc_t demuxer_desc_audio = {
  "Audio demuxer",
  "audio",
  "Audio only",
  "?",
  "Audio only files",
  DEMUXER_TYPE_AUDIO,
  0, // unsafe autodetect
  demux_audio_open,
  demux_audio_fill_buffer,
  NULL,
  demux_close_audio,
  demux_audio_seek,
  demux_audio_control
};
