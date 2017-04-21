/**
 * oggplayer.c - A very simple ogg player.
 *
 * Copyright (c) 2008-2009 DragonMinded
 * Copyright (c) 2009 Rhys "Shareese" Koedijk
 *
 * This program/include file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program/include file is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <gctypes.h>
#include <gccore.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <malloc.h>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include "oggplayer.h"

#ifndef PATH_MAX
#  define PATH_MAX 1024
#endif

#define AUDIO_BUFFER_SIZE               (1 * 1024)   /* 1k */
#define RING_BUFFER_MAX_SIZE            (128 * 1024) /* 128k */
#define RING_BUFFER_MIN_SIZE            (32 * 1024)  /* 32k */
#define DECODE_BUFFER_SIZE              (1 * 1024)   /* 1k */

static u32 audioBuffer[2][AUDIO_BUFFER_SIZE] ATTRIBUTE_ALIGN(32) = { { 0 } };
static u32 audioCurrentBuffer = 0;

static u8 ringBuffer[RING_BUFFER_MAX_SIZE] ATTRIBUTE_ALIGN(32) = { 0 };
static u32 ringRead = 0;
static u32 ringWrite = 0;

static char path[PATH_MAX] = { 0 };
static OggVorbis_File vf;
static vorbis_info *vi = NULL;
static eqState eqs[2] = { { 0 } };

static ogg_cb_pcmdecode pcmdecode = NULL;

void ringBufferPut (void *data, u32 size)
{
    // Sanity check
    if (!data || size < 0)
        return;
    
    // If the data will fit then copy it in as one chunk
    if (ringWrite + size < RING_BUFFER_MAX_SIZE) {
        memcpy(ringBuffer + ringWrite, data, size);
        ringWrite += size;
    
    // Else we will have to copy the data in as two chunks
    } else {
        u32 firstChunk = RING_BUFFER_MAX_SIZE - ringWrite;       
        memcpy(ringBuffer + ringWrite, data, firstChunk);
        memcpy(ringBuffer, ((u32 *) data) + firstChunk, size - firstChunk);
        ringWrite = size - firstChunk;        
    }
}

void ringBufferGet (void *data, u32 size)
{    
    // Sanity check
    if (!data || size < 0)
        return;
    
    // If the data will fit then copy it out as one chunk
    if (ringRead + size < RING_BUFFER_MAX_SIZE) {
        memcpy(data, ringBuffer + ringRead, size);
        ringRead += size;
    
    // Else we will have to copy the data out as two chunks
    } else {
        u32 firstChunk = RING_BUFFER_MAX_SIZE - ringRead;        
        memcpy(data, ringBuffer + ringRead, firstChunk);
        memcpy(((u32 *) data) + firstChunk, ringBuffer, size - firstChunk);
        ringRead = size - firstChunk;
    }
}

u32 ringBufferSize ()
{
    // Figure out how full the ring buffer is
    if(ringWrite < ringRead)
        return (ringWrite + RING_BUFFER_MAX_SIZE) - ringRead;
    else
        return ringWrite - ringRead;
}

void Init3BandState (eqState *es, s32 lowfreq, s32 highfreq, s32 mixfreq)
{
    memset(es, 0, sizeof(eqState));
    
    es->lg = 1.0;
    es->mg = 1.0;
    es->hg = 1.0;
    
    es->lf = 2.0f * sin(M_PI * ((f32) lowfreq / (f32) mixfreq));
    es->hf = 2.0f * sin(M_PI * ((f32) highfreq / (f32) mixfreq));
}

s16 Do3Band(eqState *es, s16 sample)
{
    static f32 VSA = (1.0/4294967295.0);
    f32 l, m, h;
    
    es->f1p0 += (es->lf * ((f32) sample - es->f1p0)) + VSA;
    es->f1p1 += (es->lf * (es->f1p0 - es->f1p1));
    es->f1p2 += (es->lf * (es->f1p1 - es->f1p2));
    es->f1p3 += (es->lf * (es->f1p2 - es->f1p3));
    l = es->f1p3;
    
    es->f2p0 += (es->hf * ((f32) sample - es->f2p0)) + VSA;
    es->f2p1 += (es->hf * (es->f2p0 - es->f2p1));
    es->f2p2 += (es->hf * (es->f2p1 - es->f2p2));
    es->f2p3 += (es->hf * (es->f2p2 - es->f2p3));
    h = es->sdm3 - es->f2p3;
    
    m = es->sdm3 - (h+l);
    
    l *= es->lg;
    m *= es->mg;
    h *= es->hg;
    
    es->sdm3 = es->sdm2;
    es->sdm2 = es->sdm1;
    es->sdm1 = (f32) sample;
    
    return (s16) (l + m + h);
}

