/****************************************************************************
*	gx_supp.c - Generic GX Support for Emulators
*	softdev 2007
*	dhewg 2008
*	sepp256 2008-2011 - Coded YUV->RGB conversion and filters in TEV
*	Extrems 2009-2011
*	Tantric / rodries 2009-2012
*
*	This program is free software; you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation; either version 2 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License along
*	with this program; if not, write to the Free Software Foundation, Inc.,
*	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
* NGC GX Video Functions
*
* These are pretty standard functions to setup and use GX scaling.
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <unistd.h>
#include <ogc/mutex.h>
#include <ogc/lwp.h>
#include <ogc/lwp_watchdog.h>
#include <wiiuse/wpad.h>

#include "../libvo/video_out.h"
#include "../libvo/csputils.h"
#include "gx_supp.h"
#include "../../utils/mem2_manager.h"
#include "../../video.h"

#define HASPECT 320
#define VASPECT 240

static int drawMode = 0;

void StartDrawThread();
void PauseAndGotoGUI();
void TakeScreenshot();
int DrawMPlayerGui();
int copyScreen = 0;
extern int pause_gui;
extern int controlledbygui;

//Global variables for brightness and contrast filters
//Ranges for both should be constrained to [-1.0,1.0]
float g_brightness = 0.0f;
float g_contrast = 0.0f;

/*** 2D ***/
extern bool need_wait;
extern u8 whichfb;
extern unsigned int *xfb[2];

static int hor_pos=0, vert_pos=0;
static float hor_zoom = 1.0f, vert_zoom = 1.0f;
static int video_diffx, video_diffy, video_haspect, video_vaspect;
int mplayerwidth = 640;
int mplayerheight = 480;

int colorspace = MP_CSP_DEFAULT;
int levelconv = 1;

/*** 3D GX ***/

/*** Texture memory ***/
static u8 *Yltexture = NULL;
static u8 *Yrtexture =  NULL;
static u8 *Utexture = NULL;
static u8 *Vtexture = NULL;

static u32 Yltexsize,Yrtexsize,UVtexsize;

static GXTexObj YltexObj,YrtexObj,UtexObj,VtexObj;
static u16 Ylwidth, Yrwidth, Ywidth, Yheight, UVwidth, UVheight;

static Mtx view;

typedef struct tagcamera {
	guVector pos;
	guVector up;
	guVector view;
} camera;

static camera cam = {
	{ 0.0f, 0.0f, 352.0f },
	{ 0.0f, 0.5f, 0.0f },
	{ 0.0f, 0.0f, -0.5f }
};

static s16 square[] ATTRIBUTE_ALIGN(32) = {
	-HASPECT,  VASPECT,  0,
	 HASPECT,  VASPECT,  0,
	 HASPECT, -VASPECT,  0,
	-HASPECT, -VASPECT,  0
};

static GXColor colors[] ATTRIBUTE_ALIGN(32) = {
	{0,255,0,255}		//G
};

static f32 texcoordsY[] ATTRIBUTE_ALIGN(32) = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f
};

static f32 texcoordsUV[] ATTRIBUTE_ALIGN(32) = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f
};

static void GX_UpdateScaling()
{
	int	xscale = video_haspect * hor_zoom;
	int	yscale = video_vaspect * vert_zoom;

	square[0] = square[9] = -xscale + video_diffx + hor_pos;
	square[3] = square[6] = xscale + video_diffx + hor_pos;
	square[1] = square[4] = yscale - video_diffy - vert_pos;
	square[7] = square[10] = -yscale - video_diffy - vert_pos;

	DCFlushRange (square, 32); // update memory BEFORE the GPU accesses it!
	GX_SetArray(GX_VA_POS, square, 3 * sizeof(s16));
	GX_InvVtxCache();
}

void GX_SetScreenPos(int _hor_pos, int _vert_pos, float _hor_zoom, float _vert_zoom)
{
	hor_pos = _hor_pos;
	vert_pos = _vert_pos;
	hor_zoom = _hor_zoom;
	vert_zoom = _vert_zoom;
	GX_UpdateScaling();
}

/****************************************************************************
 * draw_initYUV - Internal function to setup TEV for YUV->RGB conversion.
 ****************************************************************************/
