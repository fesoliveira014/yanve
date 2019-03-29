#include <graphics/gl/attribute.h>

namespace yanve::gl
{

size_t implementation::FloatAttribute::size(GLuint components, DataType dataType)
{
  switch (dataType) {
  case DataType::UnsignedByte:
  case DataType::Byte:
    return components;
  case DataType::UnsignedShort:
  case DataType::Short:
  case DataType::HalfFloat:
    return 2 * components;
  case DataType::UnsignedInt:
  case DataType::Int:
  case DataType::Float:
    return 4 * components;
  case DataType::Double:
    return 8 * components;
  }
}

size_t implementation::IntAttribute::size(GLuint components, DataType dataType)
{
  switch (dataType) {
  case DataType::UnsignedByte:
  case DataType::Byte:
    return components;
  case DataType::UnsignedShort:
  case DataType::Short:
    return 2 * components;
  case DataType::UnsignedInt:
  case DataType::Int:
    return 4 * components;
  }
}

size_t implementation::DoubleAttribute::size(GLuint components, DataType dataType)
{
  switch (dataType) {
  case DataType::Double:
    return 8 * components;
  }
}

}