void Resample(s16 *samples, int numSamples, eqState eqs[2], u32 src_samplerate)
{
    u16 val16;
    u32 val32;
    dword pos;
    s32 incr;
    
    pos.adword = 0;
    incr = (u32) (((f32) src_samplerate / 48000.0F) * 65536.0F);
    
    while(pos.aword.hi < numSamples) {
        
        // Channel 1 resample
        val16 = Do3Band(&eqs[0], samples[pos.aword.hi << 1]);
        val32 = (val16 << 16);
        
        // Channel 2 resample
        val16 = Do3Band(&eqs[1], samples[(pos.aword.hi << 1) + 1]);
        val32 |= val16;
        
        // Place into output ring buffer
        ringBufferPut(&val32, sizeof(u32));
        
        // Next sample
        pos.adword += incr;
        
    }
}

void audioSwapBuffers ()
{
    // Fill up the audio buffer from the ring buffer
    ringBufferGet(audioBuffer[audioCurrentBuffer], AUDIO_BUFFER_SIZE);
    DCFlushRange(audioBuffer[audioCurrentBuffer], AUDIO_BUFFER_SIZE);
    
    // Send the current audio buffer
    AUDIO_StopDMA();
    AUDIO_InitDMA((u32) audioBuffer[audioCurrentBuffer], AUDIO_BUFFER_SIZE);
    AUDIO_StartDMA();
    
    // Call the 'on decode' event callback (if any)
    if (pcmdecode)
        pcmdecode(audioBuffer[audioCurrentBuffer], AUDIO_BUFFER_SIZE);
    
    // Swap the audio buffers
    audioCurrentBuffer ^= 1;
}

bool oggPlay (const char *filepath, ogg_cb_pcmdecode onpcmdecode)
{
    // Sanity check
    if (!filepath)
        return false;

    // Setup the audio system
    AUDIO_SetDSPSampleRate(AI_SAMPLERATE_48KHZ);
    AUDIO_RegisterDMACallback(audioSwapBuffers);

    // Setup callbacks
    pcmdecode = onpcmdecode;
    
    // Open the audio file
    strncpy(path, filepath, PATH_MAX - 1);
    if (ov_fopen(path, &vf) != 0)
        return false;
    
    // Get the audio files stream information
    vi = ov_info(&vf, -1);
    if (!vi)
        return false;
    
    // We only support mono (1) or stereo (2) audio
    if(vi->channels > 2) {
        ov_clear(&vf);
        return false;
    }

    // Initialise the equalizer bands (for resampling)
    Init3BandState(&eqs[0], 880, 5000, 48000);
    Init3BandState(&eqs[1], 880, 5000, 48000);
    
    //...
    oggUpdate();
    
    // Start DMA transfer for the first time
    audioSwapBuffers();

    return true;
}

bool oggStop ()
{
    // Stop the audio system
    AUDIO_StopDMA();
    AUDIO_RegisterDMACallback(NULL);
    
    // Close the audio file
    ov_clear(&vf);

    return true;
}

bool oggUpdate ()
{
    int i;
    
    // Fill up the ring buffer to (at least) the minimal buffer size
    while (ringBufferSize() < RING_BUFFER_MIN_SIZE) {
        s16 samples[DECODE_BUFFER_SIZE] = { 0 };
        u32 sampleCount = 0;
        s32 bitstream = 0;
        
        // Decode audio to PCM (mono)
        if (vi->channels == 1) {
            s16 *temp = (s16 *) memalign(32, DECODE_BUFFER_SIZE / 2);
            u32 ret = ov_read(&vf, (char *) temp, DECODE_BUFFER_SIZE / 2, 1, 2, 1, &bitstream);
            if(ret > 0) {
                sampleCount = ret / 2;
                
                // Copy mono to stereo
                for(i = 0; i < sampleCount; i++) {
                    samples[i << 1] = temp[i];
                    samples[(i << 1) + 1] = temp[i];
                }
                
            } else {
                sampleCount = 0;
            }
            free(temp);
            
        // Decode audio to PCM (stereo)
        } else if (vi->channels == 2) {
            u32 ret = ov_read(&vf, (char *) samples, DECODE_BUFFER_SIZE, 1, 2, 1, &bitstream);
            if(ret > 0)
                sampleCount = ret / 4;
            else
                sampleCount = 0;
        }
        
        // If no samples were decoded then we must have finished playing
        if (sampleCount <= 0) {
            oggStop();
            return false;
        }

        // Resample the PCM and put it in the ring buffer
        Resample(samples, sampleCount, eqs, vi->rate);
    
    }
    
    return true;
}
