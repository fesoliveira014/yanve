#include <graphics/gl/abstractframebuffer.h>
#include <graphics/gl/buffer.h>
#include <utils/logger.h>

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
  // get context state
  // check if framebuffer is bound to current target
  // if not, then set current target to current framebuffer id

  _flags |= ObjectFlags::Created;
  glBindFramebuffer(GLenum(target), _id);
}

AbstractFramebuffer& AbstractFramebuffer::setViewport(const Rectangle2Di& viewport)
{
  _viewport = viewport;
  // check if framebuffer is bound to draw target and call setViewportInternal if it is

  return *this;
}

void AbstractFramebuffer::setViewportInternal()
{
  // get context framebuffer state
  // check if framebuffer is bound to draw target
  // check if state viewport is same as framebuffer viewport
  // if not set it

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