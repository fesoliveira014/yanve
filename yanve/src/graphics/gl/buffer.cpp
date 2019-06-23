#include <graphics/gl/buffer.h>

namespace yanve::gl
{

Buffer::Buffer(Target target) :
  _target{target}
{
  glGenBuffers(1, &_handle);
}

Buffer::Buffer(Buffer&& other)
{
  _handle = std::move(other._handle);
  _size = std::move(other._size);
  _target = std::move(other._target);

  other._handle = 0;
}

Buffer::~Buffer()
{
  if (_handle)
    glDeleteBuffers(1, &_handle);
}

//Buffer & Buffer::operator=(Buffer && other)
//{
//  _handle = std::move(other._handle);
//  _size = std::move(other._size);
//  _target = std::move(other._target);
//  other._handle = 0;
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
  glBindBuffer(static_cast<GLenum>(_target), _handle);
}

void Buffer::unbind()
{
  glBindBuffer(static_cast<GLenum>(_target), 0);
}

}