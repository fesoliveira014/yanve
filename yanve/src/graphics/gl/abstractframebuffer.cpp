#include <graphics/gl/abstractframebuffer.h>
#include <graphics/gl/buffer.h>
#include <utils/logger.h>

#include <graphics/gl/context.h>
#include <graphics/gl/state/state.h>
#include <graphics/gl/state/framebufferstate.h>

namespace yanve::gl
{

void AbstractFramebuffer::blit(AbstractFramebuffer& source, AbstractFramebuffer& destination, const Rectangle2Di& sourceRect, const Rectangle2Di& destRect, FramebufferBlitMask mask, FramebufferBlitFilter filter)
{
  
  glBlitNamedFramebuffer(source._id, destination._id, 
                         sourceRect.first.x, sourceRect.first.y, sourceRect.second.x, sourceRect.second.y, 
                         destRect.first.x, destRect.first.y, destRect.second.x, destRect.second.y, 
                         GLbitfield(mask), GLenum(filter));
}

void AbstractFramebuffer::bind()
{
  bindInternal(FramebufferTarget::Draw);
  setViewportInternal();
}

void AbstractFramebuffer::bindInternal(FramebufferTarget target)
{
  auto& state = *Context::current().state().framebuffer;

  if (target == FramebufferTarget::Read) {
    if (state.readBinding == _id) return;
    state.readBinding = _id;
  }
  else if (target == FramebufferTarget::Draw) {
    if (state.writeBinding == _id) return;
    state.writeBinding = _id;
  }

  _flags |= ObjectFlags::Created;
  glBindFramebuffer(GLenum(target), _id);
}

FramebufferTarget AbstractFramebuffer::bindInternal()
{
  auto& state = *Context::current().state().framebuffer;

  if (state.readBinding == _id) return FramebufferTarget::Read;
  if (state.writeBinding == _id) return FramebufferTarget::Draw;

  state.readBinding = _id;

  _flags |= ObjectFlags::Created;
  glBindFramebuffer(GLenum(FramebufferTarget::Read), _id);
  return FramebufferTarget::Read;
}

AbstractFramebuffer& AbstractFramebuffer::setViewport(const Rectangle2Di& viewport)
{
  _viewport = viewport;

  if (Context::current().state().framebuffer->writeBinding == _id)
    setViewportInternal();

  return *this;
}

void AbstractFramebuffer::setViewportInternal()
{
  auto& state = *Context::current().state().framebuffer;

  if (state.viewport == _viewport)
    return;

  state.viewport = _viewport;
  glViewport(_viewport.first.x, _viewport.first.y, _viewport.second.x, _viewport.second.y);
}

AbstractFramebuffer& AbstractFramebuffer::clear(FramebufferClearMask mask)
{
  bindInternal(FramebufferTarget::Draw);
  glClear(GLbitfield(mask));

  return *this;
}

AbstractFramebuffer& AbstractFramebuffer::clearDepth(float depth)
{
  clearf(GL_DEPTH, 0, &depth);
  return *this;
}

AbstractFramebuffer& AbstractFramebuffer::clearStencil(int stencil)
{
  cleari(GL_STENCIL, 0, &stencil);
  return *this;
}


AbstractFramebuffer& AbstractFramebuffer::clearDepthStencil(float depth, int stencil)
{
  clearfi(GL_DEPTH_STENCIL, depth, stencil);
  return *this;
}

void AbstractFramebuffer::read(const Rectangle2Di& rec, const PixelFormat format, const PixelType type, byte* imageBuffer, glm::uvec2& size)
{
  if (imageBuffer == nullptr) {
    LogError("", "imageBuffer cannot be null");
    return;
  }
  bindInternal(FramebufferTarget::Read);
  Buffer::unbindInternal(Buffer::Target::PixelPack);
  glReadPixels(rec.first.x, rec.first.y, rec.second.x, rec.second.y, GLenum(format), GLenum(type), imageBuffer);
}

void AbstractFramebuffer::copyImage(const Rectangle2Di& rect, Texture1D& texture, int level, TextureFormat internalFormat)
{
  bindInternal(FramebufferTarget::Read);
  texture.bindInternal();
  glCopyTexImage1D(GL_TEXTURE_1D, level, GLenum(internalFormat), rect.first.x, rect.first.y, rect.second.x, 0);
}

void AbstractFramebuffer::copyImage(const Rectangle2Di& rect, Texture2D& texture, int level, TextureFormat internalFormat)
{
  bindInternal(FramebufferTarget::Read);
  texture.bindInternal();
  glCopyTexImage2D(GL_TEXTURE_1D, level, GLenum(internalFormat), rect.first.x, rect.first.y, rect.second.x, rect.second.y, 0);
}

void AbstractFramebuffer::copySubImage(const Rectangle2Di& rect, Texture1D& texture, int level, int offset)
{
  bindInternal(FramebufferTarget::Read);
  glCopyTextureSubImage1D(texture.id(), level, offset, rect.first.x, rect.first.y, rect.second.x);
}

void AbstractFramebuffer::copySubImage(const Rectangle2Di& rect, Texture2D& texture, int level, const glm::uvec2& offset)
{
  bindInternal(FramebufferTarget::Read);
  glCopyTextureSubImage2D(texture.id(), level, offset.x, offset.y, rect.first.x, rect.first.y, rect.second.x, rect.second.y);
}

void AbstractFramebuffer::copySubImage(const Rectangle2Di& rect, Texture3D& texture, int level, const glm::uvec3& offset)
{
  bindInternal(FramebufferTarget::Read);
  glCopyTextureSubImage3D(texture.id(), level, offset.x, offset.y, offset.z, rect.first.x, rect.first.y, rect.second.x, rect.second.y);
}

GLenum AbstractFramebuffer::checkStatus(FramebufferTarget target)
{
  return glCheckNamedFramebufferStatus(_id, GLenum(target));
}

void AbstractFramebuffer::invalidate(const GLsizei count, const GLenum* attachments)
{
  glInvalidateNamedFramebufferData(_id, count, attachments);
}

void AbstractFramebuffer::invalidate(const GLsizei count, const GLenum* attachments, const Rectangle2Di& rect)
{
  glInvalidateNamedFramebufferSubData(_id, count, attachments, rect.first.x, rect.first.y, rect.second.x, rect.second.y);
}

void AbstractFramebuffer::cleari(const GLenum buffer, const GLint drawBuffer, const GLint* const value)
{
  glClearNamedFramebufferiv(_id, buffer, drawBuffer, value);
}

void AbstractFramebuffer::clearu(const GLenum buffer, const GLint drawBuffer, const GLuint* const value)
{
  glClearNamedFramebufferuiv(_id, buffer, drawBuffer, value);
}

void AbstractFramebuffer::clearf(const GLenum buffer, const GLint drawBuffer, const GLfloat* const value)
{
  // TODO: linter is complaining about glClearNamedFramebufferfv pointer argument
  bindInternal(FramebufferTarget::Read);
  glClearBufferfv(buffer, drawBuffer, value);
}

void AbstractFramebuffer::clearfi(const GLenum buffer, const GLfloat depth, const GLint stencil)
{
  glClearNamedFramebufferfi(_id, buffer, 0, depth, stencil);
}

void AbstractFramebuffer::drawBuffer(GLenum buffer)
{
  glNamedFramebufferDrawBuffer(_id, buffer);
}

void AbstractFramebuffer::drawBuffers(GLsizei count, const GLenum* buffers)
{
  glNamedFramebufferDrawBuffers(_id, count, buffers);
}

void AbstractFramebuffer::readBuffer(GLenum buffer)
{
  glNamedFramebufferReadBuffer(_id, buffer);
}

}