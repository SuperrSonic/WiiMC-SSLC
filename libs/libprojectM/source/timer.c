/**
 * projectM -- Milkdrop-esque visualisation SDK
 * Copyright (C)2003-2004 projectM Team
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
/**
 * $Id: timer.c,v 1.1.1.1 2005/12/23 18:05:05 psperl Exp $
 *
 * Platform-independent timer
 */

#include "timer.h"
#include <stdlib.h>
#include <gctypes.h>
#include <ogc/lwp_watchdog.h>

/** Get number of ticks since the given timestamp */
unsigned int getTicks( struct timeval *start ) {
    struct timeval now;
    unsigned int ticks;
    
    gettimeofday(&now, NULL);
    ticks=(now.tv_sec-start->tv_sec)*1000+(now.tv_usec-start->tv_usec)/1000;
    return(ticks);
}

void gekko_gettimeofday (struct timeval *tv, void *tz) {
    u64 t;
    t=gettime();
    tv->tv_sec = ticks_to_secs(t);
    tv->tv_usec = ticks_to_microsecs(t);
}
