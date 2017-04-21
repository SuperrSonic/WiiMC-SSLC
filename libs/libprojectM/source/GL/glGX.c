/**
 * glGX.c - openGL wrapper for GX.
 *
 * Copyright (c) 2009 Rhys "Shareese" Koedijk
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * See 'LICENSE' included within this release
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

#include <ogcsys.h>
#include <gctypes.h>
#include <gccore.h>

#include "GL/gl.h"
#include "GL/glGX.h"
#include "wipemalloc.h"

/**
 * GX
 */

void gxInit ()
{
    // Initialise the video system
    VIDEO_Init();
    
    // Obtain the preferred video mode from the system
    rmode = VIDEO_GetPreferredMode(NULL);
    
#if defined(__wii__)

    // Widescreen fix
    if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
        rmode->viWidth = VI_MAX_WIDTH_PAL - 12;
        rmode->viXOrigin = ((VI_MAX_WIDTH_PAL - rmode->viWidth) / 2) + 2;
    }

#endif

    // Set the video mode
    VIDEO_Configure(rmode);
    
    // Allocate the framebuffers (double buffered)
    xfb[0] = (u32 *) MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    xfb[1] = (u32 *) MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    
    // Clear the framebuffers and line up the first buffer for display
    VIDEO_ClearFrameBuffer(rmode, xfb[0], COLOR_BLACK);
    VIDEO_ClearFrameBuffer(rmode, xfb[1], COLOR_BLACK);
    VIDEO_SetNextFramebuffer(xfb[0]);
    
    // Flush the display
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE)
        VIDEO_WaitVSync();
    
    //...
    gp_fifo = wipememalign(32, GX_DEFAULT_FIFO_SIZE);
    if (gp_fifo) {
        GX_Init(gp_fifo, GX_DEFAULT_FIFO_SIZE);
        
        // Set clear colour (black)
        GX_SetCopyClear(clearColour, clearDepth);
        
        // Setup the viewport and scissors (culling)
        GX_SetViewport(0, 0, rmode->fbWidth, rmode->efbHeight, 0, 1);
        f32 yscale = GX_GetYScaleFactor(rmode->efbHeight, rmode->xfbHeight);
        u32 xfbHeight = GX_SetDispCopyYScale(yscale);
        GX_SetScissor(0, 0, rmode->fbWidth, rmode->efbHeight);
        GX_SetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
        GX_SetDispCopyDst(rmode->fbWidth, xfbHeight);
        GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
        if (rmode->viHeight == (2 * rmode->xfbHeight))
            GX_SetFieldMode(rmode->field_rendering, GX_ENABLE);
        else
            GX_SetFieldMode(rmode->field_rendering, GX_DISABLE);
        
        // Set the pixel format
        if (rmode->aa)
            GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
        else
            GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);

        //...
        GX_CopyDisp(xfb[fb], GX_TRUE);
        GX_SetDispCopyGamma(GX_GM_1_0);
        
        // ===========================================================================
        
        // Reset verticies and textures
        GX_InvVtxCache();
        GX_InvalidateTexAll();

        //...
        GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        
        //...
        GX_SetTevOrder(GX_TEVSTAGE0 + tevStage, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GX_SetTevOp(GX_TEVSTAGE0 + tevStage, GX_PASSCLR);
        
        // ===========================================================================
    
        // Initialise openGL
        glInit();
        
        //...        
        guVector cam = { 0.0F, 0.0F, 0.0F },
                  up = { 0.0F, 1.0F, 0.0F },
                look = { 0.0F, 0.0F, -1.0F };
        
        guMtxIdentity(view);
        guLookAt(view, &cam, &up, &look);
        
        //...
        glMatrixIdentity(perspective);
        guPerspective(perspective, 90.0f, 1.33f, 0.1f, 1000.0f);
        GX_LoadProjectionMtx(perspective, GX_PERSPECTIVE);

    }
    
}

void gxDestroy ()
{
    //...
}

void gxSwapBuffers ()
{
    // We are finished drawing...
    GX_DrawDone();
    
    // Flip the framebuffer and flush the display
    fb ^= 1;
    GX_SetColorUpdate(GX_TRUE);
    GX_CopyDisp(xfb[fb], GX_TRUE);
    VIDEO_SetNextFramebuffer(xfb[fb]);
    VIDEO_Flush();
    VIDEO_WaitVSync();
}

void glInit ()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glNormal3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    
    //...
}

/**
 * Miscellaneous
 */

void glClearColor (GLclampf _red,
                   GLclampf _green,
                   GLclampf _blue,
                   GLclampf _alpha)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Set the clear colour
    clearColour.r = _red * 255;
    clearColour.g = _green * 255;
    clearColour.b = _blue * 255;
    clearColour.a = _alpha * 255;
    GX_SetCopyClear(clearColour, clearDepth);
}

void glClear (GLbitfield _mask)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Figure out which buffer we are to clear
    switch (_mask) {
        case GL_COLOR_BUFFER_BIT: /* ??? */ break;
        case GL_DEPTH_BUFFER_BIT: /* ??? */ break;
        case GL_ACCUM_BUFFER_BIT: /* ??? */ break;
        case GL_STENCIL_BUFFER_BIT: /* ??? */ break;
        default: return; /* GL_INVALID_VALUE */
    }
    
    // Clear the specified buffer
    // TODO: This correctly...
    GX_SetCopyClear(clearColour, clearDepth);
}

