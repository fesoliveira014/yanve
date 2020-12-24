#pragma once

#include <common.h>
#include <utils/enumset.h>

namespace yanve::gl
{
enum class ObjectFlag : uint32_t
{
  Created = 1 << 0,
  DestroyOnDestruction = 1 << 1
};

typedef EnumSet <ObjectFlag> ObjectFlags;

class YANVE_API GLObject
{
public:
  GLObject() = default;
  ~GLObject() = default;

};

ENUMSET_OPERATORS(ObjectFlags)

}