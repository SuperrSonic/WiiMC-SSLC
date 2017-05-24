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
#include "libavcodec/h264data.h"
#include "libavcodec/h264dsp.h"
#include "dsputil_paired.h"
#include "util_paired.h"
#include "libavutil/ppc/paired.h"

#define CHROMAMC_VEC_OP {							\
	FAST_LSCALE(6, GQR_TYPE_S16);					\
	vec_s16_t iAB = {(8-x)*(8-y),(  x)*(8-y)};		\
	vec_s16_t iCD = {(8-x)*(  y),(  x)*(  y)};		\
	asm("psq_l %0,%1,0,1" : "=f"(fAB) : "o"(iAB));	\
	asm("psq_l %0,%1,0,1" : "=f"(fCD) : "o"(iCD));	\
}													\

static void put_h264_chroma_mc4_paired(uint8_t *dst, uint8_t *src, int stride, int h, int x, int y)
{
	const vec_f32_t zero = {0.0,0.0};
	const float half = 0.5;
	
	vector float fAB, fCD;
	CHROMAMC_VEC_OP
	
	vector float pair[4];
	vector float result;
	
	dst -= stride;
	uint8_t *src1 = src - stride;
	uint8_t *src2 = src;
	
	if (!paired_cmpu1(EQ, fCD, zero)) {
		do {
			pair[0] = psq_lux(src1,stride,0,4);
			pair[1] = psq_l(2,src1,0,4);
			
			pair[2] = psq_lux(src2,stride,0,4);
			pair[3] = psq_l(2,src2,0,4);
			
			result = ps_madds0(pair[0], fAB, half);
			result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
			result = ps_madds0(pair[2], fCD, result);
			result = ps_madds1(ps_merge10(pair[2], pair[3]), fCD, result);
			psq_stux(result,dst,stride,0,4);
			
			pair[0] = psq_l(4,src1,1,4);
			pair[2] = psq_l(4,src2,1,4);
			
			result = ps_madds0(pair[1], fAB, half);
			result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
			result = ps_madds0(pair[3], fCD, result);
			result = ps_madds1(ps_merge10(pair[3], pair[2]), fCD, result);
			psq_st(result,2,dst,0,4);
		} while (--h);
	} else {
		fAB = paired_sum1(fCD, fAB, fAB);
		
		if (!paired_cmpu0(EQ, fCD, zero)) {
			do {
				pair[0] = psq_lux(src1,stride,0,4);
				pair[2] = psq_lux(src2,stride,0,4);
				
				result = ps_madds0(pair[0], fAB, half);
				result = ps_madds1(pair[2], fAB, result);
				psq_stux(result,dst,stride,0,4);
				
				pair[0] = psq_l(2,src1,0,4);
				pair[2] = psq_l(2,src2,0,4);
				
				result = ps_madds0(pair[0], fAB, half);
				result = ps_madds1(pair[2], fAB, result);
				psq_st(result,2,dst,0,4);
			} while (--h);
		} else {
			do {
				pair[0] = psq_lux(src1,stride,0,4);
				pair[1] = psq_l(2,src1,0,4);
				
				result = ps_madds0(pair[0], fAB, half);
				result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
				psq_stux(result,dst,stride,0,4);
				
				pair[0] = psq_l(4,src1,1,4);
				
				result = ps_madds0(pair[1], fAB, half);
				result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
				psq_st(result,2,dst,0,4);
			} while (--h);
		}
	}
}

static void put_h264_chroma_mc8_paired(uint8_t *dst, uint8_t *src, int stride, int h, int x, int y)
{
	const vec_f32_t zero = {0.0,0.0};
	const float half = 0.5;
	
	vector float fAB, fCD;
	CHROMAMC_VEC_OP
	
	vector float pair[4];
	vector float result;
	
	dst -= stride;
	uint8_t *src1 = src - stride;
	uint8_t *src2 = src;
	
	if (!paired_cmpu1(EQ, fCD, zero)) {
		do {
			pair[0] = psq_lux(src1,stride,0,4);
			pair[1] = psq_l(2,src1,0,4);
			
			pair[2] = psq_lux(src2,stride,0,4);
			pair[3] = psq_l(2,src2,0,4);
			
			result = ps_madds0(pair[0], fAB, half);
			result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
			result = ps_madds0(pair[2], fCD, result);
			result = ps_madds1(ps_merge10(pair[2], pair[3]), fCD, result);
			psq_stux(result,dst,stride,0,4);
			
			pair[0] = psq_l(4,src1,0,4);
			pair[2] = psq_l(4,src2,0,4);
			
			result = ps_madds0(pair[1], fAB, half);
			result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
			result = ps_madds0(pair[3], fCD, result);
			result = ps_madds1(ps_merge10(pair[3], pair[2]), fCD, result);
			psq_st(result,2,dst,0,4);
			
			pair[1] = psq_l(6,src1,0,4);
			pair[3] = psq_l(6,src2,0,4);
			
			result = ps_madds0(pair[0], fAB, half);
			result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
			result = ps_madds0(pair[2], fCD, result);
			result = ps_madds1(ps_merge10(pair[2], pair[3]), fCD, result);
			psq_st(result,4,dst,0,4);
			
			pair[0] = psq_l(8,src1,1,4);
			pair[2] = psq_l(8,src2,1,4);
			
			result = ps_madds0(pair[1], fAB, half);
			result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
			result = ps_madds0(pair[3], fCD, result);
			result = ps_madds1(ps_merge10(pair[3], pair[2]), fCD, result);
			psq_st(result,6,dst,0,4);
		} while (--h);
	} else {
		fAB = paired_sum1(fCD, fAB, fAB);
		
		if (!paired_cmpu0(EQ, fCD, zero)) {
			do {
				pair[0] = psq_lux(src1,stride,0,4);
				pair[2] = psq_lux(src2,stride,0,4);
				
				result = ps_madds0(pair[0], fAB, half);
				result = ps_madds1(pair[2], fAB, result);
				psq_stux(result,dst,stride,0,4);
				
				pair[0] = psq_l(2,src1,0,4);
				pair[2] = psq_l(2,src2,0,4);
				
				result = ps_madds0(pair[0], fAB, half);
				result = ps_madds1(pair[2], fAB, result);
				psq_st(result,2,dst,0,4);
				
				pair[0] = psq_l(4,src1,0,4);
				pair[2] = psq_l(4,src2,0,4);
				
				result = ps_madds0(pair[0], fAB, half);
				result = ps_madds1(pair[2], fAB, result);
				psq_st(result,4,dst,0,4);
				
				pair[0] = psq_l(6,src1,0,4);
				pair[2] = psq_l(6,src2,0,4);
				
				result = ps_madds0(pair[0], fAB, half);
				result = ps_madds1(pair[2], fAB, result);
				psq_st(result,6,dst,0,4);
			} while (--h);
		} else {
			do {
				pair[0] = psq_lux(src1,stride,0,4);
				pair[1] = psq_l(2,src1,0,4);
				
				result = ps_madds0(pair[0], fAB, half);
				result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
				psq_stux(result,dst,stride,0,4);
				
				pair[0] = psq_l(4,src1,0,4);
				
				result = ps_madds0(pair[1], fAB, half);
				result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
				psq_st(result,2,dst,0,4);
				
				pair[1] = psq_l(6,src1,0,4);
				
				result = ps_madds0(pair[0], fAB, half);
				result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
				psq_st(result,4,dst,0,4);
				
				pair[0] = psq_l(8,src1,1,4);
				
				result = ps_madds0(pair[1], fAB, half);
				result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
				psq_st(result,6,dst,0,4);
			} while (--h);
		}
	}
}

