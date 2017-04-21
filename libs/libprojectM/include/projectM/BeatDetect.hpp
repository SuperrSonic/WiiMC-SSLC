/**
 * projectM -- Milkdrop-esque visualisation SDK
 * Copyright (C)2003-2007 projectM Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * See 'LICENSE.txt' included within this release
 *
 */

#ifndef _BEAT_DETECT_H
#define _BEAT_DETECT_H

#include <projectM/PCM.hpp>

/* Forward declarations */
typedef struct _pm_config pm_config;

/**
 * Audio beat detection class.
 * Parses various characteristics from decompressed sound buffers 
 */
class BeatDetect
{
    public:
        
        float treb ;
        float mid ;
        float bass ;
        float vol_old ;
        float treb_att ;
        float mid_att ;
        float bass_att ;
        float vol;
        
        PCM *pcm;
        
        /** Methods */
        BeatDetect(const pm_config &settings, PCM *pcm);
        ~BeatDetect();
        void initBeatDetect();
        void reset();
        void detectFromSamples();
        void getBeatVals ( const float *vdataL, const float *vdataR );
        
    private:
        
        /** Vars */
        const pm_config &settings;
        float beat_buffer[32][80],
        beat_instant[32],
        beat_history[32];
        float beat_val[32],
        beat_att[32],
        beat_variance[32];
        int beat_buffer_pos;
        float vol_buffer[80],
        vol_instant,
        vol_history;
        
};

#endif /** !_BEAT_DETECT_H */
