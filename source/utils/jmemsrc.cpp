/*
 * jmemsrc.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains decompression data source routines for the case of
 * reading JPEG data from a memory buffer that is preloaded with the entire
 * JPEG file.  This would not seem especially useful at first sight, but
 * a number of people have asked for it.
 * This is really just a stripped-down version of jdatasrc.c.  Comparison
 * of this code with jdatasrc.c may be helpful in seeing how to make
 * custom source managers for other purposes.
 */

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <jpeglib.h>
#include <jerror.h>
#include <gctypes.h>

#include "video.h"
#include "mem2_manager.h"

//only texture in mem2, internal memory managed by gcc
#define jpg_malloc malloc
#define jpg_free free
#define jpg_memalign memalign

//#define jpg_malloc mem2_malloc
//#define jpg_free mem2_free
//#define jpg_memalign mem2_memalign

// ******************************************************************************************
// ******************************************************************************************
// scaler code by David Ashley dash@xdr.com based on algorithm used in pnmscale
// ******************************************************************************************
// ******************************************************************************************
typedef struct scaler {
	void (*callback)(void *data, unsigned char *row, int len);
	void *callback_data;
	int sw,sh;
	int dw,dh;
	int bpp;
	unsigned char *accum;
	unsigned char *output;
	int yf,yr;
	int bytewidth;
	int obytewidth;
} scaler;

static inline void accumrow(unsigned char *d, int w, unsigned char *s,
		int top, int bottom)
{
	int f;
	f=0x100*top/bottom;
	while(w--)
		d[w] += s[w]*f>>8;
}

static inline void accumpixel(unsigned char *d, int bpp, unsigned char *s,
		int top, int bottom)
{
	int f=0x100*top/bottom;
	while(bpp--)
		d[bpp] += s[bpp]*f>>8;
}

static inline void xscale(unsigned char *d, int w2, unsigned char *s, int w1, int bpp)
{
	int pixelcount;
	int xf,xr;

	pixelcount=w2;

	xf=w2;
	xr=w1;

	memset(d, 0, w2*bpp);
	while(pixelcount)
	{
		if(xr>xf)
		{
			accumpixel(d, bpp, s, xf, w1);
			xr-=xf;
			xf=0;
		} else
		{
			accumpixel(d, bpp, s, xr, w1);
			xf-=xr;
			xr=0;

		}
		if(xr==0) // we can kick out a pixel
		{
			d+=bpp;
			xr=w1;
			--pixelcount;
		}
		if(xf==0) // we can step the source pointer
		{
			s+=bpp;
			xf=w2;
		}
	}
}

void *scaler_alloc(int dw, int dh, int sw, int sh, int bpp,
		void (*callback)(void *data, unsigned char *row, int len),
		void *data)
{
	scaler *s=(scaler *)malloc(sizeof(scaler));
	if(s)
	{
		s->bytewidth = sw * bpp;
		s->obytewidth = dw * bpp;
		s->accum = (unsigned char *)malloc(s->bytewidth);
		s->output = (unsigned char *)malloc(s->obytewidth);
		s->sw = sw;
		s->sh = sh;
		s->dw = dw;
		s->dh = dh;
		s->bpp = bpp;
		s->callback = callback;
		s->callback_data = data;
		s->yf=dh;
		s->yr=sh;
		if(s->accum && s->output)
			memset(s->accum, 0, s->bytewidth);
		else
		{
			if(s->accum)
				free(s->accum);
			if(s->output)
				free(s->output);
			free(s);
			s=0;
		}
	}
 	return s;
}

void scaler_free(void *p)
{
	scaler *s=(scaler *)p;
	if(s)
	{
		if(s->accum)
			free(s->accum);
		if(s->output)
			free(s->output);
		free(s);
	}
}

void scaler_feed(void *p, unsigned char *row)
{
	scaler *s=(scaler *)p;
	while(s)
	{
		if(s->yr > s->yf)
		{
			accumrow(s->accum, s->bytewidth, row, s->yf, s->sh);
			s->yr -= s->yf;
			s->yf = 0;
		} else
		{
			accumrow(s->accum, s->bytewidth, row, s->yr, s->sh);
			s->yf -= s->yr;
			s->yr = 0;
		}
		if(s->yr == 0) // we can kick out a line
		{
			xscale(s->output, s->dw, s->accum, s->sw, s->bpp);
			memset(s->accum, 0, s->bytewidth);
			s->yr = s->sh;
			s->callback(s->callback_data, s->output, s->obytewidth);
		}
		if(s->yf == 0) // we're done with the input line.
		{
			s->yf = s->dh;
			break;
		}
	}
}
// ******************************************************************************************
// ******************************************************************************************
// end of scaler code
// ******************************************************************************************
// ******************************************************************************************

