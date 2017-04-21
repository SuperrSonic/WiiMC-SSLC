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
/**
 * $Id: timer.h,v 1.1.1.1 2005/12/23 18:05:00 psperl Exp $
 *
 * Platform-independent timer
 *
 * $Log: timer.h,v $
 * Revision 1.1.1.1  2005/12/23 18:05:00  psperl
 * Imported
 *
 * Revision 1.2  2004/10/05 09:19:40  cvs
 * Fixed header include defines
 *
 * Revision 1.1.1.1  2004/10/04 12:56:00  cvs
 * Imported
 *
 */

#ifndef _TIMER_H
#define _TIMER_H

#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned int getTicks( struct timeval *start );
struct timeval GetCurrentTime();

void gekko_gettimeofday(struct timeval *tv, void *tz);

#define gettimeofday(TV, TZ) gekko_gettimeofday((TV), (TZ))

#ifdef __cplusplus
}
#endif

#endif /** _TIMER_H */
