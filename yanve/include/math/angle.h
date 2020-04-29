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
inline YANVE_API T radians(T deg)
{
  return deg * pi_over_180<T>;
}

template <typename T>
inline YANVE_API const glm::tvec3<T>& radians(glm::tvec3<T> deg)
{
  return deg * pi_over_180<T>;
}

template <typename T>
inline YANVE_API T degrees(T rad)
{
  return rad * one_over_pi_time_180<T>;
}

template <typename T>
inline YANVE_API const glm::tvec3<T>& degrees(glm::tvec3<T> rad)
{
  return rad * one_over_pi_time_180<T>;
}

inline YANVE_API const glm::vec3& eulerAngles(glm::quat q)
{
  return degrees(glm::eulerAngles(q));
}

}