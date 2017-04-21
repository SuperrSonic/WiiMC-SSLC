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

#ifndef _GLGX_H_
#define _GLGX_H_

#ifdef __cplusplus
extern "C" {
#endif

#define GL_MAX_STACK_SIZE           32

#define GX_MAX_TEXTURE_SIZE         1024
#define GX_DEFAULT_FIFO_SIZE        256 * 1024

/**
 * GX
 */

GXRModeObj *rmode = NULL;
void *gp_fifo = NULL;
void *xfb[2] = { NULL, NULL };
u32 fb = 0;

void glInit ();

/**
 * Miscellaneous
 */

bool texture2DEnabled = false;
bool lineStippleEnabled = false;
bool blendEnabled = false;
bool lineSmoothEnabled = false;
bool pointSmoothEnabled = false;
bool polygonSmoothEnabled = false;
bool depthTestEnabled = true;
bool cullFaceEnabled = true;

bool colourArrayEnabled = false;
bool edgeFlagArrayEnabled = false;
bool indexArrayEnabled = false;
bool normalArrayEnabled = false;
bool texCoordArrayEnabled = false;
bool vertexArrayEnabled = false;

bool insideBeginEndPair = false;

GXColor clearColour = { 0, 0, 0, 0 };
f32 clearDepth = 0x00FFFFFF;

u32 blendModeSrc = GX_BL_ONE;
u32 blendModeDst = GX_BL_ZERO;
u32 cullMode = GX_CULL_ALL;
u32 depthMode = GX_LESS;

u32 lineStippleFactor = 0;
u16 lineStipplePattern = 0;

f32 pointsize = 1;

const char *extensions = "";

/**
 * Transformation
 */

Mtx44 modelViewMatrixStack[GL_MAX_STACK_SIZE] = { { { 0 }, { 0 } } };   /* 0_o */
Mtx44 projectionMatrixStack[GL_MAX_STACK_SIZE] = { { { 0 }, { 0 } } };  /* o_0 */
Mtx44 textureMatrixStack[GL_MAX_STACK_SIZE] = { { { 0 }, { 0 } } };     /* o_o */
u8 modelViewMatrixStackDepth = 0;
u8 projectionMatrixStackDepth = 0;
u8 textureMatrixStackDepth = 0;

Mtx44 *matrixStack = modelViewMatrixStack;
u8 *matrixStackDepth = &modelViewMatrixStackDepth;
Mtx44 *matrix = &modelViewMatrixStack[0];

Mtx view;
Mtx44 perspective;

void glMatrixIdentity (Mtx44 mtx);
void glMatrixCopy (Mtx44 src, Mtx44 dst);
void glMatrixLoadModelView ();

/**
 * Drawing
 */

typedef struct _GLtexcoord {
    f32 s;
    f32 t;
} GLtexcoord;

typedef struct _GLnormal {
    f32 x;
    f32 y;
    f32 z;
} GLnormal;

typedef struct _GLvertex {
    f32 x;
    f32 y;
    f32 z;
    GXColor colour;
    GLnormal normal;
    GLtexcoord texCoord;
    struct _GLvertex *prev;
    struct _GLvertex *next;
} GLvertex;

u8 tevStage = 0;
u8 tevMode = GX_REPLACE;

u8 primitiveType = GX_POINTS;

GXColor colour = { 255, 255, 255, 255 };
GLnormal normal = { 0.0f, 0.0f, 0.0f };
GLtexcoord texCoord = { 0.0f, 0.0f };

GLvertex *verticies = NULL;
GLvertex *vertex = NULL;
u32 vertexCount = 0;

void glVerticiesInvalidateAll ();

/**
 * Vertex arrays
 */

typedef struct _GLvertexarray {
    GLint size;
    GLenum type;
    GLsizei stride;
    const GLvoid *ptr;
} GLvertexarray;

GLvertexarray colourArray = { 0 };
GLvertexarray normalArray = { 0 };
GLvertexarray texCoordArray = { 0 };
GLvertexarray vertexArray = { 0 };

void glVertexArrayBuild (GLvertexarray *_array, u32 _start, u32 _count, f32 **array, u32 *size);

/**
 * Lighting
 */

//...

/**
 * Textures
 */

typedef struct _GLtexture {
    GLuint name;
    GXTexObj obj;
    u8 minfilt;
    u8 magfilt;
    u8 wrap_s;
    u8 wrap_t;
    f32 priority;
    struct _GLtexture *prev;
    struct _GLtexture *next;
} GLtexture;

GLtexture *textures = NULL;
GLtexture *texture1D = NULL;
GLtexture *texture2D = NULL;
u32 textureCount = 0;

GLuint glTextureNextFreeName ();
GLtexture *glTextureGet (GLuint _name);

#ifdef __cplusplus
}
#endif

#endif /* _GLGX_H_ */
