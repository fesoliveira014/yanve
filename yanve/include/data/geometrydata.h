#pragma once

#include <common.h>
#include <graphics/gl/mesh.h>

namespace yanve::data
{

struct GeometryData 
{
public:
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec4> colors;
  std::vector<uint> indices;

  gl::MeshPrimitive primitive;

  GeometryData() : primitive{ gl::MeshPrimitive::Triangles } {}
  GeometryData(gl::MeshPrimitive primitive) : primitive{primitive} {}
};

}