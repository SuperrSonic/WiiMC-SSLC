/*
   MPlayer timer for Wii

   Copyright (C) 2008 dhewg

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


#include <unistd.h>

#include <ogc/lwp_watchdog.h>
#include <sys/time.h>
#include "timer.h"

const char timer_name[] = "gekko";


/*
int usec_sleep(unsigned int usec_delay) {
	return usleep(usec_delay);
}*
// Returns current time in microseconds
u64 GetTimer(void) {
	return ticks_to_microsecs(gettime());
}

// Returns current time in milliseconds
u64 GetTimerMS(void) {
	return ticks_to_millisecs(gettime());
}
*/

static u64 RelativeTime=0;
/*
double GetRelativeTime(void){
  u64 t,r;
  t=GetTimer();

  r=t-RelativeTime;
  RelativeTime=t;
  return (double)(r * (double)0.000001);
}
*/
u64 GetRelativeTime(void){
  u64 t,r;
  t=GetTimer();

  r=t-RelativeTime;
  RelativeTime=t;
  return r;
}
/*
double GetRelativeTime(void) {
	u64 t;
	float res;

	t = gettime();
	res = (double) ticks_to_nanosecs(diff_ticks(t, RelativeTime)) /
			(double) TB_NSPERSEC;
	RelativeTime = t;

	return res;
}
*/

void InitTimer(void) {
	GetRelativeTime();
}
