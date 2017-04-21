#include "projectM.hpp"
#include "Renderer.hpp"
#include "wipemalloc.h"
#include "math.h"
#include "Common.hpp"
#include "TextureManager.hpp"
#include <iostream>
#include <algorithm>
#include <cassert>
#include "omptl/omptl"
#include "omptl/omptl_algorithm"
#include "UserTexture.hpp"

class Preset;

Renderer::Renderer(const pm_config &settings, BeatDetect *beatDetect) : 
settings(settings), mesh(settings.meshX, settings.meshY), beatDetect(beatDetect)
{
	//int x;
	//int y;

	this->totalframes = 1;
	this->noSwitch = false;
	this->realfps = 0;

	/** Other stuff... */
	this->aspect = (float) settings.windowHeight / (float) settings.windowWidth;

    gxInit();
    
	/// @bug put these on member init list
    this->renderTarget = new RenderTarget(settings.textureSize, settings.windowWidth, settings.windowHeight);
    this->textureManager = new TextureManager(settings.presetDirectory);
	this->beatDetect = beatDetect;


	int size = (mesh.height - 1) *mesh.width * 5 * 2;
	p = ( float * ) wipemalloc ( size * sizeof ( float ) );


	for (int j = 0; j < mesh.height - 1; j++)
	{
		int base = j * mesh.width * 2 * 5;


		for (int i = 0; i < mesh.width; i++)
		{
			int index = j * mesh.width + i;
			int index2 = (j + 1) * mesh.width + i;

			int strip = base + i * 10;
			p[strip + 2] = mesh.identity[index].x;
			p[strip + 3] = mesh.identity[index].y;
			p[strip + 4] = 0;

			p[strip + 7] = mesh.identity[index2].x;
			p[strip + 8] = mesh.identity[index2].y;
			p[strip + 9] = 0;
		}
	}


#ifdef USE_CG
	shaderEngine.setParams(renderTarget->texsize, renderTarget->textureID[1], aspect, beatDetect, textureManager);
#endif

}

void Renderer::SetPipeline(Pipeline &pipeline)
{
	currentPipe = &pipeline;
#ifdef USE_CG
	shaderEngine.reset();
	shaderEngine.loadShader(pipeline.warpShader);
	shaderEngine.loadShader(pipeline.compositeShader);
#endif
}

void Renderer::ResetTextures()
{
	textureManager->Clear();

	delete (renderTarget);
    renderTarget = new RenderTarget(settings.textureSize, settings.windowWidth, settings.windowHeight);
    reset();

	textureManager->Preload();
}

void Renderer::SetupPass1(const Pipeline &pipeline, const PipelineContext &pipelineContext)
{
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();

	totalframes++;
	renderTarget->lock();
	glViewport(0, 0, renderTarget->texsize, renderTarget->texsize);

	glEnable(GL_TEXTURE_2D);

	//If using FBO, switch to FBO texture

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, 1, 0.0, 1, -40, 40);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

#ifdef USE_CG
	shaderEngine.RenderBlurTextures(pipeline, pipelineContext, renderTarget->texsize);
#endif
}

void Renderer::RenderItems(const Pipeline &pipeline, const PipelineContext &pipelineContext)
{
	renderContext.time = pipelineContext.time;
    renderContext.texsize = settings.textureSize;
    renderContext.aspectCorrect = settings.aspectCorrection;
	renderContext.aspectRatio = aspect;
	renderContext.textureManager = textureManager;
	renderContext.beatDetect = beatDetect;

	for (std::vector<RenderItem*>::const_iterator pos = pipeline.drawables.begin(); pos != pipeline.drawables.end(); ++pos)
    {
      if (*pos != NULL)
      {
		(*pos)->Draw(renderContext);
      }
    }
}

void Renderer::FinishPass1()
{
	/** Restore original view state */
	//glMatrixMode(GL_MODELVIEW);
	//glPopMatrix();

	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();

	renderTarget->unlock();

}

void Renderer::Pass2(const Pipeline &pipeline, const PipelineContext &pipelineContext)
{
	//BEGIN PASS 2
	//
	//end of texture rendering
	//now we copy the texture from the FBO or framebuffer to
	//video texture memory and render fullscreen.

	/** Reset the viewport size */
#ifdef USE_FBO
	if (renderTarget->renderToTexture)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->renderTarget->fbuffer[1]);
		glViewport(0, 0, this->renderTarget->texsize, this->renderTarget->texsize);
	}
	else
#endif
		glViewport(0, 0, settings.windowWidth, settings.windowHeight);

	glBindTexture(GL_TEXTURE_2D, this->renderTarget->textureID[0]);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.5, 0.5, -0.5, 0.5, -40, 40);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(this->renderTarget->texsize < 512 ? 1 : this->renderTarget->texsize / 512.0);

	CompositeOutput(pipeline, pipelineContext);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.5, -0.5, 0);

	glTranslatef(0.5, 0.5, 0);

#ifdef USE_FBO
	if (renderTarget->renderToTexture)
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#endif
}

