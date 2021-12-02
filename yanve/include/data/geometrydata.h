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

struct GeometryData 
{
  typedef yanve::gl::Attribute<0, glm::vec3> Position;
  typedef yanve::gl::Attribute<1, glm::vec4> Color;
  typedef yanve::gl::Attribute<2, glm::vec3> Normals;
  typedef yanve::gl::Attribute<3, glm::vec4> TextureCoordinates;

public:
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec4> colors;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> uvs;
  std::vector<uint> indices;

  gl::MeshPrimitive primitive;

  GeometryData() : 
    primitive{ gl::MeshPrimitive::Triangles }, 
    _defaultBuffers{ MeshAttribute::NumDefaultAttributes, gl::Buffer{NoCreate} }
  {}

  GeometryData(gl::MeshPrimitive primitive) : 
    primitive{primitive}, 
    _defaultBuffers{ MeshAttribute::NumDefaultAttributes, gl::Buffer{NoCreate} }
  {}

  void updateBuffers() 
  {
   // reset all data on vbos using data in attribute vectors
  }

  gl::Mesh toMesh() 
  {
    gl::Mesh mesh;

    // we need at least a position buffer in order to create a mesh
    if (!(_defaultBuffers[MeshAttribute::Position].flags() & gl::ObjectFlag::Created)) {
      LogError("GeometryData::toMesh", "Mesh requires at least a vertex buffer to be valid");
      return gl::Mesh{ NoCreate };
    }

    mesh.addBuffer(_defaultBuffers[MeshAttribute::Position], 0, Position{});

    if (_defaultBuffers[MeshAttribute::Color].flags() & gl::ObjectFlag::Created) {
      mesh.addBuffer(_defaultBuffers[MeshAttribute::Color], 0, Color{});
    }

    if (_defaultBuffers[MeshAttribute::Normal].flags() & gl::ObjectFlag::Created) {
      mesh.addBuffer(_defaultBuffers[MeshAttribute::Normal], 0, Normals{});
    }

    if (_defaultBuffers[MeshAttribute::TextureCoordinate].flags() & gl::ObjectFlag::Created) {
      mesh.addBuffer(_defaultBuffers[MeshAttribute::TextureCoordinate], 0, TextureCoordinates{});
    }

    mesh.setCount(vertices.size());

    if (_indexBuffer.flags() & gl::ObjectFlag::Created) {
      mesh.setIndexBuffer(_indexBuffer, 0, gl::Mesh::MeshIndexType::UnsignedInt);
    }
  }

private:
  // todo: find way to set buffer usage dynamicaly
  void createBuffers()
  {
    if (vertices.empty()) {
      LogError("GeometryData::createBuffers", "Mesh requires at least a vertex buffer to be valid");
      return;
    }

    // add position buffer to mesh
    gl::Buffer vertexBuffer{};
    vertexBuffer.setData(vertices.data(), vertices.size() * sizeof(glm::vec3), gl::BufferUsage::StaticDraw);
    _defaultBuffers[MeshAttribute::Position] = std::move(vertexBuffer);

    if (!colors.empty()) {
      gl::Buffer colorBuffer{};
      colorBuffer.setData(colors.data(), colors.size() * sizeof(glm::vec4), gl::BufferUsage::StaticDraw);
      _defaultBuffers[MeshAttribute::Color] = std::move(colorBuffer);
    }

    if (!normals.empty()) {
      gl::Buffer normalBuffer{};
      normalBuffer.setData(normals.data(), normals.size() * sizeof(glm::vec3), gl::BufferUsage::StaticDraw);
      _defaultBuffers[MeshAttribute::Normal] = std::move(normalBuffer);
    }

    if (!uvs.empty()) {
      gl::Buffer texBuffer{};
      texBuffer.setData(uvs.data(), uvs.size() * sizeof(glm::vec2), gl::BufferUsage::StaticDraw);
      _defaultBuffers[MeshAttribute::TextureCoordinate] = std::move(texBuffer);
    }

    if (!indices.empty()) {
      gl::Buffer indBuffer{ gl::Buffer::Target::ElementArray };
      indBuffer.setData(indices.data(), indices.size() * sizeof(uint), gl::BufferUsage::StaticDraw);
      _indexBuffer = std::move(indBuffer);
    }
  }

private:
  std::vector<gl::Buffer> _defaultBuffers;
  gl::Buffer _indexBuffer{ NoCreate };
};

}