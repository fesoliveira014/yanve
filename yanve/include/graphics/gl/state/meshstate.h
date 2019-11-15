#pragma once

#include <common.h>
#include <graphics/gl/mesh.h>

namespace yanve::gl::state
{

struct MeshState
{
  explicit MeshState();
  ~MeshState();

  void reset();
  void resetVao();

  //GLuint defaultVAO;
  GLuint currentVAO;
};

}