void Renderer::RenderFrame(const Pipeline &pipeline, const PipelineContext &pipelineContext)
{
#if defined(__wii__)
    switch (settings.pulseSource) {
        case PM_AC_NONE: wiiLightSetLevel(0); break;
        case PM_AC_BASS: wiiLightSetLevel(beatDetect->bass); break;
        case PM_AC_MIDDLE: wiiLightSetLevel(beatDetect->mid); break;
        case PM_AC_TREBLE: wiiLightSetLevel(beatDetect->treb); break;
        case PM_AC_VOLUME: wiiLightSetLevel(beatDetect->vol); break;
        default: wiiLightSetLevel(0); break;
    }
    if (settings.pulseWiiLight) {
        wiiLightOn();
    } else {
        wiiLightOff();
    }
#endif

	SetupPass1(pipeline, pipelineContext);

#ifdef USE_CG
	shaderEngine.enableShader(currentPipe->warpShader, pipeline, pipelineContext);
#endif
	Interpolation(pipeline);
#ifdef USE_CG
	shaderEngine.disableShader();
#endif

	RenderItems(pipeline, pipelineContext);
	FinishPass1();
	Pass2(pipeline, pipelineContext);
    
    glFlush();
    
    gxSwapBuffers();
    
}

void Renderer::Interpolation(const Pipeline &pipeline)
{
	if (this->renderTarget->useFBO)
		glBindTexture(GL_TEXTURE_2D, renderTarget->textureID[1]);
	else
		glBindTexture(GL_TEXTURE_2D, renderTarget->textureID[0]);

	//Texture wrapping( clamp vs. wrap)
	if (pipeline.textureWrap == 0)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

	glColor4f(1.0, 1.0, 1.0, pipeline.screenDecay);

	glEnable(GL_TEXTURE_2D);


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);


	//glVertexPointer(2, GL_FLOAT, 0, p);
	//glTexCoordPointer(2, GL_FLOAT, 0, t);
	glInterleavedArrays(GL_T2F_V3F,0,p);


	if (pipeline.staticPerPixel)
	{
		for (int j = 0; j < mesh.height - 1; j++)
		{
			int base = j * mesh.width * 2 * 5;

			for (int i = 0; i < mesh.width; i++)
			{
				int strip = base + i * 10;
				p[strip] = pipeline.x_mesh[i][j];
				p[strip + 1] = pipeline.y_mesh[i][j];

				p[strip + 5] = pipeline.x_mesh[i][j+1];
				p[strip + 6] = pipeline.y_mesh[i][j+1];
			}
		}

	}
	else
	{
		mesh.Reset();
		omptl::transform(mesh.p.begin(), mesh.p.end(), mesh.identity.begin(), mesh.p.begin(), &Renderer::PerPixel);

	for (int j = 0; j < mesh.height - 1; j++)
	{
		int base = j * mesh.width * 2 * 5;

		for (int i = 0; i < mesh.width; i++)
		{
			int strip = base + i * 10;
			int index = j * mesh.width + i;
			int index2 = (j + 1) * mesh.width + i;

			p[strip] = mesh.p[index].x;
			p[strip + 1] = mesh.p[index].y;

			p[strip + 5] = mesh.p[index2].x;
			p[strip + 6] = mesh.p[index2].y;


		}
	}

	}

	for (int j = 0; j < mesh.height - 1; j++)
		glDrawArrays(GL_TRIANGLE_STRIP,j* mesh.width* 2,mesh.width*2);


	glDisable(GL_TEXTURE_2D);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}
Pipeline* Renderer::currentPipe;

Renderer::~Renderer()
{
#if defined(__wii__)
    wiiLightOff();
#endif

	//int x;

	if (renderTarget)
		delete (renderTarget);
	if (textureManager)
		delete (textureManager);

    gxDestroy();
    
	//std::cerr << "grid assign end" << std::endl;

	free(p);

	//	std::cerr << "exiting destructor" << std::endl;
}

void Renderer::reset()
{    
#if defined(__wii__)
    wiiLightOff();
#endif

#if USE_CG
	shaderEngine.setAspect(aspect);
#endif

	glShadeModel(GL_SMOOTH);

	glCullFace(GL_BACK);
	//glFrontFace( GL_CCW );

	glClearColor(0, 0, 0, 0);

	glViewport(0, 0, settings.windowWidth, settings.windowHeight);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_POINT_SMOOTH);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineStipple(2, 0xAAAA);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	if (!this->renderTarget->useFBO)
	{
        this->renderTarget->fallbackRescale(settings.windowWidth, settings.windowHeight);
	}
}

GLuint Renderer::initRenderToTexture()
{
	return renderTarget->initRenderToTexture();
}

void Renderer::CompositeOutput(const Pipeline &pipeline, const PipelineContext &pipelineContext)
{

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Overwrite anything on the screen
	glBlendFunc(GL_ONE, GL_ZERO);
	glColor4f(1.0, 1.0, 1.0, 1.0f);

	glEnable(GL_TEXTURE_2D);

#ifdef USE_CG
	shaderEngine.enableShader(currentPipe->compositeShader, pipeline, pipelineContext);
#endif

	float tex[4][2] =
	{
	{ 0, 1 },
	{ 0, 0 },
	{ 1, 0 },
	{ 1, 1 } };

	float points[4][2] =
	{
	{ -0.5, -0.5 },
	{ -0.5, 0.5 },
	{ 0.5, 0.5 },
	{ 0.5, -0.5 } };

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, points);
	glTexCoordPointer(2, GL_FLOAT, 0, tex);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

#ifdef USE_CG
	shaderEngine.disableShader();
#endif

	for (std::vector<RenderItem*>::const_iterator pos = pipeline.compositeDrawables.begin(); pos
			!= pipeline.compositeDrawables.end(); ++pos)
		(*pos)->Draw(renderContext);

}

