/* Copyright (c) 2007-2008 CSIRO
   Copyright (c) 2007-2008 Xiph.Org Foundation
   Written by Jean-Marc Valin */
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* This is a simple MDCT implementation that uses a N/4 complex FFT
   to do most of the work. It should be relatively straightforward to
   plug in pretty much and FFT here.

   This replaces the Vorbis FFT (and uses the exact same API), which
   was a bit too messy and that was ending up duplicating code
   (might as well use the same FFT everywhere).

   The algorithm is similar to (and inspired from) Fabrice Bellard's
   MDCT implementation in FFMPEG, but has differences in signs, ordering
   and scaling in many places.
*/

#ifndef SKIP_CONFIG_H
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#endif

#include "mdct.h"
#include "kiss_fft.h"
#include "_kiss_fft_guts.h"
#include <math.h>
#include "os_support.h"
#include "mathops.h"
#include "stack_alloc.h"

#ifdef CUSTOM_MODES

int clt_mdct_init(mdct_lookup *l,int N, int maxshift)
{
   int i;
   int N4;
   kiss_twiddle_scalar *trig;
#if defined(FIXED_POINT)
   int N2=N>>1;
#endif
   l->n = N;
   N4 = N>>2;
   l->maxshift = maxshift;
   for (i=0;i<=maxshift;i++)
   {
      if (i==0)
         l->kfft[i] = opus_fft_alloc(N>>2>>i, 0, 0);
      else
         l->kfft[i] = opus_fft_alloc_twiddles(N>>2>>i, 0, 0, l->kfft[0]);
#ifndef ENABLE_TI_DSPLIB55
      if (l->kfft[i]==NULL)
         return 0;
#endif
   }
   l->trig = trig = (kiss_twiddle_scalar*)opus_alloc((N4+1)*sizeof(kiss_twiddle_scalar));
   if (l->trig==NULL)
     return 0;
   /* We have enough points that sine isn't necessary */
#if defined(FIXED_POINT)
   for (i=0;i<=N4;i++)
      trig[i] = TRIG_UPSCALE*celt_cos_norm(DIV32(ADD32(SHL32(EXTEND32(i),17),N2),N));
#else
   for (i=0;i<=N4;i++)
      trig[i] = (kiss_twiddle_scalar)cos(2*PI*i/N);
#endif
   return 1;
}

void clt_mdct_clear(mdct_lookup *l)
{
   int i;
   for (i=0;i<=l->maxshift;i++)
      opus_fft_free(l->kfft[i]);
   opus_free((kiss_twiddle_scalar*)l->trig);
}

#endif /* CUSTOM_MODES */

