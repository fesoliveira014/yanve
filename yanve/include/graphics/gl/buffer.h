#pragma once

#include <common.h>

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

class Buffer
{
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

  explicit YANVE_API Buffer(Target target = Target::Array);
  inline YANVE_API Buffer(Buffer&& other);
  YANVE_API ~Buffer();

  inline Buffer& operator=(Buffer&& other) noexcept
  {
    _handle = std::move(other._handle);
    _size = std::move(other._size);
    _target = std::move(other._target);
    other._handle = 0;
    return *this;
  }

  void YANVE_API setData(void* data, size_t size, BufferUsage usage);
  void YANVE_API setSubData(void* data, size_t size, size_t offset);

  void YANVE_API getData(size_t size, void* data);
  void YANVE_API getSubData(size_t offset, size_t size, void* data);

  void YANVE_API bind();
  void YANVE_API unbind();

  GLuint YANVE_API handle() const { return _handle; }
  
private:
  explicit Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;
  GLuint _handle;
  Target _target;
  size_t _size;
};

}