void glBlendFunc (GLenum _sfactor, GLenum _dfactor)
{    
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Determine the source blending mode
    switch (_sfactor) {
        case GL_ZERO: blendModeSrc = GX_BL_ZERO; break;
        case GL_ONE: blendModeSrc = GX_BL_ONE; break;
        case GL_SRC_COLOR: blendModeSrc = GX_BL_SRCCLR; break;
        case GL_ONE_MINUS_SRC_COLOR: blendModeSrc = GX_BL_SRCCLR; break; /* ??? */
        case GL_DST_COLOR: blendModeSrc = GX_BL_DSTCLR; break;
        case GL_ONE_MINUS_DST_COLOR: blendModeSrc = GX_BL_DSTCLR; break; /* ??? */
        case GL_SRC_ALPHA: blendModeSrc = GX_BL_SRCALPHA; break;
        case GL_ONE_MINUS_SRC_ALPHA: blendModeSrc = GX_BL_SRCALPHA; break; /* ??? */
        case GL_DST_ALPHA: blendModeSrc = GX_BL_DSTALPHA; break;
        case GL_ONE_MINUS_DST_ALPHA: blendModeSrc = GX_BL_DSTALPHA; break; /* ??? */
        default: return; /* GL_INVALID_ENUM */
    }
    
    // Determine the destination blending mode
    switch (_dfactor) {
        case GL_ZERO: blendModeDst = GX_BL_ZERO; break;
        case GL_ONE: blendModeDst = GX_BL_ONE; break;
        case GL_SRC_COLOR: blendModeDst = GX_BL_SRCCLR; break;
        case GL_ONE_MINUS_SRC_COLOR: blendModeDst = GX_BL_SRCCLR; break; /* ??? */
        case GL_DST_COLOR: blendModeDst = GX_BL_DSTCLR; break;
        case GL_ONE_MINUS_DST_COLOR: blendModeDst = GX_BL_DSTCLR; break; /* ??? */
        case GL_SRC_ALPHA: blendModeDst = GX_BL_SRCALPHA; break;
        case GL_ONE_MINUS_SRC_ALPHA: blendModeDst = GX_BL_SRCALPHA; break; /* ??? */
        case GL_DST_ALPHA: blendModeDst = GX_BL_DSTALPHA; break;
        case GL_ONE_MINUS_DST_ALPHA: blendModeDst = GX_BL_DSTALPHA; break; /* ??? */
        default: return; /* GL_INVALID_ENUM */
    }
    
    // Set the blend mode (if enabled)
    if (blendEnabled)
        GX_SetBlendMode(GX_BM_BLEND, blendModeSrc, blendModeDst, GX_LO_CLEAR);
}

void glCullFace (GLenum _mode)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Determine the culling mode
    switch (_mode) {
        case GL_FRONT: cullMode = GX_CULL_FRONT;
        case GL_BACK: cullMode = GX_CULL_BACK;
        case GL_FRONT_AND_BACK: cullMode = GX_CULL_ALL;
        default: return; /* GL_INVALID_ENUM */
    }
    
    // Set the culling mode (if enabled)
    if (cullFaceEnabled)
        GX_SetCullMode(cullMode);
}

void glPointSize (GLfloat _size)
{
    // Sanity check
    if (_size <= 0)
        return; /* GL_INVALID_VALUE */
    
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Set the point size
    GX_SetPointSize(_size, GX_TO_ZERO);
}

void glLineWidth (GLfloat _width)
{
    // Sanity check
    if (_width <= 0)
        return; /* GL_INVALID_VALUE */
    
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Set the line width
    GX_SetLineWidth(_width, GX_TO_ZERO);
}

void glLineStipple (GLint _factor, GLushort _pattern)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Set the line stipple mode
    lineStippleFactor = _factor;
    lineStipplePattern = _pattern;
    
    if (lineStippleEnabled) {
        //...
    }
}

void glDrawBuffer (GLenum _mode)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    switch (_mode) {
        
        case GL_NONE:
            /* ??? */
            break;
            
        case GL_FRONT_AND_BACK:
            /* ??? */
            break;
            
        case GL_FRONT:
        case GL_FRONT_LEFT:
        case GL_FRONT_RIGHT:
            /* ??? */
            break;
        case GL_BACK:
        case GL_BACK_LEFT:
        case GL_BACK_RIGHT:
            /* ??? */
            break;
            
        case GL_LEFT: break; /* ??? */
        case GL_RIGHT: break; /* ??? */
    
        default: return; /* GL_INVALID_ENUM */
        
    }
}

void glReadBuffer (GLenum _mode)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    switch (_mode) {

        case GL_FRONT:
        case GL_FRONT_LEFT:
        case GL_FRONT_RIGHT:
            /* ??? */
            break;
        case GL_BACK:
        case GL_BACK_LEFT:
        case GL_BACK_RIGHT:
            /* ??? */
            break;
            
        case GL_LEFT: break; /* ??? */
        case GL_RIGHT: break; /* ??? */
        
        default: return; /* GL_INVALID_ENUM */
        
    }
}