static void avg_h264_chroma_mc4_paired(uint8_t *dst, uint8_t *src, int stride, int h, int x, int y)
{
	const vec_f32_t zero = {0.0,0.0};
	const float half = 0.5;
	
	vector float fAB, fCD;
	CHROMAMC_VEC_OP
	
	vector float pair[4];
	vector float result;
	
	dst -= stride;
	uint8_t *src1 = src - stride;
	uint8_t *src2 = src;
	
	if (!paired_cmpu1(EQ, fCD, zero)) {
		do {
			pair[0] = psq_lux(src1,stride,0,4);
			pair[1] = psq_l(2,src1,0,4);
			
			pair[2] = psq_lux(src2,stride,0,4);
			pair[3] = psq_l(2,src2,0,4);
			
			result = psq_lux(dst,stride,0,4);
			
			result = ps_madds0(pair[0], fAB, result);
			result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
			result = ps_madds0(pair[2], fCD, result);
			result = ps_madds1(ps_merge10(pair[2], pair[3]), fCD, result);
			psq_st(ps_madd(result, half, half),0,dst,0,4);
			
			pair[0] = psq_l(4,src1,1,4);
			pair[2] = psq_l(4,src2,1,4);
			
			result = psq_l(2,dst,0,4);
			
			result = ps_madds0(pair[1], fAB, result);
			result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
			result = ps_madds0(pair[3], fCD, result);
			result = ps_madds1(ps_merge10(pair[3], pair[2]), fCD, result);
			psq_st(ps_madd(result, half, half),2,dst,0,4);
		} while (--h);
	} else {
		fAB = paired_sum1(fCD, fAB, fAB);
		
		if (!paired_cmpu0(EQ, fCD, zero)) {
			do {
				pair[0] = psq_lux(src1,stride,0,4);
				pair[1] = psq_lux(src2,stride,0,4);
				
				result = psq_lux(dst,stride,0,4);
				
				result = ps_madds0(pair[0], fAB, result);
				result = ps_madds1(pair[1], fAB, result);
				psq_st(ps_madd(result, half, half),0,dst,0,4);
				
				pair[0] = psq_l(2,src1,0,4);
				pair[1] = psq_l(2,src2,0,4);
				
				result = psq_l(2,dst,0,4);
				
				result = ps_madds0(pair[0], fAB, result);
				result = ps_madds1(pair[1], fAB, result);
				psq_st(ps_madd(result, half, half),2,dst,0,4);
			} while (--h);
		} else {
			do {
				pair[0] = psq_lux(src1,stride,0,4);
				pair[1] = psq_l(2,src1,0,4);
				
				result = psq_lux(dst,stride,0,4);
				
				result = ps_madds0(pair[0], fAB, result);
				result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
				psq_st(ps_madd(result, half, half),0,dst,0,4);
				
				pair[0] = psq_l(4,src1,1,4);
				result = psq_l(2,dst,0,4);
				
				result = ps_madds0(pair[1], fAB, result);
				result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
				psq_st(ps_madd(result, half, half),2,dst,0,4);
			} while (--h);
		}
	}
}

