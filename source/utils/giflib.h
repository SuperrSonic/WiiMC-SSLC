/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * giflib.h
 * Memory based GIF decoding to 4x4RGBA texture
 ***************************************************************************/
#include <gctypes.h>

#ifndef GIFLIB_H
#define GIFLIB_H

extern "C" u8 * DecodeGIF(const u8 *src, u32 len, int *width, int *height, u8 *dst);

#endif
