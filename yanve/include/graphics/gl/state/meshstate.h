#pragma once

#include <common.h>
#include <graphics/gl/mesh.h>

namespace yanve::gl::detail
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