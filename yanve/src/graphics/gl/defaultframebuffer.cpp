#include <graphics/gl/context.h>
#include <graphics/gl/state/state.h>
#include <graphics/gl/state/framebufferstate.h>
#include <graphics/gl/defaultframebuffer.h>

namespace yanve::gl
{

DefaultFramebuffer defaultFramebuffer;

DefaultFramebuffer::Status DefaultFramebuffer::checkStatus(FramebufferTarget target)
{
  return Status(AbstractFramebuffer::checkStatus(target));
}

DefaultFramebuffer& DefaultFramebuffer::clearColor(const glm::vec4& color)
{
  //float c[] = { color.x, color.y, color.z, color.w};
  AbstractFramebuffer::clearf(GL_COLOR, 0, glm::value_ptr(color));
  return *this;
}

DefaultFramebuffer& DefaultFramebuffer::clearColor(const glm::ivec4& color)
{
  AbstractFramebuffer::cleari(GL_COLOR, 0, glm::value_ptr(color));
  return *this;
}

DefaultFramebuffer& DefaultFramebuffer::clearColor(const glm::uvec4& color)
{
  AbstractFramebuffer::clearu(GL_COLOR, 0, glm::value_ptr(color));
  return *this;
}

DefaultFramebuffer& DefaultFramebuffer::mapForDraw(std::initializer_list<std::pair<uint, DefaultFramebuffer::DrawAttachment>> attachments)
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

DefaultFramebuffer& DefaultFramebuffer::mapForDraw(DefaultFramebuffer::DrawAttachment attachment)
{
  AbstractFramebuffer::drawBuffer(GLenum(attachment));
  return *this;
}

DefaultFramebuffer& DefaultFramebuffer::mapForRead(const DefaultFramebuffer::ReadAttachment attachment)
{
  AbstractFramebuffer::readBuffer(GLenum(attachment));
  return *this;
}

void DefaultFramebuffer::invalidate(std::initializer_list<InvalidationAttachment> attachments)
{
  std::vector<GLenum> _attachments(attachments.size());
  for (size_t i = 0; i < attachments.size(); ++i)
    _attachments[i] = GLenum(*(attachments.begin() + i));
  
  AbstractFramebuffer::invalidate(_attachments.size(), _attachments.data());
}

void DefaultFramebuffer::invalidate(std::initializer_list<InvalidationAttachment> attachments, const Rectangle2Di& rect)
{
  std::vector<GLenum> _attachments(attachments.size());
  for (size_t i = 0; i < attachments.size(); ++i)
    _attachments[i] = GLenum(*(attachments.begin() + i));

  AbstractFramebuffer::invalidate(_attachments.size(), _attachments.data(), rect);
}

void DefaultFramebuffer::initializeContext(Context& context)
{
  //auto& state = *context.state().framebuffer;

  //GLint viewport[4];
  //glGetIntegerv(GL_VIEWPORT, viewport);
  //glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
  //defaultFramebuffer._viewport = state.viewport = Rectangle2Di{ {viewport[0], viewport[1]}, {viewport[2], viewport[3]} };
}

}