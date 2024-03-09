/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * video.cpp
 * Video routines
 ***************************************************************************/

#include <gccore.h>
#include <ogcsys.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiiuse/wpad.h>

#include "input.h"
#include "libwiigui/gui.h"
#include "menu.h"
#include "wiimc.h"
#include "utils/mem2_manager.h"
#include "settings.h"

#include "utils/vi_encoder.h"

extern bool AutobootExit; //if autobooting a video, ignore delay.

extern "C" {

#define DEFAULT_FIFO_SIZE 384 * 1024
static unsigned char *gp_fifo; // must be in MEM1
static Mtx GXmodelView2D;

unsigned int *xfb[2] = { NULL, NULL }; // Double buffered
//unsigned int *xfb[3] = { NULL, NULL, NULL };
u8 whichfb = 0; // Switch
bool need_wait=false;
bool flip_pending=false;

GXRModeObj *vmode; // Menu video mode
u8 * videoScreenshot = NULL;
int screenheight = 480;
int screenwidth = 640;
u32 FrameTimer = 0;
bool drawGui = false;
bool pal = false;
bool pal60 = false;
static bool want576i = false;
u8 night = 22;
bool fade_boot = false;
bool fade_not = false;

bool delayrender = true; //fixes bottom screen garbage when loaded by channel.
//bool menu_black = false;
bool tiledBlack = false;
unsigned timerBlack = 12;
extern bool wiiTiledRender;
extern bool ssIsActive;
extern int sync_interlace;

/****************************************************************************
 * TakeScreenshot
 *
 * Copies the current screen into a GX texture
 ***************************************************************************/

void TakeScreenshot()
{
	GX_SetTexCopySrc(0, 0, vmode->fbWidth, vmode->efbHeight);
	GX_SetTexCopyDst(vmode->fbWidth, vmode->efbHeight, GX_TF_RGBA8, GX_FALSE);
	DCInvalidateRange(videoScreenshot, vmode->fbWidth * vmode->efbHeight * 4);
	GX_CopyTex(videoScreenshot, GX_FALSE);
	GX_PixModeSync();
	EnableVideoImg();
}

void ResetVideo_Menu()
{
	Mtx44 p;

	GX_SetNumChans(1);
	GX_SetNumTevStages(1);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);

	GX_SetNumTexGens(1);
	GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GX_ClearVtxDesc();
	GX_InvVtxCache ();
	GX_InvalidateTexAll();

	GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
	GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
	GX_SetVtxDesc (GX_VA_CLR0, GX_DIRECT);

	GX_SetVtxAttrFmt (GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GX_SetVtxAttrFmt (GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GX_SetZMode (GX_FALSE, GX_LEQUAL, GX_TRUE);

	guMtxIdentity(GXmodelView2D);
	guMtxTransApply (GXmodelView2D, GXmodelView2D, 0.0F, 0.0F, -50.0F);
	GX_LoadPosMtxImm(GXmodelView2D,GX_PNMTX0);

	guOrtho(p,0,screenheight,0,screenwidth,0,300);
	GX_LoadProjectionMtx(p, GX_ORTHOGRAPHIC);
}

/****************************************************************************
 * StopGX
 *
 * Stops GX (when exiting)
 ***************************************************************************/
void StopGX()
{
	GX_AbortFrame();
	GX_Flush();

	VIDEO_SetBlack(TRUE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	VIDEO_WaitVSync();
}

void nightfade_cb()
{
	fade_boot = true;
	fade_not = false;
}

void nofade_cb()
{
	fade_not = true;
	fade_boot = false;
}

void fadein_copyfilter()
{
	night++;
	if (night > 21 && fade_not) {
    	night = 22;
    	fade_not = false;
	}

	u8 sharp[7] = {0, 0, 21, night, 21, 0, 0};
	u8* vfilter = sharp;

	GX_SetCopyFilter(vmode->aa,vmode->sample_pattern,GX_TRUE,vfilter);

	GX_Flush();

	VIDEO_Configure(vmode);
	VIDEO_Flush();
}

void fadeout_copyfilter()
{
	night--;
	if (night < 1 && fade_boot) {
		night = 0;
		fade_boot = false;
	}

	u8 sharp[7] = {0, 0, 21, night, 21, 0, 0};
	u8* vfilter = sharp;

	GX_SetCopyFilter(vmode->aa,vmode->sample_pattern,GX_TRUE,vfilter);
	GX_Flush();

	VIDEO_Configure(vmode);
	VIDEO_Flush();
}

/****************************************************************************
 * Menu_Render
 *
 * Renders everything current sent to GX, and flushes video
 ***************************************************************************/
void Menu_Render()
{
	//This fixes some noticeable garbage flicker when a video ends.
	if(tiledBlack) {
		VIDEO_SetBlack(TRUE);
		--timerBlack;
		if(!AutobootExit && timerBlack == 0)
			VIDEO_SetBlack(FALSE);
		if(timerBlack == 0)
			tiledBlack = false;
	}
	
	//whichfb ^= 1; // flip framebuffer
	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GX_SetColorUpdate(GX_TRUE);
	GX_CopyDisp(xfb[whichfb],GX_TRUE);
	//VIDEO_SetNextFramebuffer(xfb[whichfb]);
	GX_DrawDone();
	if (flip_pending) {
		VIDEO_SetNextFramebuffer(xfb[whichfb]);
		VIDEO_Flush();
		whichfb ^= 1;
		/*++whichfb;
		if (whichfb > 2)
			whichfb = 0;*/
		flip_pending = false;
		VIDEO_WaitVSync();
	}

	if (fade_boot)
		fadeout_copyfilter();
	else if (fade_not)
		fadein_copyfilter();
	++FrameTimer;
	
	if(delayrender && FrameTimer > 4) {
		if(!AutobootExit)
			VIDEO_SetBlack(FALSE);
		delayrender = false;
	}
}

/****************************************************************************
 * Menu_DrawImg
 *
 * Draws the specified image on screen using GX
 ***************************************************************************/
void Menu_DrawImg(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[],
	f32 degrees, f32 scaleX, f32 scaleY, u8 alpha, u8 format)
{
	if(data == NULL)
		return;

	// if width/height are not multiples of 4, assume that we correctly 
	// padded the texture and also pad the dimensions
	if(width%4) width += (4-width%4);
	if(height%4) height += (4-height%4);

	GXTexObj texObj;

	//u8 mipmap = GX_FALSE;
	//if(width == 600 && height == 600)
	//	mipmap = GX_TRUE;
	
	GX_InitTexObj(&texObj, data, width, height, format, GX_CLAMP, GX_CLAMP, GX_FALSE);
	//GX_InitTexObjFilterMode(&texObj, GX_NEAR, GX_NEAR);
	GX_LoadTexObj(&texObj, GX_TEXMAP0);
	GX_InvalidateTexAll();

	GX_SetTevOp (GX_TEVSTAGE0, GX_MODULATE);
	GX_SetVtxDesc (GX_VA_TEX0, GX_DIRECT);

	Mtx m,m1,m2, mv;
	width  >>= 1;
	height >>= 1;

	guMtxIdentity (m1);
	guMtxScaleApply(m1,m1,scaleX,scaleY,1.0);
	guVector axis = (guVector) {0 , 0, 1 };
	guMtxRotAxisDeg (m2, &axis, degrees);
	guMtxConcat(m2,m1,m);

	guMtxTransApply(m,m, xpos+width,ypos+height,0);
	guMtxConcat (GXmodelView2D, m, mv);
	GX_LoadPosMtxImm (mv, GX_PNMTX0);

	GX_Begin(GX_QUADS, GX_VTXFMT0,4);
	GX_Position3f32(-width, -height,  0);
	GX_Color4u8(0xFF,0xFF,0xFF,alpha);
	GX_TexCoord2f32(0, 0);

	GX_Position3f32(width, -height,  0);
	GX_Color4u8(0xFF,0xFF,0xFF,alpha);
	GX_TexCoord2f32(1, 0);

	GX_Position3f32(width, height,  0);
	GX_Color4u8(0xFF,0xFF,0xFF,alpha);
	GX_TexCoord2f32(1, 1);

	GX_Position3f32(-width, height,  0);
	GX_Color4u8(0xFF,0xFF,0xFF,alpha);
	GX_TexCoord2f32(0, 1);
	GX_End();
	
	GX_LoadPosMtxImm (GXmodelView2D, GX_PNMTX0);

	GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
	GX_SetVtxDesc (GX_VA_TEX0, GX_NONE);
}

/****************************************************************************
 * Menu_DrawRectangle
 *
 * Draws a rectangle at the specified coordinates using GX
 ***************************************************************************/
void Menu_DrawRectangle(f32 x, f32 y, f32 width, f32 height, GXColor color, u8 filled)
{
	long n = 4;
	f32 x2 = x+width;
	f32 y2 = y+height;
	guVector v[] = {{x,y,0.0f}, {x2,y,0.0f}, {x2,y2,0.0f}, {x,y2,0.0f}, {x,y,0.0f}};
	u8 fmt = GX_TRIANGLEFAN;

	if(!filled)
	{
		fmt = GX_LINESTRIP;
		n = 5;
	}

	GX_Begin(fmt, GX_VTXFMT0, n);
	for(long i=0; i<n; ++i)
	{
		GX_Position3f32(v[i].x, v[i].y,  v[i].z);
		GX_Color4u8(color.r, color.g, color.b, color.a);
	}
	GX_End();
}

bool safe_gc;
extern bool point_on;

int DrawMPlayerGui()
{
	UpdatePads();
	if(safe_gc) { // GC controller reads 1 input as 2 inputs when using tiled rendering
		MPlayerInput();
		return 0;
	} else if (!wiiTiledRender)
		MPlayerInput();
	else if(point_on)
		MPlayerInput();

	if(!drawGui && wiiIsPaused())
		return 1;

	if(!drawGui)
		return 0;

	ResetVideo_Menu(); // reconfigure GX for GUI
	DoMPlayerGuiDraw(); // draw GUI
	return 1;
}

/****************************************************************************
 * InitVideo
 *
 * This function MUST be called at startup.
 * - also sets up menu video mode
 ***************************************************************************/
void Draw_VIDEO()
{
	need_wait=false;
	//VIDEO_Flush();
	flip_pending = true;
}

static void vblank_cb(u32 retraceCnt)
{
	if (flip_pending) {
		if(sync_interlace == 1 && vmode->fbWidth > 640) {
			if(!VIDEO_GetNextField()) {
				VIDEO_SetNextFramebuffer(xfb[whichfb]);
				VIDEO_Flush();
				whichfb ^= 1;
				/*++whichfb;
				if(whichfb > 2)
					whichfb = 0;*/
				flip_pending = false;
			}
		} else if(sync_interlace == 2 && vmode->fbWidth > 640) {
			if(VIDEO_GetNextField()) {
				VIDEO_SetNextFramebuffer(xfb[whichfb]);
				VIDEO_Flush();
				whichfb ^= 1;
				flip_pending = false;
			}
		} else {
			VIDEO_SetNextFramebuffer(xfb[whichfb]);
			VIDEO_Flush();
			whichfb ^= 1;
			flip_pending = false;
		}
	}
}

void HOffset()
{
	if (pal)
	{
		vmode->viXOrigin = (VI_MAX_WIDTH_PAL - vmode->viWidth) / 2;
		vmode->viYOrigin = (VI_MAX_HEIGHT_PAL - vmode->viHeight) / 2;
	}
	else
	{
		vmode->viXOrigin = (VI_MAX_WIDTH_NTSC - vmode->viWidth) / 2;
		vmode->viYOrigin = (VI_MAX_HEIGHT_NTSC - vmode->viHeight) / 2;
	}

	s8 hoffset = 0;

	if (CONF_GetDisplayOffsetH(&hoffset) == 0)
		vmode->viXOrigin += hoffset;
}

void SetDoubleStrikeOff()
{
	CONF_GetVideo();
	vmode = VIDEO_GetPreferredMode(NULL);

	GX_SetViewport(1.0f/24.0f,1.0f/24.0f,vmode->fbWidth,vmode->efbHeight,0,1);
	f32 yscale = GX_GetYScaleFactor(vmode->efbHeight,vmode->xfbHeight);
	u32 xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,vmode->fbWidth,vmode->efbHeight);
	GX_SetDispCopySrc(0,0,vmode->fbWidth,vmode->efbHeight);
	GX_SetDispCopyDst(vmode->fbWidth,xfbHeight);
	GX_SetFieldMode(GX_DISABLE, GX_DISABLE);
	GX_Flush();

    VIDEO_Configure(vmode);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    VIDEO_WaitVSync();
}

void SetDoubleStrike()
{
	CONF_GetVideo();
	if(pal60 || pal)
		vmode = &TVEurgb60Hz240Ds;
	else
		vmode = &TVNtsc240Ds;

	GX_SetViewport(1.0f/24.0f,1.0f/24.0f,vmode->fbWidth,vmode->efbHeight,0,1);
	f32 yscale = GX_GetYScaleFactor(vmode->efbHeight,vmode->xfbHeight);
	u32 xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,vmode->fbWidth,vmode->efbHeight);
	GX_SetDispCopySrc(0,0,vmode->fbWidth,vmode->efbHeight);
	GX_SetDispCopyDst(vmode->fbWidth,xfbHeight);
	u8 sharp[7] = {0,0,21,night,21,0,0};
	u8* vfilter = sharp;
	GX_SetCopyFilter(vmode->aa,vmode->sample_pattern,GX_TRUE,vfilter);
	GX_SetFieldMode(GX_DISABLE, GX_ENABLE);
	GX_Flush();

    VIDEO_Configure(vmode);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    VIDEO_WaitVSync();
}

void Set576pOff()
{
	//Need to check for NTSC
	pal = false;
	CONF_GetVideo();
	vmode = VIDEO_GetPreferredMode(NULL);

	GX_SetViewport(1.0f/24.0f,1.0f/24.0f,vmode->fbWidth,vmode->efbHeight,0,1);
	f32 yscale = GX_GetYScaleFactor(vmode->efbHeight,vmode->xfbHeight);
	u32 xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,vmode->fbWidth,vmode->efbHeight);
	GX_SetDispCopySrc(0,0,vmode->fbWidth,vmode->efbHeight);
	GX_SetDispCopyDst(vmode->fbWidth,xfbHeight);
	GX_SetFieldMode(GX_DISABLE, GX_DISABLE);
	GX_Flush();

	// Allocate framebuffers
/*	xfb[0] = (u32 *) SYS_AllocateFramebuffer (vmode);
	xfb[1] = (u32 *) SYS_AllocateFramebuffer (vmode);
	DCInvalidateRange(xfb[0], VIDEO_GetFrameBufferSize(vmode));
	DCInvalidateRange(xfb[1], VIDEO_GetFrameBufferSize(vmode));
	xfb[0] = (u32 *) MEM_K0_TO_K1 (xfb[0]);
	xfb[1] = (u32 *) MEM_K0_TO_K1 (xfb[1]);*/

	// Clear framebuffers
	VIDEO_ClearFrameBuffer (vmode, xfb[0], COLOR_BLACK);
	VIDEO_ClearFrameBuffer (vmode, xfb[1], COLOR_BLACK);
	VIDEO_SetNextFramebuffer (xfb[0]);

    VIDEO_Configure(vmode);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    VIDEO_WaitVSync();
}

