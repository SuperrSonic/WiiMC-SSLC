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

#include "libavcodec/dsputil.h"
#include "dsputil_paired.h"
#include "libavutil/ppc/paired.h"

static void put_pixels_clamped_paired(const DCTELEM *block, uint8_t *restrict pixels, int line_size)
{
	vector float pair;
	
	pixels -= line_size;
	block -= 8;
	
	for (int i = 0; i < 8; i++) {
		pair = psq_lu(16,block,0,7);
		psq_stux(pair,pixels,line_size,0,4);
		
		pair = psq_l(4,block,0,7);
		psq_st(pair,2,pixels,0,4);
		
		pair = psq_l(8,block,0,7);
		psq_st(pair,4,pixels,0,4);
		
		pair = psq_l(12,block,0,7);
		psq_st(pair,6,pixels,0,4);
	}
}

static void put_signed_pixels_clamped_paired(const DCTELEM *block, uint8_t *restrict pixels, int line_size)
{
	const float bias = 128.0;
	vector float pair;
	
	pixels -= line_size;
	block -= 8;
	
	for (int i = 0; i < 8; i++) {
		pair = psq_lu(16,block,0,7);
		pair = ps_add(pair, bias);
		psq_stux(pair,pixels,line_size,0,4);
		
		pair = psq_l(4,block,0,7);
		pair = ps_add(pair, bias);
		psq_st(pair,2,pixels,0,4);
		
		pair = psq_l(8,block,0,7);
		pair = ps_add(pair, bias);
		psq_st(pair,4,pixels,0,4);
		
		pair = psq_l(12,block,0,7);
		pair = ps_add(pair, bias);
		psq_st(pair,6,pixels,0,4);
	}
}

static void add_pixels_clamped_paired(const DCTELEM *block, uint8_t *restrict pixels, int line_size)
{
	vector float pair[2];
	
	pixels -= line_size;
	block -= 8;
	
	for (int i = 0; i < 8; i++) {
		pair[0] = psq_lux(pixels,line_size,0,4);
		pair[1] = psq_lu(16,block,0,7);
		pair[0] = paired_add(pair[0], pair[1]);
		psq_st(pair[0],0,pixels,0,4);
		
		pair[0] = psq_l(2,pixels,0,4);
		pair[1] = psq_l(4,block,0,7);
		pair[0] = paired_add(pair[0], pair[1]);
		psq_st(pair[0],2,pixels,0,4);
		
		pair[0] = psq_l(4,pixels,0,4);
		pair[1] = psq_l(8,block,0,7);
		pair[0] = paired_add(pair[0], pair[1]);
		psq_st(pair[0],4,pixels,0,4);
		
		pair[0] = psq_l(6,pixels,0,4);
		pair[1] = psq_l(12,block,0,7);
		pair[0] = paired_add(pair[0], pair[1]);
		psq_st(pair[0],6,pixels,0,4);
	}
}

static void vorbis_inverse_coupling_paired(float *mag, float *ang, int blocksize)
{
	vector float pair[2], result[2];
	vector float negv, selv;
	
	for (int i = 0; i < blocksize*4; i += 8) {
		pair[0] = paired_lx(i, mag);
		pair[1] = paired_lx(i, ang);
		
		negv = paired_neg(pair[1]);
		selv = paired_sel(pair[0], pair[1], negv);
		negv = paired_neg(selv);
		selv = paired_sel(pair[1], negv, selv);
		selv = paired_add(pair[0], selv);
		
		result[0] = paired_sel(pair[1], pair[0], selv);
		result[1] = paired_sel(pair[1], selv, pair[0]);
		
		paired_stx(result[0], i, mag);
		paired_stx(result[1], i, ang);
	}
}

static void ac3_downmix_paired(float (*samples)[256], float (*matrix)[2], int out_ch, int in_ch, int len)
{
	const vec_f32_t zero = {0.0,0.0};
	
	vector float result[2];
	vector float pair, coeffs;
	
	if (out_ch == 2) {
		for (int i = 0; i < len*4; i += 8) {
			result[0] = result[1] = zero;
			
			for (int c = 0; c < in_ch; c++) {
				coeffs = psq_l(0,matrix[c],0,0);
				pair = paired_lx(i, samples[c]);
				result[0] = paired_madds0(pair, coeffs, result[0]);
				result[1] = paired_madds1(pair, coeffs, result[1]);
			}
			
			paired_stx(result[0], i, samples[0]);
			paired_stx(result[1], i, samples[1]);
		}
	} else if (out_ch == 1) {
		for (int i = 0; i < len*4; i += 16) {
			result[0] = result[1] = zero;
			
			for (int c = 0; c < in_ch; c++) {
				coeffs = psq_l(0,matrix[c],1,0);
				pair = paired_lx(i, samples[c]);
				result[0] = paired_madds0(pair, coeffs, result[0]);
				pair = paired_lx(i+8, samples[c]);
				result[1] = paired_madds0(pair, coeffs, result[1]);
			}
			
			paired_stx(result[0], i, samples[0]);
			paired_stx(result[1], i+8, samples[0]);
		}
	}
}

void ff_dsputil_init_paired(DSPContext *c, AVCodecContext *avctx)
{
	register uint32_t gqr;
	__asm__ volatile (
		"li		%0,4\n"
		"oris	%0,%0,4\n"
		"mtspr	916,%0\n"
		"li		%0,5\n"
		"oris	%0,%0,5\n"
		"mtspr	917,%0\n"
		"li		%0,6\n"
		"oris	%0,%0,6\n"
		"mtspr	918,%0\n"
		"li		%0,7\n"
		"oris	%0,%0,7\n"
		"mtspr	919,%0"
		: "=r"(gqr)
	);
	
	c->put_pixels_clamped = put_pixels_clamped_paired;
	c->put_signed_pixels_clamped = put_signed_pixels_clamped_paired;
	c->add_pixels_clamped = add_pixels_clamped_paired;
	
	if (CONFIG_VORBIS_DECODER)
		c->vorbis_inverse_coupling = vorbis_inverse_coupling_paired;
	
	if (CONFIG_AC3_DECODER)
		c->ac3_downmix = ac3_downmix_paired;
}