/* Expanded data source object for memory input */

/**
 * \brief very low level C 'structure', used to decode jpeg file
 * Should not appear in the Doxygen supplied documentation
 */
typedef struct
{
	struct jpeg_source_mgr pub; /* public fields */
	JOCTET eoi_buffer[2]; /* a place to put a dummy EOI */
} my_source_mgr;

typedef my_source_mgr * my_src_ptr;

/*
 * Initialize source --- called by jpeg_read_header
 * before any data is actually read.
 */

METHODDEF(void)
init_source (j_decompress_ptr cinfo)
{
	/* No work, since jpeg_memory_src set up the buffer pointer and count.
	 * Indeed, if we want to read multiple JPEG images from one buffer,
	 * this *must* not do anything to the pointer.
	 */
	(void)cinfo;
}

/*
 * Fill the input buffer --- called whenever buffer is emptied.
 *
 * In this application, this routine should never be called; if it is called,
 * the decompressor has overrun the end of the input buffer, implying we
 * supplied an incomplete or corrupt JPEG datastream.  A simple error exit
 * might be the most appropriate response.
 *
 * But what we choose to do in this code is to supply dummy EOI markers
 * in order to force the decompressor to finish processing and supply
 * some sort of output image, no matter how corrupted.
 */

METHODDEF(boolean)
fill_input_buffer (j_decompress_ptr cinfo)
{
	my_src_ptr src = (my_src_ptr) cinfo->src;

	WARNMS(cinfo, JWRN_JPEG_EOF);

	/* Create a fake EOI marker */
	src->eoi_buffer[0] = (JOCTET) 0xFF;
	src->eoi_buffer[1] = (JOCTET) JPEG_EOI;
	src->pub.next_input_byte = src->eoi_buffer;
	src->pub.bytes_in_buffer = 2;

	return TRUE;
}

/*
 * Skip data --- used to skip over a potentially large amount of
 * uninteresting data (such as an APPn marker).
 *
 * If we overrun the end of the buffer, we let fill_input_buffer deal with
 * it.  An extremely large skip could cause some time-wasting here, but
 * it really isn't supposed to happen ... and the decompressor will never
 * skip more than 64K anyway.
 */

METHODDEF(void)
skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
	my_src_ptr src = (my_src_ptr) cinfo->src;

	if (num_bytes> 0)
	{
		while (num_bytes> (long) src->pub.bytes_in_buffer)
		{
			num_bytes -= (long) src->pub.bytes_in_buffer;
			(void) fill_input_buffer(cinfo);
			/* note we assume that fill_input_buffer will never return FALSE,
			 * so suspension need not be handled.
			 */
		}
		src->pub.next_input_byte += (size_t) num_bytes;
		src->pub.bytes_in_buffer -= (size_t) num_bytes;
	}
}

/*
 * An additional method that can be provided by data source modules is the
 * resync_to_restart method for error recovery in the presence of RST markers.
 * For the moment, this source module just uses the default resync method
 * provided by the JPEG library.  That method assumes that no backtracking
 * is possible.
 */

/*
 * Terminate source --- called by jpeg_finish_decompress
 * after all data has been read.  Often a no-op.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

METHODDEF(void)
term_source (j_decompress_ptr cinfo)
{
	/* no work necessary here */
	(void)cinfo;
}

/*
 * Prepare for input from a memory buffer.
 */

GLOBAL(void)
jpeg_memory_src (j_decompress_ptr cinfo, const JOCTET * buffer, size_t bufsize)
{
	my_src_ptr src;

	/* The source object is made permanent so that a series of JPEG images
	 * can be read from a single buffer by calling jpeg_memory_src
	 * only before the first one.
	 * This makes it unsafe to use this manager and a different source
	 * manager serially with the same JPEG object.  Caveat programmer.
	 */
	if (cinfo->src == NULL)
	{ /* first time for this JPEG object? */
		cinfo->src = (struct jpeg_source_mgr *)
		(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
		sizeof(my_source_mgr));
	}

	src = (my_src_ptr) cinfo->src;
	src->pub.init_source = init_source;
	src->pub.fill_input_buffer = fill_input_buffer;
	src->pub.skip_input_data = skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
	src->pub.term_source = term_source;

	src->pub.next_input_byte = buffer;
	src->pub.bytes_in_buffer = bufsize;
}

