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

#include "libavcodec/h264pred.h"
#include "libavutil/ppc/paired.h"

static void pred4x4_tm_vp8_paired(uint8_t *src, const uint8_t *topright, int stride)
{
	vector float pair[2];
	vector float base, offset;
	
	src -= stride + 1;
	base = psq_l(0,src,1,4);
	
	pair[0] = psq_l(1,src,0,4);
	pair[1] = psq_l(3,src,0,4);
	
	for (int y = 0; y < 4; y++) {
		offset = psq_lux(src,stride,1,4);
		asm("fsubs %0,%1,%0" : "+f"(offset) : "f"(base));
		
		psq_st(ps_sub(pair[0], offset),1,src,0,4);
		psq_st(ps_sub(pair[1], offset),3,src,0,4);
	}
}

/*static void pred16x16_plane_paired(uint8_t *src, int stride)
{
	const float scale = 0.03125;
	vector float fHV, fH, fV, fA, fB;
	
	const uint8_t *src0 = src + 7 - stride;
	const uint8_t *src1 = src + 8 * stride - 1;
	const uint8_t *src2 = src1 - 2 * stride;
	
	int iH = src0[1] - src0[-1];
	int iV = src1[0] - src2[0];
	
	for (int k = 2; k <= 8; k++) {
		src1 += stride; src2 -= stride;
		iH += k * (src0[k] - src0[-k]);
		iV += k * (src1[0] - src2[0]);
	}
	
	iH = (5 * iH + 32) >> 6;
	iV = (5 * iV + 32) >> 6;
	
	vec_s16_t iHV = {iH, iV};
	uint16_t iA = 16 * (src1[0] + src2[16] + 1) - 7 * (iV + iH);
	
	asm("psq_l	%0,%2,0,7\n"
		"psq_l	%1,%3,1,5\n"
		: "=f"(fHV), "=f"(fA)
		: "o"(iHV), "o"(iA));
	
	fH = ps_muls0(scale, fHV);
	fV = ps_muls1(scale, fHV);
	
	asm("fmuls %0,%0,%1" : "+f"(fA) : "f"(scale));
	fA = paired_sum1(fA, fA, fH);
	
	vector float add[2];
	add[0] = paired_add(fH, fH);
	add[1] = paired_add(add[0], add[0]);
	
	src -= stride;
	
	for (int j = 16; j > 0; j--) {
		psq_stux(fA,src,stride,0,4);
		psq_st(paired_add(fA, add[0]),2,src,0,4);
		fB = paired_add(fA, add[1]);
		
		psq_st(fB,4,src,0,4);
		psq_st(paired_add(fB, add[0]),6,src,0,4);
		fB = paired_add(fB, add[1]);
		
		psq_st(fB,8,src,0,4);
		psq_st(paired_add(fB, add[0]),10,src,0,4);
		fB = paired_add(fB, add[1]);
		
		psq_st(fB,12,src,0,4);
		psq_st(paired_add(fB, add[0]),14,src,0,4);
		fA = paired_add(fA, fV);
	}
}*/

