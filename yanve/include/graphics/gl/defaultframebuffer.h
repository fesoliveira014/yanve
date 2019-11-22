#pragma once

#include <common.h>
#include <graphics/gl/abstractframebuffer.h>
#include <graphics/gl/globject.h>


namespace yanve::gl
{

class Context;

class YANVE_API DefaultFramebuffer : public AbstractFramebuffer
{
public:
  enum class Status : GLenum
  {
    Complete = GL_FRAMEBUFFER_COMPLETE,
    Undefined = GL_FRAMEBUFFER_UNDEFINED
  };

  enum class DrawAttachment : GLenum
  {
    None = GL_NONE,
    FrontLeft = GL_FRONT_LEFT,
    FrontRight = GL_FRONT_RIGHT,
    BackLeft = GL_BACK_LEFT,
    BackRight = GL_BACK_RIGHT,
    Back = GL_BACK_LEFT
  };

  enum class ReadAttachment : GLenum
  {
    None = GL_NONE,
    FrontLeft = GL_FRONT_LEFT,
    FrontRight = GL_FRONT_RIGHT,
    BackLeft = GL_BACK_LEFT,
    BackRight = GL_BACK_RIGHT,
    Left = GL_LEFT,
    Right = GL_RIGHT,
    Back = GL_BACK,
    Front = GL_FRONT,
    FrontAndBack = GL_FRONT_AND_BACK
  };

  enum class InvalidationAttachment : GLenum
  {
    FrontLeft = GL_FRONT_LEFT,
    FrontRight = GL_FRONT_RIGHT,
    BackLeft = GL_BACK_LEFT,
    BackRight = GL_BACK_RIGHT,
    Color = GL_COLOR,
    Depth = GL_DEPTH,
    Stencil = GL_STENCIL
  };

  constexpr explicit DefaultFramebuffer() : AbstractFramebuffer{ 0, {}, ObjectFlags::Created | ObjectFlags::DestroyOnDestruction } {}

  DefaultFramebuffer(const DefaultFramebuffer&) = delete;
  DefaultFramebuffer(DefaultFramebuffer&&) = delete;

  DefaultFramebuffer& operator=(const DefaultFramebuffer&) = delete;
  DefaultFramebuffer& operator=(DefaultFramebuffer&&) = delete;

  Status checkStatus(FramebufferTarget target);

  DefaultFramebuffer& clearColor(const glm::vec4& color);
  DefaultFramebuffer& clearColor(const glm::ivec4& color);
  DefaultFramebuffer& clearColor(const glm::uvec4& color);

  DefaultFramebuffer& mapForDraw(std::initializer_list<std::pair<uint, DrawAttachment>> attachments);
  DefaultFramebuffer& mapForDraw(DrawAttachment attachment);

  DefaultFramebuffer& mapForRead(ReadAttachment attachment);

  void invalidate(std::initializer_list<InvalidationAttachment> attachments);
  void invalidate(std::initializer_list<InvalidationAttachment> attachments, const Rectangle2Di& rect);

  DefaultFramebuffer& setViewport(const Rectangle2Di& rect)
  {
    AbstractFramebuffer::setViewport(rect);
    return *this;
  }

  DefaultFramebuffer& clear(FramebufferClearMask mask)
  {
    AbstractFramebuffer::clear(mask);
    return *this;
  }

  DefaultFramebuffer& clearDepth(float depth)
  {
    AbstractFramebuffer::clearDepth(depth);
    return *this;
  }

  DefaultFramebuffer& clearStencil(int stencil)
  {
    AbstractFramebuffer::clearStencil(stencil);
    return *this;
  }

  DefaultFramebuffer& clearDepthStencil(float depth, int stencil)
  {
    AbstractFramebuffer::clearDepthStencil(depth, stencil);
    return *this;
  }

private:
  friend class Context;

  static void initializeContext(Context& context);
};

extern YANVE_API DefaultFramebuffer defaultFramebuffer;

}