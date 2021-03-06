#include <graphics/gl/meshview.h>

namespace yanve::gl
{

MeshView::MeshView(Mesh& mesh) : _mesh{mesh} 
{

}

MeshView& MeshView::setIndexRange(GLuint first, GLuint start, GLuint end)
{
  if (_mesh.get()._indexBuffer.id()) {
    _indexOffset = _mesh.get()._indexOffset + first * _mesh.get().indexTypeSize();
    _indexStart = start;
    _indexEnd = end;
  }
  return *this;
}

MeshView& MeshView::setIndexRange(GLuint first)
{
  if (_mesh.get()._indexBuffer.id()) {
    _indexOffset = _mesh.get()._indexOffset + first * _mesh.get().indexTypeSize();
  }
  return *this;
}

MeshView& MeshView::draw(ShaderPipeline& shader)
{
  shader.draw(*this);
  return *this;
}

}