/* Forward MDCT trashes the input array */
void clt_mdct_forward(const mdct_lookup *l, kiss_fft_scalar *in, kiss_fft_scalar * restrict out,
      const opus_val16 *window, int overlap, int shift, int stride)
{
   int i;
   int N, N2, N4;
   kiss_twiddle_scalar sine;
   VARDECL(kiss_fft_scalar, f);
   SAVE_STACK;
   N = l->n;
   N >>= shift;
   N2 = N>>1;
   N4 = N>>2;
   ALLOC(f, N2, kiss_fft_scalar);
   /* sin(x) ~= x here */
#ifdef FIXED_POINT
   sine = TRIG_UPSCALE*(QCONST16(0.7853981f, 15)+N2)/N;
#else
   sine = (kiss_twiddle_scalar)2*PI*(.125f)/N;
#endif

   /* Consider the input to be composed of four blocks: [a, b, c, d] */
   /* Window, shuffle, fold */
   {
      /* Temp pointers to make it really clear to the compiler what we're doing */
      const kiss_fft_scalar * restrict xp1 = in+(overlap>>1);
      const kiss_fft_scalar * restrict xp2 = in+N2-1+(overlap>>1);
      kiss_fft_scalar * restrict yp = f;
      const opus_val16 * restrict wp1 = window+(overlap>>1);
      const opus_val16 * restrict wp2 = window+(overlap>>1)-1;
      for(i=0;i<(overlap>>2);i++)
      {
         /* Real part arranged as -d-cR, Imag part arranged as -b+aR*/
         *yp++ = MULT16_32_Q15(*wp2, xp1[N2]) + MULT16_32_Q15(*wp1,*xp2);
         *yp++ = MULT16_32_Q15(*wp1, *xp1)    - MULT16_32_Q15(*wp2, xp2[-N2]);
         xp1+=2;
         xp2-=2;
         wp1+=2;
         wp2-=2;
      }
      wp1 = window;
      wp2 = window+overlap-1;
      for(;i<N4-(overlap>>2);i++)
      {
         /* Real part arranged as a-bR, Imag part arranged as -c-dR */
         *yp++ = *xp2;
         *yp++ = *xp1;
         xp1+=2;
         xp2-=2;
      }
      for(;i<N4;i++)
      {
         /* Real part arranged as a-bR, Imag part arranged as -c-dR */
         *yp++ =  -MULT16_32_Q15(*wp1, xp1[-N2]) + MULT16_32_Q15(*wp2, *xp2);
         *yp++ = MULT16_32_Q15(*wp2, *xp1)     + MULT16_32_Q15(*wp1, xp2[N2]);
         xp1+=2;
         xp2-=2;
         wp1+=2;
         wp2-=2;
      }
   }
   /* Pre-rotation */
   {
      kiss_fft_scalar * restrict yp = f;
      const kiss_twiddle_scalar *t = &l->trig[0];
      for(i=0;i<N4;i++)
      {
         kiss_fft_scalar re, im, yr, yi;
         re = yp[0];
         im = yp[1];
         yr = -S_MUL(re,t[i<<shift])  -  S_MUL(im,t[(N4-i)<<shift]);
         yi = -S_MUL(im,t[i<<shift])  +  S_MUL(re,t[(N4-i)<<shift]);
         /* works because the cos is nearly one */
         *yp++ = yr + S_MUL(yi,sine);
         *yp++ = yi - S_MUL(yr,sine);
      }
   }

   /* N/4 complex FFT, down-scales by 4/N */
   opus_fft(l->kfft[shift], (kiss_fft_cpx *)f, (kiss_fft_cpx *)in);

   /* Post-rotate */
   {
      /* Temp pointers to make it really clear to the compiler what we're doing */
      const kiss_fft_scalar * restrict fp = in;
      kiss_fft_scalar * restrict yp1 = out;
      kiss_fft_scalar * restrict yp2 = out+stride*(N2-1);
      const kiss_twiddle_scalar *t = &l->trig[0];
      /* Temp pointers to make it really clear to the compiler what we're doing */
      for(i=0;i<N4;i++)
      {
         kiss_fft_scalar yr, yi;
         yr = S_MUL(fp[1],t[(N4-i)<<shift]) + S_MUL(fp[0],t[i<<shift]);
         yi = S_MUL(fp[0],t[(N4-i)<<shift]) - S_MUL(fp[1],t[i<<shift]);
         /* works because the cos is nearly one */
         *yp1 = yr - S_MUL(yi,sine);
         *yp2 = yi + S_MUL(yr,sine);;
         fp += 2;
         yp1 += 2*stride;
         yp2 -= 2*stride;
      }
   }
   RESTORE_STACK;
}

