#include <graphics/gl/mesh.h>
#include <graphics/gl/shaderpipeline.h>

#include <graphics/gl/context.h>
#include <graphics/gl/state/state.h>
#include <graphics/gl/state/meshstate.h>
#include <graphics/gl/state/bufferstate.h>

namespace yanve::gl
{

Mesh::Mesh(MeshPrimitive primitive) :
  _primitive{ primitive }
{
  glCreateVertexArrays(1, &_vao);
  _flags |= ObjectFlag::Created;
}

Mesh::Mesh(GLuint vao, MeshPrimitive primitive, ObjectFlags flags) :
  _vao{ vao },
  _primitive{ primitive }
{
  _flags = flags;
}

Mesh::Mesh(Mesh&& other) noexcept :
  _vao{other._vao},
  _primitive{other._primitive},
  _attributes{std::move(other._attributes)},
  _indexBuffer{std::move(other._indexBuffer)},
  _indexType{other._indexType},
  _indexOffset{other._indexOffset},
  _indexStart{other._indexStart},
  _indexEnd{ other._indexEnd },
  _baseVertex{other._baseVertex},
  _count{other._count}
{
  _flags = other._flags;
  other._vao = 0;
}

Mesh::~Mesh()
{
  if (!(_vao && (_flags & ObjectFlag::DestroyOnDestruction))) return;

  auto& state = *Context::current().state().mesh;

  if (state.currentVAO == _vao) {
    state.currentVAO = 0;
  }

  glDeleteVertexArrays(1, &_vao);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
  std::swap(_vao, other._vao);
  std::swap(_flags, other._flags);
  std::swap(_primitive, other._primitive);
  std::swap(_attributes, other._attributes);
  std::swap(_indexBuffer, other._indexBuffer);
  std::swap(_indexOffset, other._indexOffset);
  std::swap(_indexType, other._indexType);
  std::swap(_indexStart, other._indexStart);
  std::swap(_indexEnd, other._indexEnd);
  std::swap(_baseVertex, other._baseVertex);
  std::swap(_count, other._count);

  return *this;
}

Mesh& Mesh::setIndexBuffer(Buffer&& buffer, GLintptr offset, MeshIndexType type, GLuint start, GLuint end)
{
  bindVAO();
  buffer.bind();

  _indexBuffer = std::move(buffer);
  _indexOffset = offset;
  _indexType = type;
  _indexStart = start;
  _indexEnd = end;

  return *this;
}

Mesh& Mesh::draw(ShaderPipeline& shader)
{
  shader.draw(*this);
  return *this;
}

void Mesh::drawInternal(GLsizei count, GLuint baseVertex, GLsizei instanceCount, GLuint baseInstance, GLintptr indexOffset, GLuint indexStart, GLuint indexEnd)
{
  bindVAO();

  if (instanceCount == 1) {
    if (!_indexBuffer.id())
      glDrawArrays(GLenum(_primitive), _baseVertex, _count);

    else if (_baseVertex) {
      if (_indexEnd)
        glDrawRangeElementsBaseVertex(GLenum(_primitive), indexStart, indexEnd, count, GLenum(_indexType), reinterpret_cast<GLvoid*>(indexOffset), baseVertex);
      else
        glDrawElementsBaseVertex(GLenum(_primitive), count, GLenum(_indexType), reinterpret_cast<GLvoid*>(indexOffset), baseVertex);
    }

    else {
      if (_indexEnd)
        glDrawRangeElements(GLenum(_primitive), indexStart, indexEnd, count, GLenum(_indexType), reinterpret_cast<GLvoid*>(indexOffset));
      else
        glDrawElements(GLenum(_primitive), count, GLenum(_indexType), reinterpret_cast<GLvoid*>(indexOffset));
    }
  }
  else {
    if (!_indexBuffer.id()) {
      if (baseInstance) {
        glDrawArraysInstancedBaseInstance(GLenum(_primitive), baseVertex, count, instanceCount, baseInstance);
      }
      else {
        glDrawArraysInstanced(GLenum(_primitive), baseVertex, count, instanceCount);
      }
    }
    else if (baseVertex) {
      if (baseInstance) {
        glDrawElementsInstancedBaseVertexBaseInstance(GLenum(_primitive), count, GLenum(_indexType), reinterpret_cast<GLvoid*>(indexOffset), instanceCount, baseVertex, baseInstance);
      }
      else {
        glDrawElementsInstancedBaseVertex(GLenum(_primitive), count, GLenum(_indexType), reinterpret_cast<GLvoid*>(indexOffset), instanceCount, baseVertex);
      }
    }
    else {
      if (baseInstance) {
        glDrawElementsInstancedBaseInstance(GLenum(_primitive), count, GLenum(_indexType), reinterpret_cast<GLvoid*>(indexOffset), instanceCount, baseInstance);
      }
      else {
        glDrawElementsInstanced(GLenum(_primitive), count, GLenum(_indexType), reinterpret_cast<GLvoid*>(indexOffset), instanceCount);
      }
    }
  }
  
}

GLsizei Mesh::meshIndexTypeSize(MeshIndexType type) const
{
  switch (type) {
  case MeshIndexType::UnsignedByte:  return 1;
  case MeshIndexType::UnsignedShort: return 2;
  case MeshIndexType::UnsignedInt:   return 4;
  default: return 0;
  }
}

void Mesh::bindVAO()
{
  auto& current = Context::current().state().mesh->currentVAO;

  if (current == _vao) return;

  _flags |= ObjectFlag::Created;
  bindVAOInternal(_vao);

  Context::current().state()
    .buffer->bindings[detail::BufferState::indexForTarget(Buffer::Target::ElementArray)] = _indexBuffer.id();
}

void Mesh::bindVAOInternal(GLint id)
{
  glBindVertexArray(Context::current().state().mesh->currentVAO = id);
}

void Mesh::bindIndexBuffer(Buffer& buffer)
{
  bindVAO();
  buffer.bind();
}


void Mesh::attributePointer(Buffer& buffer, const GLuint location, const GLint size, const GLenum type, GLint normalized, const GLintptr offset, const GLsizei stride/*, GLint divisor*/)
{
  attributePointerInternal(AttributeLayout{ buffer, location, size, type, normalized, offset, stride });
}

void Mesh::attributePointerInternal(AttributeLayout&& attribute)
{
  bindVAO();
  vertexAttribPointer(std::move(attribute));
}

void Mesh::vertexAttribPointer(AttributeLayout&& attribute)
{
  glEnableVertexAttribArray(attribute.location);
  attribute.buffer.bind();

  glVertexAttribPointer(attribute.location, attribute.size, attribute.type, 
                        attribute.normalized, attribute.stride, reinterpret_cast<void*>(attribute.offset));

  if (attribute.divisor) {
    bindVAO();
    glVertexAttribDivisor(attribute.location, attribute.divisor);
  }

  _attributes.push_back(std::move(attribute));
}

}