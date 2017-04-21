#ifndef Renderer_HPP
#define Renderer_HPP

#include "FBO.hpp"
#include "BeatDetect.hpp"
#include <string>
#include <set>

#include "GL/gl.h"
#include "Gekko.h"

#include "Pipeline.hpp"
#include "PerPixelMesh.hpp"
#include "Transformation.hpp"
#include "ShaderEngine.hpp"

/* Forward declarations */
typedef struct _pm_config pm_config;
class UserTexture;
class BeatDetect;
class TextureManager;

class Renderer
{

public:

  bool noSwitch;

  int totalframes;
  float realfps;


  Renderer( const pm_config &settings, BeatDetect *beatDetect );
  ~Renderer();

  void RenderFrame(const Pipeline &pipeline, const PipelineContext &pipelineContext);
  void ResetTextures();
  void reset();
  GLuint initRenderToTexture();


  void SetPipeline(Pipeline &pipeline);

private:

  const pm_config &settings;
  PerPixelMesh mesh;
  RenderTarget *renderTarget;
  BeatDetect *beatDetect;
  TextureManager *textureManager;
  static Pipeline* currentPipe;
  RenderContext renderContext;
  //per pixel equation variables
#ifdef USE_CG
  ShaderEngine shaderEngine;
#endif
  
  float* p;

  float aspect;
  
  void SetupPass1(const Pipeline &pipeline, const PipelineContext &pipelineContext);
  void Interpolation(const Pipeline &pipeline);
  void RenderItems(const Pipeline &pipeline, const PipelineContext &pipelineContext);
  void FinishPass1();
  void Pass2 (const Pipeline &pipeline, const PipelineContext &pipelineContext);
  void CompositeOutput(const Pipeline &pipeline, const PipelineContext &pipelineContext);

  inline static Point PerPixel(Point p, PerPixelContext &context)
  {
	  return currentPipe->PerPixel(p,context);
  }

  void rescale_per_pixel_matrices();

};

#endif
