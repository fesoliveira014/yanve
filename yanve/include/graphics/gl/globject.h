#pragma once

#include <common.h>

namespace yanve::gl
{
class GLObject
{
protected:
  enum GLFlags : uint32_t
  {
    Created = 1 << 0,
    DestroyOnDescrution = 1 << 1
  };

  uint32_t _flags;

public:
  GLObject() = default;
  ~GLObject() = default;

};
}