void glEnable(GLenum _type)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Enable the specified server-side capability
    switch (_type) {
        case GL_TEXTURE_2D:
            texture2DEnabled = true;
            GX_SetNumTexGens(1);
            GX_SetTevOp(GX_TEVSTAGE0 + tevStage, tevMode);
            break;
        case GL_LINE_STIPPLE:
            lineStippleEnabled = true;
            break;
        case GL_BLEND:
            blendEnabled = true;
            GX_SetBlendMode(GX_BM_BLEND, blendModeSrc, blendModeDst, GX_LO_CLEAR);
            GX_SetAlphaUpdate(GX_TRUE);
            break;
        case GL_LINE_SMOOTH:
            lineSmoothEnabled = true;
            break;
        case GL_POINT_SMOOTH:
            pointSmoothEnabled = true;
            break;
        case GL_POLYGON_SMOOTH:
            polygonSmoothEnabled = true;
            break;
        case GL_CULL_FACE:
            cullFaceEnabled = true;
            GX_SetCullMode(cullMode);
            break;
        case GL_DEPTH_TEST:
            depthTestEnabled = true;
            GX_SetZMode(GX_TRUE, depthMode, GX_TRUE /* ??? */);
            break;
        default: return; /* GL_INVALID_ENUM */
    };
}

void glDisable (GLenum _type)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Disable the specified server-side capability
    switch (_type) {
        case GL_TEXTURE_2D:
            texture2DEnabled = false;
            GX_SetNumTexGens(0);
            GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);         
            break;
        case GL_LINE_STIPPLE:
            lineStippleEnabled = false;
            break;
        case GL_BLEND:
            blendEnabled = false;
            GX_SetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR);
            GX_SetAlphaUpdate(GX_FALSE);
            break;
        case GL_LINE_SMOOTH:
            lineSmoothEnabled = false;
            break;
        case GL_POINT_SMOOTH:
            pointSmoothEnabled = false;
            break;
        case GL_POLYGON_SMOOTH:
            polygonSmoothEnabled = false;
            break;
        case GL_CULL_FACE:
            cullFaceEnabled = false;
            GX_SetCullMode(GX_CULL_NONE);
            break;
        case GL_DEPTH_TEST:
            depthTestEnabled = false;
            GX_SetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
            break;
        default: return; /* GL_INVALID_ENUM */
    };
}

void glEnableClientState (GLenum _cap)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Enable the specified client-side state
    switch (_cap) {
        case GL_COLOR_ARRAY: colourArrayEnabled = true; break;
        case GL_EDGE_FLAG_ARRAY: edgeFlagArrayEnabled = true; break;
        case GL_INDEX_ARRAY: indexArrayEnabled = true; break;
        case GL_NORMAL_ARRAY: normalArrayEnabled = true; break;
        case GL_TEXTURE_COORD_ARRAY: texCoordArrayEnabled = true; break;
        case GL_VERTEX_ARRAY: vertexArrayEnabled = true; break;
        default: break;
    }
}

void glDisableClientState (GLenum _cap)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Disable the specified client-side state
    switch (_cap) {
        case GL_COLOR_ARRAY: colourArrayEnabled = false; break;
        case GL_EDGE_FLAG_ARRAY: edgeFlagArrayEnabled = false; break;
        case GL_INDEX_ARRAY: indexArrayEnabled = false; break;
        case GL_NORMAL_ARRAY: normalArrayEnabled = false; break;
        case GL_TEXTURE_COORD_ARRAY: texCoordArrayEnabled = false; break;
        case GL_VERTEX_ARRAY: vertexArrayEnabled = false; break;
        default: break;
    }
}

void glGetIntegerv (GLenum _pname, GLint *_params)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Sanity check
    if (!_params)
        return; /* GL_INVALID_VALUE */
        
    // Get the specified integer constant
    switch (_pname) {
        case GL_MAX_TEXTURE_SIZE: *_params = GX_MAX_TEXTURE_SIZE; break;
        default: return; /* GL_INVALID_ENUM */
    }
}

const GLubyte *glGetString (GLenum _name)
{
    // Sanity check
    if (insideBeginEndPair)
        return NULL; /* GL_INVALID_OPERATION */
    
    // Get the specified string constant
    switch (_name) {
        case GL_EXTENSIONS: return (const GLubyte *) extensions;
        default: return NULL; /* GL_INVALID_ENUM */
    }
}

void glFlush ()
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Finished drawing
    GX_DrawDone();
}

/**
 * Transformation
 */

void glMatrixIdentity (Mtx44 mtx)
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (i == j)
                mtx[i][j] = 1.0f;
            else
                mtx[i][j] = 0.0f;
}

void glMatrixCopy (Mtx44 src, Mtx44 dst)
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            dst[i][j] = src[i][j];
}

void glMatrixLoadModelView ()
{
    Mtx modelView;
    Mtx inverseModelView;
    
    // Load the current model view matrix into memory
    guMtxCopy(modelViewMatrixStack[modelViewMatrixStackDepth], modelView);    
    GX_LoadPosMtxImm(modelView, GX_PNMTX0);
    guMtxInverse(modelView, inverseModelView); 
    guMtxTranspose(inverseModelView, modelView); 
    GX_LoadNrmMtxImm(modelView, GX_PNMTX0);
    GX_SetCurrentMtx(GX_PNMTX0);
}

void glMatrixMode(GLenum _mode)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Determine the current matrix stack
    switch (_mode) {
        case GL_MODELVIEW:
            matrixStack = modelViewMatrixStack;
            matrixStackDepth = &modelViewMatrixStackDepth;
            break;
        case GL_PROJECTION:
            matrixStack = projectionMatrixStack;
            matrixStackDepth = &projectionMatrixStackDepth;
            break;
        case GL_TEXTURE:
            matrixStack = textureMatrixStack;
            matrixStackDepth = &textureMatrixStackDepth;
            break;
        default: return; /* GL_INVALID_ENUM */
    }
    
    // Grab the current matrix
    matrix = &matrixStack[*matrixStackDepth];
}

