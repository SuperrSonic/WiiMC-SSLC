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

#if CONFIG_MDCT
void ff_imdct_calc_paired(FFTContext *s, FFTSample *output, const FFTSample *input);
void ff_imdct_half_paired(FFTContext *s, FFTSample *output, const FFTSample *input);
#endif

av_cold void ff_fft_init_paired(FFTContext *s)
{
#if CONFIG_MDCT
	s->imdct_calc = ff_imdct_calc_paired;
	s->imdct_half = ff_imdct_half_paired;
#endif
}
