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

#ifndef AVCODEC_PPC_DEINTERLACE_H
#define AVCODEC_PPC_DEINTERLACE_H

#include "config.h"

#if HAVE_PAIRED
#include "libavutil/ppc/paired.h"

static void ff_deinterlace_line_paired(uint8_t *dst, const uint8_t *lum_m4, const uint8_t *lum_m3, const uint8_t *lum_m2, const uint8_t *lum_m1, const uint8_t *lum, int size)
{
	const float scalar = 0.125;
	const vec_f32_t power = {2.0,4.0};
	const float half = 0.5;
	
	vector float pair, result;
	
	for (int i = 0; i < size; i += 2) {
		pair = psq_lx(i,lum_m4,0,4);
		result = paired_neg(pair);
		
		pair = psq_lx(i,lum_m3,0,4);
		result = paired_madds1(pair, power, result);
		
		pair = psq_lx(i,lum_m2,0,4);
		result = paired_madds0(pair, power, result);
		
		pair = psq_lx(i,lum_m1,0,4);
		result = paired_madds1(pair, power, result);
		
		pair = psq_lx(i,lum,0,4);
		result = paired_sub(result, pair);
		
		result = ps_madd(result, scalar, half);
		psq_stx(result,i,dst,0,4);
	}
}

static void ff_deinterlace_line_inplace_paired(uint8_t *lum_m4, uint8_t *lum_m3, uint8_t *lum_m2, uint8_t *lum_m1, uint8_t *lum, int size)
{
	const float scalar = 0.125;
	const vec_f32_t power = {2.0,4.0};
	const float half = 0.5;
	
	vector float pair, result;
	
	for (int i = 0; i < size; i += 2) {
		pair = psq_lx(i,lum_m4,0,4);
		result = paired_neg(pair);
		
		pair = psq_lx(i,lum_m3,0,4);
		result = paired_madds1(pair, power, result);
		
		pair = psq_lx(i,lum_m2,0,4);
		psq_stx(pair,i,lum_m4,0,4);
		result = paired_madds0(pair, power, result);
		
		pair = psq_lx(i,lum_m1,0,4);
		result = paired_madds1(pair, power, result);
		
		pair = psq_lx(i,lum,0,4);
		result = paired_sub(result, pair);
		
		result = ps_madd(result, scalar, half);
		psq_stx(result,i,lum_m2,0,4);
	}
}

#endif /* HAVE_PAIRED */

#endif /* AVCODEC_PPC_DEINTERLACE_H */
