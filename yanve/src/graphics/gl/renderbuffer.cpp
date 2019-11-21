#include <graphics/gl/renderbuffer.h>

#include <graphics/gl/context.h>
#include <graphics/gl/renderbufferformat.h>

#include <graphics/gl/state/state.h>
#include <graphics/gl/state/framebufferstate.h>

namespace yanve::gl
{

Renderbuffer::Renderbuffer() : _flags{ObjectFlags::DestroyOnDestruction}
{
  create();
}

void Renderbuffer::create()
{
  glCreateRenderbuffers(1, &_id);
  _flags |= ObjectFlags::Created;
}

Renderbuffer::~Renderbuffer()
{
  if (!_id || !(_flags & ObjectFlags::DestroyOnDestruction)) return;

  auto& binding = Context::current().state().framebuffer->renderbufferBinding;
  if (binding == _id) binding = 0;

  glDeleteRenderbuffers(1, &_id);
}

void Renderbuffer::setStorage(RenderbufferFormat internalFormat, const glm::ivec2& size)
{
  glNamedRenderbufferStorage(_id, GLenum(internalFormat), size.x, size.y);
}

void Renderbuffer::setStorageMultisample(i32 samples, RenderbufferFormat internalFormat, const glm::ivec2& size)
{
  glNamedRenderbufferStorageMultisample(_id, samples, GLenum(internalFormat), size.x, size.y);
}

}