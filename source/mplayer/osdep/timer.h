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

#ifndef MPLAYER_TIMER_H
#define MPLAYER_TIMER_H

extern const char timer_name[];

void InitTimer(void);
#ifdef GEKKO
#include <gctypes.h>
#include <ogc/lwp_watchdog.h>
#include <unistd.h>
inline static u64 GetTimer(void) {return ticks_to_microsecs(gettime());}
inline static u64 GetTimerMS(void){return ticks_to_millisecs(gettime());}
inline static int usec_sleep(unsigned long usec_delay){return usleep(usec_delay);}
u64 GetRelativeTime(void);
#else
unsigned int GetTimer(void);
unsigned int GetTimerMS(void);
float GetRelativeTime(void);

int usec_sleep(int usec_delay);
#endif
/* timer's callback handling */
typedef void timer_callback( void );
unsigned set_timer_callback(unsigned ms,timer_callback func);
void restore_timer(void);

#endif /* MPLAYER_TIMER_H */
