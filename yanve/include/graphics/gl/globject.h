#pragma once

#include <common.h>

namespace yanve::gl
{
class YANVE_API GLObject
{
protected:
  enum Flags : uint32_t
  {
    Created = 1 << 0,
    DestroyOnDestruction = 1 << 1
  };

  uint32_t _flags;

public:
  GLObject() = default;
  ~GLObject() = default;

};
}