static void avg_h264_chroma_mc8_paired(uint8_t *dst, uint8_t *src, int stride, int h, int x, int y)
{
	const vec_f32_t zero = {0.0,0.0};
	const float half = 0.5;
	
	vector float fAB, fCD;
	CHROMAMC_VEC_OP
	
	vector float pair[4];
	vector float result;
	
	dst -= stride;
	uint8_t *src1 = src - stride;
	uint8_t *src2 = src;
	
	if (!paired_cmpu1(EQ, fCD, zero)) {
		do {
			pair[0] = psq_lux(src1,stride,0,4);
			pair[1] = psq_l(2,src1,0,4);
			
			pair[2] = psq_lux(src2,stride,0,4);
			pair[3] = psq_l(2,src2,0,4);
			
			result = psq_lux(dst,stride,0,4);
			
			result = ps_madds0(pair[0], fAB, result);
			result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
			result = ps_madds0(pair[2], fCD, result);
			result = ps_madds1(ps_merge10(pair[2], pair[3]), fCD, result);
			psq_st(ps_madd(result, half, half),0,dst,0,4);
			
			pair[0] = psq_l(4,src1,0,4);
			pair[2] = psq_l(4,src2,0,4);
			
			result = psq_l(2,dst,0,4);
			
			result = ps_madds0(pair[1], fAB, result);
			result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
			result = ps_madds0(pair[3], fCD, result);
			result = ps_madds1(ps_merge10(pair[3], pair[2]), fCD, result);
			psq_st(ps_madd(result, half, half),2,dst,0,4);
			
			pair[1] = psq_l(6,src1,0,4);
			pair[3] = psq_l(6,src2,0,4);
			
			result = psq_l(4,dst,0,4);
			
			result = ps_madds0(pair[0], fAB, result);
			result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
			result = ps_madds0(pair[2], fCD, result);
			result = ps_madds1(ps_merge10(pair[2], pair[3]), fCD, result);
			psq_st(ps_madd(result, half, half),4,dst,0,4);
			
			pair[0] = psq_l(8,src1,1,4);
			pair[2] = psq_l(8,src2,1,4);
			
			result = psq_l(6,dst,0,4);
			
			result = ps_madds0(pair[1], fAB, result);
			result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
			result = ps_madds0(pair[3], fCD, result);
			result = ps_madds1(ps_merge10(pair[3], pair[2]), fCD, result);
			psq_st(ps_madd(result, half, half),6,dst,0,4);
		} while (--h);
	} else {
		fAB = paired_sum1(fCD, fAB, fAB);
		
		if (!paired_cmpu0(EQ, fCD, zero)) {
			do {
				pair[0] = psq_lux(src1,stride,0,4);
				pair[1] = psq_lux(src2,stride,0,4);
				
				result = psq_lux(dst,stride,0,4);
				
				result = ps_madds0(pair[0], fAB, result);
				result = ps_madds1(pair[1], fAB, result);
				psq_st(ps_madd(result, half, half),0,dst,0,4);
				
				pair[0] = psq_l(2,src1,0,4);
				pair[1] = psq_l(2,src2,0,4);
				
				result = psq_l(2,dst,0,4);
				
				result = ps_madds0(pair[0], fAB, result);
				result = ps_madds1(pair[1], fAB, result);
				psq_st(ps_madd(result, half, half),2,dst,0,4);
				
				pair[0] = psq_l(4,src1,0,4);
				pair[1] = psq_l(4,src2,0,4);
				
				result = psq_l(4,dst,0,4);
				
				result = ps_madds0(pair[0], fAB, result);
				result = ps_madds1(pair[1], fAB, result);
				psq_st(ps_madd(result, half, half),4,dst,0,4);
				
				pair[0] = psq_l(6,src1,0,4);
				pair[1] = psq_l(6,src2,0,4);
				
				result = psq_l(6,dst,0,4);
				
				result = ps_madds0(pair[0], fAB, result);
				result = ps_madds1(pair[1], fAB, result);
				psq_st(ps_madd(result, half, half),6,dst,0,4);
			} while (--h);
		} else {
			do {
				pair[0] = psq_lux(src1,stride,0,4);
				pair[1] = psq_l(2,src1,0,4);
				
				result = psq_lux(dst,stride,0,4);
				
				result = ps_madds0(pair[0], fAB, result);
				result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
				psq_st(ps_madd(result, half, half),0,dst,0,4);
				
				pair[0] = psq_l(4,src1,0,4);
				result = psq_l(2,dst,0,4);
				
				result = ps_madds0(pair[1], fAB, result);
				result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
				psq_st(ps_madd(result, half, half),2,dst,0,4);
				
				pair[1] = psq_l(6,src1,0,4);
				result = psq_l(4,dst,0,4);
				
				result = ps_madds0(pair[0], fAB, result);
				result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
				psq_st(ps_madd(result, half, half),4,dst,0,4);
				
				pair[0] = psq_l(8,src1,1,4);
				result = psq_l(6,dst,0,4);
				
				result = ps_madds0(pair[1], fAB, result);
				result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
				psq_st(ps_madd(result, half, half),6,dst,0,4);
			} while (--h);
		}
	}
}

void ff_put_vc1_chroma_mc8_paired_nornd(uint8_t *dst, uint8_t *src, int stride, int h, int x, int y)
{
	const vec_f32_t zero = {0.0,0.0};
	const float offset = 0.4375;
	
	vector float fAB, fCD;
	CHROMAMC_VEC_OP
	
	vector float pair[4];
	vector float result;
	
	dst -= stride;
	uint8_t *src1 = src - stride;
	uint8_t *src2 = src;
	
	if (!paired_cmpu1(EQ, fCD, zero)) {
		do {
			pair[0] = psq_lux(src1,stride,0,4);
			pair[1] = psq_l(2,src1,0,4);
			
			pair[2] = psq_lux(src2,stride,0,4);
			pair[3] = psq_l(2,src2,0,4);
			
			result = ps_madds0(pair[0], fAB, offset);
			result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
			result = ps_madds0(pair[2], fCD, result);
			result = ps_madds1(ps_merge10(pair[2], pair[3]), fCD, result);
			psq_stux(result,dst,stride,0,4);
			
			pair[0] = psq_l(4,src1,0,4);
			pair[2] = psq_l(4,src2,0,4);
			
			result = ps_madds0(pair[1], fAB, offset);
			result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
			result = ps_madds0(pair[3], fCD, result);
			result = ps_madds1(ps_merge10(pair[3], pair[2]), fCD, result);
			psq_st(result,2,dst,0,4);
			
			pair[1] = psq_l(6,src1,0,4);
			pair[3] = psq_l(6,src2,0,4);
			
			result = ps_madds0(pair[0], fAB, offset);
			result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
			result = ps_madds0(pair[2], fCD, result);
			result = ps_madds1(ps_merge10(pair[2], pair[3]), fCD, result);
			psq_st(result,4,dst,0,4);
			
			pair[0] = psq_l(8,src1,1,4);
			pair[2] = psq_l(8,src2,1,4);
			
			result = ps_madds0(pair[1], fAB, offset);
			result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
			result = ps_madds0(pair[3], fCD, result);
			result = ps_madds1(ps_merge10(pair[3], pair[2]), fCD, result);
			psq_st(result,6,dst,0,4);
		} while (--h);
	} else {
		fAB = paired_sum1(fCD, fAB, fAB);
		
		if (!paired_cmpu0(EQ, fCD, zero)) {
			do {
				pair[0] = psq_lux(src1,stride,0,4);
				pair[2] = psq_lux(src2,stride,0,4);
				
				result = ps_madds0(pair[0], fAB, offset);
				result = ps_madds1(pair[2], fAB, result);
				psq_stux(result,dst,stride,0,4);
				
				pair[0] = psq_l(2,src1,0,4);
				pair[2] = psq_l(2,src2,0,4);
				
				result = ps_madds0(pair[0], fAB, offset);
				result = ps_madds1(pair[2], fAB, result);
				psq_st(result,2,dst,0,4);
				
				pair[0] = psq_l(4,src1,0,4);
				pair[2] = psq_l(4,src2,0,4);
				
				result = ps_madds0(pair[0], fAB, offset);
				result = ps_madds1(pair[2], fAB, result);
				psq_st(result,4,dst,0,4);
				
				pair[0] = psq_l(6,src1,0,4);
				pair[2] = psq_l(6,src2,0,4);
				
				result = ps_madds0(pair[0], fAB, offset);
				result = ps_madds1(pair[2], fAB, result);
				psq_st(result,6,dst,0,4);
			} while (--h);
		} else {
			do {
				pair[0] = psq_lux(src1,stride,0,4);
				pair[1] = psq_l(2,src1,0,4);
				
				result = ps_madds0(pair[0], fAB, offset);
				result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
				psq_stux(result,dst,stride,0,4);
				
				pair[0] = psq_l(4,src1,0,4);
				
				result = ps_madds0(pair[1], fAB, offset);
				result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
				psq_st(result,2,dst,0,4);
				
				pair[1] = psq_l(6,src1,0,4);
				
				result = ps_madds0(pair[0], fAB, offset);
				result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
				psq_st(result,4,dst,0,4);
				
				pair[0] = psq_l(8,src1,1,4);
				
				result = ps_madds0(pair[1], fAB, offset);
				result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
				psq_st(result,6,dst,0,4);
			} while (--h);
		}
	}
}

