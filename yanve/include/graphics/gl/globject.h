#pragma once

#include <common.h>
#include <utils/enumoperators.h>

namespace yanve::gl
{
enum ObjectFlags : uint32_t
{
  Created = 1 << 0,
  DestroyOnDestruction = 1 << 1
};



//inline ObjectFlags operator |(ObjectFlags a, ObjectFlags b) { return static_cast<ObjectFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b)); }
//inline ObjectFlags operator &(ObjectFlags a, ObjectFlags b) { return static_cast<ObjectFlags>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b)); }
//inline ObjectFlags& operator |=(ObjectFlags& a, ObjectFlags b) { return a = a | b; }
//inline ObjectFlags& operator &=(ObjectFlags& a, ObjectFlags b) { return a = a | b; }

class YANVE_API GLObject
{
public:
  GLObject() = default;
  ~GLObject() = default;

};

ENUM_OPERATORS(ObjectFlags, uint32_t)

}