#pragma once

#include <common.h>
#include <graphics/gl/globject.h>
#include <utils/nocreate.h>

namespace yanve::gl
{

namespace detail
{
  struct FramebufferState;
}

enum class RenderbufferFormat : GLenum;

class YANVE_API Renderbuffer
{
  friend detail::FramebufferState;

public:
  static Renderbuffer wrap(GLuint id, ObjectFlags flags = {}) 
  {
    return Renderbuffer{ id, flags };
  }

  explicit Renderbuffer();
  explicit Renderbuffer(NoCreateT) noexcept : _id{ 0 }, _flags{ ObjectFlag::DestroyOnDestruction } {}

  Renderbuffer(const Renderbuffer&) = delete;
  inline Renderbuffer(Renderbuffer&& other) noexcept;

  ~Renderbuffer();

  Renderbuffer& operator=(const Renderbuffer&) = delete;
  inline Renderbuffer& operator=(Renderbuffer&& other) noexcept;

  GLuint id() const { return _id; }
  inline GLuint release();

  void setStorage(RenderbufferFormat internalFormat, const glm::ivec2& size);
  void setStorageMultisample(i32 samples, RenderbufferFormat internalFormat, const glm::ivec2& size);

private:
  explicit Renderbuffer(GLuint id, ObjectFlags flags) noexcept : _id{ id }, _flags{ flags } {}
  void create();

  GLuint _id;
  ObjectFlags _flags;
};

inline Renderbuffer::Renderbuffer(Renderbuffer&& other) noexcept : _id{ other._id }, _flags{ other._flags } 
{
  other._id = 0;
}

inline Renderbuffer& Renderbuffer::operator=(Renderbuffer&& other) noexcept 
{
  std::swap(_id, other._id);
  std::swap(_flags, other._flags);
  return *this;
}

inline GLuint Renderbuffer::release() 
{
  const GLuint id = _id;
  _id = 0;
  return id;
}

}