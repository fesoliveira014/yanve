#pragma once

#include <glm/gtc/constants.hpp>

namespace yanve::math
{

template <typename T>
constexpr YANVE_API T pi_over_180 = glm::pi<T>() / static_cast<T>(180);

template <typename T>
constexpr YANVE_API T one_over_pi_time_180 = glm::one_over_pi<T>() * static_cast<T>(180);

}