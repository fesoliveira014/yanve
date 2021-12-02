#include <data/vertexformat.h>
#include <utils/logger.h>

namespace yanve::data
{

size_t vertexFormatSize(const VertexFormat format)
{
  switch (format) {
  case VertexFormat::UnsignedByte:
  case VertexFormat::Byte:
    return 1;

  case VertexFormat::UnsignedShort:
  case VertexFormat::Short:
  case VertexFormat::Vector2b:
  case VertexFormat::Vector2ub:
    return 2;

  case VertexFormat::Vector3ub:
  case VertexFormat::Vector3b:
    return 3;

  case VertexFormat::Float:
  case VertexFormat::UnsignedInt:
  case VertexFormat::Int:
  case VertexFormat::Vector2us:
  case VertexFormat::Vector2s:
  case VertexFormat::Vector2ui:
  case VertexFormat::Vector4ub:
  case VertexFormat::Vector4b:
    return 4;

  case VertexFormat::Vector3us:
  case VertexFormat::Vector3s:
    return 6;

  case VertexFormat::Vector2:
  case VertexFormat::Vector2i:
  case VertexFormat::Vector4us:
  case VertexFormat::Vector4s:
    return 8;

  case VertexFormat::Vector3:
  case VertexFormat::Vector3ui:
  case VertexFormat::Vector3i:
    return 12;

  case VertexFormat::Vector4:
  case VertexFormat::Vector4ui:
  case VertexFormat::Vector4i:
  case VertexFormat::Matrix2x2:
    return 16;

  case VertexFormat::Matrix2x3:
  case VertexFormat::Matrix3x2:
    return 24;
    
  case VertexFormat::Matrix2x4:
  case VertexFormat::Matrix4x2:
    return 32;

  case VertexFormat::Matrix3x3:
    return 36;

  case VertexFormat::Matrix3x4:
  case VertexFormat::Matrix4x3:
    return 48;

  case VertexFormat::Matrix4x4:
    return 64;

  default:
    LogError(__func__, "Invalid vertex format: %d", format);
    return 0;
  }


}

unsigned int vertexFormatComponentCount(const VertexFormat format)
{
  switch (format) {
  case VertexFormat::Float:
  case VertexFormat::UnsignedByte:
  case VertexFormat::Byte:
  case VertexFormat::UnsignedShort:
  case VertexFormat::Short:
  case VertexFormat::UnsignedInt:
  case VertexFormat::Int:
    return 1;
  
  case VertexFormat::Vector2:
  case VertexFormat::Vector2ub:
  case VertexFormat::Vector2b:
  case VertexFormat::Vector2us:
  case VertexFormat::Vector2s:
  case VertexFormat::Vector2ui:
  case VertexFormat::Vector2i:
  case VertexFormat::Matrix2x2:
  case VertexFormat::Matrix3x2:
  case VertexFormat::Matrix4x2:
    return 2;
  
  case VertexFormat::Vector3:
  case VertexFormat::Vector3ub:
  case VertexFormat::Vector3b:
  case VertexFormat::Vector3us:
  case VertexFormat::Vector3s:
  case VertexFormat::Vector3ui:
  case VertexFormat::Vector3i:
  case VertexFormat::Matrix2x3:
  case VertexFormat::Matrix3x3:
  case VertexFormat::Matrix4x3:
    return 3;

  case VertexFormat::Vector4:
  case VertexFormat::Vector4ub:
  case VertexFormat::Vector4b:
  case VertexFormat::Vector4us:
  case VertexFormat::Vector4s:
  case VertexFormat::Vector4ui:
  case VertexFormat::Vector4i:
  case VertexFormat::Matrix2x4:
  case VertexFormat::Matrix3x4:
  case VertexFormat::Matrix4x4:
    return 4;

  default:
    LogError(__func__, "Invalid vertex format: %d", format);
    return 0;
  }
}

VertexFormat vertexFormatComponentFormat(const VertexFormat format)
{
  switch (format) {
  case VertexFormat::Float:
  case VertexFormat::Vector2:
  case VertexFormat::Vector3:
  case VertexFormat::Vector4:
  case VertexFormat::Matrix2x2:
  case VertexFormat::Matrix2x3:
  case VertexFormat::Matrix2x4:
  case VertexFormat::Matrix3x2:
  case VertexFormat::Matrix3x3:
  case VertexFormat::Matrix3x4:
  case VertexFormat::Matrix4x2:
  case VertexFormat::Matrix4x3:
  case VertexFormat::Matrix4x4:
    return VertexFormat::Float;

  case VertexFormat::UnsignedByte:
  case VertexFormat::Vector2ub:
  case VertexFormat::Vector3ub:
  case VertexFormat::Vector4ub:
    return VertexFormat::UnsignedByte;

  case VertexFormat::Byte:
  case VertexFormat::Vector2b:
  case VertexFormat::Vector3b:
  case VertexFormat::Vector4b:
    return VertexFormat::Byte;

  case VertexFormat::UnsignedShort:
  case VertexFormat::Vector2us:
  case VertexFormat::Vector3us:
  case VertexFormat::Vector4us:
    return VertexFormat::UnsignedShort;

  case VertexFormat::Short:
  case VertexFormat::Vector2s:
  case VertexFormat::Vector3s:
  case VertexFormat::Vector4s:
    return VertexFormat::Short;

  case VertexFormat::UnsignedInt:
  case VertexFormat::Vector2ui:
  case VertexFormat::Vector3ui:
  case VertexFormat::Vector4ui:
    return VertexFormat::UnsignedInt;

  case VertexFormat::Int:
  case VertexFormat::Vector2i:
  case VertexFormat::Vector3i:
  case VertexFormat::Vector4i:
    return VertexFormat::Int;

  default:
    LogError(__func__, "Invalid vertex format: %d", format);
    return VertexFormat::None;
  }
}

unsigned int vertexFormatVectorCount(const VertexFormat format)
{
  switch (format) {
  case VertexFormat::Float:
  case VertexFormat::UnsignedByte:
  case VertexFormat::Byte:
  case VertexFormat::UnsignedShort:
  case VertexFormat::Short:
  case VertexFormat::UnsignedInt:
  case VertexFormat::Int:
  case VertexFormat::Vector2:
  case VertexFormat::Vector2ub:
  case VertexFormat::Vector2b:
  case VertexFormat::Vector2us:
  case VertexFormat::Vector2s:
  case VertexFormat::Vector2ui:
  case VertexFormat::Vector2i:
  case VertexFormat::Vector3:
  case VertexFormat::Vector3ub:
  case VertexFormat::Vector3b:
  case VertexFormat::Vector3us:
  case VertexFormat::Vector3s:
  case VertexFormat::Vector3ui:
  case VertexFormat::Vector3i:
  case VertexFormat::Vector4:
  case VertexFormat::Vector4ub:
  case VertexFormat::Vector4b:
  case VertexFormat::Vector4us:
  case VertexFormat::Vector4s:
  case VertexFormat::Vector4ui:
  case VertexFormat::Vector4i:
    return 1;

  case VertexFormat::Matrix2x2:
  case VertexFormat::Matrix2x3:
  case VertexFormat::Matrix2x4:
    return 2;

  case VertexFormat::Matrix3x2:
  case VertexFormat::Matrix3x3:
  case VertexFormat::Matrix3x4:
    return 3;

  case VertexFormat::Matrix4x2:
  case VertexFormat::Matrix4x3:
  case VertexFormat::Matrix4x4:
    return 4;
  
  default:
    LogError(__func__, "Invalid vertex format: %d", format);
    return 0;
  }
}

unsigned int vertexFormatVectorStride(const VertexFormat format)
{
  switch (format) {
  case VertexFormat::UnsignedByte:
  case VertexFormat::Byte:
    return 1;

  case VertexFormat::UnsignedShort:
  case VertexFormat::Short:
  case VertexFormat::UnsignedInt:
  case VertexFormat::Vector2ub:
  case VertexFormat::Vector2b:
    return 2;

  case VertexFormat::Vector3ub:
  case VertexFormat::Vector3b:
    return 3;

  case VertexFormat::Float:
  case VertexFormat::Int:
  case VertexFormat::Vector2us:
  case VertexFormat::Vector2s:
  case VertexFormat::Vector4ub:
  case VertexFormat::Vector4b:
    return 4;

  case VertexFormat::Vector3us:
  case VertexFormat::Vector3s:
    return 6;

  case VertexFormat::Vector2:
  case VertexFormat::Vector2ui:
  case VertexFormat::Vector2i:
  case VertexFormat::Vector4us:
  case VertexFormat::Vector4s:
  case VertexFormat::Matrix2x2:
  case VertexFormat::Matrix3x2:
  case VertexFormat::Matrix4x2:
    return 8;

  case VertexFormat::Vector3:
  case VertexFormat::Vector3ui:
  case VertexFormat::Vector3i:
  case VertexFormat::Matrix2x3:
  case VertexFormat::Matrix3x3:
  case VertexFormat::Matrix4x3:
    return 12;

  case VertexFormat::Vector4:
  case VertexFormat::Vector4ui:
  case VertexFormat::Vector4i:
  case VertexFormat::Matrix2x4:
  case VertexFormat::Matrix3x4:
  case VertexFormat::Matrix4x4:
    return 16;

  default:
    LogError(__func__, "Invalid vertex format: %d", format);
    return 0;
  }
}

}