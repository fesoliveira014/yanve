#include <graphics/gl/buffer.h>

#include <graphics/gl/context.h>
#include <graphics/gl/state/state.h>
#include <graphics/gl/state/bufferstate.h>

namespace yanve::gl
{
Buffer::Buffer(Target target) :
  GLObject(),
  _id{0},
  _target{target},
  _size{0}
{
  glGenBuffers(1, &_id);
  _flags |= ObjectFlag::Created | ObjectFlag::DestroyOnDestruction;
}

Buffer::Buffer(GLuint id, Target target, ObjectFlags flags):
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
  if (!_id || !(_flags & ObjectFlag::DestroyOnDestruction)) return;

  auto& state = *Context::current().state().buffer;

  for (uint i = 0; i < detail::BufferState::TargetCount; ++i) {
    if (state.bindings[i] == _id) state.bindings[i] = 0;
  }
  
  glDeleteBuffers(1, &_id);
}

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
  bindInternal(_target);
}

Buffer& Buffer::bind(Target target, GLuint index)
{
  glBindBufferBase(GLenum(target), index, _id);
  return *this;
}

Buffer& Buffer::bind(Target target, GLuint index, GLintptr offset, GLsizeiptr size)
{
  glBindBufferRange(GLenum(target), index, _id, offset, size);
  return *this;
}

void Buffer::unbind()
{
  unbindInternal(_target);
}

void Buffer::unbind(Target target, GLint index)
{
  glBindBufferBase(GLenum(target), index, 0);
}

//void Buffer::unbind(Target target, GLuint index, GLintptr offset, GLsizeiptr size)
//{
//  glBindBufferRange(GLenum(target), index, 0, offset, size);
//}

void Buffer::bindInternal(Target target, Buffer *const buffer)
{
  const GLuint id = buffer ? buffer->_id : 0;
  auto& bound = Context::current().state().buffer->bindings[detail::BufferState::indexForTarget(target)];

  if (bound == id) return;

  bound = id;
  if (id) buffer->_flags |= ObjectFlag::Created;
  glBindBuffer(GLenum(target), id);
}

}