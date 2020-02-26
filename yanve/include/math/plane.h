#pragma once

#include <common.h>

namespace yanve::math
{

struct Plane
{
  glm::vec3 normal;
  float scalar;

  explicit Plane() = default;
  explicit Plane(glm::vec3 normal, float scalar) : normal{ normal }, scalar{ scalar } {}

  Plane(float a, float b, float c, float d)
  {
    normal = glm::normalize(glm::vec3(a, b, c));
    scalar = d / normal.length();
  }

  Plane(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
  {
    normal = v1 - v0;
    normal = glm::cross(normal, v2 - v0);
    normal = glm::normalize(normal);
    scalar = -glm::dot(normal, v2);
  }

  Plane(const Plane&) = default;
  Plane(Plane&&) = default;
  inline Plane& operator=(const Plane& other) = default;
  inline Plane& operator=(Plane&& other) = default;
};

}