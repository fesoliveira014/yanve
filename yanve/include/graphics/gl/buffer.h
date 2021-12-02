#pragma once

#include <common.h>
#include <graphics/gl/globject.h>
#include <utils/nocreate.h>

namespace yanve::gl
{

enum class BufferUsage : GLenum
{
  StreamDraw = GL_STREAM_DRAW,
  StreamRead = GL_STREAM_READ,
  StreamCopy = GL_STREAM_COPY,

  StaticDraw = GL_STATIC_DRAW,
  StaticRead = GL_STATIC_READ,
  StaticCopy = GL_STATIC_COPY,

  DynamicDraw = GL_DYNAMIC_DRAW,
  DynamicRead = GL_DYNAMIC_READ,
  DynamicCopy = GL_DYNAMIC_COPY
};

class YANVE_API Buffer : public GLObject
{
  friend class AbstractFramebuffer;

public:
  enum class Target : GLenum
  {
    Array = GL_ARRAY_BUFFER,
    ElementArray = GL_ELEMENT_ARRAY_BUFFER,
    CopyWrite = GL_COPY_WRITE_BUFFER,
    DrawIndirect = GL_DRAW_INDIRECT_BUFFER,
    PixelPack = GL_PIXEL_PACK_BUFFER,
    PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
    TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
    // GL > 3.1
    CopyRead = GL_COPY_READ_BUFFER,
    Uniform = GL_UNIFORM_BUFFER,
    Texture = GL_TEXTURE_BUFFER,
    // GL > 4.2
    AtomicCounter = GL_ATOMIC_COUNTER_BUFFER,
    // GL > 4.3
    DispatchIndirect = GL_DISPATCH_INDIRECT_BUFFER,
    ShaderStorage = GL_SHADER_STORAGE_BUFFER,
    // GL > 4.4
    Query = GL_QUERY_BUFFER
  };
  
  explicit Buffer(Target target = Target::Array);
  explicit Buffer(NoCreateT);
  inline Buffer(Buffer&& other) noexcept;
  ~Buffer();

  static Buffer wrap(GLuint id, Target target = Target::Array, ObjectFlags flags = {})
  {
    return Buffer{id, target, flags};
  }

  static Buffer wrap(GLuint id, ObjectFlags flags)
  {
    return Buffer{ id, Target::Array, flags };
  }

  inline Buffer& operator=(Buffer&& other) noexcept
  {
    _id = std::move(other._id);
    _size = std::move(other._size);
    _target = std::move(other._target);
    other._id = 0;
    return *this;
  }

  void setData(void* data, size_t size, BufferUsage usage);
  void setSubData(void* data, size_t size, size_t offset);

  void getData(size_t size, void* data);
  void getSubData(size_t offset, size_t size, void* data);

  
  void bind();
  Buffer& bind(Target target, GLuint index);
  Buffer& bind(Target target, GLuint index, GLintptr offset, GLsizeiptr size);

  void unbind();
  void unbind(Target target, GLint index);
  //void unbind(Target target, GLuint index, GLintptr offset, GLsizeiptr size);

  GLuint id() const { return _id; }
  Target target() const { return _target; }
  ObjectFlags flags() const { return _flags; }
  
protected:
  explicit Buffer(GLuint id, Target target, ObjectFlags flags = {});
  explicit Buffer(const Buffer&) = delete;

  Buffer& operator=(const Buffer&) = delete;

  static void bindInternal(Target target, Buffer *const buffer);
  static void unbindInternal(Target target) { bindInternal(target, nullptr); };
  void bindInternal(Target target) { bindInternal(target, this); }
 
  GLuint _id;
  Target _target;
  size_t _size;
  ObjectFlags _flags;
};

inline Buffer::Buffer(NoCreateT) : _id{ 0 }, _target{}, _flags{} {}

}