void ff_avg_vc1_chroma_mc8_paired_nornd(uint8_t *dst, uint8_t *src, int stride, int h, int x, int y)
{
	const vec_f32_t zero = {0.0,0.0};
	const float offset = 0.4375;
	const float half = 0.5;
	
	vector float fAB, fCD;
	CHROMAMC_VEC_OP
	
	vector float pair[4];
	vector float result;
	
	dst -= stride;
	uint8_t *src1 = src - stride;
	uint8_t *src2 = src;
	
	if (!paired_cmpu1(EQ, fCD, zero)) {
		do {
			pair[0] = psq_lux(src1,stride,0,4);
			pair[1] = psq_l(2,src1,0,4);
			
			pair[2] = psq_lux(src2,stride,0,4);
			pair[3] = psq_l(2,src2,0,4);
			
			result = psq_lux(dst,stride,0,4);
			
			result = ps_madds0(pair[0], fAB, result);
			result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
			result = ps_madds0(pair[2], fCD, result);
			result = ps_madds1(ps_merge10(pair[2], pair[3]), fCD, result);
			psq_st(ps_madd(result, half, offset),0,dst,0,4);
			
			pair[0] = psq_l(4,src1,0,4);
			pair[2] = psq_l(4,src2,0,4);
			
			result = psq_l(2,dst,0,4);
			
			result = ps_madds0(pair[1], fAB, result);
			result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
			result = ps_madds0(pair[3], fCD, result);
			result = ps_madds1(ps_merge10(pair[3], pair[2]), fCD, result);
			psq_st(ps_madd(result, half, offset),2,dst,0,4);
			
			pair[1] = psq_l(6,src1,0,4);
			pair[3] = psq_l(6,src2,0,4);
			
			result = psq_l(4,dst,0,4);
			
			result = ps_madds0(pair[0], fAB, result);
			result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
			result = ps_madds0(pair[2], fCD, result);
			result = ps_madds1(ps_merge10(pair[2], pair[3]), fCD, result);
			psq_st(ps_madd(result, half, offset),4,dst,0,4);
			
			pair[0] = psq_l(8,src1,1,4);
			pair[2] = psq_l(8,src2,1,4);
			
			result = psq_l(6,dst,0,4);
			
			result = ps_madds0(pair[1], fAB, result);
			result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
			result = ps_madds0(pair[3], fCD, result);
			result = ps_madds1(ps_merge10(pair[3], pair[2]), fCD, result);
			psq_st(ps_madd(result, half, offset),6,dst,0,4);
		} while (--h);
	} else {
		fAB = paired_sum1(fCD, fAB, fAB);
		
		if (!paired_cmpu0(EQ, fCD, zero)) {
			do {
				pair[0] = psq_lux(src1,stride,0,4);
				pair[1] = psq_lux(src2,stride,0,4);
				
				result = psq_lux(dst,stride,0,4);
				
				result = ps_madds0(pair[0], fAB, result);
				result = ps_madds1(pair[1], fAB, result);
				psq_st(ps_madd(result, half, offset),0,dst,0,4);
				
				pair[0] = psq_l(2,src1,0,4);
				pair[1] = psq_l(2,src2,0,4);
				
				result = psq_l(2,dst,0,4);
				
				result = ps_madds0(pair[0], fAB, result);
				result = ps_madds1(pair[1], fAB, result);
				psq_st(ps_madd(result, half, offset),2,dst,0,4);
				
				pair[0] = psq_l(4,src1,0,4);
				pair[1] = psq_l(4,src2,0,4);
				
				result = psq_l(4,dst,0,4);
				
				result = ps_madds0(pair[0], fAB, result);
				result = ps_madds1(pair[1], fAB, result);
				psq_st(ps_madd(result, half, offset),4,dst,0,4);
				
				pair[0] = psq_l(6,src1,0,4);
				pair[1] = psq_l(6,src2,0,4);
				
				result = psq_l(6,dst,0,4);
				
				result = ps_madds0(pair[0], fAB, result);
				result = ps_madds1(pair[1], fAB, result);
				psq_st(ps_madd(result, half, offset),6,dst,0,4);
			} while (--h);
		} else {
			do {
				pair[0] = psq_lux(src1,stride,0,4);
				pair[1] = psq_l(2,src1,0,4);
				
				result = psq_lux(dst,stride,0,4);
				
				result = ps_madds0(pair[0], fAB, result);
				result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
				psq_st(ps_madd(result, half, offset),0,dst,0,4);
				
				pair[0] = psq_l(4,src1,0,4);
				result = psq_l(2,dst,0,4);
				
				result = ps_madds0(pair[1], fAB, result);
				result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
				psq_st(ps_madd(result, half, offset),2,dst,0,4);
				
				pair[1] = psq_l(6,src1,0,4);
				result = psq_l(4,dst,0,4);
				
				result = ps_madds0(pair[0], fAB, result);
				result = ps_madds1(ps_merge10(pair[0], pair[1]), fAB, result);
				psq_st(ps_madd(result, half, offset),4,dst,0,4);
				
				pair[0] = psq_l(8,src1,1,4);
				result = psq_l(6,dst,0,4);
				
				result = ps_madds0(pair[1], fAB, result);
				result = ps_madds1(ps_merge10(pair[1], pair[0]), fAB, result);
				psq_st(ps_madd(result, half, offset),6,dst,0,4);
			} while (--h);
		}
	}
}

