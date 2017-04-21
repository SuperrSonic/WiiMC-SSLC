/****************************************************************************
*   gx_supp.h - Generic GX Scaler 
*   softdev 2007
*   dhewg 2008
*   sepp256 2008 - Coded YUV->RGB conversion in TEV.
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License along
*   with this program; if not, write to the Free Software Foundation, Inc.,
*   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
* NGC GX Video Functions
*
* These are pretty standard functions to setup and use GX scaling.
****************************************************************************/
#ifndef _WII_GX_SUPP_H_
#define _WII_GX_SUPP_H_

#include <gccore.h>

#ifdef __cplusplus
extern "C" {
#endif

extern GXRModeObj *vmode;

void GX_InitVideo();
void GX_SetScreenPos(int xshift, int yshift, float xzoom, float yzoom);
void DrawMPlayer();
void GX_StartYUV(u16 width, u16 height, u16 haspect, u16 vaspect);
void GX_RenderTexture();
void GX_AllocTextureMemory();
void GX_FillTextureYUV(u8 *buffer[3], int stride[3]);
void GX_ConfigTextureYUV(u16 width, u16 height, u16 chroma_width, u16 chroma_height);
void vo_draw_alpha_gekko(int x0, int y0, int w, int h, unsigned char *src, unsigned char *srca, int stride);

#ifdef __cplusplus
}
#endif

#endif
