#pragma once

#include <common.h>
#include <graphics/gl/buffer.h>
#include <graphics/gl/attribute.h>
#include <graphics/gl/globject.h>

/**
 * Mesh class. Encapsulates the concept of an OpenGL mesh. 
 * Heavily adapted from Magnum's mesh class:
 * https://github.com/mosra/magnum/blob/master/src/Magnum/GL/Mesh.h
 */

namespace yanve::gl
{

namespace state
{
struct MeshState;
}

enum class MeshPrimitive
{
  Points = GL_POINTS,
  Lines = GL_LINES,
  LineLoop = GL_LINE_LOOP,
  LineStrip = GL_LINE_STRIP,
  Triangles = GL_TRIANGLES,
  TriangleFan = GL_TRIANGLE_FAN,
  TriangleStrip = GL_TRIANGLE_STRIP,
  Patches = GL_PATCHES,
  
  // GL > 3.2
  LinesAdjacency = GL_LINES_ADJACENCY,
  LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
  TriangleAdjacency = GL_TRIANGLES_ADJACENCY,
  TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY
};

class ShaderProgram;

class YANVE_API Mesh : public GLObject
{
public:
  enum class MeshIndexType : GLenum
  {
    UnsignedByte = GL_UNSIGNED_BYTE,
    UnsignedShort = GL_UNSIGNED_SHORT,
    UnsignedInt = GL_UNSIGNED_INT
  };

protected:
  struct AttributeLayout
  {
    Buffer buffer;
    GLuint location;
    GLint size;
    GLint normalized;
    GLenum type;
    GLintptr offset;
    GLsizei stride;
    GLuint divisor;

    AttributeLayout(Buffer& buffer, GLuint location, GLint size, GLenum type, GLint normalized, GLintptr offset, GLsizei stride, GLuint divisor = 0) :
      buffer{Buffer::wrap(buffer.id())},
      location{location},
      size{size},
      type{type},
      normalized{normalized},
      offset{offset},
      stride{stride},
      divisor{divisor}
    {
    }

    AttributeLayout(AttributeLayout&&) noexcept = default;
    AttributeLayout(const AttributeLayout&) noexcept = delete;
    AttributeLayout& operator=(AttributeLayout&&) noexcept = default;
    AttributeLayout& operator=(const AttributeLayout&) noexcept = delete;
  };

  GLuint _vao;
  MeshPrimitive _primitive;

  std::vector<AttributeLayout> _attributes;
  
  Buffer _indexBuffer{NoCreate};
  GLintptr _indexOffset;
  MeshIndexType _indexType;
  GLuint _indexStart;
  GLuint _indexEnd;
  
  GLuint _baseVertex;
  GLsizei _count;
  ObjectFlags _flags;

public:
  explicit Mesh(MeshPrimitive primitive = MeshPrimitive::Triangles);
  explicit Mesh(NoCreateT);
  explicit Mesh(const Mesh&) = delete;
  explicit Mesh(Mesh&& mesh) noexcept;

  ~Mesh();

  Mesh& operator=(const Mesh&) = delete;
  Mesh& operator=(Mesh&& other) noexcept;

  Mesh& setPrimitive(MeshPrimitive primitive) { _primitive = primitive; return *this; }
  Mesh& setCount(GLsizei count) { _count = count; return *this; }
  Mesh& setBaseVertex(GLuint baseVertex) { _baseVertex = baseVertex; return *this; }
  
  Mesh& setIndexBuffer(Buffer&& buffer, GLintptr offset, MeshIndexType type, GLuint start, GLuint end);

  Mesh& setIndexBuffer(Buffer& buffer, GLintptr offset, MeshIndexType type, GLuint start, GLuint end) {
    setIndexBuffer(Buffer::wrap(buffer.id(), buffer.target()), offset, type, start, end);
    return *this;
  }

  Mesh& setIndexBuffer(Buffer& buffer, GLintptr offset, MeshIndexType type) {
    setIndexBuffer(buffer, offset, type, 0, 0);
    return *this;
  }

  MeshPrimitive primitive() const { return _primitive; }
  GLsizei count() const { return _count; }
  GLuint baseVertex() const { return _baseVertex; }
  bool isIndexed() const { return _indexBuffer.id() != 0; }
  MeshIndexType indexType() const { return _indexType; }
  GLsizei indexTypeSize() const { return meshIndexTypeSize(_indexType); }

  Mesh& draw(ShaderProgram& shader);
  Mesh& draw(ShaderProgram&& shader) { return draw(shader); }

  template <class ... T>
  Mesh& addBuffer(Buffer& buffer, GLintptr offset, const T& ... attributes){
    addVertexBufferInternal(buffer, offset, strideOfInterleaved(attributes...), 0, attributes...);
    return *this;
  }

protected:
  friend struct state::MeshState;
  explicit Mesh(GLuint vao, MeshPrimitive primitive, ObjectFlags flags);

  void bindVAO();
  static void bindVAOInternal(GLint id);
  void bindIndexBuffer(Buffer& buffer);

  template<GLuint location, class T, class ...U> 
  static GLsizei strideOfInterleaved(const Attribute<location, T>& attribute, const U& ... attributes) {
    return attribute.size() * Attribute<location, T>::VectorCount + strideOfInterleaved(attributes...);
  }

  static GLsizei strideOfInterleaved() { return 0; }

  template <GLuint location, class T, class ...U>
  void addVertexBufferInternal(Buffer& buffer, GLintptr offset, GLsizei stride, GLuint divisor, const Attribute<location, T>& attribute, const U& ... attributes) {
    addVertexAttribute(buffer, attribute, offset, stride);
    addVertexBufferInternal(buffer, offset + attribute.size() * Attribute<location, T>::VectorCount, stride, divisor, attributes...);
  }

  void addVertexBufferInternal(Buffer&, GLintptr, GLsizei, GLuint) {}

  template <GLuint location, class T>
  void addVertexAttribute(typename std::enable_if<std::is_same<typename implementation::Attribute<T>::ScalarType, float>::value, Buffer&>::type buffer, const Attribute<location, T>& attribute, GLintptr offset, GLsizei stride/*, GLint divisor*/)
  {
    attributePointer(
      buffer,
      location,
      GLint(attribute.components()),
      GLenum(attribute.dataType()),
      GLint(attribute.dataOption()),
      offset,
      stride/*,
      divisor*/
    );
  }

  void attributePointer(Buffer& buffer, const GLuint location, const GLint size, const GLenum type, GLint normalized, const GLintptr offset, const GLsizei stride/*, GLint divisor*/);
  void attributePointerInternal(AttributeLayout&& attribute);

  void vertexAttribPointer(AttributeLayout&& attribute);
  
  GLsizei meshIndexTypeSize(MeshIndexType type) const;
};

inline Mesh::Mesh(NoCreateT) : _vao{ 0 }, _flags{ObjectFlags::DestroyOnDestruction} {}

}