static void draw_initYUV()
{
	//Setup TEV
	GX_SetNumChans(1);
	GX_SetNumTexGens(4);
	GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
	GX_SetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY);
	GX_SetTexCoordGen(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX2, GX_IDENTITY);
	
	//Yl: GX_TEXMAP0, GX_TEXCOORD0 <- left half of Y buffer
	//Yr: GX_TEXMAP1, GX_TEXCOORD1 <- right half of Y buffer
	//U:  GX_TEXMAP2, GX_TEXCOORD2 (was MAP1 COORD1)
	//V:  GX_TEXMAP3, GX_TEXCOORD2

	//Y'UV->RGB formulation 3
	GX_SetNumTevStages(15);
	GX_SetTevKColor(GX_KCOLOR0, (GXColor){255,	 0,   0, levelconv ? 18 : 0});	//R {1, 0, 0, 16*1.164}
	GX_SetTevKColor(GX_KCOLOR1, (GXColor){	0,	 0, 255, levelconv ? 41 : 0});	//B {0, 0, 1, 0.164}
	
	static const GXColor uv_coeffs[MP_CSP_COUNT][2] = {
		[MP_CSP_DEFAULT] = {
			{203, 103,	 0, 255},	// {1.596/2, 0.813/2, 0}
			{  0,  24, 128, 255}	// {0, 0.391/4, 2.018/4}
		},
		[MP_CSP_BT_601] = {
			{179,  90,	 0, 255},	// {1.403/2, 0.714/2, 0}
			{  0,  21, 112, 255}	// {0, 0.344/4, 1.773/4}
		},
		[MP_CSP_BT_709] = {
			{200,  59,	 0, 255},	// {1.5701/2, 0.4664/2, 0}
			{  0,  11, 118, 255}	// {0, 0.187/4, 1.8556/4}
		},
		[MP_CSP_SMPTE_240M] = {
			{201,  63,	 0, 255},	// {1.5756/2, 0.5/2, 0}
			{  0,  13, 116, 255}	// {0, 0.2253/4, 1.827/4}
		},
		[MP_CSP_EBU] = {
			{145,  73,	 0, 255},	// {1.140/2, 0.581/2, 0}
			{  0,  24, 129, 255}	// {0, 0.396/4, 2.029/4}
		},
	};
	
	//Brightness/Contrast Filter Setup:
	// color_out = clamp[(Image - 128)*C + 128 + B] = clamp[(Image*C) - 128*C + 128 + B]
	// 2 constant values will be stored in the alpha components of GX_KCOLOR2 & GX_KCOLOR3
	g_brightness = (g_brightness>1.0f) ? 1.0f : (g_brightness<-1.0f) ? -1.0f : g_brightness;
	g_contrast = (g_contrast>1.0f) ? 1.0f : (g_contrast<-1.0f) ? -1.0f : g_contrast;
	int brightness_offset;
	u8 contrast_konst, contrast_InD, brightness_konst, brightness_TB, brightness_OP;
	if (g_contrast <= 0.0f) {
		contrast_konst = (u8)((1 + (g_contrast * 0.5))*255);
		brightness_offset = -((int)contrast_konst)/2 + 128 + ((int)(g_brightness*255));
		contrast_InD = GX_CC_ZERO;
	}
	else {
		contrast_konst = (u8)(g_contrast * 255);
		brightness_offset = -((int)contrast_konst)/2 + ((int)(g_brightness*255));
		contrast_InD = GX_CC_CPREV;
	}

	if (brightness_offset > 255) {
		brightness_konst = (u8)(brightness_offset - 128);
		brightness_TB = GX_TB_ADDHALF;
		brightness_OP = GX_TEV_ADD;
	}
	else if (brightness_offset >= 0) {
		brightness_konst = (u8)(brightness_offset);
		brightness_TB = GX_TB_ZERO;
		brightness_OP = GX_TEV_ADD;
	}
	else if (brightness_offset >= -255) {
		brightness_konst = (u8)(-brightness_offset);
		brightness_TB = GX_TB_ZERO;
		brightness_OP = GX_TEV_SUB;
	}
	else {
		brightness_konst = (u8)(-brightness_offset - 128);
		brightness_TB = GX_TB_SUBHALF;
		brightness_OP = GX_TEV_SUB;
	}

	GXColor KColor2 = (GXColor) {uv_coeffs[colorspace][0].r, uv_coeffs[colorspace][0].g,
		uv_coeffs[colorspace][0].b, contrast_konst};
	GXColor KColor3 = (GXColor) {uv_coeffs[colorspace][1].r, uv_coeffs[colorspace][1].g,
		uv_coeffs[colorspace][1].b, brightness_konst};

	GX_SetTevKColor(GX_KCOLOR2, KColor2);
	GX_SetTevKColor(GX_KCOLOR3, KColor3);

	//Stage 0: TEVREG0 <- { 0, 2Um, 2Up }; TEVREG0A <- {16*1.164}
	GX_SetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K1);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR0A0);
	GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_RASC, GX_CC_KONST, GX_CC_TEXC, GX_CC_ZERO);
	GX_SetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_SUBHALF, GX_CS_SCALE_2, GX_ENABLE, GX_TEVREG0);
	GX_SetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
	GX_SetTevAlphaIn (GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_KONST, GX_CA_ZERO);
	GX_SetTevAlphaOp (GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVREG0);
	//Stage 1: TEVREG1 <- { 0, 2Up, 2Um };
	GX_SetTevKColorSel(GX_TEVSTAGE1, GX_TEV_KCSEL_K1);
	GX_SetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR0A0);
	GX_SetTevColorIn(GX_TEVSTAGE1, GX_CC_KONST, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
	GX_SetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_SUBHALF, GX_CS_SCALE_2, GX_ENABLE, GX_TEVREG1);
	GX_SetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
	GX_SetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	//Stage 2: TEVREG2 <- { Vp, Vm, 0 }
	GX_SetTevKColorSel(GX_TEVSTAGE2, GX_TEV_KCSEL_K0);
	GX_SetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD2, GX_TEXMAP3, GX_COLOR0A0);
	GX_SetTevColorIn(GX_TEVSTAGE2, GX_CC_RASC, GX_CC_KONST, GX_CC_TEXC, GX_CC_ZERO);
	GX_SetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_SUBHALF, GX_CS_SCALE_1, GX_ENABLE, GX_TEVREG2);
	GX_SetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
	GX_SetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	//Stage 3: TEVPREV <- { (Vm), (Vp), 0 }
	GX_SetTevKColorSel(GX_TEVSTAGE3, GX_TEV_KCSEL_K0);
	GX_SetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD2, GX_TEXMAP3, GX_COLOR0A0);
	GX_SetTevColorIn(GX_TEVSTAGE3, GX_CC_KONST, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
	GX_SetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_SUBHALF, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	GX_SetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
	GX_SetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	//Stage 4: TEVPREV <- { (-1.598Vm), (-0.813Vp), 0 }; TEVPREVA <- {Yl'}
	GX_SetTevKColorSel(GX_TEVSTAGE4, GX_TEV_KCSEL_K2);
	GX_SetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD0, GX_TEXMAP0, GX_COLORNULL);
	GX_SetTevColorIn(GX_TEVSTAGE4, GX_CC_ZERO, GX_CC_KONST, GX_CC_CPREV, GX_CC_ZERO);
	GX_SetTevColorOp(GX_TEVSTAGE4, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_2, GX_DISABLE, GX_TEVPREV);
	GX_SetTevAlphaIn(GX_TEVSTAGE4, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
	GX_SetTevAlphaOp(GX_TEVSTAGE4, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVPREV);
	//Stage 5: TEVPREV <- { -1.598Vm (+1.139/2Vp), -0.813Vp +0.813/2Vm), 0 }; TEVREG1A <- {Yl' + Yr'} = {Y'}
	GX_SetTevKColorSel(GX_TEVSTAGE5, GX_TEV_KCSEL_K2);
	GX_SetTevOrder(GX_TEVSTAGE5, GX_TEXCOORD1, GX_TEXMAP1, GX_COLORNULL);
	GX_SetTevColorIn(GX_TEVSTAGE5, GX_CC_ZERO, GX_CC_KONST, GX_CC_C2, GX_CC_CPREV);
	GX_SetTevColorOp(GX_TEVSTAGE5, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVPREV);
	GX_SetTevAlphaIn(GX_TEVSTAGE5, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
	GX_SetTevAlphaOp(GX_TEVSTAGE5, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVREG1);
	//Stage 6: TEVPREV <- {	-1.598Vm (+1.598Vp), -0.813Vp (+0.813Vm), 0 } = {	(+1.598V), (-0.813V), 0 }; TEVPREVA <- {Y' - 16*1.164}
	GX_SetTevKColorSel(GX_TEVSTAGE6, GX_TEV_KCSEL_K2);
	GX_SetTevOrder(GX_TEVSTAGE6, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLORNULL);
	GX_SetTevColorIn(GX_TEVSTAGE6, GX_CC_ZERO, GX_CC_KONST, GX_CC_C2, GX_CC_CPREV);
	GX_SetTevColorOp(GX_TEVSTAGE6, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVPREV);
	GX_SetTevKAlphaSel(GX_TEVSTAGE6, GX_TEV_KASEL_1);
	GX_SetTevAlphaIn(GX_TEVSTAGE6, GX_CA_ZERO, GX_CA_KONST, GX_CA_A0, GX_CA_A1);
	GX_SetTevAlphaOp(GX_TEVSTAGE6, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVPREV);
	//Stage 7: TEVPREV <- {	-1.598Vm (+1.598Vp), -0.813Vp (+0.813Vm), 0 } = {	(+1.598V), (-0.813V), 0 }; TEVREG2A <- {Y' -16*1.164 - Y'*0.164} = {(Y'-16)*1.164}
	GX_SetTevOrder(GX_TEVSTAGE7, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLORNULL);
	GX_SetTevColorIn(GX_TEVSTAGE7, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
	GX_SetTevColorOp(GX_TEVSTAGE7, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVPREV);
	GX_SetTevKAlphaSel(GX_TEVSTAGE7, GX_TEV_KASEL_K1_A);
	GX_SetTevAlphaIn(GX_TEVSTAGE7, GX_CA_ZERO, GX_CA_KONST, GX_CA_A1, GX_CA_APREV);
	GX_SetTevAlphaOp(GX_TEVSTAGE7, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVREG2);
	//Stage 8: TEVPREV <- {	((Y'-16)*1.164) +1.598V, ((Y'-16)*1.164) -0.813V, ((Y'-16)*1.164) }
	GX_SetTevKColorSel(GX_TEVSTAGE8, GX_TEV_KCSEL_1);
	GX_SetTevOrder(GX_TEVSTAGE8, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLORNULL);
	GX_SetTevColorIn(GX_TEVSTAGE8, GX_CC_ZERO, GX_CC_ONE, GX_CC_A2, GX_CC_CPREV);
	GX_SetTevColorOp(GX_TEVSTAGE8, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVPREV);
	GX_SetTevAlphaIn(GX_TEVSTAGE8, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
	GX_SetTevAlphaOp(GX_TEVSTAGE8, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	//Stage 9: TEVPREV <- {	(Y'-16)*1.164 +1.598V, (Y'-16)*1.164 -0.813V (-.394/2Up), (Y'-16)*1.164 (-2.032/2Um)}
	GX_SetTevKColorSel(GX_TEVSTAGE9, GX_TEV_KCSEL_K3);
	GX_SetTevOrder(GX_TEVSTAGE9, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLORNULL);
	GX_SetTevColorIn(GX_TEVSTAGE9, GX_CC_ZERO, GX_CC_KONST, GX_CC_C1, GX_CC_CPREV);
	GX_SetTevColorOp(GX_TEVSTAGE9, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVPREV);
	GX_SetTevAlphaIn(GX_TEVSTAGE9, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
	GX_SetTevAlphaOp(GX_TEVSTAGE9, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	//Stage 10: TEVPREV <- { (Y'-16)*1.164 +1.598V, (Y'-16)*1.164 -0.813V (-.394Up), (Y'-16)*1.164 (-2.032Um)}
	GX_SetTevKColorSel(GX_TEVSTAGE10, GX_TEV_KCSEL_K3);
	GX_SetTevOrder(GX_TEVSTAGE10, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLORNULL);
	GX_SetTevColorIn(GX_TEVSTAGE10, GX_CC_ZERO, GX_CC_KONST, GX_CC_C1, GX_CC_CPREV);
	GX_SetTevColorOp(GX_TEVSTAGE10, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVPREV);
	GX_SetTevAlphaIn(GX_TEVSTAGE10, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
	GX_SetTevAlphaOp(GX_TEVSTAGE10, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	//Stage 11: TEVPREV <- { (Y'-16)*1.164 +1.598V, (Y'-16)*1.164 -0.813V -.394Up (+.394/2Um), (Y'-16)*1.164 -2.032Um (+2.032/2Up)}
	GX_SetTevKColorSel(GX_TEVSTAGE11, GX_TEV_KCSEL_K3);
	GX_SetTevOrder(GX_TEVSTAGE11, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLORNULL);
	GX_SetTevColorIn(GX_TEVSTAGE11, GX_CC_ZERO, GX_CC_KONST, GX_CC_C0, GX_CC_CPREV);
	GX_SetTevColorOp(GX_TEVSTAGE11, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVPREV);
	GX_SetTevAlphaIn(GX_TEVSTAGE11, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
	GX_SetTevAlphaOp(GX_TEVSTAGE11, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	//Stage 12: TEVPREV <- { (Y'-16)*1.164 +1.598V, (Y'-16)*1.164 -0.813V -.394Up (+.394Um), (Y'-16)*1.164 -2.032Um (+2.032Up)} = { (Y'-16)*1.164 +1.139V, (Y'-16)*1.164 -0.58V -.394U, (Y'-16)*1.164 +2.032U}
	//			TEVPREVA <- contrast_konst
	GX_SetTevKColorSel(GX_TEVSTAGE12, GX_TEV_KCSEL_K3);
	GX_SetTevOrder(GX_TEVSTAGE12, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLORNULL);
	GX_SetTevColorIn(GX_TEVSTAGE12, GX_CC_ZERO, GX_CC_KONST, GX_CC_C0, GX_CC_CPREV);
	GX_SetTevColorOp(GX_TEVSTAGE12, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	GX_SetTevKAlphaSel(GX_TEVSTAGE12, GX_TEV_KASEL_K2_A);
	GX_SetTevAlphaIn(GX_TEVSTAGE12, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
	GX_SetTevAlphaOp(GX_TEVSTAGE12, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	//Stage 13: TEVPREV <- Image * contrast_konst; TEVPREVA <- brightness_konst
	GX_SetTevOrder(GX_TEVSTAGE13, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLORNULL);
	GX_SetTevColorIn(GX_TEVSTAGE13, GX_CC_ZERO, GX_CC_CPREV, GX_CC_APREV, contrast_InD);
	GX_SetTevColorOp(GX_TEVSTAGE13, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVPREV);
	GX_SetTevKAlphaSel(GX_TEVSTAGE13, GX_TEV_KASEL_K3_A);
	GX_SetTevAlphaIn(GX_TEVSTAGE13, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
	GX_SetTevAlphaOp(GX_TEVSTAGE13, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	//Stage 14: TEVPREV <- Image * contrast_konst + brightness_konst
	GX_SetTevOrder(GX_TEVSTAGE14, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLORNULL);
	GX_SetTevColorIn(GX_TEVSTAGE14, GX_CC_APREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
	GX_SetTevColorOp(GX_TEVSTAGE14, brightness_OP, brightness_TB, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
	GX_SetTevKAlphaSel(GX_TEVSTAGE14, GX_TEV_KCSEL_1);
	GX_SetTevAlphaIn(GX_TEVSTAGE14, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
	GX_SetTevAlphaOp(GX_TEVSTAGE14, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);

	//Setup vertex description/format
	GX_ClearVtxDesc();
	GX_SetVtxDesc(GX_VA_POS, GX_INDEX8);
	GX_SetVtxDesc(GX_VA_CLR0, GX_INDEX8);
	GX_SetVtxDesc(GX_VA_TEX0, GX_INDEX8);
	GX_SetVtxDesc(GX_VA_TEX1, GX_INDEX8);
	GX_SetVtxDesc(GX_VA_TEX2, GX_INDEX8);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX2, GX_TEX_ST, GX_F32, 0);

	GX_SetArray(GX_VA_POS, square, 3 * sizeof(s16));
	GX_SetArray(GX_VA_CLR0, colors, sizeof(GXColor));
	GX_SetArray(GX_VA_TEX0, texcoordsY, 2 * sizeof(f32));
	GX_SetArray(GX_VA_TEX1, texcoordsY, 2 * sizeof(f32));
	GX_SetArray(GX_VA_TEX2, texcoordsUV, 2 * sizeof(f32));

	//init YUV texture objects
	GX_InitTexObj(&YltexObj, Yltexture, (u16) Ylwidth, (u16) Yheight, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
	GX_InitTexObjLOD(&YltexObj, GX_LINEAR, GX_LINEAR, 0.0, 0.0, 0.0, GX_TRUE, GX_TRUE, GX_ANISO_4);
	GX_InitTexObj(&YrtexObj, Yrtexture, (u16) Yrwidth, (u16) Yheight, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
	GX_InitTexObjLOD(&YrtexObj, GX_LINEAR, GX_LINEAR, 0.0, 0.0, 0.0, GX_TRUE, GX_TRUE, GX_ANISO_4);
	GX_InitTexObj(&UtexObj, Utexture, (u16) UVwidth, (u16) UVheight, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
	GX_InitTexObjLOD(&UtexObj, GX_LINEAR, GX_LINEAR, 0.0, 0.0, 0.0, GX_TRUE, GX_TRUE, GX_ANISO_4);
	GX_InitTexObj(&VtexObj, Vtexture, (u16) UVwidth, (u16) UVheight, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
	GX_InitTexObjLOD(&VtexObj, GX_LINEAR, GX_LINEAR, 0.0, 0.0, 0.0, GX_TRUE, GX_TRUE, GX_ANISO_4);

	GX_LoadTexObj(&YltexObj, GX_TEXMAP0);	// MAP0 <- Yl
	GX_LoadTexObj(&YrtexObj, GX_TEXMAP1);	// MAP1 <- Yr
	GX_LoadTexObj(&UtexObj, GX_TEXMAP2);	// MAP2 <- U
	GX_LoadTexObj(&VtexObj, GX_TEXMAP3);	// MAP3 <- V

}

//------- rodries change: to avoid image_buffer intermediate ------
static int w1,w2,h1,h2,wl,wr,st0,st1;
static int p01,p02,p03,p11,p12,p13;
static u16 Yrowpitch,UVrowpitch;
static u64 *Yldst, *Yrdst, *Udst, *Vdst;

static void draw_scaling()
{
	Mtx m, mv;
	memset(&view, 0, sizeof(Mtx));
	guLookAt(view, &cam.pos, &cam.up, &cam.view);
	guMtxIdentity(m);
	guMtxTransApply(m, m, 0, 0, -100);
	guMtxConcat(view, m, mv);
	GX_LoadPosMtxImm(mv, GX_PNMTX0);
	GX_SetViewport(0, 0, vmode->fbWidth, vmode->efbHeight, 0, 1);
}

void GX_ConfigTextureYUV(u16 width, u16 height, u16 chroma_width, u16 chroma_height)
{
	int wp;

	Ywidth=(width+7)&~7;
	UVwidth = (chroma_width+7)&~7;

	st0=st1=0;
	
	w1 = Ywidth / 8;        
	w2 = UVwidth / 8;
            
	wl = w1 > 1024/8 ? 1024/8 - 1 : w1;
	wr = w1 > 1024/8 ? w1 - 1024/8 + 1 : 0;

	Ylwidth = Ywidth > 1016 ? 1024 : Ywidth;
	Yrwidth = Ywidth > 1024 ? Ywidth - 1016 + 8: 8;

	Yheight =  (height+3)&~3;
	UVheight = (chroma_height+3)&~3;

	f32 YtexcoordS = (double)width / (double)Ywidth;
	f32 UVtexcoordS = (double)chroma_width / (double)UVwidth;

	f32 YtexcoordT = (double)height / (double)Yheight;
	f32 UVtexcoordT = (double)chroma_height / (double)UVheight;

	if (Ywidth <= 1024)
	{
		texcoordsY[2] = texcoordsY[4] = YtexcoordS;
		texcoordsY[5] = texcoordsY[7] = YtexcoordT;
		texcoordsY[8] = texcoordsY[14] = 0.0f;
	}
	else
	{
		// not sure about this code
		texcoordsY[2] = texcoordsY[4] = (double)Ywidth / 1024.0f;
		texcoordsY[5] = texcoordsY[7] = YtexcoordT;
		texcoordsY[8] = texcoordsY[14] = (-1016.0f + 8.0f) / (double)Yrwidth;
	}
	texcoordsUV[2] = texcoordsUV[4] = UVtexcoordS;
	texcoordsUV[5] = texcoordsUV[7] = UVtexcoordT;

	DCFlushRange (texcoordsY, 16*sizeof(f32)); // update memory BEFORE the GPU accesses it!
	DCFlushRange (texcoordsUV, 8*sizeof(f32)); // update memory BEFORE the GPU accesses it!

	// Update scaling
	draw_initYUV();
	draw_scaling();
}
#include "osdep/timer.h"

inline void DrawMPlayer()
{
	DCFlushRange(Yltexture, Yltexsize);
	if (wr>0) DCFlushRange(Yrtexture, Yrtexsize);
	DCFlushRange(Utexture, UVtexsize);
	DCFlushRange(Vtexture, UVtexsize);

	if(need_wait)
		GX_WaitDrawDone();
		
	GX_InvVtxCache();
	GX_InvalidateTexAll();

	GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
		GX_Position1x8(0); GX_Color1x8(0); GX_TexCoord1x8(0); GX_TexCoord1x8(4); GX_TexCoord1x8(0);
		GX_Position1x8(1); GX_Color1x8(0); GX_TexCoord1x8(1); GX_TexCoord1x8(5); GX_TexCoord1x8(1);
		GX_Position1x8(2); GX_Color1x8(0); GX_TexCoord1x8(2); GX_TexCoord1x8(6); GX_TexCoord1x8(2);
		GX_Position1x8(3); GX_Color1x8(0); GX_TexCoord1x8(3); GX_TexCoord1x8(7); GX_TexCoord1x8(3);
	GX_End();

	if(copyScreen == 1)
	{
		if(controlledbygui != 2)
			TakeScreenshot();
		copyScreen = 2;
	}
	else
	{
		drawMode = DrawMPlayerGui();
	}
	
	if(copyScreen == 2)
	{
		copyScreen = 0;
		pause_gui = 1;
	}
	else if(drawMode != 0)
	{
		// reconfigure GX for MPlayer
		Mtx44 p;
		draw_initYUV();
		draw_scaling();
		guOrtho(p, mplayerheight/2, -(mplayerheight/2), -(mplayerwidth/2), mplayerwidth/2, 10, 1000);
		GX_LoadProjectionMtx (p, GX_ORTHOGRAPHIC);
		drawMode = 0;
	}
	
	whichfb ^= 1; // flip framebuffer

	GX_CopyDisp(xfb[whichfb], GX_TRUE);
	VIDEO_SetNextFramebuffer(xfb[whichfb]);
	GX_SetDrawDone();
	need_wait=true;
}

void GX_AllocTextureMemory()
{
	Yltexture = (u8*)mem2_memalign(32, 1024*MAX_HEIGHT, MEM2_VIDEO);
	Yrtexture = (u8*)mem2_memalign(32, (MAX_WIDTH-1024)*MAX_HEIGHT, MEM2_VIDEO);
	Utexture = (u8*)mem2_memalign(32, 1024*(MAX_HEIGHT/2), MEM2_VIDEO);
	Vtexture = (u8*)mem2_memalign(32, 1024*(MAX_HEIGHT/2), MEM2_VIDEO);        
}

/****************************************************************************
 * GX_StartYUV - Initialize GX for given width/height.
 ****************************************************************************/
void GX_StartYUV(u16 width, u16 height, u16 haspect, u16 vaspect)
{
	int w,wYl,wYr,h;
	Mtx44 p;

	need_wait=false;

	// Allocate 32byte aligned texture memory
	wYl = width < 1024 ? width : 1016;
	wYl += 8 - (wYl % 8);
	wYr = width <= 1024 ? 0 : width - 1024;
	wYr += 8 - (wYr % 8);
	w=(width+15)&~15;
	h=(height+7)&~7;

	Yltexsize = (wYl*h);
    Yrtexsize = (wYr*h);
	UVtexsize = (w*h)/4;

    memset(Yltexture, 0, 1024*MAX_HEIGHT);
    memset(Yrtexture, 0, (MAX_WIDTH-1024)*MAX_HEIGHT);
    memset(Utexture, 0x80, 1024*(MAX_HEIGHT/2));
    memset(Vtexture, 0x80, 1024*(MAX_HEIGHT/2));	
	

	// center, to correct difference between pitch and real width
	video_diffx = (w - width)/2.0;
	video_diffy = (h - height)/2.0;
	video_haspect = haspect;
	video_vaspect = vaspect;

	GX_UpdateScaling();

	GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
	GX_SetCullMode(GX_CULL_NONE);
	GX_SetClipMode(GX_DISABLE);
	GX_SetZMode(GX_FALSE, GX_ALWAYS, GX_TRUE);
	GX_CopyDisp(xfb[whichfb ^ 1], GX_TRUE);
	GX_SetDispCopyGamma(GX_GM_1_0);
	guOrtho(p, mplayerheight/2.0, -(mplayerheight/2.0), -(mplayerwidth/2.0), mplayerwidth/2.0, 10.0, 1000.0);
	GX_LoadProjectionMtx (p, GX_ORTHOGRAPHIC);

	GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
	GX_SetAlphaUpdate(GX_ENABLE);
	GX_SetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
	GX_SetColorUpdate(GX_ENABLE);

	GX_Flush();
}

#define LUMA_COPY(type) \
{ \
	type *Yldst = (type *)Yltexture - 1; \
	type *Yrdst = (type *)Yrtexture - 1; \
	 \
	type *Ysrc1 = (type *) (buffer[0]) - 1; \
	type *Ysrc2 = (type *)((buffer[0]) + stride[0]) - 1; \
	type *Ysrc3 = (type *)((buffer[0]) + (stride[0] * 2)) - 1; \
	type *Ysrc4 = (type *)((buffer[0]) + (stride[0] * 3)) - 1; \
	 \
	Yrdst += 4; \
	int rows = Yheight / 4; \
	int tiles; \
	while (rows--) { \
		tiles = wl; \
		 \
		while (tiles--) { \		
			__asm__ volatile("dcbz 0,%0" : : "b" (++Yldst)); \ 
			*Yldst = *++Ysrc1; \
			*++Yldst = *++Ysrc2; \
			*++Yldst = *++Ysrc3; \
			*++Yldst = *++Ysrc4; \
		} \	
		if (wr>0){ \
			tiles = wr; \
			 \
			while (tiles--) { \
				__asm__ volatile("dcbz 0,%0" : : "b" (++Yrdst)); \
				*Yrdst = *++Ysrc1; \
				*++Yrdst = *++Ysrc2; \
				*++Yrdst = *++Ysrc3; \
				*++Yrdst = *++Ysrc4; \
			} \
			Yldst += 4; \
			Yrdst += 4; \
		}\
		 \
		Ysrc1 = (type *)((u32)Ysrc1 + Yrowpitch); \
		Ysrc2 = (type *)((u32)Ysrc2 + Yrowpitch); \
		Ysrc3 = (type *)((u32)Ysrc3 + Yrowpitch); \
		Ysrc4 = (type *)((u32)Ysrc4 + Yrowpitch); \
	} \
}

#define CHROMA_COPY(type) \
{ \
	type *Udst = (type *)Utexture - 1; \
	type *Vdst = (type *)Vtexture - 1; \
	 \
	type *Usrc1 = (type *) (buffer[1]) - 1; \
	type *Usrc2 = (type *)((buffer[1]) + stride[1]) - 1; \
	type *Usrc3 = (type *)((buffer[1]) + (stride[1] * 2)) - 1; \
	type *Usrc4 = (type *)((buffer[1]) + (stride[1] * 3)) - 1; \
	 \
	type *Vsrc1 = (type *) (buffer[2]) - 1; \
	type *Vsrc2 = (type *)((buffer[2]) + stride[2]) - 1; \
	type *Vsrc3 = (type *)((buffer[2]) + (stride[2] * 2)) - 1; \
	type *Vsrc4 = (type *)((buffer[2]) + (stride[2] * 3)) - 1; \
	 \
	int rows = UVheight / 4; \
	int tiles, ntiles = UVwidth / 8; \
		\
	while (rows--) { \
		tiles = ntiles; \
		while (tiles--) { \
			__asm__ volatile("dcbz 0,%0" : : "b" (++Udst) ); \
			*Udst = *++Usrc1; \
			*++Udst = *++Usrc2; \
			*++Udst = *++Usrc3; \
			*++Udst = *++Usrc4; \
			 \
			__asm__ volatile("dcbz 0,%0" : : "b" (++Vdst) ); \ 
			*Vdst = *++Vsrc1; \
			*++Vdst = *++Vsrc2; \
			*++Vdst = *++Vsrc3; \
			*++Vdst = *++Vsrc4; \
		} \
		 \
		Usrc1 = (type *)((u32)Usrc1 + UVrowpitch); \
		Usrc2 = (type *)((u32)Usrc2 + UVrowpitch); \
		Usrc3 = (type *)((u32)Usrc3 + UVrowpitch); \
		Usrc4 = (type *)((u32)Usrc4 + UVrowpitch); \
		 \
		Vsrc1 = (type *)((u32)Vsrc1 + UVrowpitch); \
		Vsrc2 = (type *)((u32)Vsrc2 + UVrowpitch); \
		Vsrc3 = (type *)((u32)Vsrc3 + UVrowpitch); \
		Vsrc4 = (type *)((u32)Vsrc4 + UVrowpitch); \
	} \
}


void GX_FillTextureYUV(u8 *buffer[3], int stride[3])
{

	if(st0!=stride[0] || st1!=stride[1])
	{
		st0=stride[0];
		st1=stride[1];
		Yrowpitch = (stride[0] * 4) - Ywidth;
		UVrowpitch = (stride[1] * 4) - UVwidth;
	}

	if(need_wait)
		GX_WaitDrawDone();

	if (stride[0] & 7)
		LUMA_COPY(u64)
	else
		LUMA_COPY(double)

	if (stride[1] & 7)
		CHROMA_COPY(u64)
	else
		CHROMA_COPY(double) 
}

void GX_RenderTexture()
{
	DrawMPlayer();  
}

void vo_draw_alpha_gekko(int x0, int y0, int w, int h, unsigned char *src, unsigned char *srca, int stride)
{
	s16 pitch = stride - w;
	u8 *Ydst;

	int dxs,x;
	int dys = y0;
	int w1 = w < 1024 ? w : 1024;
	u8 *yt;

	h = h < 1024 ? h : 1024;

	for (int y = 0; y < h; y++) 
	{
		yt = Yltexture + ((dys & (~3)) * Ylwidth) + ((dys & 3) << 3);
		dxs = x0;
		for (x = 0; x < w1; x++) 
		{
			if (*srca) 
			{
				Ydst = yt + ((dxs & (~7)) << 2)  + (dxs & 7);
				*Ydst = (((*Ydst) * (*srca)) >> 8) + (*src);
			}
			dxs++;
			src++; srca++;
		}
		if(w>1024)
		{
			yt = Yrtexture + ((dys & (~3)) * Yrwidth) + ((dys & 3) << 3);
			for (; x < w; x++) 
			{
				if (*srca) 
				{
					Ydst = yt + ((dxs & (~7)) << 2) + (dxs & 7);
					*Ydst = (((*Ydst) * (*srca)) >> 8) + (*src);
				}
				dxs++;
				src++; srca++;
			}
		}
		dys++;
		src += pitch;
		srca += pitch;
	}
}