void glOrtho (GLdouble _left, GLdouble _right,
              GLdouble _bottom, GLdouble _top,
              GLdouble _near_val, GLdouble _far_val)
{
    Mtx44 temp;
    
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Sanity check
    if (!matrix ||
        _left == _right ||
        _bottom == _top ||
        _near_val == _far_val)
        return;
    
    // Multiply the current matrix with an orthographic matrix
    guOrtho(temp, _top, _bottom, _left, _right, _near_val, _far_val);
    guMtxConcat(*matrix, temp, *matrix); /* ??? */
    
    // Set the orthographic matrix
    GX_LoadProjectionMtx(*matrix, GX_ORTHOGRAPHIC);
}

void glViewport (GLint _x, GLint _y,
                 GLsizei _width, GLsizei _height)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Sanity check
    if (_width < 0 || _height < 0)
        return; /* GL_INVALID_VALUE */
    
    // Set the viewport
    GX_SetViewport(_x, _y, _width, _height, 0, 1);
    GX_SetScissor(_x, _y, _width, _height);
}

void glPopMatrix (void)
{    
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Sanity check
    if (!matrix || !matrixStack || !matrixStackDepth)
        return; /* GL_INVALID_VALUE */

    // Sanity check
    if (*matrixStackDepth <= 0)
        return; /* GL_UNDERFLOW */

    // Pop the current matrix from the current stack
    matrix = &matrixStack[*matrixStackDepth--];
}

void glPushMatrix (void)
{
    Mtx44 *new_matrix = NULL;
    
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Sanity check
    if (!matrix || !matrixStack || !matrixStackDepth)
        return; /* GL_INVALID_VALUE */

    // Sanity check
    if (*matrixStackDepth >= (GL_MAX_STACK_SIZE - 1))
        return; /* GL_OVERFLOW */

    // Push a new matrix onto the current stack
    new_matrix = &matrixStack[*matrixStackDepth++];
    glMatrixCopy(*matrix, *new_matrix);
    matrix = new_matrix;
}

void glLoadIdentity (void)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Sanity check
    if (!matrix)
        return /* GL_INVALID_VALUE */
        
    // Replace the current matrix with the identity matrix
    glMatrixIdentity(*matrix);
}

void glTranslatef (GLfloat _x, GLfloat _y, GLfloat _z)
{
    Mtx temp;
    
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Multiply the current matrix by the translation matrix
    guMtxIdentity(temp);
    guMtxTrans(temp, _x, _y, _z);
    guMtxConcat(*matrix, temp, *matrix);
}

void glRotatef (GLfloat _angle, GLfloat _x, GLfloat _y, GLfloat _z)
{
    Mtx temp;
    guVector axis;
    
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Build the axis of rotation
    axis.x = _x;
    axis.y = _y;
    axis.z = _z;
    
    // Multiply the current matrix by the rotation matrix
    guMtxIdentity(temp);
    guMtxRotAxisDeg(temp, &axis, _angle);
    guMtxConcat(*matrix, temp, *matrix);
}

void glScalef(GLfloat _x, GLfloat _y, GLfloat _z)
{
    Mtx temp;
    
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Multiply the current matrix by the general scaling matrix
    guMtxIdentity(temp);
    guMtxScale(temp, _x, _y, _z);
    guMtxConcat(*matrix, temp, *matrix);
}

/**
 * Drawing
 */

void glBegin (GLenum _type)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // We are now in a begin/end pair
    insideBeginEndPair = true;
    
    // Determine the primitive type
    switch (_type) {
        case GL_POINTS: primitiveType = GX_POINTS; break;
        case GL_LINES: primitiveType = GX_LINES; break;
        case GL_LINE_STRIP: primitiveType = GX_LINESTRIP; break;
        case GL_LINE_LOOP: primitiveType = GX_POINTS; break; /* ??? */
        case GL_TRIANGLES: primitiveType = GX_TRIANGLES; break;
        case GL_TRIANGLE_STRIP: primitiveType = GX_TRIANGLESTRIP; break;
        case GL_TRIANGLE_FAN: primitiveType = GX_TRIANGLEFAN; break;
        case GL_QUADS: primitiveType = GX_QUADS; break; 
        case GL_QUAD_STRIP: primitiveType = GX_POINTS; break; /* ??? */
        case GL_POLYGON: primitiveType = GX_POINTS; break; /* ??? */
        default: return; /* GL_INVALID_ENUM */
    }
    
    // Load the current model view matrix into memory
    glMatrixLoadModelView();
    
    // Load the currently bound texture(s) (if enabled)
    if (texture2DEnabled) {
        
    }
    
    // Destory all verticies
    glVerticiesInvalidateAll();
    
    // Setup vertex descriptions
    GX_ClearVtxDesc();
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_NRM, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    
    // Setup vertex formats
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
}

