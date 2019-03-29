#pragma once

#include <common.h>

/**
 This code was addapted from the brilliant solution used in Magnum:
 https://github.com/mosra/magnum/blob/master/src/Magnum/GL/Attribute.h
*/

namespace yanve::gl
{

namespace implementation { template <class T> struct Attribute {}; }

template <unsigned int location, class T>
class Attribute
{
public:
  enum : size_t {
    Location = location,
    VectorCount = implementation::Attribute<T>::VectorCount
  };

  typedef T Type;
  typedef typename implementation::Attribute<T>::ScalarType ScalarType;
  typedef typename implementation::Attribute<T>::Components Components;
  typedef typename implementation::Attribute<T>::DataType DataType;
  typedef typename implementation::Attribute<T>::DataOption DataOption;

  constexpr Attribute(Components components, DataType dataType = implementation::Attribute<T>::DefaultDataType, DataOption dataOption = implementation::Attribute<T>::DefaultDataOption) : _components{components}, _dataType{dataType}, _dataOption{dataOption} {}
  constexpr Attribute(DataType dataType = implementation::Attribute<T>::DefaultDataType, DataOption dataOption = implementation::Attribute<T>::DefaultDataOption) : _components{ implementation::Attribute<T>::DefaultComponents }, _dataType{ dataType }, _dataOption{ dataOption } {}

  constexpr Components components() { return _components; }
  constexpr DataType dataType() { return _dataType; }
  constexpr DataOption dataOption() { return _dataOption; }

  size_t size() const
  {
    return implementation::Attribute<T>::size(GLuint(_components), _dataType);
  }

private:
  Components _components;
  DataType _dataType;
  DataOption _dataOption;
};

namespace implementation
{

template <size_t cols, size_t rows> struct SizedAttribute{};
template <size_t cols> struct SizedVectorAttribute { enum : size_t { VectorCount = cols }; };

template <> struct SizedAttribute<1,1> : SizedVectorAttribute<1>
{
  enum class Components : GLint { One = 1 };
  constexpr static Components DefaultComponents = Components::One;
};

template <> struct SizedAttribute<1, 2> : SizedVectorAttribute<2>
{
  enum class Components : GLint { One = 1, Two = 2 };
  constexpr static Components DefaultComponents = Components::Two;
};

template <> struct SizedAttribute<1, 3> : SizedVectorAttribute<3>
{
  enum class Components : GLint { One = 1 , Two = 2, Three = 3 };
  constexpr static Components DefaultComponents = Components::Three;
};

template <> struct SizedAttribute<1, 4> : SizedVectorAttribute<4>
{
  enum class Components : GLint { One = 1, Two = 2, Three = 3, Four = 4 };
  constexpr static Components DefaultComponents = Components::Four;
};

template<std::size_t rows> struct SizedMatrixAttribute;

template<> struct SizedMatrixAttribute<2> 
{
  enum class Components : GLint { Two = 2 };
  constexpr static Components DefaultComponents = Components::Two;
};

template<> struct SizedMatrixAttribute<3> 
{
  enum class Components : GLint { Three = 3 };
  constexpr static Components DefaultComponents = Components::Three;
};

template<> struct SizedMatrixAttribute<4> 
{
  enum class Components : GLint { Four = 4 };
  constexpr static Components DefaultComponents = Components::Four;
};

template<> struct SizedAttribute<2, 2> : SizedVectorAttribute<2>, SizedMatrixAttribute<2> {};
template<> struct SizedAttribute<3, 3> : SizedVectorAttribute<3>, SizedMatrixAttribute<3> {};
template<> struct SizedAttribute<4, 4> : SizedVectorAttribute<4>, SizedMatrixAttribute<4> {};
template<> struct SizedAttribute<2, 3> : SizedVectorAttribute<2>, SizedMatrixAttribute<3> {};
template<> struct SizedAttribute<3, 2> : SizedVectorAttribute<3>, SizedMatrixAttribute<2> {};
template<> struct SizedAttribute<2, 4> : SizedVectorAttribute<2>, SizedMatrixAttribute<4> {};
template<> struct SizedAttribute<4, 2> : SizedVectorAttribute<4>, SizedMatrixAttribute<2> {};
template<> struct SizedAttribute<3, 4> : SizedVectorAttribute<3>, SizedMatrixAttribute<4> {};
template<> struct SizedAttribute<4, 3> : SizedVectorAttribute<4>, SizedMatrixAttribute<3> {};

//template <class> struct Attribute{};

struct FloatAttribute
{
  typedef float ScalarType;

