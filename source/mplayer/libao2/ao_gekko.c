/*
   ao_gekko.c - MPlayer audio driver for Wii

   Copyright (C) 2008 dhewg
   Improved by Tantric, rodries and Extrems

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301 USA.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <ogcsys.h>

#include "config.h"
#include "libaf/af.h"
#include "libaf/af_format.h"
#include "audio_out.h"
#include "audio_out_internal.h"
#include "mp_msg.h"
#include "help_mp.h"
#include "osdep/ave-rvl.h"

#define BUFFER_SIZE 	4096
#define BUFFER_COUNT 	32

#define HW_CHANNELS 	2

#define PAN_CENTER 		0.7071067811865475f		// sqrt(1/2)
#define PAN_SIDE 		0.871779788708134f      // sqrt(19/25)						
#define PAN_SIDE_INV 	0.4898979485566356f		// sqrt(6/25)

#define PHASE_SHF 		0.25f					// "90 degrees"
#define PHASE_SHF_INV 	0.75f

static const ao_info_t info = {
	"gekko audio output",
	"gekko",
	"Team Twiizers",
	""
};

LIBAO_EXTERN(gekko)

static bool playing = false;
static u8 buffers[BUFFER_COUNT][BUFFER_SIZE] ATTRIBUTE_ALIGN(32);
static u8 silence[BUFFER_SIZE] ATTRIBUTE_ALIGN(32);
static u8 buffer_fill = 0;
static u8 buffer_play = 0;
static u8 quality = AI_SAMPLERATE_48KHZ;
static int buffered = 0;
static int request_size = BUFFER_SIZE;
static float request_mult = 1.0;
static ao_control_vol_t volume = { 0x8E, 0x8E };

static void switch_buffers()
{
	if (playing && buffered > 0)
	{
		AUDIO_InitDMA((u32)buffers[buffer_play], BUFFER_SIZE);
		buffer_play = (buffer_play + 1) % BUFFER_COUNT;
		buffered -= BUFFER_SIZE;
	}
	else
	{
		AUDIO_InitDMA((u32)silence, BUFFER_SIZE);
	}
}

static int control(int cmd, void *arg)
{
	switch (cmd)
	{
		case AOCONTROL_QUERY_FORMAT:
			return CONTROL_TRUE;
		case AOCONTROL_GET_VOLUME:
		case AOCONTROL_SET_VOLUME:
		{
			ao_control_vol_t *vol = (ao_control_vol_t *)arg;
			
			if (cmd == AOCONTROL_SET_VOLUME)
			{
				volume.left = (vol->left / 100.0) * 0xFF;
				volume.right = (vol->right / 100.0) * 0xFF;
				
				VIWriteI2CRegister8(AVE_AI_VOLUME, clamp(volume.left, 0x00, 0xFF));
				VIWriteI2CRegister8(AVE_AI_VOLUME + 1, clamp(volume.right, 0x00, 0xFF));
			}
			
			vol->left = (volume.left / 0xFF) * 100.0;
			vol->right = (volume.right / 0xFF) * 100.0;
			
			return CONTROL_OK;
		}
		default:
			return CONTROL_UNKNOWN;
	}
}

void reinit_audio()
{
	AUDIO_RegisterDMACallback(switch_buffers);
/*
	if(!playing)
	{
		switch_buffers();
		AUDIO_StartDMA();
	}
*/	
}

static int init(int rate, int channels, int format, int flags)
{
	AUDIO_StopDMA();

	if(rate>32000) quality = AI_SAMPLERATE_48KHZ; 
	else quality = AI_SAMPLERATE_32KHZ;
	ao_data.samplerate = (quality==AI_SAMPLERATE_48KHZ) ? 48000 : 32000;
	ao_data.channels = clamp(channels, 2, 6);
	ao_data.format = AF_FORMAT_S16_NE;
	ao_data.bps = ao_data.channels * ao_data.samplerate * sizeof(s16);
	request_mult = (float)ao_data.channels / HW_CHANNELS;
	request_size = BUFFER_SIZE * request_mult;
	ao_data.buffersize = request_size * BUFFER_COUNT;
	ao_data.outburst = request_size;
	
	for (int counter = 0; counter < BUFFER_COUNT; counter++)
	{
		memset(buffers[counter], 0, BUFFER_SIZE);
		DCFlushRange(buffers[counter], BUFFER_SIZE);
	}

	memset(silence, 0, BUFFER_SIZE);
	DCFlushRange(silence, BUFFER_SIZE);

	buffer_fill = 0;
	buffer_play = 0;
	buffered = 0;

	playing = false;

	AUDIO_SetDSPSampleRate(quality);
	AUDIO_RegisterDMACallback(switch_buffers);

	return CONTROL_TRUE;
}

