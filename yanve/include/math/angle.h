#pragma once

#include <cmath>
#include <glm/gtc/constants.hpp>
#include <math/constants.h>

namespace yanve::math
{

template <typename T>
inline T radians(T deg)
{
  return deg * pi_over_180<T>;
}

template <typename T>
inline T degrees(T rad)
{
  return rad * one_over_pi_time_180<T>;
}

}