#include <data/geometrydata.h>

namespace yanve::data
{

GeometryData::GeometryData() :
  vertices{},
  colors{},
  normals{},
  uvs{},
  indices{},
  primitive{ gl::MeshPrimitive::Triangles }
{
}

GeometryData::GeometryData(gl::MeshPrimitive primitive) :
  vertices{},
  colors{},
  normals{},
  uvs{},
  indices{},
  primitive{ primitive }
{
}

GeometryData::GeometryData(const GeometryData& other) :
  vertices{other.vertices},
  colors{other.colors},
  normals{other.normals},
  uvs{other.uvs},
  indices{other.indices},
  primitive{other.primitive}
{
}

GeometryData::GeometryData(GeometryData&& other) noexcept :
  vertices{std::move(other.vertices)},
  colors{std::move(other.colors)},
  normals{std::move(other.normals)},
  uvs{std::move(other.uvs)},
  indices{std::move(other.indices)},
  primitive{other.primitive}
{

}

//GeometryData::~GeometryData()
//{
//  vertices.clear();
//  colors.clear();
//  normals.clear();
//  uvs.clear();
//  indices.clear();
//}

GeometryData& GeometryData::operator=(const GeometryData& rhs)
{
  this->vertices = rhs.vertices;
  this->colors = rhs.colors;
  this->normals = rhs.normals;
  this->uvs = rhs.uvs;
  this->indices = rhs.indices;
  this->primitive = rhs.primitive;

  return *this;
}

GeometryData& GeometryData::operator=(GeometryData&& other) noexcept
{
  this->vertices = std::move(other.vertices);
  this->colors = std::move(other.colors);
  this->normals = std::move(other.normals);
  this->uvs = std::move(other.uvs);
  this->indices = std::move(other.indices);
  this->primitive = std::move(other.primitive);

  return *this;
}

}