  enum class DataType
  {
    Byte = GL_BYTE,
    UnsignedByte = GL_UNSIGNED_BYTE,
    Short = GL_SHORT,
    UnsignedShort = GL_UNSIGNED_SHORT,
    Int = GL_INT,
    UnsignedInt = GL_UNSIGNED_INT,
    HalfFloat = GL_HALF_FLOAT,
    Float = GL_FLOAT,
    Double = GL_DOUBLE,
  };
  constexpr static DataType DefaultDataType = DataType::Float;

  enum class DataOption : unsigned char {
    Default = 0,
    Normalized = 1
  };

  constexpr static DataOption DefaultDataOption = DataOption::Default;
  static size_t YANVE_API size(GLuint components, DataType dataType);  
};

struct IntAttribute
{
  typedef int ScalarType;

  enum class DataType
  {
    Byte = GL_BYTE,
    UnsignedByte = GL_UNSIGNED_BYTE,
    Short = GL_SHORT,
    UnsignedShort = GL_UNSIGNED_SHORT,
    Int = GL_INT,
    UnsignedInt = GL_UNSIGNED_INT
  };
  constexpr static DataType DefaultDataType = DataType::Int;

  enum class DataOption : unsigned char {
    Default = 0
  };

  constexpr static DataOption DefaultDataOption = DataOption::Default;

  static size_t YANVE_API size(GLuint components, DataType dataType);
};

struct UnsignedIntAttribute
{
  typedef unsigned int ScalarType;

  typedef IntAttribute::DataType DataType;
  constexpr static DataType DefaultDataType = DataType::UnsignedInt;

  typedef IntAttribute::DataOption DataOption;
  constexpr static DataOption DefaultDataOption = DataOption::Default;

  static size_t YANVE_API size(GLuint components, DataType dataType)
  {
    return IntAttribute::size(components, dataType);
  }
};

struct DoubleAttribute
{
  typedef double ScalarType;

  enum class DataType
  {
    Double = GL_DOUBLE,
  };
  constexpr static DataType DefaultDataType = DataType::Double;

  typedef IntAttribute::DataOption DataOption;
  constexpr static DataOption DefaultDataOption = DataOption::Default;

  static size_t YANVE_API size(GLuint components, DataType dataType);
};

template <> struct Attribute<int> : IntAttribute {};
template <> struct Attribute<unsigned int> : UnsignedIntAttribute {};
template <> struct Attribute<float> : FloatAttribute {};
template <> struct Attribute<double> : DoubleAttribute {};

template <> struct Attribute<glm::ivec2> : IntAttribute, SizedAttribute<1, 2> {};
template <> struct Attribute<glm::ivec3> : IntAttribute, SizedAttribute<1, 3> {};
template <> struct Attribute<glm::ivec4> : IntAttribute, SizedAttribute<1, 4> {};
template <> struct Attribute<glm::uvec2> : UnsignedIntAttribute, SizedAttribute<1, 2> {};
template <> struct Attribute<glm::uvec3> : UnsignedIntAttribute, SizedAttribute<1, 3> {};
template <> struct Attribute<glm::uvec4> : UnsignedIntAttribute, SizedAttribute<1, 4> {};
template <> struct Attribute<glm::vec2> : FloatAttribute, SizedAttribute<1, 2> {};
template <> struct Attribute<glm::vec3> : FloatAttribute, SizedAttribute<1, 3> {};
template <> struct Attribute<glm::vec4> : FloatAttribute, SizedAttribute<1, 4> {};
template <> struct Attribute<glm::dvec2> : DoubleAttribute, SizedAttribute<1, 2> {};
template <> struct Attribute<glm::dvec3> : DoubleAttribute, SizedAttribute<1, 3> {};
template <> struct Attribute<glm::dvec4> : DoubleAttribute, SizedAttribute<1, 4> {};

template <> struct Attribute<glm::mat3> : FloatAttribute, SizedAttribute<3, 3> {};
template <> struct Attribute<glm::mat4> : FloatAttribute, SizedAttribute<4, 4> {};
}

}