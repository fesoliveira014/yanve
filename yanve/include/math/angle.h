#pragma once

#include <common.h>
#include <cmath>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <math/constants.h>

namespace yanve::math
{

template <typename T>
YANVE_API inline T radians(const T& deg)
{
  return deg * pi_over_180<T>;
}

template <typename T>
YANVE_API inline const glm::tvec3<T>& radians(const glm::tvec3<T>& deg)
{
  return deg * pi_over_180<T>;
}

template <typename T>
YANVE_API inline T degrees(T& rad)
{
  return rad * one_over_pi_time_180<T>;
}

template <typename T>
YANVE_API inline const glm::tvec3<T>& degrees(const glm::tvec3<T>& rad)
{
  return rad * one_over_pi_time_180<T>;
}

YANVE_API inline const glm::vec3& eulerAngles(const glm::quat& q)
{
  return degrees(glm::eulerAngles(q));
}

YANVE_API inline const glm::quat& eulerToQuat(const glm::vec3& v)
{
  glm::vec3 euler = v * math::pi_over_180<float>;
  glm::quat roll{ cosf(euler.x * 0.5f), sinf(euler.x * 0.5f), 0, 0 };
  glm::quat pitch{ cosf(euler.y * 0.5f), 0, sinf(euler.y * 0.5f), 0 };
  glm::quat yaw{ cosf(euler.z * 0.5f), 0, 0, sinf(euler.z * 0.5f) };
  return pitch * roll * yaw;
}

}