void Set576p()
{
	CONF_GetVideo();
	vmode = &TVPal576ProgScale;

	pal = true;
	GX_SetViewport(1.0f/24.0f,1.0f/24.0f,vmode->fbWidth,vmode->efbHeight,0,1);
	f32 yscale = GX_GetYScaleFactor(vmode->efbHeight,576);
	u32 xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,vmode->fbWidth,vmode->efbHeight);
	GX_SetDispCopySrc(0,0,vmode->fbWidth,vmode->efbHeight);
	GX_SetDispCopyDst(vmode->fbWidth,xfbHeight);
	GX_SetFieldMode(GX_DISABLE,GX_DISABLE);
	GX_Flush();
	
	// Allocate framebuffers
/*	xfb[0] = (u32 *) SYS_AllocateFramebuffer (vmode);
	xfb[1] = (u32 *) SYS_AllocateFramebuffer (vmode);
	DCInvalidateRange(xfb[0], VIDEO_GetFrameBufferSize(vmode));
	DCInvalidateRange(xfb[1], VIDEO_GetFrameBufferSize(vmode));
	xfb[0] = (u32 *) MEM_K0_TO_K1 (xfb[0]);
	xfb[1] = (u32 *) MEM_K0_TO_K1 (xfb[1]);*/

	// Clear framebuffers
	VIDEO_ClearFrameBuffer (vmode, xfb[0], COLOR_BLACK);
	VIDEO_ClearFrameBuffer (vmode, xfb[1], COLOR_BLACK);
	VIDEO_SetNextFramebuffer (xfb[0]);

    VIDEO_Configure(vmode);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    VIDEO_WaitVSync();
}

