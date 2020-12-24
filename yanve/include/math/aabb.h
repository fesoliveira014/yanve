#pragma once

#include <common.h>

namespace yanve::math
{

struct YANVE_API AABB
{
  glm::vec3 min, max;

  AABB() : min{}, max{} {}
  AABB(const glm::vec3& min, const glm::vec3& max) : min{ min }, max{ max } {}
  AABB(const AABB& other) : min{ other.min }, max{ other.max } {}
  AABB(AABB&& other) noexcept : min{ other.min }, max{ other.max } {other.min = other.max = glm::vec3{ 0 }; }
  
  bool operator==(const AABB& rhs)
  {
    return min == rhs.min && max == rhs.max;
  }

  AABB& operator=(const AABB& rhs) 
  { 
    if (this != &rhs) {
      min = rhs.min;
      max = rhs.max;
    }
    return *this;
  }

  const glm::vec3& getCorner(uint corner) const
  {
    switch (corner) {
    case 0: 
      return glm::vec3{min.x, min.y, max.z};
    case 1: 
      return glm::vec3{max.x, min.y, max.z};
    case 2: 
      return glm::vec3{max.x, max.y, max.z};
    case 3: 
      return glm::vec3{min.x, max.y, max.z};
    case 4: 
      return glm::vec3{min.x, min.y, min.z};
    case 5: 
      return glm::vec3{max.x, min.y, min.z};
    case 6: 
      return glm::vec3{max.x, max.y, min.z};
    case 7: 
      return glm::vec3{min.x, max.y, min.z};
    default: 
      return glm::vec3{ 0 };
    }
  }

  void transform(const glm::mat4& mat) 
  {
    // Efficient algorithm for transforming an AABB, taken from Graphics Gems

    float minA[3] = { min.x, min.y, min.z }, minB[3];
    float maxA[3] = { max.x, max.y, max.z }, maxB[3];

    for (size_t i = 0; i < 3; ++i) {
      minB[i] = mat[3][i];
      maxB[i] = mat[3][i];

      for (size_t j = 0; j < 3; ++j) {
        float x = minA[j] * mat[j][i];
        float y = maxA[j] * mat[j][i];
        minB[i] += std::min(x, y);
        maxB[i] += std::max(x, y);
      }
    }

    min = glm::vec3(minB[0], minB[1], minB[2]);
    max = glm::vec3(maxB[0], maxB[1], maxB[2]);
  }

  bool makeUnion(const AABB& box)
  {
    bool changed = false;

    if (min == max) {
      changed = true;
      min = box.min;
      max = box.max;
    }
    else if (box.min != box.max) {
      if (box.min.x < min.x) { changed = true; min.x = box.min.x; }
      if (box.min.y < min.y) { changed = true; min.y = box.min.y; }
      if (box.min.z < min.z) { changed = true; min.z = box.min.z; }

      if (box.max.x > max.x) { changed = true; max.x = box.max.x; }
      if (box.max.y > max.y) { changed = true; max.y = box.max.y; }
      if (box.max.z > max.z) { changed = true; max.z = box.max.z; }
    }

    return changed;
  }
};

}