#pragma once

#include <core/types.h>
#include <glm/detail/type_vec2.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_vec4.hpp>

namespace yanve::utils
{

template <size_t dimension, typename T>
struct dimensionsHelper 
{
  dimensionsHelper() = delete;
};

template <size_t dimension, typename T> using vecTypeFor = typename dimensionsHelper<dimension, T>::value;

template <>
struct dimensionsHelper<1, uint>
{
  dimensionsHelper() = delete;
  typedef uint value;
};

template <>
struct dimensionsHelper<1, i32>
{
  dimensionsHelper() = delete;
  typedef i32 value;
};

template <>
struct dimensionsHelper<1, f32>
{
  dimensionsHelper() = delete;
  typedef f32 value;
};

template <>
struct dimensionsHelper<2, uint>
{
  dimensionsHelper() = delete;
  typedef glm::uvec2 value;
};

template <>
struct dimensionsHelper<2, i32>
{
  dimensionsHelper() = delete;
  typedef glm::ivec2 value;
};

template <>
struct dimensionsHelper<2, f32>
{
  dimensionsHelper() = delete;
  typedef glm::vec2 value;
};
template <>
struct dimensionsHelper<3, uint>
{
  dimensionsHelper() = delete;
  typedef glm::uvec3 value;
};

template <>
struct dimensionsHelper<3, i32>
{
  dimensionsHelper() = delete;
  typedef glm::ivec3 value;
};

template <>
struct dimensionsHelper<3, f32>
{
  dimensionsHelper() = delete;
  typedef glm::vec3 value;
};

}