void SetVIscale()
{
	vmode->viWidth = VI_MAX_WIDTH_NTSC;
	HOffset();
	VIDEO_Configure (vmode);
	VIDEO_Flush();
}

void SetVIscaleback()
{
	if (CONF_GetAspectRatio() == CONF_ASPECT_16_9)
		vmode->viWidth = 711;
	else
		vmode->viWidth = 704;
	HOffset();
	VIDEO_Configure (vmode);
	VIDEO_Flush();
}

void SetDf()
{
	if (vmode == &TVNtsc240Ds || vmode == &TVEurgb60Hz240Ds) {
		return;
	}

	u8 deflicker[7] = {8, 8, 10, 12, 10, 8, 8};
	u8* vfilter = deflicker;

	GX_SetCopyFilter(vmode->aa,vmode->sample_pattern,GX_TRUE,vfilter);
	GX_Flush();
}

void SetDfOff()
{
	u8 sharp[7] = {0, 0, 21, night, 21, 0, 0};
	u8* vfilter = sharp;

	GX_SetCopyFilter(vmode->aa,vmode->sample_pattern,GX_TRUE,vfilter);
	GX_Flush();
}

void SetInterlaceOff()
{
	if(!WiiSettings.interlaceHandle)
		return;
	else if(WiiSettings.interlaceHandle == 1)
		SetDoubleStrikeOff();
		
	//handle deflicker
	if(WiiSettings.videoDf == 1)
		SetDf();
	else
		SetDfOff();
	//handle 240p and 576p
	if(WiiSettings.doubleStrike == 1)
		SetDoubleStrike();
	if(WiiSettings.force576p == 1)
		Set576p();
}