void glEnd (void)
{
    // Sanity check
    if (!insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Short circuit case were we don't actually have to do anything
    if (vertexCount == 0 || !verticies)
        return;
    
    // ===========================================================================
    GX_Begin(primitiveType, GX_VTXFMT0, vertexCount);
    // ===========================================================================
    
    // Upload all verticies to memory
    GLvertex *vert = verticies;
    for (; vert; vert = vert->next) {
        GX_Position3f32(vert->x, vert->y, vert->z); 
        GX_Normal3f32(vert->normal.x, vert->normal.y, vert->normal.z);
        GX_Color3f32(vert->colour.r, vert->colour.g, vert->colour.b);
        GX_TexCoord2f32(vert->texCoord.s, vert->texCoord.t);      
    }
    
    // ===========================================================================
    GX_End();
    // ===========================================================================

    // Destory all verticies
    glVerticiesInvalidateAll();
    
    // We are no longer in a begin/end pair
    insideBeginEndPair = false;    
}

void glVerticiesInvalidateAll ()
{
    GLvertex *vert = verticies;
    GLvertex *nextVert = NULL;
    
    // Destroy all verticies
    while (vert) {
        nextVert = vert->next;
        wipefree(vert);
        vert = nextVert;
    }
    
    // Reset verticies
    verticies = NULL;
    vertex = NULL;
    vertexCount = 0;
}

void glVertex2f (GLfloat _x, GLfloat _y)
{
    glVertex3f(_x, _y, 0);
}

void glVertex3f (GLfloat _x, GLfloat _y, GLfloat _z)
{
    // Allocate the vertex
    GLvertex *vert = wipemalloc(sizeof(GLvertex));
    if (!vert)
        return;
    
    // Setup the vertex
    vert->x = _x;
    vert->y = _y;
    vert->x = _z;
    vert->colour.r = colour.r;
    vert->colour.g = colour.g;
    vert->colour.b = colour.b;
    vert->colour.a = colour.a;
    vert->normal.x = normal.x;
    vert->normal.y = normal.y;
    vert->normal.z = normal.z;
    vert->texCoord.s = texCoord.s;
    vert->texCoord.t = texCoord.t;
    
    // Insert the vertex into the double-linked FILO list of allocated verticies
    if (verticies) {
        vert->next = verticies;
        verticies->prev = vert;
    }
    verticies = vert;
    vertexCount++;
    
}

void glNormal3f (GLfloat _x, GLfloat _y, GLfloat _z)
{
    normal.x = _x;
    normal.y = _y;
    normal.z = _z;
}

void glColor3f (GLfloat _red, GLfloat _green, GLfloat _blue)
{
    // Set the current colour
    colour.r = _red * 255;
    colour.g = _green * 255;
    colour.b = _blue * 255;
    colour.a = 0.0f;
}

void glColor4f (GLfloat _red, GLfloat _green,
                GLfloat _blue, GLfloat _alpha)
{
    // Set the current colour
    colour.r = _red * 255;
    colour.g = _green * 255;
    colour.b = _blue * 255;
    colour.a = _alpha * 255;
}

void glTexCoord2f (GLfloat _s, GLfloat _t)
{
    texCoord.s = _s;
    texCoord.t = _t;
};

void glRectd (GLdouble _x1, GLdouble _y1, GLdouble _x2, GLdouble _y2)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // If the second vertex is above and to the right of the first
    // then build the rectangle with counterclockwise winding
    if (_x2 > _x1 && _y2 > _y1) {
        glBegin(GX_QUADS);
        glVertex2f(_x1, _y2);
        glVertex2f(_x2, _y2);
        glVertex2f(_x2, _y1);
        glVertex2f(_x1, _y1);
        glEnd();
    } else {
        glBegin(GX_QUADS);
        glVertex2f(_x1, _y1);
        glVertex2f(_x2, _y1);
        glVertex2f(_x2, _y2);
        glVertex2f(_x1, _y2);
        glEnd();
    }
}

/**
 * Vertex arrays
 */

void glVertexPointer (GLint _size, GLenum _type,
                      GLsizei _stride, const GLvoid *_ptr)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Sanity check
    if ((_size != 2 && _size != 3 && _size != 4) ||
        _stride < 0 ||
        !_ptr)
        return; /* GL_INVALID_VALUE */
    
    // Set the vertex array
    vertexArray.size = _size;
    vertexArray.type = _type;
    vertexArray.stride = _stride;
    vertexArray.ptr = _ptr;    
}


void glNormalPointer (GLenum _type, GLsizei _stride,
                      const GLvoid *_ptr)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Sanity check
    if (_stride < 0 ||
        !_ptr)
        return; /* GL_INVALID_VALUE */
    
    // Set the normal array
    normalArray.size = vertexArray.size;
    normalArray.type = _type;
    normalArray.stride = _stride;
    normalArray.ptr = _ptr;    
}

void glColorPointer (GLint _size, GLenum _type,
                     GLsizei _stride, const GLvoid *_ptr)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Sanity check
    if ((_size != 2 && _size != 3 && _size != 4) ||
        _stride < 0 ||
        !_ptr)
        return; /* GL_INVALID_VALUE */
        
    // Set the colour array
    colourArray.size = _size;
    colourArray.type = _type;
    colourArray.stride = _stride;
    colourArray.ptr = _ptr;
}

void glTexCoordPointer (GLint _size, GLenum _type,
                        GLsizei _stride, const GLvoid *_ptr)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Sanity check
    if ((_size != 2 && _size != 3 && _size != 4) ||
        _stride < 0 ||
        !_ptr)
        return; /* GL_INVALID_VALUE */
    
    // Set the texture coordinate array
    texCoordArray.size = _size;
    texCoordArray.type = _type;
    texCoordArray.stride = _stride;
    texCoordArray.ptr = _ptr;
}