static void ff_h264_idct_add_paired(uint8_t *dst, DCTELEM *block, int stride)
{
	const float half = 0.5;
	vector float pair[2], subv[2], addv[2];
	vector float result, coeffs[8];
	
	uint8_t *dst1 = dst;
	uint8_t *dst2 = dst + 2;
	
	block[0] += 32;
	QUANT_REVERSE(6, GQR_TYPE_U8);
	
	pair[0] = psq_l(0,block,0,7);
	pair[1] = psq_l(16,block,0,7);
	addv[0] = paired_add(pair[0], pair[1]);
	subv[0] = paired_sub(pair[0], pair[1]);
	
	pair[0] = psq_l(8,block,0,7);
	pair[1] = psq_l(24,block,0,7);
	subv[1] = ps_msub(pair[0], half, pair[1]);
	addv[1] = ps_madd(pair[1], half, pair[0]);
	
	pair[0] = paired_add(addv[0], addv[1]);
	pair[1] = paired_add(subv[0], subv[1]);
	
	coeffs[0] = paired_merge00(pair[0], pair[1]);
	coeffs[2] = paired_merge11(pair[0], pair[1]);
	
	pair[0] = paired_sub(subv[0], subv[1]);
	pair[1] = paired_sub(addv[0], addv[1]);
	
	coeffs[1] = paired_merge00(pair[0], pair[1]);
	coeffs[3] = paired_merge11(pair[0], pair[1]);
	
	pair[0] = psq_l(4,block,0,7);
	pair[1] = psq_l(20,block,0,7);
	addv[0] = paired_add(pair[0], pair[1]);
	subv[0] = paired_sub(pair[0], pair[1]);
	
	pair[0] = psq_l(12,block,0,7);
	pair[1] = psq_l(28,block,0,7);
	subv[1] = ps_msub(pair[0], half, pair[1]);
	addv[1] = ps_madd(pair[1], half, pair[0]);
	
	pair[0] = paired_add(addv[0], addv[1]);
	pair[1] = paired_add(subv[0], subv[1]);
	
	coeffs[4] = paired_merge00(pair[0], pair[1]);
	coeffs[6] = paired_merge11(pair[0], pair[1]);
	
	pair[0] = paired_sub(subv[0], subv[1]);
	pair[1] = paired_sub(addv[0], addv[1]);
	
	coeffs[5] = paired_merge00(pair[0], pair[1]);
	coeffs[7] = paired_merge11(pair[0], pair[1]);
	
	addv[0] = paired_add(coeffs[0], coeffs[4]);
	subv[0] = paired_sub(coeffs[0], coeffs[4]);
	
	subv[1] = ps_msub(coeffs[2], half, coeffs[6]);
	addv[1] = ps_madd(coeffs[6], half, coeffs[2]);
	
	pair[0] = paired_add(addv[0], addv[1]);
	pair[1] = paired_add(subv[0], subv[1]);
	
	result = psq_l(0,dst1,0,1);
	psq_st(ps_add(pair[0], result),0,dst1,0,1);
	
	result = psq_lux(dst1,stride,0,1);
	psq_st(ps_add(pair[1], result),0,dst1,0,1);
	
	pair[0] = paired_sub(subv[0], subv[1]);
	pair[1] = paired_sub(addv[0], addv[1]);
	
	result = psq_lux(dst1,stride,0,1);
	psq_st(ps_add(pair[0], result),0,dst1,0,1);
	
	result = psq_lux(dst1,stride,0,1);
	psq_st(ps_add(pair[1], result),0,dst1,0,1);
	
	addv[0] = paired_add(coeffs[1], coeffs[5]);
	subv[0] = paired_sub(coeffs[1], coeffs[5]);
	
	subv[1] = ps_msub(coeffs[3], half, coeffs[7]);
	addv[1] = ps_madd(coeffs[7], half, coeffs[3]);
	
	pair[0] = paired_add(addv[0], addv[1]);
	pair[1] = paired_add(subv[0], subv[1]);
	
	result = psq_l(0,dst2,0,1);
	psq_st(ps_add(pair[0], result),0,dst2,0,1);
	
	result = psq_lux(dst2,stride,0,1);
	psq_st(ps_add(pair[1], result),0,dst2,0,1);
	
	pair[0] = paired_sub(subv[0], subv[1]);
	pair[1] = paired_sub(addv[0], addv[1]);
	
	result = psq_lux(dst2,stride,0,1);
	psq_st(ps_add(pair[0], result),0,dst2,0,1);
	
	result = psq_lux(dst2,stride,0,1);
	psq_st(ps_add(pair[1], result),0,dst2,0,1);
}

static void ff_h264_idct_dc_add_paired(uint8_t *dst, DCTELEM *block, int stride)
{
	const float half = 0.5;
	const float scale = 0.015625;
	vector float pair, offset;
	
	offset = psq_l(0,block,1,7);
	asm("fmadds %0,%0,%1,%2" : "+f"(offset) : "f"(scale), "f"(half));
	
	dst -= stride;
	
	for (int i = 0; i < 4; i++) {
		pair = psq_lux(dst,stride,0,4);
		pair = paired_add(pair, offset);
		psq_st(pair,0,dst,0,4);
		
		pair = psq_l(2,dst,0,4);
		pair = paired_add(pair, offset);
		psq_st(pair,2,dst,0,4);
	}
}

static void ff_h264_idct_add16_paired(uint8_t *dst, const int *block_offset, DCTELEM *block, int stride, const uint8_t nnzc[48])
{
	for (int i = 0; i < 16; i++) {
		int nnz = nnzc[scan8[i]];
		if (nnz) {
			if (nnz == 1 && block[i*16])
				ff_h264_idct_dc_add_paired(dst + block_offset[i], block + i*16, stride);
			else ff_h264_idct_add_paired(dst + block_offset[i], block + i*16, stride);
		}
	}
}