static void reset(void)
{
	AUDIO_RegisterDMACallback(NULL);

	while(AUDIO_GetDMABytesLeft() > 0)
		usleep(100);

	playing = false;

	buffer_fill = 0;
	buffer_play = 0;
	buffered = 0;
	for (int counter = 0; counter < BUFFER_COUNT; counter++)
	{
		memset(buffers[counter], 0, BUFFER_SIZE);
		DCFlushRange(buffers[counter], BUFFER_SIZE);
	}
	AUDIO_RegisterDMACallback(switch_buffers);
}

static void uninit(int immed)
{
	reset();
	AUDIO_RegisterDMACallback(NULL);
	while(AUDIO_GetDMABytesLeft() > 0)
		usleep(100);
	AUDIO_StopDMA();
}

static void audio_pause(void)
{
	playing = false;
}

static void audio_resume(void)
{
	playing = true;
	if(AUDIO_GetDMAEnableFlag()==0)
	{
		switch_buffers();
		AUDIO_StartDMA();
	}
}

static int get_space(void)
{
	return ((BUFFER_SIZE * (BUFFER_COUNT - 2)) - buffered) * request_mult;
}

static inline void copy_channels(s16 *dst, s16 *src, int len, int processed, int remaining)
{
	s32 left=0, right=0;
	int cws=0, crs=0;
	int prs = -1, nrs = 1;
	int top = len - 1;

	for (int counter = 0; counter < len; ++counter)
	{
		if (ao_data.channels > 1)
		{
			left = src[crs];
			right = src[crs + 1];
		}
		else
		{
			left = right = src[crs];
		}


		switch (ao_data.channels)
		{
			case 6:
			case 5:
				// Left rear
				left += ((src[crs + 2] * PHASE_SHF_INV) + (src[nrs + 2] * PHASE_SHF)) * PAN_SIDE;
				right += ((src[crs + 2] * PHASE_SHF_INV) + (src[prs + 2] * PHASE_SHF)) * PAN_SIDE_INV;
				
				// Right rear
				left += ((src[crs + 3] * PHASE_SHF_INV) + (src[nrs + 3] * PHASE_SHF)) * PAN_SIDE_INV;
				right += ((src[crs + 3] * PHASE_SHF_INV) + (src[prs + 3] * PHASE_SHF)) * PAN_SIDE;
				
				// Center front
				left += src[crs + 4] * PAN_CENTER;
				right += src[crs + 4] * PAN_CENTER;
				break;
			case 4:
				// Center rear
				left += ((src[crs + 2] * PHASE_SHF_INV) + (src[nrs + 2] * PHASE_SHF)) * PAN_CENTER;
				right += ((src[crs + 2] * PHASE_SHF_INV) + (src[prs + 2] * PHASE_SHF)) * PAN_CENTER;
				
				// Center front
				left += src[crs + 3] * PAN_CENTER;
				right += src[crs + 3] * PAN_CENTER;
				break;
		}
	
		dst[cws++] = clamp(right, SHRT_MIN, SHRT_MAX);
		dst[cws++] = clamp(left, SHRT_MIN, SHRT_MAX);

		crs += ao_data.channels;
		prs++;
		nrs++;
	}
}

static int play(void *data, int remaining, int flags)
{
	int processed = 0;
	int samples = BUFFER_SIZE / (sizeof(s16) * HW_CHANNELS);
	s16 *source = (s16 *)data;

	while (remaining >= request_size && get_space() >= request_size)
	{
		copy_channels((s16 *)buffers[buffer_fill], source, samples, processed, remaining);
		DCStoreRangeNoSync(buffers[buffer_fill], BUFFER_SIZE);

		buffer_fill = (buffer_fill + 1) % BUFFER_COUNT;

		processed += request_size;
		source += request_size / sizeof(s16);
		buffered += BUFFER_SIZE;
		remaining -= request_size;		
	}

	if ((flags & AOPLAY_FINAL_CHUNK) && remaining > 0)
	{
		samples = remaining / (sizeof(s16) * HW_CHANNELS);
		memset(buffers[buffer_fill], 0, BUFFER_SIZE);
		copy_channels((s16 *)buffers[buffer_fill], source, samples, processed, 0);
		DCStoreRangeNoSync(buffers[buffer_fill], BUFFER_SIZE);
		buffer_fill = (buffer_fill + 1) % BUFFER_COUNT;

		processed += remaining;
		buffered += BUFFER_SIZE;
	}

	if (!playing)// && buffered > request_size)
	{
		playing = true;
		switch_buffers();
		AUDIO_StartDMA();
	}
	return processed;
}

static float get_delay(void)
{
	if (playing)
		return (float)((buffered + AUDIO_GetDMABytesLeft()) * request_mult) / (float)ao_data.bps;
	else
		return (float)(buffered * request_mult) / (float)ao_data.bps;
}
