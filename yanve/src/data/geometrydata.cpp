#include <data/geometrydata.h>

namespace yanve::data
{

GeometryData::GeometryData() :
  primitive{ gl::MeshPrimitive::Triangles },
  _defaultBuffers{ }
{
  for (int i = 0; i < MeshAttribute::NumDefaultAttributes; ++i) {
    _defaultBuffers.push_back(std::move(gl::Buffer{ NoCreate }));
  }
}

GeometryData::GeometryData(gl::MeshPrimitive primitive) :
  primitive{ primitive },
  _defaultBuffers{  }
{
  for (int i = 0; i < MeshAttribute::NumDefaultAttributes; ++i) {
    _defaultBuffers.push_back(std::move(gl::Buffer{ NoCreate }));
  }
}

void GeometryData::updateBuffers() 
{
  createBuffers();
}

// mesh should be created manually, this was just for testing
void GeometryData::toMesh(gl::Mesh& mesh)
{
  // we need at least a position buffer in order to create a mesh
  if (!(_defaultBuffers[MeshAttribute::Position].flags() & gl::ObjectFlag::Created)) {
    LogError("GeometryData::toMesh", "Mesh requires at least a vertex buffer to be valid");
    mesh = std::move(gl::Mesh{ NoCreate });
  }

  mesh = std::move(gl::Mesh{ primitive });

  mesh.addBuffer(_defaultBuffers[MeshAttribute::Position], 0, Position{});

  if (created(MeshAttribute::Color)) {
    mesh.addBuffer(_defaultBuffers[MeshAttribute::Color], 0, Color{});
  }

  if (created(MeshAttribute::Normal)) {
    mesh.addBuffer(_defaultBuffers[MeshAttribute::Normal], 0, Normals{});
  }

  if (created(MeshAttribute::TextureCoordinate)) {
    mesh.addBuffer(_defaultBuffers[MeshAttribute::TextureCoordinate], 0, TextureCoordinates{});
  }

  mesh.setCount(vertices.size());

  if (_indexBuffer.flags() & gl::ObjectFlag::Created) {
    mesh.setIndexBuffer(_indexBuffer, 0, gl::Mesh::MeshIndexType::UnsignedInt);
  }
}

// todo: find way to set buffer usage dynamicaly
void GeometryData::createBuffers()
{
  if (vertices.empty()) {
    LogError("GeometryData::createBuffers", "Mesh requires at least a vertex buffer to be valid");
    return;
  }

  // add position buffer to mesh
  if (!created(MeshAttribute::Position)) {
    gl::Buffer vertexBuffer{};
    vertexBuffer.setData(vertices.data(), vertices.size() * sizeof(glm::vec3), gl::BufferUsage::StaticDraw);
    _defaultBuffers[MeshAttribute::Position] = std::move(vertexBuffer);
  }

  if (created(MeshAttribute::Color)) {
    if (!colors.empty()) {
      _defaultBuffers[MeshAttribute::Color].setData(colors.data(), colors.size() * sizeof(glm::vec4), gl::BufferUsage::StaticDraw);
    }
    else {
      _defaultBuffers[MeshAttribute::Color] = std::move(gl::Buffer{ NoCreate });
    }
  }
  else if (!colors.empty()) {
    gl::Buffer colorBuffer{};
    colorBuffer.setData(colors.data(), colors.size() * sizeof(glm::vec4), gl::BufferUsage::StaticDraw);
    _defaultBuffers[MeshAttribute::Color] = std::move(colorBuffer);
  }

  if (created(MeshAttribute::Normal)) {
    if (!normals.empty()) {
      _defaultBuffers[MeshAttribute::Normal].setData(normals.data(), normals.size() * sizeof(glm::vec3), gl::BufferUsage::StaticDraw);
    }
    else {
      _defaultBuffers[MeshAttribute::Normal] = std::move(gl::Buffer{ NoCreate });
    }
  }
  else if (!normals.empty()) {
    gl::Buffer normalBuffer{};
    normalBuffer.setData(normals.data(), normals.size() * sizeof(glm::vec3), gl::BufferUsage::StaticDraw);
    _defaultBuffers[MeshAttribute::Normal] = std::move(normalBuffer);
  }

  if (created(MeshAttribute::TextureCoordinate)) {
    if (!uvs.empty()) {
      _defaultBuffers[MeshAttribute::TextureCoordinate].setData(uvs.data(), uvs.size() * sizeof(glm::vec2), gl::BufferUsage::StaticDraw);
    }
    else {
      _defaultBuffers[MeshAttribute::TextureCoordinate] = std::move(gl::Buffer{ NoCreate });
    }
  }
  else if (!uvs.empty()) {
    gl::Buffer texBuffer{};
    texBuffer.setData(uvs.data(), uvs.size() * sizeof(glm::vec2), gl::BufferUsage::StaticDraw);
    _defaultBuffers[MeshAttribute::TextureCoordinate] = std::move(texBuffer);
  }

  if (_indexBuffer.flags() & gl::ObjectFlag::Created) {
    if (!indices.empty()) {
      _indexBuffer.setData(indices.data(), indices.size() * sizeof(uint), gl::BufferUsage::StaticDraw);
    }
    else {
      _indexBuffer = std::move(gl::Buffer{ NoCreate });
    }
  }
  else if (!indices.empty()) {
    gl::Buffer indBuffer{ gl::Buffer::Target::ElementArray };
    indBuffer.setData(indices.data(), indices.size() * sizeof(uint), gl::BufferUsage::StaticDraw);
    _indexBuffer = std::move(indBuffer);
  }
}

}