static void pred16x16_tm_vp8_paired(uint8_t *src, int stride)
{
	vector float pair[8];
	vector float base, offset;
	
	src -= stride + 1;
	base = psq_l(0,src,1,4);
	
	pair[0] = psq_l(1,src,0,4);
	pair[1] = psq_l(3,src,0,4);
	pair[2] = psq_l(5,src,0,4);
	pair[3] = psq_l(7,src,0,4);
	pair[4] = psq_l(9,src,0,4);
	pair[5] = psq_l(11,src,0,4);
	pair[6] = psq_l(13,src,0,4);
	pair[7] = psq_l(15,src,0,4);
	
	for (int y = 0; y < 16; y++) {
		offset = psq_lux(src,stride,1,4);
		asm("fsubs %0,%1,%0" : "+f"(offset) : "f"(base));
		
		psq_st(ps_sub(pair[0], offset),1,src,0,4);
		psq_st(ps_sub(pair[1], offset),3,src,0,4);
		psq_st(ps_sub(pair[2], offset),5,src,0,4);
		psq_st(ps_sub(pair[3], offset),7,src,0,4);
		psq_st(ps_sub(pair[4], offset),9,src,0,4);
		psq_st(ps_sub(pair[5], offset),11,src,0,4);
		psq_st(ps_sub(pair[6], offset),13,src,0,4);
		psq_st(ps_sub(pair[7], offset),15,src,0,4);
	}
}
/*
static void pred8x8_plane_paired(uint8_t *src, int stride)
{
	const float scale = 0.03125;
	vector float fHV, fH, fV, fA;
	
	const uint8_t *src0 = src + 3 - stride;
	const uint8_t *src1 = src + 4 * stride - 1;
	const uint8_t *src2 = src1 - 2 * stride;
	
	int iH = src0[1] - src0[-1];
	int iV = src1[0] - src2[0];
	
	for (int k = 2; k <= 4; k++) {
		src1 += stride; src2 -= stride;
		iH += k * (src0[k] - src0[-k]);
		iV += k * (src1[0] - src2[0]);
	}
	
	iH = (17 * iH + 16) >> 5;
	iV = (17 * iV + 16) >> 5;
	
	vec_s16_t iHV = {iH, iV};
	uint16_t iA = 16 * (src1[0] + src2[8] + 1) - 3 * (iV + iH);
	
	asm("psq_l	%0,%2,0,7\n"
		"psq_l	%1,%3,1,5\n"
		: "=f"(fHV), "=f"(fA)
		: "o"(iHV), "o"(iA));
	
	fH = ps_muls0(scale, fHV);
	fV = ps_muls1(scale, fHV);
	
	asm("fmuls %0,%0,%1" : "+f"(fA) : "f"(scale));
	fA = paired_sum1(fA, fA, fH);
	
	vector float add[3];
	add[0] = paired_add(fH, fH);
	add[1] = paired_add(add[0], add[0]);
	add[2] = paired_add(add[1], add[0]);
	
	src -= stride;
	
	for (int j = 8; j > 0; j--) {
		psq_stux(fA,src,stride,0,4);
		psq_st(paired_add(fA, add[0]),2,src,0,4);
		psq_st(paired_add(fA, add[1]),4,src,0,4);
		psq_st(paired_add(fA, add[2]),6,src,0,4);
		fA = paired_add(fA, fV);
	}
}*/

static void pred8x8_tm_vp8_paired(uint8_t *src, int stride)
{
	vector float pair[4];
	vector float base, offset;
	
	src -= stride + 1;
	base = psq_l(0,src,1,4);
	
	pair[0] = psq_l(1,src,0,4);
	pair[1] = psq_l(3,src,0,4);
	pair[2] = psq_l(5,src,0,4);
	pair[3] = psq_l(7,src,0,4);
	
	for (int y = 0; y < 8; y++) {
		offset = psq_lux(src,stride,1,4);
		asm("fsubs %0,%1,%0" : "+f"(offset) : "f"(base));
		
		psq_st(ps_sub(pair[0], offset),1,src,0,4);
		psq_st(ps_sub(pair[1], offset),3,src,0,4);
		psq_st(ps_sub(pair[2], offset),5,src,0,4);
		psq_st(ps_sub(pair[3], offset),7,src,0,4);
	}
}

void ff_h264_pred_init_ppc(H264PredContext *h, int codec_id, const int bit_depth, const int chroma_format_idc)
{
	if (codec_id == CODEC_ID_VP8) {
		h->pred4x4[TM_VP8_PRED] = pred4x4_tm_vp8_paired;
		h->pred8x8[PLANE_PRED8x8] = pred8x8_tm_vp8_paired;
		h->pred16x16[PLANE_PRED8x8] = pred16x16_tm_vp8_paired;
	} //else {
		//h->pred8x8[PLANE_PRED8x8] = pred8x8_plane_paired;
		//if (codec_id == CODEC_ID_H264)
			//h->pred16x16[PLANE_PRED8x8] = pred16x16_plane_paired;
	//}
}
