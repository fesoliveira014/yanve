#pragma once

#include <common.h>

namespace yanve::math
{

struct AABB
{
  glm::vec3 min, max;

  AABB() : min{}, max{} {}
  AABB(const glm::vec3& min, const glm::vec3& max) : min{ min }, max{ max } {}
};

}