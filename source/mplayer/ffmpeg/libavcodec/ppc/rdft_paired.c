/*
 * Copyright (c) 2010 Extrems <metaradil@gmail.com>
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

#include "libavcodec/fft.h"
#include "libavcodec/rdft.h"
#include "libavutil/ppc/paired.h"

static void ff_rdft_calc_paired(RDFTContext *s, FFTSample *data)
{
	vector float result, pair[2];
	const int n = 1 << s->nbits;
	const float k1 = 0.5;
	vector float k2 = {0.5-s->inverse,-(0.5-s->inverse)};
	const FFTSample *tcos = s->tcos;
	const FFTSample *tsin = s->tsin;
	
	if (!s->inverse) {
		s->fft.fft_permute(&s->fft, (FFTComplex *)data);
		s->fft.fft_calc(&s->fft, (FFTComplex *)data);
	}
	
	result = psq_l(0,data,0,0);
	pair[0] = paired_sum0(result, result, result);
	pair[1] = paired_neg(result);
	result = paired_sum1(result, pair[0], pair[1]);
	psq_st(result,0,data,0,0);
	
	FFTSample *base[2] = {data,data+n};
	vector float even, odd;
	vector float merge, neg;
	
	int i;
	for (i=1; i<(n>>2); i++) {
		pair[0] = psq_lu(8,base[0],0,0);
		pair[1] = psq_lu(-8,base[1],0,0);
		
		neg = paired_neg(pair[1]);
		merge = paired_merge01(pair[1], neg);
		even = paired_add(pair[0], merge);
		even = ps_muls0(even, k1);
		
		merge = paired_merge10(pair[0], pair[0]);
		neg = paired_merge10(pair[1], neg);
		odd = paired_add(merge, neg);
		odd = paired_mul(odd, k2);
		
		float cos = tcos[i];
		float sin = tsin[i];
		
		merge = paired_merge10(odd, odd);
		neg = paired_neg(merge);
		neg = paired_merge01(neg, merge);
		
		result = ps_madds0(neg, sin, even);
		result = ps_madds0(odd, cos, result);
		psq_st(result,0,base[0],0,0);
		
		result = ps_muls0(odd, cos);
		pair[0] = paired_merge01(even, result);
		pair[1] = paired_merge01(result, even);
		result = paired_sub(pair[0], pair[1]);
		result = ps_madds0(merge, sin, result);
		psq_st(result,0,base[1],0,0);
	}
	
	data[2*i+1] = s->sign_convention * data[2*i+1];
	
	if (s->inverse) {
		result = psq_l(0,data,0,0);
		result = ps_muls0(result, k1);
		psq_st(result,0,data,0,0);
		
		s->fft.fft_permute(&s->fft, (FFTComplex *)data);
		s->fft.fft_calc(&s->fft, (FFTComplex *)data);
	}
}

av_cold void ff_rdft_init_paired(RDFTContext *s)
{
	s->rdft_calc = ff_rdft_calc_paired;
}
