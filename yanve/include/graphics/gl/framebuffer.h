#pragma once

#include <common.h>
#include <graphics/gl/globject.h>
#include <graphics/gl/abstractframebuffer.h>
#include <utils/nocreate.h>

namespace yanve::gl
{

// TODO: move all forward declarations to a single file
namespace details
{
  struct FramebufferState;
}

class Renderbuffer;

template <size_t> class Texture;
typedef Texture<1> Texture1D;
typedef Texture<2> Texture2D;
typedef Texture<3> Texture3D;

class YANVE_API Framebuffer : public AbstractFramebuffer, public GLObject
{
  friend details::FramebufferState;
public:
  
  class ColorAttachment
  {
    friend Framebuffer;
  public:
    constexpr explicit ColorAttachment(uint id) : attachment{ GL_COLOR_ATTACHMENT0 + id } {}
    constexpr explicit operator GLenum() const { return attachment; }

  private:
    GLenum attachment;
  };

  class YANVE_API DrawAttachment
  {
  public:
    static const DrawAttachment None;
    constexpr DrawAttachment(Framebuffer::ColorAttachment attachment) : attachment{GLenum(attachment)} {}
    constexpr explicit operator GLenum() const { return attachment; }

  private:
    const explicit DrawAttachment(GLenum attachment) : attachment{attachment} {}

    GLenum attachment;
  };

  class YANVE_API BufferAttachment
  {
  public:
    static const BufferAttachment Depth;
    static const BufferAttachment Stencil;
    static const BufferAttachment DepthStencil;

    constexpr BufferAttachment(Framebuffer::ColorAttachment attachment) : attachment{GLenum(attachment)} {}
    constexpr explicit operator GLenum() const { return attachment; }

  private:
    constexpr explicit BufferAttachment(GLenum attachment) : attachment{attachment} {}

    GLenum attachment;
  };

  class YANVE_API InvalidationAttachment
  {
  public:
    static const InvalidationAttachment Depth;
    static const InvalidationAttachment Stencil;
    
    constexpr InvalidationAttachment(Framebuffer::ColorAttachment attachment) : attachment{GLenum(attachment)} {}
    constexpr explicit operator GLenum() const { return attachment; }

  private:
    constexpr explicit InvalidationAttachment(GLenum attachment) : attachment{attachment} {}

    GLenum attachment;
  };

  enum class Status : GLenum 
  {
    Complete = GL_FRAMEBUFFER_COMPLETE,
    IncompleteAttachment = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
    IncompleteMissingAttachment = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
    IncompleteDrawBuffer = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
    IncompleteReadBuffer = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
    Unsupported = GL_FRAMEBUFFER_UNSUPPORTED,
    IncompleteMultisample = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
    IncompleteLayerTargets = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS
  };

  static i32 maxColorAttachments();

  static Framebuffer wrap(GLuint id, const Rectangle2Di& viewport, ObjectFlags flags = {})
  {
    return Framebuffer{ id, viewport, flags };
  }

  explicit Framebuffer(const Rectangle2Di& viewport);
  explicit Framebuffer(NoCreateT) noexcept : AbstractFramebuffer{{}, {}, {}} {}
  
  Framebuffer(const Framebuffer&) = delete;
  Framebuffer(Framebuffer&&) noexcept = default;

  ~Framebuffer();

  Framebuffer& operator=(const Framebuffer&) = delete;
  Framebuffer& operator=(Framebuffer&&) noexcept = default;

  GLuint id() const { return _id; }
  inline GLuint release();

  Status checkStatus(FramebufferTarget target);

  Framebuffer& clearColor(i32 attachment, const glm::vec4& color);
  Framebuffer& clearColor(i32 attachment, const glm::ivec4& color);
  Framebuffer& clearColor(i32 attachment, const glm::uvec4& color);

  Framebuffer& mapForDraw(std::initializer_list<std::pair<uint, DrawAttachment>> attachments);
  Framebuffer& mapForDraw(DrawAttachment attachment);

  Framebuffer& mapForRead(const ColorAttachment attachment);

  void invalidate(std::initializer_list<InvalidationAttachment> attachments);
  void invalidate(std::initializer_list<InvalidationAttachment> attachments, const Rectangle2Di& rect);

  Framebuffer& attachRenderbuffer(BufferAttachment attachment, Renderbuffer& renderbuffer);

  Framebuffer& attachTexture(BufferAttachment attachment, Texture1D& texture, i32 level);
  Framebuffer& attachTexture(BufferAttachment attachment, Texture2D& texture, i32 level);
  Framebuffer& attachTextureLayer(BufferAttachment attachment, Texture3D& texture, i32 level, i32 layer);
  Framebuffer& attachLayeredTexture(BufferAttachment attachment, Texture3D& texture, i32 level);

  Framebuffer& detach(BufferAttachment attachment);

  Framebuffer& setViewport(const Rectangle2Di& rectangle)
  {
    AbstractFramebuffer::setViewport(rectangle);
    return *this;
  }

  Framebuffer& clear(FramebufferClearMask mask)
  {
    AbstractFramebuffer::clear(mask);
    return *this;
  }

  Framebuffer& clearDepth(float depth) 
  {
    AbstractFramebuffer::clearDepth(depth);
    return *this;
  }

  Framebuffer& clearStencil(i32 stencil) 
  {
    AbstractFramebuffer::clearStencil(stencil);
    return *this;
  }

  Framebuffer& clearDepthStencil(float depth, i32 stencil) 
  {
    AbstractFramebuffer::clearDepthStencil(depth, stencil);
    return *this;
  }

private:
  explicit Framebuffer(GLuint id, const Rectangle2Di& viewport, ObjectFlags flags) noexcept : AbstractFramebuffer{id, viewport, flags} {}
  void create();
};

inline GLuint Framebuffer::release()
{
  const GLuint id = _id;
  _id = 0;
  return id;
}

}