void glVertexArrayBuild (GLvertexarray *_array, u32 _start, u32 _count, f32 **array, u32 *size)
{
    const void *ptr = NULL;
    int i;
    
    // Sanity check
    if (!array || !*array || !size ||
        !_array || _array->size <= 0)
        return;
    
    // Allocate the destination array
    *size = (_array->size * sizeof(f32));
    *array = wipememalign(32, *size);
    if (!array)
        return;
    
    // Build a continuous array of coordinates from the specified vertex array
    ptr = _array->ptr;
    /*for (i = _start; i < _count; i++) {
        switch (_array->type) {
            case GL_SHORT:
                GLshort *val = (GLshort *) (_array->ptr + (i * (_array->stride + sizeof(GLshort))));
                ptr += (_array->stride + sizeof(GLshort));
                *array[i] = (f32)val;
                break;
            case GL_INT:
                GLint *val = (GLint *) (_array->ptr + (i * (_array->stride + sizeof(GLint))));
                ptr += (_array->stride + sizeof(GLint));
                *array[i] = (f32)val;
                break;
            case GL_FLOAT:
                GLfloat *val = (GLfloat *) (_array->ptr + (i * (_array->stride + sizeof(GLfloat))));
                ptr += (_array->stride + sizeof(GLfloat));
                *array[i] = (f32)val;
                break;
            case GL_DOUBLE:
                GLdouble *val = (GLdouble *) (_array->ptr + (i * (_array->stride + sizeof(GLdouble))));
                ptr += (_array->stride + sizeof(GLdouble));
                *array[i] = (f32)val;
                break;
        }
    }*/

    // Flush the destination array
    DCFlushRange(*array, *size);
}

void glDrawArrays (GLenum _mode, GLint _first, GLsizei _count)
{
    f32 *vtxVerticies = NULL;
    u32 vtxVerticiesCount = 0;
    f32 *vtxNormals = NULL;
    u32 vtxNormalsCount = 0;
    f32 *vtxColours = NULL;
    u32 vtxColoursCount = 0;
    f32 *vtxTexCoords = NULL;
    u32 vtxTexCoordsCount = 0;
    u8 type = GX_POINTS;
    
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Sanity check
    if (_count < 0)
        return; /* GL_INVALID_VALUE */

    // Short circuit cases were we don't actually have to do anything
    if (!vertexArrayEnabled)
        return;
    
    /*
    // Build the vertex arrays
    glVertexArrayBuild(&vertexArray, &vtxVerticies, &vtxVerticiesCount);
    glVertexArrayBuild(&normalArray, &vtxNormals, &vtxNormalsCount);
    glVertexArrayBuild(&colourArray, &vtxColours, &vtxColoursCount);
    glVertexArrayBuild(&texCoordArray, &vtxTexCoords, &vtxTexCoordsCount);
    
    GX_ClearVtxDesc();
    GX_SetVtxDesc(GX_VA_POS, GX_INDEX16);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GX_SetArray(GX_VA_POS, vtxVerticies, 0);
    
    if (normalArrayEnabled) {
        GX_SetVtxDesc(GX_VA_NRM, GX_INDEX16);
        GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
        GX_SetArray(GX_VA_NRM, vtxNormals, 0);
    }
    
    if (colourArrayEnabled) {
        GX_SetVtxDesc(GX_VA_CLR0, GX_INDEX16);
        GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GX_SetArray(GX_VA_CLR0, vtxColours, 0);
    }
    
    // TODO: This correctly...
    if (texture2DEnabled && texCoordArrayEnabled) {
        GX_SetVtxDesc(GX_VA_TEX0, GX_INDEX16);
        GX_SetVtxDesc(GX_VA_TEX0MTXIDX, GX_TEXMTX0);
        GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GX_SetArray(GX_VA_TEX0, vtxTexCoords, 0);
        GX_SetTevOrder(GX_TEVSTAGE0 + tevStage, GX_TEXCOORD0, GX_TEXMAP0, GX_COLORNULL);
        GX_LoadTexObj(&texture2D, GX_TEXMAP0);
    } else {
        GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
        GX_SetVtxDesc(GX_VA_TEX0MTXIDX, GX_NONE);
    }*/
    
    // Load the current model view matrix into memory
    glMatrixLoadModelView();
    
    // ===========================================================================
    GX_Begin(type, GX_VTXFMT0, _count);
    // ===========================================================================
    
    // Upload all verticies to memory
    //...
    
    // ===========================================================================
    GX_End();
    // ===========================================================================

}

void glInterleavedArrays (GLenum _format, GLsizei _stride,
                          const GLvoid *_pointer)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Sanity check
    if (_stride < 0)
        return; /* GL_INVALID_VALUE */
    
    //...
}

/**
 * Lighting
 */

void glShadeModel (GLenum _mode)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Determine the shading mode
    switch (_mode) {
        case GL_FLAT: /* ??? */ break;
        case GL_SMOOTH: /* ??? */ break;
        default: return; /* GL_INVALUD_ENUM */
    }
}

/**
 * Textures
 */

GLuint glTextureNextFreeName ()
{
    int name = 0;
    GLtexture *tex = textures;
    
    // Iterate through all textures till an unused name is found
    while (tex) {
        if (tex->name == name) {
            tex = textures;
            name++;
        } else {
            tex = tex->next;
        }
    }
    
    return name;
}

