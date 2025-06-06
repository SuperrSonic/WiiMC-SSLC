#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef CUSTOM_MODES
#define CUSTOM_MODES
#endif

#define CELT_C

#include "mathops.c"
#include "entenc.c"
#include "entdec.c"
#include "entcode.c"
#include "bands.c"
#include "vq.c"
#include "cwrs.c"
#include <stdio.h>
#include <math.h>

#ifdef FIXED_POINT
#define WORD "%d"
#else
#define WORD "%f"
#endif

int ret = 0;

void testdiv(void)
{
   opus_int32 i;
   for (i=1;i<=327670;i++)
   {
      double prod;
      opus_val32 val;
      val = celt_rcp(i);
#ifdef FIXED_POINT
      prod = (1./32768./65526.)*val*i;
#else
      prod = val*i;
#endif
      if (fabs(prod-1) > .00025)
      {
         fprintf (stderr, "div failed: 1/%d="WORD" (product = %f)\n", i, val, prod);
         ret = 1;
      }
   }
}

void testsqrt(void)
{
   opus_int32 i;
   for (i=1;i<=1000000000;i++)
   {
      double ratio;
      opus_val16 val;
      val = celt_sqrt(i);
      ratio = val/sqrt(i);
      if (fabs(ratio - 1) > .0005 && fabs(val-sqrt(i)) > 2)
      {
         fprintf (stderr, "sqrt failed: sqrt(%d)="WORD" (ratio = %f)\n", i, val, ratio);
         ret = 1;
      }
      i+= i>>10;
   }
}

void testbitexactcos(void)
{
   int i;
   opus_int32 min_d,max_d,last,chk;
   chk=max_d=0;
   last=min_d=32767;
   for(i=64;i<=16320;i++)
   {
      opus_int32 d;
      opus_int32 q=bitexact_cos(i);
      chk ^= q*i;
      d = last - q;
      if (d>max_d)max_d=d;
      if (d<min_d)min_d=d;
      last = q;
   }
   if ((chk!=89408644)||(max_d!=5)||(min_d!=0)||(bitexact_cos(64)!=32767)||
       (bitexact_cos(16320)!=200)||(bitexact_cos(8192)!=23171))
   {
      fprintf (stderr, "bitexact_cos failed\n");
      ret = 1;
   }
}

void testbitexactlog2tan(void)
{
   int i,fail;
   opus_int32 min_d,max_d,last,chk;
   fail=chk=max_d=0;
   last=min_d=15059;
   for(i=64;i<8193;i++)
   {
      opus_int32 d;
      opus_int32 mid=bitexact_cos(i);
      opus_int32 side=bitexact_cos(16384-i);
      opus_int32 q=bitexact_log2tan(mid,side);
      chk ^= q*i;
      d = last - q;
      if (q!=-1*bitexact_log2tan(side,mid))
        fail = 1;
      if (d>max_d)max_d=d;
      if (d<min_d)min_d=d;
      last = q;
   }
   if ((chk!=15821257)||(max_d!=61)||(min_d!=-2)||fail||
       (bitexact_log2tan(32767,200)!=15059)||(bitexact_log2tan(30274,12540)!=2611)||
       (bitexact_log2tan(23171,23171)!=0))
   {
      fprintf (stderr, "bitexact_log2tan failed\n");
      ret = 1;
   }
}

#ifndef FIXED_POINT
void testlog2(void)
{
   float x;
   for (x=0.001;x<1677700.0;x+=(x/8.0))
   {
      float error = fabs((1.442695040888963387*log(x))-celt_log2(x));
      if (error>0.0009)
      {
         fprintf (stderr, "celt_log2 failed: fabs((1.442695040888963387*log(x))-celt_log2(x))>0.001 (x = %f, error = %f)\n", x,error);
         ret = 1;
      }
   }
}

void testexp2(void)
{
   float x;
   for (x=-11.0;x<24.0;x+=0.0007)
   {
      float error = fabs(x-(1.442695040888963387*log(celt_exp2(x))));
      if (error>0.0002)
      {
         fprintf (stderr, "celt_exp2 failed: fabs(x-(1.442695040888963387*log(celt_exp2(x))))>0.0005 (x = %f, error = %f)\n", x,error);
         ret = 1;
      }
   }
}

void testexp2log2(void)
{
   float x;
   for (x=-11.0;x<24.0;x+=0.0007)
   {
      float error = fabs(x-(celt_log2(celt_exp2(x))));
      if (error>0.001)
      {
         fprintf (stderr, "celt_log2/celt_exp2 failed: fabs(x-(celt_log2(celt_exp2(x))))>0.001 (x = %f, error = %f)\n", x,error);
         ret = 1;
      }
   }
}
#else
void testlog2(void)
{
   opus_val32 x;
   for (x=8;x<1073741824;x+=(x>>3))
   {
      float error = fabs((1.442695040888963387*log(x/16384.0))-celt_log2(x)/1024.0);
      if (error>0.003)
      {
         fprintf (stderr, "celt_log2 failed: x = %ld, error = %f\n", (long)x,error);
         ret = 1;
      }
   }
}

void testexp2(void)
{
   opus_val16 x;
   for (x=-32768;x<15360;x++)
   {
      float error1 = fabs(x/1024.0-(1.442695040888963387*log(celt_exp2(x)/65536.0)));
      float error2 = fabs(exp(0.6931471805599453094*x/1024.0)-celt_exp2(x)/65536.0);
      if (error1>0.0002&&error2>0.00004)
      {
    	 fprintf (stderr, "celt_exp2 failed: x = "WORD", error1 = %f, error2 = %f\n", x,error1,error2);
         ret = 1;
      }
   }
}

void testexp2log2(void)
{
   opus_val32 x;
   for (x=8;x<65536;x+=(x>>3))
   {
      float error = fabs(x-0.25*celt_exp2(celt_log2(x)))/16384;
      if (error>0.004)
      {
         fprintf (stderr, "celt_log2/celt_exp2 failed: fabs(x-(celt_exp2(celt_log2(x))))>0.001 (x = %ld, error = %f)\n", (long)x,error);
         ret = 1;
      }
   }
}

void testilog2(void)
{
   opus_val32 x;
   for (x=1;x<=268435455;x+=127)
   {
      opus_val32 error = abs(celt_ilog2(x)-(int)floor(log2(x)));
      if (error!=0)
      {
         printf("celt_ilog2 failed: celt_ilog2(x)!=floor(log2(x)) (x = %d, error = %d)\n",x,error);
         ret = 1;
      }
   }
}
#endif

int main(void)
{
   testbitexactcos();
   testbitexactlog2tan();
   testdiv();
   testsqrt();
   testlog2();
   testexp2();
   testexp2log2();
#ifdef FIXED_POINT
   testilog2();
#endif
   return ret;
}