void clt_mdct_backward(const mdct_lookup *l, kiss_fft_scalar *in, kiss_fft_scalar * restrict out,
      const opus_val16 * restrict window, int overlap, int shift, int stride)
{
   int i;
   int N, N2, N4;
   kiss_twiddle_scalar sine;
   VARDECL(kiss_fft_scalar, f);
   VARDECL(kiss_fft_scalar, f2);
   SAVE_STACK;
   N = l->n;
   N >>= shift;
   N2 = N>>1;
   N4 = N>>2;
   ALLOC(f, N2, kiss_fft_scalar);
   ALLOC(f2, N2, kiss_fft_scalar);
   /* sin(x) ~= x here */
#ifdef FIXED_POINT
   sine = TRIG_UPSCALE*(QCONST16(0.7853981f, 15)+N2)/N;
#else
   sine = (kiss_twiddle_scalar)2*PI*(.125f)/N;
#endif

   /* Pre-rotate */
   {
      /* Temp pointers to make it really clear to the compiler what we're doing */
      const kiss_fft_scalar * restrict xp1 = in;
      const kiss_fft_scalar * restrict xp2 = in+stride*(N2-1);
      kiss_fft_scalar * restrict yp = f2;
      const kiss_twiddle_scalar *t = &l->trig[0];
      for(i=0;i<N4;i++)
      {
         kiss_fft_scalar yr, yi;
         yr = -S_MUL(*xp2, t[i<<shift]) + S_MUL(*xp1,t[(N4-i)<<shift]);
         yi =  -S_MUL(*xp2, t[(N4-i)<<shift]) - S_MUL(*xp1,t[i<<shift]);
         /* works because the cos is nearly one */
         *yp++ = yr - S_MUL(yi,sine);
         *yp++ = yi + S_MUL(yr,sine);
         xp1+=2*stride;
         xp2-=2*stride;
      }
   }

   /* Inverse N/4 complex FFT. This one should *not* downscale even in fixed-point */
   opus_ifft(l->kfft[shift], (kiss_fft_cpx *)f2, (kiss_fft_cpx *)f);

   /* Post-rotate */
   {
      kiss_fft_scalar * restrict fp = f;
      const kiss_twiddle_scalar *t = &l->trig[0];

      for(i=0;i<N4;i++)
      {
         kiss_fft_scalar re, im, yr, yi;
         re = fp[0];
         im = fp[1];
         /* We'd scale up by 2 here, but instead it's done when mixing the windows */
         yr = S_MUL(re,t[i<<shift]) - S_MUL(im,t[(N4-i)<<shift]);
         yi = S_MUL(im,t[i<<shift]) + S_MUL(re,t[(N4-i)<<shift]);
         /* works because the cos is nearly one */
         *fp++ = yr - S_MUL(yi,sine);
         *fp++ = yi + S_MUL(yr,sine);
      }
   }
   /* De-shuffle the components for the middle of the window only */
   {
      const kiss_fft_scalar * restrict fp1 = f;
      const kiss_fft_scalar * restrict fp2 = f+N2-1;
      kiss_fft_scalar * restrict yp = f2;
      for(i = 0; i < N4; i++)
      {
         *yp++ =-*fp1;
         *yp++ = *fp2;
         fp1 += 2;
         fp2 -= 2;
      }
   }
   out -= (N2-overlap)>>1;
   /* Mirror on both sides for TDAC */
   {
      kiss_fft_scalar * restrict fp1 = f2+N4-1;
      kiss_fft_scalar * restrict xp1 = out+N2-1;
      kiss_fft_scalar * restrict yp1 = out+N4-overlap/2;
      const opus_val16 * restrict wp1 = window;
      const opus_val16 * restrict wp2 = window+overlap-1;
      for(i = 0; i< N4-overlap/2; i++)
      {
         *xp1 = *fp1;
         xp1--;
         fp1--;
      }
      for(; i < N4; i++)
      {
         kiss_fft_scalar x1;
         x1 = *fp1--;
         *yp1++ +=-MULT16_32_Q15(*wp1, x1);
         *xp1-- += MULT16_32_Q15(*wp2, x1);
         wp1++;
         wp2--;
      }
   }
   {
      kiss_fft_scalar * restrict fp2 = f2+N4;
      kiss_fft_scalar * restrict xp2 = out+N2;
      kiss_fft_scalar * restrict yp2 = out+N-1-(N4-overlap/2);
      const opus_val16 * restrict wp1 = window;
      const opus_val16 * restrict wp2 = window+overlap-1;
      for(i = 0; i< N4-overlap/2; i++)
      {
         *xp2 = *fp2;
         xp2++;
         fp2++;
      }
      for(; i < N4; i++)
      {
         kiss_fft_scalar x2;
         x2 = *fp2++;
         *yp2--  = MULT16_32_Q15(*wp1, x2);
         *xp2++  = MULT16_32_Q15(*wp2, x2);
         wp1++;
         wp2--;
      }
   }
   RESTORE_STACK;
}
