#pragma once

#include <common.h>

#include <graphics/gl/mesh.h>
#include <graphics/gl/buffer.h>
#include <graphics/gl/globject.h>
#include <graphics/gl/attribute.h>
#include <utils/logger.h>

namespace yanve::data
{

enum MeshAttribute : unsigned short {
  Position = 0,
  Color, 
  Normal,
  TextureCoordinate,
  Tangent,
  Bitangent,
  ObjectId,

  // only add here if default attribute

  NumDefaultAttributes,

  Custom = 32768

  // add custom here
};

struct YANVE_API GeometryData
{
public:
  std::vector<glm::vec3> vertices{};
  std::vector<glm::vec4> colors{};
  std::vector<glm::vec3> normals{};
  std::vector<glm::vec2> uvs{};
  std::vector<uint> indices{};

  gl::MeshPrimitive primitive;

  explicit GeometryData();
  explicit GeometryData(gl::MeshPrimitive primitive);

  explicit GeometryData(const GeometryData& other);
  GeometryData(GeometryData&& other) noexcept;

  ~GeometryData() = default;

  GeometryData& operator=(const GeometryData& rhs);
  GeometryData& operator=(GeometryData&& other) noexcept;

};

}