void SetInterlace()
{
	if(!WiiSettings.interlaceHandle)
		return;

	if(WiiSettings.interlaceHandle == 2 && vmode->vfilter[0] == 0) //deflicker in video only to hide combing
		SetDf();
	else if(WiiSettings.interlaceHandle == 2 && vmode->vfilter[0] != 0)
		return;
	if(WiiSettings.interlaceHandle != 1 || vmode->viTVMode != VI_PROGRESSIVE)
		return;

	CONF_GetVideo();
	if (want576i)
		vmode = &TVPal576IntDfScale;
	else if(pal60)
		vmode = &TVEurgb60Hz480IntDf;
	else
		vmode = &TVNtsc480IntDf;

	GX_SetViewport(1.0f/24.0f,1.0f/24.0f,vmode->fbWidth,vmode->efbHeight,0,1);
	f32 yscale = GX_GetYScaleFactor(vmode->efbHeight,vmode->xfbHeight);
	u32 xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,vmode->fbWidth,vmode->efbHeight);
	GX_SetDispCopySrc(0,0,vmode->fbWidth,vmode->efbHeight);
	GX_SetDispCopyDst(vmode->fbWidth,xfbHeight);
	GX_SetFieldMode(GX_DISABLE, GX_DISABLE);
	GX_Flush();

    VIDEO_Configure(vmode);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    VIDEO_WaitVSync();

	//otherwise we waste our time
	SetDfOff();
}

