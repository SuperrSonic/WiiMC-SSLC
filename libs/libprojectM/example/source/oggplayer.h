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

#ifndef _OGGPLAYER_H_
#define _OGGPLAYER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _eqState {
    f32 lf;
    f32 f1p0;
    f32 f1p1;
    f32 f1p2;
    f32 f1p3;
    
    f32 hf;
    f32 f2p0;
    f32 f2p1;
    f32 f2p2;
    f32 f2p3;
    
    f32 sdm1;
    f32 sdm2;
    f32 sdm3;
    
    f32 lg;
    f32 mg;
    f32 hg;
} eqState;

typedef union _dword {
    struct {
        u16 hi;
        u16 lo;
    } aword;
    u32 adword;
} dword;

// Event callbacks
typedef void (*ogg_cb_pcmdecode) (u32 *pcm, u32 samples);

// Ogg player routines
bool oggPlay (const char *filepath, ogg_cb_pcmdecode onpcmdecode);
bool oggStop ();
bool oggUpdate ();

#ifdef __cplusplus
}
#endif

#endif /* _OGGPLAYER_H_ */