static void ff_h264_idct_add16intra_paired(uint8_t *dst, const int *block_offset, DCTELEM *block, int stride, const uint8_t nnzc[48])
{
	for (int i = 0; i < 16; i++) {
		if (nnzc[scan8[i]])
			ff_h264_idct_add_paired(dst + block_offset[i], block + i*16, stride);
		else if (block[i*16])
			ff_h264_idct_dc_add_paired(dst + block_offset[i], block + i*16, stride);
	}
}

static void ff_h264_idct_add8_paired(uint8_t **dest, const int *block_offset, DCTELEM *block, int stride, const uint8_t nnzc[48])
{
	for (int i = 16; i < 24; i++) {
		if (nnzc[scan8[i]])
			ff_h264_idct_add_paired(dest[(i&4)>>2] + block_offset[i], block + i*16, stride);
		else if (block[i*16])
			ff_h264_idct_dc_add_paired(dest[(i&4)>>2] + block_offset[i], block + i*16, stride);
	}
}

#if 1
// FIXME: Rounding errors.
static void ff_h264_idct8_add_paired(uint8_t *dst, DCTELEM *block, int stride)
{
	const float half = 0.5;
	const float quarter = 0.25;
	vector float pair[4], subv[4], addv[4];
	vector float result, prev[4];
	
	block[0] += 32;
	QUANT_REVERSE(6, GQR_TYPE_U8);
	
	float coeffs[64];
	float *dct = coeffs - 16;
	block -= 2;
	
	for (int i = 0; i < 4; i++) {
		pair[0] = psq_lu(4,block,0,7);
		pair[1] = psq_l(64,block,0,7);
		prev[0] = paired_add(pair[0], pair[1]);
		prev[1] = paired_sub(pair[0], pair[1]);
		
		pair[2] = psq_l(32,block,0,7);
		pair[3] = psq_l(96,block,0,7);
		prev[2] = ps_msub(pair[2], half, pair[3]);
		prev[3] = ps_madd(pair[3], half, pair[2]);
		
		addv[0] = paired_add(prev[0], prev[3]);
		addv[1] = paired_add(prev[1], prev[2]);
		subv[0] = paired_sub(prev[1], prev[2]);
		subv[1] = paired_sub(prev[0], prev[3]);
		
		pair[0] = psq_l(16,block,0,7);
		pair[1] = psq_l(48,block,0,7);
		pair[2] = psq_l(80,block,0,7);
		pair[3] = psq_l(112,block,0,7);
		
		prev[0] = paired_sub(ps_sub(pair[2], pair[1]), ps_madd(pair[3], half, pair[3]));
		prev[1] = paired_sub(ps_add(pair[0], pair[3]), ps_madd(pair[1], half, pair[1]));
		prev[2] = paired_add(ps_sub(pair[3], pair[0]), ps_madd(pair[2], half, pair[2]));
		prev[3] = paired_add(ps_add(pair[1], pair[2]), ps_madd(pair[0], half, pair[0]));
		
		addv[2] = ps_madd(prev[3], quarter, prev[0]);
		addv[3] = ps_madd(prev[2], quarter, prev[1]);
		subv[2] = ps_msub(prev[1], quarter, prev[2]);
		subv[3] = ps_nmsub(prev[0], quarter, prev[3]);
		
		pair[0] = paired_add(addv[0], subv[3]);
		pair[1] = paired_add(addv[1], subv[2]);
		
		psq_stu(ps_merge00(pair[0], pair[1]),64,dct,0,0);
		psq_st(ps_merge11(pair[0], pair[1]),32,dct,0,0);
		
		pair[2] = paired_add(subv[0], addv[3]);
		pair[3] = paired_add(subv[1], addv[2]);
		
		psq_st(ps_merge00(pair[2], pair[3]),8,dct,0,0);
		psq_st(ps_merge11(pair[2], pair[3]),40,dct,0,0);
		
		pair[0] = paired_sub(subv[1], addv[2]);
		pair[1] = paired_sub(subv[0], addv[3]);
		
		psq_st(ps_merge00(pair[0], pair[1]),16,dct,0,0);
		psq_st(ps_merge11(pair[0], pair[1]),48,dct,0,0);
		
		pair[2] = paired_sub(addv[1], subv[2]);
		pair[3] = paired_sub(addv[0], subv[3]);
		
		psq_st(ps_merge00(pair[2], pair[3]),24,dct,0,0);
		psq_st(ps_merge11(pair[2], pair[3]),56,dct,0,0);
	}
	
	uint8_t *dst1 = dst - 2;
	uint8_t *dst2 = dst;
	dct = coeffs - 2;
	
	for (int i = 0; i < 4; i++) {
		pair[0] = psq_lu(8,dct,0,0);
		pair[1] = psq_l(128,dct,0,0);
		prev[0] = paired_add(pair[0], pair[1]);
		prev[1] = paired_sub(pair[0], pair[1]);
		
		pair[2] = psq_l(64,dct,0,0);
		pair[3] = psq_l(192,dct,0,0);
		prev[2] = ps_msub(pair[2], half, pair[3]);
		prev[3] = ps_madd(pair[3], half, pair[2]);
		
		addv[0] = paired_add(prev[0], prev[3]);
		addv[1] = paired_add(prev[1], prev[2]);
		subv[0] = paired_sub(prev[1], prev[2]);
		subv[1] = paired_sub(prev[0], prev[3]);
		
		pair[0] = psq_l(32,dct,0,0);
		pair[1] = psq_l(96,dct,0,0);
		pair[2] = psq_l(160,dct,0,0);
		pair[3] = psq_l(224,dct,0,0);
		
		prev[0] = paired_sub(ps_sub(pair[2], pair[1]), ps_madd(pair[3], half, pair[3]));
		prev[1] = paired_sub(ps_add(pair[0], pair[3]), ps_madd(pair[1], half, pair[1]));
		prev[2] = paired_add(ps_sub(pair[3], pair[0]), ps_madd(pair[2], half, pair[2]));
		prev[3] = paired_add(ps_add(pair[1], pair[2]), ps_madd(pair[0], half, pair[0]));
		
		addv[2] = ps_madd(prev[3], quarter, prev[0]);
		addv[3] = ps_madd(prev[2], quarter, prev[1]);
		subv[2] = ps_msub(prev[1], quarter, prev[2]);
		subv[3] = ps_nmsub(prev[0], quarter, prev[3]);
		
		pair[0] = paired_add(addv[0], subv[3]);
		pair[1] = paired_add(addv[1], subv[2]);
		
		result = psq_lu(2,dst1,0,1); dst2 = dst1;
		psq_st(ps_add(pair[0], result),0,dst1,0,1);
		
		result = psq_lux(dst2,stride,0,1);
		psq_st(ps_add(pair[1], result),0,dst2,0,1);
		
		pair[2] = paired_add(subv[0], addv[3]);
		pair[3] = paired_add(subv[1], addv[2]);
		
		result = psq_lux(dst2,stride,0,1);
		psq_st(ps_add(pair[2], result),0,dst2,0,1);
		
		result = psq_lux(dst2,stride,0,1);
		psq_st(ps_add(pair[3], result),0,dst2,0,1);
		
		pair[0] = paired_sub(subv[1], addv[2]);
		pair[1] = paired_sub(subv[0], addv[3]);
		
		result = psq_lux(dst2,stride,0,1);
		psq_st(ps_add(pair[0], result),0,dst2,0,1);
		
		result = psq_lux(dst2,stride,0,1);
		psq_st(ps_add(pair[1], result),0,dst2,0,1);
		
		pair[2] = paired_sub(addv[1], subv[2]);
		pair[3] = paired_sub(addv[0], subv[3]);
		
		result = psq_lux(dst2,stride,0,1);
		psq_st(ps_add(pair[2], result),0,dst2,0,1);
		
		result = psq_lux(dst2,stride,0,1);
		psq_st(ps_add(pair[3], result),0,dst2,0,1);
	}
}
#endif

