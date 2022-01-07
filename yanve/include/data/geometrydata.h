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

class YANVE_API GeometryData
{
  typedef yanve::gl::Attribute<0, glm::vec3> Position;
  typedef yanve::gl::Attribute<1, glm::vec4> Color;
  typedef yanve::gl::Attribute<2, glm::vec3> Normals;
  typedef yanve::gl::Attribute<3, glm::vec2> TextureCoordinates;

public:
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec4> colors;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> uvs;
  std::vector<uint> indices;

  gl::MeshPrimitive primitive;

  GeometryData();
  GeometryData(gl::MeshPrimitive primitive);

  void updateBuffers();

  void toMesh(gl::Mesh& mesh);

private:
  void createBuffers();

  inline bool created(MeshAttribute bufferId)
  {
    return (bool)(_defaultBuffers[bufferId].flags() & gl::ObjectFlag::Created);
  }
  
private:
  std::vector<gl::Buffer> _defaultBuffers;
  gl::Buffer _indexBuffer{ NoCreate };
};

}