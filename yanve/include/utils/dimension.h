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

template <size_t dimension, typename T>
struct matrixDimensionsHelper
{
  matrixDimensionsHelper() = delete;
};

template <size_t dimension, typename T> using vecTypeFor = typename dimensionsHelper<dimension, T>::value;
template <size_t dimension, typename T> using matTypeFor = typename matrixDimensionsHelper<dimension, T>::value;

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

template <>
struct matrixDimensionsHelper<2, uint>
{
  matrixDimensionsHelper() = delete;
  typedef glm::mat<2, 2, glm::u32> value;
};

template <>
struct matrixDimensionsHelper<2, i32>
{
  matrixDimensionsHelper() = delete;
  typedef glm::mat<2, 2, glm::i32> value;
};

template <>
struct matrixDimensionsHelper<2, f32>
{
  matrixDimensionsHelper() = delete;
  typedef glm::mat<2, 2, glm::f32> value;
};

template <>
struct matrixDimensionsHelper<3, uint>
{
  matrixDimensionsHelper() = delete;
  typedef glm::mat<3, 3, glm::u32> value;
};

template <>
struct matrixDimensionsHelper<3, i32>
{
  matrixDimensionsHelper() = delete;
  typedef glm::mat<3, 3, glm::i32> value;
};

template <>
struct matrixDimensionsHelper<3, f32>
{
  matrixDimensionsHelper() = delete;
  typedef glm::mat<3, 3, glm::f32> value;
};

template <>
struct matrixDimensionsHelper<4, uint>
{
  matrixDimensionsHelper() = delete;
  typedef glm::mat<4, 4, glm::u32> value;
};

template <>
struct matrixDimensionsHelper<4, i32>
{
  matrixDimensionsHelper() = delete;
  typedef glm::mat<4, 4, glm::i32> value;
};

template <>
struct matrixDimensionsHelper<4, f32>
{
  matrixDimensionsHelper() = delete;
  typedef glm::mat<4, 4, glm::f32> value;
};
}