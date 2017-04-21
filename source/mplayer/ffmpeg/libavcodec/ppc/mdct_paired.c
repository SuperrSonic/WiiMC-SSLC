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
#include "libavutil/ppc/paired.h"

#define CMUL(d0,d1,a0,a1,b0,b1,c0,c1)	\
{										\
	vector float dd;					\
	dd = paired_merge00(a0,a1);			\
	d0 = paired_mul(dd,c0);				\
	d1 = paired_mul(dd,c1);				\
	dd = paired_merge11(b0,b1);			\
	d0 = paired_msub(dd,c1,d0);			\
	d1 = paired_madd(dd,c0,d1);			\
}

void ff_imdct_half_paired(FFTContext *s, FFTSample *output, const FFTSample *input)
{
	int n = 1 << s->mdct_bits;
	int n2 = n >> 1;
	int n4 = n >> 2;
	int n8 = n >> 3;
	
	const uint16_t *revtab = s->revtab;
	const FFTSample *tcos = s->tcos;
	const FFTSample *tsin = s->tsin;
	
	vector float pair[4], sub[2], add[2];
	vector float result, cos, sin;
	
	FFTSample *base[6][2] = {{input-2,input+n2},{tcos-2,tcos+n4},{tsin-2,tsin+n4},
					{output+n4,output+n4-2},{tcos+n8,tcos+n8-2},{tsin+n8,tsin+n8-2}};
	
	int k, j;
	for (k=0, j=n4-2; k<n8; k+=2, j-=2) {
 		pair[0] = psq_lu(8,base[0][0],0,0);
		pair[1] = psq_lu(8,base[0][0],0,0);
		
		pair[2] = psq_lu(-8,base[0][1],0,0);
		pair[3] = psq_lu(-8,base[0][1],0,0);
		
		cos = psq_lu(8,base[1][0],0,0);
		sin = psq_lu(8,base[2][0],0,0);
		
		CMUL(sub[0],add[0],pair[0],pair[1],pair[2],pair[3],sin,cos);
		
		cos = psq_lu(-8,base[1][1],0,0);
		sin = psq_lu(-8,base[2][1],0,0);
		
		CMUL(sub[1],add[1],pair[3],pair[2],pair[1],pair[0],sin,cos);
		
		result = paired_merge00(sub[0], add[0]);
		psq_stx(result,revtab[k]*8,output,0,0);
		result = paired_merge11(sub[0], add[0]);
		psq_stx(result,revtab[k+1]*8,output,0,0);
		
		result = paired_merge00(sub[1], add[1]);
		psq_stx(result,revtab[j]*8,output,0,0);
		result = paired_merge11(sub[1], add[1]);
		psq_stx(result,revtab[j+1]*8,output,0,0);
	}
	
	s->fft_calc(s, (FFTComplex *)output);
	
	for (k=0; k<n8; k+=2) {
		pair[0] = psq_lu(-8,base[3][0],0,0);
		pair[1] = psq_l(-8,base[3][0],0,0);
		
		cos = psq_lu(-8,base[4][0],0,0);
		sin = psq_lu(-8,base[5][0],0,0);
		
		CMUL(sub[0],add[1],pair[1],pair[0],pair[1],pair[0],cos,sin);
		
		pair[0] = psq_lu(8,base[3][1],0,0);
		pair[1] = psq_l(8,base[3][1],0,0);
		
		cos = psq_lu(8,base[4][1],0,0);
		sin = psq_lu(8,base[5][1],0,0);
		
		CMUL(sub[1],add[0],pair[0],pair[1],pair[0],pair[1],cos,sin);
		
		result = paired_merge10(sub[0], add[0]);
		psq_st(result,0,base[3][0],0,0);
		result = paired_merge01(sub[0], add[0]);
		psq_stu(result,-8,base[3][0],0,0);
		
		result = paired_merge01(sub[1], add[1]);
		psq_st(result,0,base[3][1],0,0);
		result = paired_merge10(sub[1], add[1]);
		psq_stu(result,8,base[3][1],0,0);
	}
}

void ff_imdct_calc_paired(FFTContext *s, FFTSample *output, const FFTSample *input)
{
	int n = 1 << s->mdct_bits;
	int n2 = n >> 1;
	int n4 = n >> 2;
	
	ff_imdct_half_paired(s, output+n4, input);
	
	vector float pair;
	FFTSample *base[2][2] = {{output+n2,output-2},{output+n2-2,output+n}};
	
	for (int k=0; k<n4; k+=2) {
		pair = psq_lu(-8,base[0][0],0,0);
		pair = paired_neg(pair);
		pair = paired_merge10(pair, pair);
		psq_stu(pair,8,base[0][1],0,0);
		
		pair = psq_lu(8,base[1][0],0,0);
		pair = paired_merge10(pair, pair);
		psq_stu(pair,-8,base[1][1],0,0);
	}
}
