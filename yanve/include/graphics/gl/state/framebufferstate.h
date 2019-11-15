#pragma once

#include <common.h>
#include <graphics/gl/abstractframebuffer.h>

namespace yanve::gl::state
{

struct FramebufferState
{
  constexpr static const Rectangle2Di DisengagedViewport{ {}, {-1,-1} };

  explicit FramebufferState();
  
  void reset();

  GLuint readBinding, writeBinding, renderbufferBinding;
  Rectangle2Di viewport;
};

}