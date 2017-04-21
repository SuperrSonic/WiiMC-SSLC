/*
 * Copyright (c) 2010-2011 Extrems <metaradil@gmail.com>
 *
 * This file is part of MPlayer CE.
 *
 * MPlayer CE is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * MPlayer CE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with MPlayer CE; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVCODEC_PPC_UTIL_PAIRED_H
#define AVCODEC_PPC_UTIL_PAIRED_H

#include <stdint.h>

#define FAST_LSCALE(power, type) \
({	register uint32_t gqr; \
	gqr = (((power) << 8) | (type)) << 16; \
	asm volatile("mtspr 913,%0" : : "r"(gqr)); })

#define FAST_STSCALE(power, type) \
({	register uint32_t gqr; \
	gqr = ((power) << 8) | (type); \
	asm volatile("mtspr 913,%0" : : "r"(gqr)); })

#define QUANT_REVERSE(power, type) \
({	register uint32_t gqr; \
	gqr  = (64 - (power) << 8) | (type); \
	gqr |= (gqr << 16); \
	asm volatile("mtspr 913,%0" : : "r"(gqr)); })

#endif /* AVCODEC_PPC_UTIL_PAIRED_H */
