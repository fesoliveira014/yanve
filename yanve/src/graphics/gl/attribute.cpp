#include <graphics/gl/attribute.h>

namespace yanve::gl
{

GLsizei implementation::FloatAttribute::size(GLuint components, DataType dataType)
{
  switch (dataType) {
  case DataType::UnsignedByte:
  case DataType::Byte:
    return components;
  case DataType::UnsignedShort:
  case DataType::Short:
  case DataType::HalfFloat:
    return 2U * components;
  case DataType::UnsignedInt:
  case DataType::Int:
  case DataType::Float:
    return 4U * components;
  case DataType::Double:
    return 8U * components;
  }
}

GLsizei implementation::IntAttribute::size(GLuint components, DataType dataType)
{
  switch (dataType) {
  case DataType::UnsignedByte:
  case DataType::Byte:
    return components;
  case DataType::UnsignedShort:
  case DataType::Short:
    return 2U * components;
  case DataType::UnsignedInt:
  case DataType::Int:
    return 4U * components;
  }
}

GLsizei implementation::DoubleAttribute::size(GLuint components, DataType dataType)
{
  switch (dataType) {
  case DataType::Double:
    return 8U * components;
  }
}

}

