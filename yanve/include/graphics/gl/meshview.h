#pragma once

#include <common.h>
#include <graphics/gl/mesh.h>
#include <graphics/gl/shaderpipeline.h>

namespace yanve::gl
{
  
class YANVE_API MeshView
{
public:
  MeshView(Mesh& mesh);

  Mesh& mesh() { return *_mesh; }
  const Mesh& mesh() const { return *_mesh; }

  GLsizei count() { return _count; }
  MeshView& setCount(GLsizei count)
  {
    _count = count;
    return *this;
  }

  GLuint baseVertex() { return _baseVertex; }
  MeshView& setBaseVertex(GLuint baseVertex)
  {
    _baseVertex = baseVertex;
    return *this;
  }

  inline MeshView& setIndexRange(GLuint first, GLuint start, GLuint end);
  MeshView& setIndexRange(GLuint first);

  GLuint instanceCount() { return _instanceCount; }
  MeshView& setInstanceCount(GLuint instanceCount)
  {
    _instanceCount = instanceCount;
    return *this;
  }

  GLuint baseInstance() { return _baseInstance; }
  MeshView& setBaseInstance(GLuint baseInstance)
  {
    _baseInstance = baseInstance;
    return *this;
  }

  MeshView& draw(ShaderPipeline& shader);

private:
  friend class ShaderPipeline;

  // todo: maybe change to std::optional<std::reference_wrapper<Mesh>>
  optional_ref<Mesh> _mesh;
  GLsizei _count;
  GLuint _baseVertex;
  GLuint _instanceCount{ 1 };
  GLuint _baseInstance;
  GLintptr _indexOffset;
  GLsizei _indexStart, _indexEnd;
};
}