void glGenTextures (GLsizei _n, GLuint *_textures)
{
    int i;
    
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Sanity check
    if (_n < 0 || !_textures)
        return; /* GL_INVALID_VALUE */
    
    // Allocate the specified number of textures
    for(i = 0; i < _n; i++) {
        
        // Allocate the next texture
        GLtexture *tex = wipemalloc(sizeof(GLtexture));
        if (!tex) {
            _textures[i] = 0;
            continue;
        }
        
        // Setup the texture
        tex->name = glTextureNextFreeName();
        tex->wrap_s = GX_REPEAT;
        tex->wrap_t = GX_REPEAT;
        tex->minfilt = GX_NEAR_MIP_LIN;
        tex->magfilt = GX_LINEAR;
        tex->priority = 0.0f;
        
        // Insert the texture into the double-linked FILO list of allocated textures
        if (textures) {
            tex->next = textures;
            textures->prev = tex;
        }
        textures = tex;
        textureCount++;
        
        // Pass the textures name back to our caller
        _textures[i] = tex->name;
        
    }
    
}

void glDeleteTextures (GLsizei _n, const GLuint *_textures)
{
    int i;
    
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Sanity check
    if (_n < 0 || !_textures)
        return; /* GL_INVALID_VALUE */
    
    // Destroy the specified textures
    for(i = 0; i < _n; i++) {
        GLtexture *tex = glTextureGet(_textures[i]);
        if (tex) {
            
            // TODO: Destroy tex->obj?
            
            // Remove the texture from the double-linked FILO list of allocated textures
            textureCount--;
            if (tex->next)
                tex->next->prev = tex->prev;
            if (tex->prev)
                tex->prev->next = tex->next;
            else
                textures = tex->next;
            
            // Free the texture
            wipefree(tex);
            
        }
    }
    
}

GLtexture *glTextureGet (GLuint _name)
{
    GLtexture *tex = textures;
    
    // Find the texture with the specified name (if possible)
    while (tex) {
        if (tex->name == _name)
            return tex;
        tex = tex->next;
    }
    
    return NULL;
}

void glBindTexture (GLenum _target, GLuint _texture)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Bind the texture to the specified target
    switch (_target) {
        case GL_TEXTURE_1D: texture1D = glTextureGet(_texture); break;
        case GL_TEXTURE_2D: texture2D = glTextureGet(_texture); break;
        default: return; /* GL_INVALUD_ENUM */
    }
}

void glTexImage2D (GLenum _target, GLint _level,
                   GLint _internalFormat,
                   GLsizei _width, GLsizei _height,
                   GLint _border, GLenum _format, GLenum _type,
                   const GLvoid *_pixels)
{
    u8 format;

    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Sanity check
    if (!texture2D ||
        _level < 0 || _level > log(GX_MAX_TEXTURE_SIZE) ||
        _width < 64 || _width > GX_MAX_TEXTURE_SIZE + 2 ||
        _height < 64 || _height > GX_MAX_TEXTURE_SIZE + 2)
        return; /* GL_INVALID_VALUE */
    
    // Determine the textures format
    switch (_internalFormat) {
        
        case 0: /* ??? */ break;
        
        case 4:
        case GL_RGBA:
        case GL_RGBA8: format = GX_TF_RGBA8; break;
        
        // Everything else, unsupported...
        // TODO: Support more formats!?
        default: return; /* GL_INVALID_VALUE */
        
    }

    // Determine the format of the pixel data
    switch (_format) {
        case GL_LUMINANCE: /* ??? */ break;
        case GL_RGBA: break;
        
        // Everything else, unsupported...
        // TODO: Support more formats!?
        default: return; /* GL_INVALID_VALUE */
        
    }
    
    // Determine the data type of the pixel data
    switch (_type) {
        case GL_UNSIGNED_BYTE: break;
        case GL_FLOAT: /* ??? */ break;
        
        // Everything else, unsupported...
        // TODO: Support more formats!?
        default: return; /* GL_INVALID_VALUE */
        
    }
    
    // TODO: Borders!?
    
    // Initialise the texture
    if (_target == GL_TEXTURE_2D) {
        GX_InitTexObj(&texture2D->obj, (void *) _pixels, _width, _height,
                      format, texture2D->wrap_s, texture2D->wrap_t, _level);
    } else if (_target == GL_PROXY_TEXTURE_2D) {
        GX_InitTexObj(&texture2D->obj, NULL, _width, _height,
                      format, texture2D->wrap_s, texture2D->wrap_t, _level); /* ??? */
    }
    
    // Setup the textures properties
    GX_InitTexObjLOD(&texture2D->obj, texture2D->minfilt, texture2D->magfilt, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    
    // Synchronize
    GX_InvalidateTexAll();
}

void glCopyTexSubImage2D (GLenum _target, GLint _level,
                          GLint _xoffset, GLint _yoffset,
                          GLint _x, GLint _y,
                          GLsizei _width, GLsizei _height)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Sanity check
    if (!texture2D ||
        _level < 0 || _level > log(GX_MAX_TEXTURE_SIZE))
        return; /* GL_INVALID_VALUE */
    
    //...
}

void glCompressedTexImage2DARB (GLenum param1, GLint param2, GLenum param3, 
                                GLsizei param4, GLsizei param5, GLint param6, 
                                GLsizei param7, const GLvoid *param8)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Sanity check
    if (!texture2D)
        return; /* GL_INVALID_VALUE */
    
    //...
}

