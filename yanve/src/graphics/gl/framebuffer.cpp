#include <graphics/gl/framebuffer.h>

#include <graphics/gl/context.h>
#include <graphics/gl/texture.h>
#include <graphics/gl/defaultframebuffer.h>
#include <graphics/gl/renderbuffer.h>

#include <graphics/gl/state/state.h>
#include <graphics/gl/state/framebufferstate.h>

namespace yanve::gl
{

const Framebuffer::DrawAttachment Framebuffer::DrawAttachment::None = Framebuffer::DrawAttachment(GL_NONE);
const Framebuffer::BufferAttachment Framebuffer::BufferAttachment::Depth = Framebuffer::BufferAttachment(GL_DEPTH_ATTACHMENT);
const Framebuffer::BufferAttachment Framebuffer::BufferAttachment::Stencil = Framebuffer::BufferAttachment(GL_STENCIL_ATTACHMENT);
const Framebuffer::BufferAttachment Framebuffer::BufferAttachment::DepthStencil = Framebuffer::BufferAttachment(GL_DEPTH_STENCIL_ATTACHMENT);
const Framebuffer::InvalidationAttachment Framebuffer::InvalidationAttachment::Depth = Framebuffer::InvalidationAttachment(GL_DEPTH_ATTACHMENT);
const Framebuffer::InvalidationAttachment Framebuffer::InvalidationAttachment::Stencil = Framebuffer::InvalidationAttachment(GL_STENCIL_ATTACHMENT);

i32 Framebuffer::maxColorAttachments()
{
  GLint& value = Context::current().state().framebuffer->maxColorAttachments;

  if (value == 0)
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &value);

  return value;
}

Framebuffer::Framebuffer(const Rectangle2Di& viewport) : AbstractFramebuffer{ 0, viewport, ObjectFlags::DestroyOnDestruction }
{
  _viewport = viewport;
  create();
}

void Framebuffer::create()
{
  glCreateFramebuffers(1, &_id);
  _flags |= ObjectFlags::Created;
}

Framebuffer::~Framebuffer()
{
  if (!_id || !(_flags & ObjectFlags::DestroyOnDestruction)) return;

  auto& state = *Context::current().state().framebuffer;
  
  if (state.readBinding == _id) state.readBinding = 0;
  if (state.writeBinding == _id) {
    state.writeBinding = 0;
    // TODO: need to find a way to reset viewport to default framebuffer in this case
  }

  glDeleteFramebuffers(1, &_id);
}

Framebuffer::Status Framebuffer::checkStatus(FramebufferTarget target)
{
  return Status(AbstractFramebuffer::checkStatus(target));
}

Framebuffer& Framebuffer::clearColor(i32 attachment, const glm::vec4& color)
{
  AbstractFramebuffer::clearf(GL_COLOR, attachment, glm::value_ptr(color));
  return *this;
}

Framebuffer& Framebuffer::clearColor(i32 attachment, const glm::ivec4& color)
{
  AbstractFramebuffer::cleari(GL_COLOR, attachment, glm::value_ptr(color));
  return *this;
}

Framebuffer& Framebuffer::clearColor(i32 attachment, const glm::uvec4& color)
{
  AbstractFramebuffer::clearu(GL_COLOR, attachment, glm::value_ptr(color));
  return *this;
}

Framebuffer& Framebuffer::mapForDraw(std::initializer_list<std::pair<uint, Framebuffer::DrawAttachment>> attachments)
{
  std::size_t max = 0;
  for (const auto& attachment : attachments) {
    if (attachment.first > max) max = attachment.first;
  }

  std::vector<GLenum> _attachments(max + 1, 0);
  for (const auto& attachment : attachments) {
    _attachments[attachment.first] = GLenum(attachment.second);
  }

  AbstractFramebuffer::drawBuffers(_attachments.size(), _attachments.data());
  return *this;
}

Framebuffer& Framebuffer::mapForDraw(DrawAttachment attachment)
{
  AbstractFramebuffer::drawBuffer(GLenum(attachment));
  return *this;
}

Framebuffer& Framebuffer::mapForRead(const ColorAttachment attachment)
{
  AbstractFramebuffer::readBuffer(GLenum(attachment));
  return *this;
}

void Framebuffer::invalidate(std::initializer_list<InvalidationAttachment> attachments)
{
  std::vector<GLenum> _attachments(attachments.size());
  for (size_t i = 0; i < attachments.size(); ++i)
    _attachments[i] = GLenum(*(attachments.begin() + i));

  AbstractFramebuffer::invalidate(_attachments.size(), _attachments.data());
}

void Framebuffer::invalidate(std::initializer_list<InvalidationAttachment> attachments, const Rectangle2Di& rect)
{
  std::vector<GLenum> _attachments(attachments.size());
  for (size_t i = 0; i < attachments.size(); ++i)
    _attachments[i] = GLenum(*(attachments.begin() + i));

  AbstractFramebuffer::invalidate(_attachments.size(), _attachments.data(), rect);
}

Framebuffer& Framebuffer::attachRenderbuffer(BufferAttachment attachment, Renderbuffer& renderbuffer)
{
  glNamedFramebufferRenderbuffer(_id, GLenum(attachment), GL_RENDERBUFFER, renderbuffer.id());
  return *this;
}

Framebuffer& Framebuffer::attachTexture(BufferAttachment attachment, Texture1D& texture, i32 level)
{
  glFramebufferTexture1D(GLenum(bindInternal()), GLenum(attachment), GL_TEXTURE_1D, texture.id(), level);
  return *this;
}

Framebuffer& Framebuffer::attachTexture(BufferAttachment attachment, Texture2D& texture, i32 level)
{
  glFramebufferTexture2D(GLenum(bindInternal()), GLenum(attachment), GL_TEXTURE_2D, texture.id(), level);
  return *this;
}

Framebuffer& Framebuffer::attachTextureLayer(BufferAttachment attachment, Texture3D& texture, i32 level, i32 layer)
{
  glFramebufferTextureLayer(GLenum(bindInternal()), GLenum(attachment), texture.id(), level, layer);
  return *this;
}

Framebuffer& Framebuffer::attachLayeredTexture(BufferAttachment attachment, Texture3D& texture, i32 level)
{
  glNamedFramebufferTexture(GLenum(bindInternal()), GLenum(attachment), texture.id(), level);
  return *this;
}

Framebuffer& Framebuffer::detach(BufferAttachment attachment)
{
  glNamedFramebufferRenderbuffer(_id, GLenum(attachment), GL_RENDERBUFFER, 0);
  return *this;
}

}