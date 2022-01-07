#include <graphics/gl/meshview.h>

namespace yanve::gl
{

MeshView::MeshView(Mesh& mesh) : _mesh{mesh} 
{

}

MeshView::MeshView(const MeshView& other) : _mesh{ other._mesh }
{

}

MeshView& MeshView::setIndexRange(GLuint first, GLuint start, GLuint end)
{
  if (_mesh->_indexBuffer.id()) {
    _indexOffset = _mesh->_indexOffset + first * _mesh->indexTypeSize();
    _indexStart = start;
    _indexEnd = end;
  }
  return *this;
}

MeshView& MeshView::setIndexRange(GLuint first)
{
  if (_mesh->_indexBuffer.id()) {
    _indexOffset = _mesh->_indexOffset + first * _mesh->indexTypeSize();
  }
  return *this;
}

MeshView& MeshView::draw(ShaderPipeline& shader)
{
  shader.draw(*this);
  return *this;
}

}