#if 1
void SetMplTiled()
{
	if(vmode->fbWidth == 720 || !wiiTiledRender)
		return;
	
	vmode->viWidth = VI_MAX_WIDTH_NTSC;
	vmode->fbWidth = VI_MAX_WIDTH_NTSC;
	HOffset();
	
	GX_SetViewport(1.0f/24.0f,1.0f/24.0f,vmode->fbWidth,vmode->efbHeight,0,1);
	f32 yscale = GX_GetYScaleFactor(vmode->efbHeight,vmode->xfbHeight);
	u32 xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,vmode->fbWidth,vmode->efbHeight);
	//GX_SetDispCopySrc(0,0,vmode->fbWidth,vmode->efbHeight);
	GX_SetDispCopySrc(0, 0, ((640) + 15) & ~15, vmode->efbHeight);
	GX_SetDispCopyDst(vmode->fbWidth,xfbHeight);
	GX_SetFieldMode(GX_DISABLE,((vmode->viHeight==2*vmode->xfbHeight)?GX_ENABLE:GX_DISABLE));
	GX_Flush();
	
	// Clear framebuffers
	VIDEO_SetNextFramebuffer (xfb[0]);

    VIDEO_Configure(vmode);
    VIDEO_Flush();
}

void SetMplTiledOff()
{
	if(!WiiSettings.tiledRender || !wiiTiledRender || ssIsActive)
		return;
	
	// This fixes tearing when calling the menu.
	int i = 0;
	do {
		VIDEO_WaitVSync();
		VIDEO_ClearFrameBuffer (vmode, xfb[i], COLOR_BLACK);
		i++;
	} while(i < 2);

	timerBlack = 12; //12 is good, lower might be too annoying.
	tiledBlack = true;
	// VIDEO_SetBlack causes refresh message when in 576p
	// it also causes 0 fps when changing music in ss
	VIDEO_SetBlack(TRUE);

	GX_SetScissorBoxOffset(0, 0);

	if(WiiSettings.viWidth)
		//SetVIscale();
		vmode->viWidth = VI_MAX_WIDTH_NTSC;
	else if (CONF_GetAspectRatio() == CONF_ASPECT_16_9)
		vmode->viWidth = 711;
	else
		vmode->viWidth = 704;
		//SetVIscaleback();
	vmode->fbWidth = 640;
	HOffset();

	GX_SetViewport(1.0f/24.0f,1.0f/24.0f,vmode->fbWidth,vmode->efbHeight,0,1);
	f32 yscale = GX_GetYScaleFactor(vmode->efbHeight,vmode->xfbHeight);
	u32 xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,vmode->fbWidth,vmode->efbHeight);
	//GX_SetDispCopySrc(0,0,vmode->fbWidth,vmode->efbHeight);
	GX_SetDispCopySrc(0, 0, ((640) + 15) & ~15, vmode->efbHeight);
	GX_SetDispCopyDst(vmode->fbWidth,xfbHeight);
	GX_SetFieldMode(GX_DISABLE,((vmode->viHeight==2*vmode->xfbHeight)?GX_ENABLE:GX_DISABLE));
	
	GX_Flush();

	// Clear framebuffers
	VIDEO_ClearFrameBuffer (vmode, xfb[0], COLOR_BLACK);
	VIDEO_ClearFrameBuffer (vmode, xfb[1], COLOR_BLACK);
	VIDEO_SetNextFramebuffer (xfb[0]);

    VIDEO_Configure(vmode);
    VIDEO_Flush();
}
#endif

