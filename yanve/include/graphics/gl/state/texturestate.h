#pragma once

#include <common.h>
#include <graphics/gl/abstracttexture.h>

namespace yanve::gl::detail
{

struct TextureState
{
  TextureState();
  ~TextureState();

  void reset();

  // TODO: implement getting these
  /*GLint maxSize, max3DSize, maxCubeMapSize, maxRectangleSize;
  GLint maxBufferSize;
  GLfloat maxLodBias;
  GLint maxColorSamples, maxDepthSamples, maxIntegerSamples;*/
  GLint maxTextureUnits;
  GLint currentTextureUnit;
  GLint bufferOffsetAlignment;
  
  std::vector<std::pair<GLenum, GLint>> bindings;
  /* Texture object ID, level, layered, layer, access */
  std::vector<std::tuple<GLuint, GLint, GLboolean, GLint, GLenum>> imageBindings;
};

}