void glTexGend (GLenum _coord, GLenum _pname, GLdouble _param)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
    
    // Determine the texture-coordinate generation function we are setting
    switch(_pname) {
        
        case GL_TEXTURE_GEN_MODE:
            switch ((GLint) _param) {
                case GL_OBJECT_LINEAR: /* ??? */ break;
                case GL_EYE_LINEAR: /* ??? */ break;
                case GL_SPHERE_MAP: /* ??? */ break;
                case GL_NORMAL_MAP: /* ??? */ break;
                case GL_REFLECTION_MAP: /* ??? */ break;
                default: return; /* GL_INVALID_ENUM */
            }
            break;
        
        case GL_OBJECT_PLANE:
            return; /* GL_INVALID_OPERATION */
            break;
            
        case GL_EYE_PLANE:
            return; /* GL_INVALID_OPERATION */
            break;
            
        default: return; /* GL_INVALID_ENUM */
        
    }
}

void glTexGenf (GLenum _coord, GLenum _pname, GLfloat _param)
{
    glTexGend(_coord, _pname, (GLdouble) _param);
}

void glTexGeni (GLenum _coord, GLenum _pname, GLint _param)
{
    glTexGend(_coord, _pname, (GLdouble) _param);
}

void glTexEnvf (GLenum _target, GLenum _pname, GLfloat _param)
{
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
        
    // Sanity check
    if (_target != GL_TEXTURE_ENV)
        return; /* GL_INVALID_ENUM */
    
    // Determine which texture environment parameter we are setting
    switch (_pname) {
        
        // Texture environment operation mode
        case GL_TEXTURE_ENV_MODE:
            switch ((GLint) _param) {
                case GL_MODULATE: tevMode = GX_MODULATE; break;
                case GL_DECAL: tevMode = GX_DECAL; break;
                case GL_BLEND: tevMode = GX_BLEND; break;
                case GL_REPLACE: tevMode = GX_REPLACE; break;
                default: return; /* GL_INVALID_ENUM */
            }
            if (texture2DEnabled)
                GX_SetTevOp(GX_TEVSTAGE0 + tevStage, tevMode);
            break;

        default: return; /* GL_INVALID_ENUM */
        
    }
}

void glTexEnvi (GLenum _target, GLenum _pname, GLint _param)
{
    glTexEnvf(_target, _pname, (GLfloat) _param);
}

void glTexParameterf (GLenum _target, GLenum _pname, GLfloat _param)
{
    GLtexture *tex = NULL;
    
    // Sanity check
    if (insideBeginEndPair)
        return; /* GL_INVALID_OPERATION */
            
    // Determine which texture this operation will be applied to
    switch (_target) {
        case GL_TEXTURE_1D: tex = texture1D; break;
        case GL_TEXTURE_2D: tex = texture2D; break;
        default: return; /* GL_INVALID_ENUM */
    }
    
    // Sanity check
    if (tex) {
            
        // Determine which texture parameter we are setting
        switch (_pname) {
            
            // Minifying filter mode
            case GL_TEXTURE_MIN_FILTER:
                switch ((GLint) _param) {
                    case GL_NEAREST: tex->minfilt = GX_NEAR; break;
                    case GL_LINEAR: tex->minfilt = GX_LINEAR; break;
                    case GL_NEAREST_MIPMAP_NEAREST: tex->minfilt = GX_NEAR_MIP_NEAR; break;
                    case GL_LINEAR_MIPMAP_NEAREST: tex->minfilt = GX_LIN_MIP_NEAR; break;
                    case GL_NEAREST_MIPMAP_LINEAR: tex->minfilt = GX_NEAR_MIP_LIN; break;
                    case GL_LINEAR_MIPMAP_LINEAR: tex->minfilt = GX_LIN_MIP_LIN; break;
                    default: return; /* GL_INVALID_ENUM */
                }
                GX_InitTexObjFilterMode(&tex->obj, tex->minfilt, tex->magfilt);
                break;
                
            // Magnification filter mode
            case GL_TEXTURE_MAG_FILTER:
                switch ((GLint) _param) {
                    case GL_NEAREST: tex->magfilt = GX_NEAR; break;
                    case GL_LINEAR: tex->magfilt = GX_LINEAR; break;
                    default: return; /* GL_INVALID_ENUM */
                }
                GX_InitTexObjFilterMode(&tex->obj, tex->minfilt, tex->magfilt);
                break;
                
            // 's' coordinate wrap mode
            case GL_TEXTURE_WRAP_S:
                switch ((GLint) _param) {
                    case GL_CLAMP: tex->wrap_s = GX_CLAMP; break;
                    case GL_REPEAT: tex->wrap_s = GX_REPEAT; break;
                    default: return; /* GL_INVALID_ENUM */
                }
                GX_InitTexObjWrapMode(&tex->obj, tex->wrap_s, tex->wrap_t);
                break;
                
            // 't' coordinate wrap mode
            case GL_TEXTURE_WRAP_T:
                switch ((GLint) _param) {
                    case GL_CLAMP: tex->wrap_t = GX_CLAMP; break;
                    case GL_REPEAT: tex->wrap_t = GX_REPEAT; break;
                    default: return; /* GL_INVALID_ENUM */
                }
                GX_InitTexObjWrapMode(&tex->obj, tex->wrap_s, tex->wrap_t);
                break;
                
            // Resident priority
            // NOTE: We don't really need this?
            case GL_TEXTURE_PRIORITY:
                tex->priority = _param;
                break;

            default: return; /* GL_INVALID_ENUM */
            
        }

    }
}

void glTexParameteri (GLenum _target, GLenum _pname, GLint _param)
{
    glTexParameterf(_target, _pname, (GLfloat) _param);
}
