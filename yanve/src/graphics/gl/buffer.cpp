#include <graphics/gl/buffer.h>

namespace yanve::gl
{
Buffer::Buffer(Target target) :
  GLObject(),
  _id{0},
  _target{target},
  _size{0}
{
  glGenBuffers(1, &_id);
  _flags |= GLObject::Flags::Created | GLObject::Flags::DestroyOnDestruction;
}

Buffer::Buffer(GLuint id, Target target, Flags flags):
  _id{ id },
  _target{ target },
  _size{0}
{
  _flags = flags;
}

Buffer::Buffer(Buffer&& other) noexcept
{
  _id = std::move(other._id);
  _size = std::move(other._size);
  _target = std::move(other._target);
  _flags = std::move(other._flags);

  other._id = 0;
}

Buffer::~Buffer()
{
  if (_id && (_flags & GLObject::Flags::DestroyOnDestruction))
    glDeleteBuffers(1, &_id);
}

//Buffer & Buffer::operator=(Buffer && other)
//{
//  _id = std::move(other._id);
//  _size = std::move(other._size);
//  _target = std::move(other._target);
//  other._id = 0;
//  return *this;
//}

void Buffer::setData(void* data, size_t size, BufferUsage usage)
{
  bind();
  glBufferData(static_cast<GLenum>(_target), size, data, static_cast<GLenum>(usage));

  _size = size;
}

void Buffer::setSubData(void* data, size_t size, size_t offset)
{
  bind();
  glBufferSubData(static_cast<GLenum>(_target), offset, size, data);
}

void Buffer::getData(size_t size, void* data)
{
  bind();
  glGetBufferSubData(static_cast<GLenum>(_target), 0, size, data);
}

void Buffer::getSubData(size_t offset, size_t size, void* data)
{
  bind();
  glGetBufferSubData(static_cast<GLenum>(_target), offset, size, data);
}

void Buffer::bind()
{
  if (_id) _flags |= GLObject::Flags::Created;
  glBindBuffer(static_cast<GLenum>(_target), _id);
}

void Buffer::unbind()
{
  glBindBuffer(static_cast<GLenum>(_target), 0);
}

}