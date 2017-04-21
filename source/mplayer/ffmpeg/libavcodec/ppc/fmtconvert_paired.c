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

#include "libavcodec/fmtconvert.h"
#include "dsputil_paired.h"
#include "libavutil/ppc/paired.h"

static void int32_to_float_fmul_scalar_paired(float *dst, const int *src, float mul, int len)
{
	vector float pair;
	
	for (int i = 0; i < len*4; i += 8) {
		float src0 = *src++;
		float src1 = *src++;
		
		pair = ps_merge00(src0, src1);
		pair = ps_muls0(pair, mul);
		paired_stx(pair, i, dst);
	}
}

static void float_to_int16_paired(int16_t *dst, const float *src, long len)
{
	src -= 2;
	dst -= 2;
	
	for (int i = 0; i < (len>>1); i++) {
		vec_f32_t pair = psq_lu(8,src,0,0);
		psq_stu(pair,4,dst,0,7);
	}
}

static void float_to_int16_interleave_paired(int16_t *dst, const float **src, long len, int channels)
{
	vector float pair[2];
	vector float result;
	
	if (channels > 2) {
		dst -= 2;
		for (int i = 0; i < len*4; i += 8, dst += channels) {
			for (int c = 0; c < channels - 1; c += 2) {
				pair[0] = paired_lx(i, src[c]);
				pair[1] = paired_lx(i, src[c+1]);
				
				result = paired_merge00(pair[0], pair[1]);
				psq_stu(result,4,dst,0,7);
				
				result = paired_merge11(pair[0], pair[1]);
				psq_stx(result,channels*2,dst,0,7);
			}
		}
	} else {
		if (channels == 2) {
			for (int i = 0; i < len*4; i += 8) {
				pair[0] = paired_lx(i, src[0]);
				pair[1] = paired_lx(i, src[1]);
				
				result = paired_merge00(pair[0], pair[1]);
				psq_stx(result,i,dst,0,7);
				
				result = paired_merge11(pair[0], pair[1]);
				psq_stx(result,i,dst+2,0,7);
			}
		} else float_to_int16_paired(dst, src[0], len);
	}
}

void ff_fmt_convert_init_ppc(FmtConvertContext *c, AVCodecContext *avctx)
{
	c->int32_to_float_fmul_scalar = int32_to_float_fmul_scalar_paired;
	c->float_to_int16 = float_to_int16_paired;
	c->float_to_int16_interleave = float_to_int16_interleave_paired;
}