static void ff_h264_idct8_dc_add_paired(uint8_t *dst, DCTELEM *block, int stride)
{
	const float half = 0.5;
	const float scale = 0.015625;
	vector float pair, offset;
	
	offset = psq_l(0,block,1,7);
	asm("fmadds %0,%0,%1,%2" : "+f"(offset) : "f"(scale), "f"(half));
	
	dst -= stride;
	
	for (int i = 0; i < 8; i++) {
		pair = psq_lux(dst,stride,0,4);
		pair = paired_add(pair, offset);
		psq_st(pair,0,dst,0,4);
		
		pair = psq_l(2,dst,0,4);
		pair = paired_add(pair, offset);
		psq_st(pair,2,dst,0,4);
		
		pair = psq_l(4,dst,0,4);
		pair = paired_add(pair, offset);
		psq_st(pair,4,dst,0,4);
		
		pair = psq_l(6,dst,0,4);
		pair = paired_add(pair, offset);
		psq_st(pair,6,dst,0,4);
	}
}

static void ff_h264_idct8_add4_paired(uint8_t *dst, const int *block_offset, DCTELEM *block, int stride, const uint8_t nnzc[48])
{
	for (int i = 0; i < 16; i += 4) {
		int nnz = nnzc[scan8[i]];
		if (nnz) {
			if (nnz == 1 && block[i*16])
				ff_h264_idct8_dc_add_paired(dst + block_offset[i], block + i*16, stride);
			else ff_h264_idct8_add_8_c(dst + block_offset[i], block + i*16, stride);
		}
	}
}

