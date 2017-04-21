/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * jmemsrc.h
 * Memory based JPEG decoding to 4x4RGBA texture
 ***************************************************************************/

#ifndef _JMEMSRC_H_
#define _JMEMSRC_H_

#include <gctypes.h>

u8 * DecodeJPEG(const u8 *src, u32 len, int *width, int *height, u8 *dst);

#endif
