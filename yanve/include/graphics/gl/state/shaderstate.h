#pragma once

#include <common.h>
#include <graphics/gl/shaderpipeline.h>

namespace yanve::gl::detail
{

struct ShaderState
{
  ShaderState();
  ~ShaderState();

  void reset();

  GLuint current;
};

}