#define H264_WEIGHT(W,H) \
static void weight_h264_pixels ## W ## x ## H ## _paired(uint8_t *block, int stride, int log2_denom, int weight, int offset) \
{ \
	vector float pair; \
	 \
	FAST_LSCALE(log2_denom, GQR_TYPE_S16); \
	int16_t weighti = weight; \
	vector float weightf; \
	asm("psq_l %0,%1,1,1" : "=f"(weightf) : "o"(weighti)); \
	 \
	int16_t offseti = offset; \
	register float offsetf; \
	asm("psq_l %0,%1,1,7" : "=f"(offsetf) : "o"(offseti)); \
	offsetf += 0.5; \
	 \
	block -= stride; \
	 \
	for (int y = 0; y < H; y++) { \
		pair = psq_lux(block,stride,0,4); \
		pair = ps_madds0(pair, weightf, offsetf); \
		psq_st(pair,0,block,0,4); \
		 \
		if (W == 2) continue; \
		 \
		pair = psq_l(2,block,0,4); \
		pair = ps_madds0(pair, weightf, offsetf); \
		psq_st(pair,2,block,0,4); \
		 \
		if (W == 4) continue; \
		 \
		pair = psq_l(4,block,0,4); \
		pair = ps_madds0(pair, weightf, offsetf); \
		psq_st(pair,4,block,0,4); \
		 \
		pair = psq_l(6,block,0,4); \
		pair = ps_madds0(pair, weightf, offsetf); \
		psq_st(pair,6,block,0,4); \
		 \
		if (W == 8) continue; \
		 \
		pair = psq_l(8,block,0,4); \
		pair = ps_madds0(pair, weightf, offsetf); \
		psq_st(pair,8,block,0,4); \
		 \
		pair = psq_l(10,block,0,4); \
		pair = ps_madds0(pair, weightf, offsetf); \
		psq_st(pair,10,block,0,4); \
		 \
		pair = psq_l(12,block,0,4); \
		pair = ps_madds0(pair, weightf, offsetf); \
		psq_st(pair,12,block,0,4); \
		 \
		pair = psq_l(14,block,0,4); \
		pair = ps_madds0(pair, weightf, offsetf); \
		psq_st(pair,14,block,0,4); \
	} \
} \
static void biweight_h264_pixels ## W ## x ## H ## _paired(uint8_t *dst, uint8_t *src, int stride, int log2_denom, int weightd, int weights, int offset) \
{ \
	vector float pair[2]; \
	 \
	FAST_LSCALE(log2_denom+1, GQR_TYPE_S16); \
	vec_s16_t weighti = {weightd,weights}; \
	vector float weightf; \
	asm("psq_l %0,%1,0,1" : "=f"(weightf) : "o"(weighti)); \
	 \
	int16_t offseti = offset; \
	register float offsetf; \
	asm("psq_l %0,%1,1,7" : "=f"(offsetf) : "o"(offseti)); \
	offsetf += 0.5; \
	 \
	dst -= stride; \
	src -= stride; \
	 \
	for (int y = 0; y < H; y++) { \
		pair[0] = psq_lux(dst,stride,0,4); \
		pair[1] = psq_lux(src,stride,0,4); \
		pair[0] = ps_madds1(pair[1], weightf, ps_madds0(pair[0], weightf, offsetf)); \
		psq_st(pair[0],0,dst,0,4); \
		 \
		if (W == 2) continue; \
		 \
		pair[0] = psq_l(2,dst,0,4); \
		pair[1] = psq_l(2,src,0,4); \
		pair[0] = ps_madds1(pair[1], weightf, ps_madds0(pair[0], weightf, offsetf)); \
		psq_st(pair[0],2,dst,0,4); \
		 \
		if (W == 4) continue; \
		 \
		pair[0] = psq_l(4,dst,0,4); \
		pair[1] = psq_l(4,src,0,4); \
		pair[0] = ps_madds1(pair[1], weightf, ps_madds0(pair[0], weightf, offsetf)); \
		psq_st(pair[0],4,dst,0,4); \
		 \
		pair[0] = psq_l(6,dst,0,4); \
		pair[1] = psq_l(6,src,0,4); \
		pair[0] = ps_madds1(pair[1], weightf, ps_madds0(pair[0], weightf, offsetf)); \
		psq_st(pair[0],6,dst,0,4); \
		 \
		if (W == 8) continue; \
		 \
		pair[0] = psq_l(8,dst,0,4); \
		pair[1] = psq_l(8,src,0,4); \
		pair[0] = ps_madds1(pair[1], weightf, ps_madds0(pair[0], weightf, offsetf)); \
		psq_st(pair[0],8,dst,0,4); \
		 \
		pair[0] = psq_l(10,dst,0,4); \
		pair[1] = psq_l(10,src,0,4); \
		pair[0] = ps_madds1(pair[1], weightf, ps_madds0(pair[0], weightf, offsetf)); \
		psq_st(pair[0],10,dst,0,4); \
		 \
		pair[0] = psq_l(12,dst,0,4); \
		pair[1] = psq_l(12,src,0,4); \
		pair[0] = ps_madds1(pair[1], weightf, ps_madds0(pair[0], weightf, offsetf)); \
		psq_st(pair[0],12,dst,0,4); \
		 \
		pair[0] = psq_l(14,dst,0,4); \
		pair[1] = psq_l(14,src,0,4); \
		pair[0] = ps_madds1(pair[1], weightf, ps_madds0(pair[0], weightf, offsetf)); \
		psq_st(pair[0],14,dst,0,4); \
	} \
}

H264_WEIGHT(16,16)
H264_WEIGHT(16,8)
H264_WEIGHT(8,16)
H264_WEIGHT(8,8)
H264_WEIGHT(8,4)
H264_WEIGHT(4,8)
H264_WEIGHT(4,4)
H264_WEIGHT(4,2)
H264_WEIGHT(2,4)
H264_WEIGHT(2,2)

void ff_dsputil_h264_init_ppc(DSPContext *c, AVCodecContext *avctx)
{
	c->put_h264_chroma_pixels_tab[0] = put_h264_chroma_mc8_paired;
	c->put_h264_chroma_pixels_tab[1] = put_h264_chroma_mc4_paired;
	c->avg_h264_chroma_pixels_tab[0] = avg_h264_chroma_mc8_paired;
	c->avg_h264_chroma_pixels_tab[1] = avg_h264_chroma_mc4_paired;
}

void ff_h264dsp_init_ppc(H264DSPContext *c, const int bit_depth, const int chroma_format_idc)
{
	c->h264_idct_add = ff_h264_idct_add_paired;
	c->h264_idct_add8 = ff_h264_idct_add8_paired;
	c->h264_idct_add16 = ff_h264_idct_add16_paired;
	c->h264_idct_add16intra = ff_h264_idct_add16intra_paired;
	c->h264_idct_dc_add = ff_h264_idct_dc_add_paired;
	c->h264_idct8_add = ff_h264_idct8_add_paired;
	c->h264_idct8_dc_add = ff_h264_idct8_dc_add_paired;
	c->h264_idct8_add4 = ff_h264_idct8_add4_paired;
	
	c->weight_h264_pixels_tab[0] = weight_h264_pixels16x16_paired;
	c->weight_h264_pixels_tab[1] = weight_h264_pixels16x8_paired;
	c->weight_h264_pixels_tab[2] = weight_h264_pixels8x16_paired;
	c->weight_h264_pixels_tab[3] = weight_h264_pixels8x8_paired;
	c->weight_h264_pixels_tab[4] = weight_h264_pixels8x4_paired;
	c->weight_h264_pixels_tab[5] = weight_h264_pixels4x8_paired;
	c->weight_h264_pixels_tab[6] = weight_h264_pixels4x4_paired;
	c->weight_h264_pixels_tab[7] = weight_h264_pixels4x2_paired;
	c->weight_h264_pixels_tab[8] = weight_h264_pixels2x4_paired;
	c->weight_h264_pixels_tab[9] = weight_h264_pixels2x2_paired;
	c->biweight_h264_pixels_tab[0] = biweight_h264_pixels16x16_paired;
	c->biweight_h264_pixels_tab[1] = biweight_h264_pixels16x8_paired;
	c->biweight_h264_pixels_tab[2] = biweight_h264_pixels8x16_paired;
	c->biweight_h264_pixels_tab[3] = biweight_h264_pixels8x8_paired;
	c->biweight_h264_pixels_tab[4] = biweight_h264_pixels8x4_paired;
	c->biweight_h264_pixels_tab[5] = biweight_h264_pixels4x8_paired;
	c->biweight_h264_pixels_tab[6] = biweight_h264_pixels4x4_paired;
	c->biweight_h264_pixels_tab[7] = biweight_h264_pixels4x2_paired;
	c->biweight_h264_pixels_tab[8] = biweight_h264_pixels2x4_paired;
	c->biweight_h264_pixels_tab[9] = biweight_h264_pixels2x2_paired;
}