void
InitVideo ()
{
	VIDEO_Init();
	vmode = VIDEO_GetPreferredMode(NULL); // get default video mode
	vmode->viWidth = 704;

	//static vu32* const _vigReg = (vu32*)0xCC002030;
	//*_vigReg = 0x1001; //0x1001(30fps), 0x120E(60fps)

	if (vmode == &TVPal576IntDfScale || vmode == &TVPal576ProgScale) { // 50Hz
		pal = true;
		if(vmode == &TVPal576IntDfScale)
			want576i = true;
	} else if (CONF_GetVideo() == CONF_VIDEO_PAL) // 60Hz
		pal60 = true;

	if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
		vmode->viWidth = 711;
		screenwidth = 854;
	}

	HOffset();

	VIDEO_SetBlack (TRUE);
	VIDEO_Configure (vmode);
}

void
InitVideo2 ()
{
	// Allocate framebuffers
	vmode->fbWidth = 720;
	vmode->xfbHeight = 576;
	xfb[0] = (u32 *) SYS_AllocateFramebuffer (vmode);
	xfb[1] = (u32 *) SYS_AllocateFramebuffer (vmode);
	DCInvalidateRange(xfb[0], VIDEO_GetFrameBufferSize(vmode));
	DCInvalidateRange(xfb[1], VIDEO_GetFrameBufferSize(vmode));
	xfb[0] = (u32 *) MEM_K0_TO_K1 (xfb[0]);
	xfb[1] = (u32 *) MEM_K0_TO_K1 (xfb[1]);

	// Clear framebuffers
	VIDEO_ClearFrameBuffer (vmode, xfb[0], COLOR_BLACK);
	VIDEO_ClearFrameBuffer (vmode, xfb[1], COLOR_BLACK);
	//VIDEO_ClearFrameBuffer (vmode, xfb[2], COLOR_BLACK);
	// It's immportant to clear the fb before going back to 640x480
	// otherwise changing to 720x480 will show some green lines on the first frame.
	vmode->fbWidth = 640;
	vmode->xfbHeight = want576i ? 576 : 480;
	VIDEO_SetNextFramebuffer (xfb[0]);

	VIDEO_Flush();
	VIDEO_WaitVSync();
	VIDEO_WaitVSync();

/*	if (vmode->viTVMode & VI_NON_INTERLACE)
		VIDEO_WaitVSync();
	else
		while (VIDEO_GetNextField())
			VIDEO_WaitVSync();
*/

	// TODO: Should this be a setting?
	VIDEO_SetTrapFilter(1);

	// Initialize GX
	GXColor background = { 0, 0, 0, 0xff };
	gp_fifo=(unsigned char *)memalign(32,DEFAULT_FIFO_SIZE);
	memset (gp_fifo, 0, DEFAULT_FIFO_SIZE);
	GX_Init (gp_fifo, DEFAULT_FIFO_SIZE);
	GX_SetCopyClear (background, 0x00ffffff);
	GX_SetDispCopyGamma (GX_GM_1_0);
	GX_SetCullMode (GX_CULL_NONE);
	GX_SetViewport(1.0f/24.0f,1.0f/24.0f,vmode->fbWidth,vmode->efbHeight,0,1);
	GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
	GX_SetAlphaUpdate(GX_TRUE);
	f32 yscale = GX_GetYScaleFactor(vmode->efbHeight,vmode->xfbHeight);
	u32 xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,vmode->fbWidth,vmode->efbHeight);
	GX_SetDispCopySrc(0,0,vmode->fbWidth,vmode->efbHeight);
//	GX_SetDispCopySrc(0, 0, ((vmode->fbWidth >> 1) + 15) & ~15, vmode->efbHeight);
//	GX_SetDispCopySrc(0, 0, ((640) + 15) & ~15, vmode->efbHeight);
	GX_SetDispCopyDst(vmode->fbWidth,xfbHeight);
	GX_SetFieldMode(GX_DISABLE,((vmode->viHeight==2*vmode->xfbHeight)?GX_ENABLE:GX_DISABLE));

	VIDEO_SetPreRetraceCallback(vblank_cb);

	GX_SetDrawDoneCallback(Draw_VIDEO);
	GX_Flush();

	videoScreenshot = (u8 *) mem2_malloc(vmode->fbWidth * vmode->efbHeight * 4, MEM2_VIDEO);
}

}
