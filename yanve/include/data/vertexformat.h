#pragma once

#include <common.h>

namespace yanve::data {

  enum class VertexFormat : unsigned int
  {
    Float = 1,
    UnsignedByte,
    Byte,
    UnsignedShort,
    Short,
    UnsignedInt,
    Int,
    Vector2,
    Vector2ub,
    Vector2b,
    Vector2us,
    Vector2s,
    Vector2ui,
    Vector2i,
    Vector3,
    Vector3ub,
    Vector3b,
    Vector3us,
    Vector3s,
    Vector3ui,
    Vector3i,
    Vector4,
    Vector4ub,
    Vector4b,
    Vector4us,
    Vector4s,
    Vector4ui,
    Vector4i,
    Matrix2x2,
    Matrix2x3,
    Matrix2x4,
    Matrix3x2,
    Matrix3x3,
    Matrix3x4,
    Matrix4x2,
    Matrix4x3,
    Matrix4x4,
    None
  };

  size_t vertexFormatSize(const VertexFormat format);
  unsigned int vertexFormatComponentCount(const VertexFormat format);
  VertexFormat vertexFormatComponentFormat(const VertexFormat format);
  unsigned int vertexFormatVectorCount(const VertexFormat format);
  unsigned int vertexFormatVectorStride(const VertexFormat format);

}
