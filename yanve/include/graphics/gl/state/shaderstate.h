#pragma once

#include <common.h>
#include <graphics/gl/shaderprogram.h>

namespace yanve::gl::state
{

struct ShaderState
{
  ShaderState();
  ~ShaderState();

  void reset();

  GLuint current;
};

}