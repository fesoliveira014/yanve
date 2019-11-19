#pragma once

#include <common.h>
#include <graphics/gl/buffer.h>

namespace yanve::gl::detail
{

struct BufferState
{
  enum : std::size_t {
    TargetCount = 13 + 1
  };

  BufferState();

  static std::size_t indexForTarget(Buffer::Target target);
  static Buffer::Target targetForIndex[TargetCount-1];

  void reset();

  GLuint bindings[TargetCount];
};

}