static inline u32 coordsRGBA8(u32 x, u32 y, u32 w)
{
	return ((((y >> 2) * (w >> 2) + (x >> 2)) << 5) + ((y & 3) << 2) + (x & 3)) << 1;
}

static u8 * RawTo4x4RGBA(u8 *src, u32 width, u32 height, u32 rowsize, int * dstWidth, int * dstHeight, u8 *dstPtr)
{
	u8 *dst;
	int x, y, offset;
	u8 *pixel;

	int padWidth = width;
	int padHeight = height;
	if(padWidth%4) padWidth += (4-padWidth%4);
	if(padHeight%4) padHeight += (4-padHeight%4);

	int len = (padWidth * padHeight) << 2;
	if(len%32) len += (32-len%32);

	if(dstPtr)
		dst = dstPtr; // use existing allocation
	else
		dst = (u8 *)mem2_memalign (32, len, MEM2_GUI);

	if(!dst)
		return NULL;

	for (y = 0; y < padHeight; y++)
	{
		for (x = 0; x < padWidth; x++)
		{
			offset = coordsRGBA8(x, y, padWidth);

			if(x >= (int)width || y >= (int)height)
			{
				dst[offset] = 0;
				dst[offset+1] = 255;
				dst[offset+32] = 255;
				dst[offset+33] = 255;
			}
			else
			{
				pixel = &src[rowsize*y+x*3];
				dst[offset] = 255; // Alpha
				dst[offset+1] = pixel[0]; // Red
				dst[offset+32] = pixel[1]; // Green
				dst[offset+33] = pixel[2]; // Blue
			}
		}
	}

	*dstWidth = padWidth;
	*dstHeight = padHeight;
	DCFlushRange(dst, len);
	return dst;
}

struct stuffer {
	unsigned char *dest;
	int stride;
};

static void got_row(void *data, unsigned char *row, int len)
{
	struct stuffer *st = (struct stuffer *)data;
	memcpy(st->dest, row, len);
	st->dest += st->stride;
}

u8 * DecodeJPEG(const u8 *src, u32 len, int *width, int *height, u8 *dstPtr)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	int output_width, output_height;
	void *scaler;
	float scale, scale2;
	struct stuffer stuffer;

	jpeg_create_decompress(&cinfo);
	cinfo.err = jpeg_std_error(&jerr);
	cinfo.progress = NULL;
	jpeg_memory_src(&cinfo, src, len);
	jpeg_read_header(&cinfo, TRUE);
	if(cinfo.jpeg_color_space == JCS_GRAYSCALE)
		cinfo.out_color_space = JCS_RGB; 

	jpeg_calc_output_dimensions(&cinfo);
	jpeg_start_decompress(&cinfo);

	scale = (float)MAX_TEX_WIDTH / cinfo.output_width;
	scale2 = (float)MAX_TEX_HEIGHT / cinfo.output_height;
	if(scale2 < scale)
		scale = scale2;
	// guarantee that one of width or height ends up at max allowed value
	output_width = scale * cinfo.output_width;
	output_height = scale * cinfo.output_height;

	stuffer.stride = output_width * cinfo.output_components;

	u8 *tmpData = (u8 *)jpg_malloc(stuffer.stride * output_height);
	JSAMPROW row_pointer[1];
	row_pointer[0] = (u8 *)jpg_malloc(cinfo.output_width * cinfo.output_components);

	stuffer.dest = tmpData;

	scaler = scaler_alloc(output_width, output_height,
		cinfo.output_width, cinfo.output_height,
		cinfo.output_components, got_row, &stuffer);

	if(!tmpData || !row_pointer[0] || !scaler)
	{
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		if(row_pointer[0])
			jpg_free(row_pointer[0]);
		if(tmpData)
			jpg_free(tmpData);
		if(scaler)
			scaler_free(scaler);
		return NULL;
	}

	while (cinfo.output_scanline < cinfo.output_height)
	{
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
		scaler_feed(scaler, row_pointer[0]);
	}
	scaler_free(scaler);

	u8 *dst = RawTo4x4RGBA(tmpData, output_width, output_height, stuffer.stride, width, height, dstPtr);

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	jpg_free(row_pointer[0]);
	jpg_free(tmpData);
	return dst;
}
