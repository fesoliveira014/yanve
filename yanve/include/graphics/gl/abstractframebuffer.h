#pragma once

#include <common.h>
#include <utils/enumoperators.h>
#include <graphics/gl/globject.h>
#include <graphics/gl/texture.h>
#include <graphics/gl/pixelformat.h>

namespace yanve::gl
{

enum class FramebufferClearMask : GLbitfield 
{
  Color = GL_COLOR_BUFFER_BIT,
  Depth = GL_DEPTH_BUFFER_BIT,
  Stencil = GL_STENCIL_BUFFER_BIT
};

enum class FramebufferBlitMask : GLbitfield 
{
  Color = GL_COLOR_BUFFER_BIT,
  Depth = GL_DEPTH_BUFFER_BIT,
  Stencil = GL_STENCIL_BUFFER_BIT
};

enum class FramebufferBlitFilter : GLenum 
{
  Nearest = GL_NEAREST,
  Linear = GL_LINEAR
};

enum class FramebufferTarget : GLenum 
{
  Read = GL_READ_FRAMEBUFFER,
  Draw = GL_DRAW_FRAMEBUFFER,
};

class YANVE_API AbstractFramebuffer
{
public:
  using Rectangle2Di = std::pair<glm::ivec2, glm::ivec2>;

  static void blit(AbstractFramebuffer& source, AbstractFramebuffer& destination, const Rectangle2Di& sourceRect, const Rectangle2Di& destRect, FramebufferBlitMask mask, FramebufferBlitFilter filter);
  static void blit(AbstractFramebuffer& source, AbstractFramebuffer& destination, const Rectangle2Di& rect, FramebufferBlitMask mask)
  {
    blit(source, destination, rect, rect, mask, FramebufferBlitFilter::Nearest);
  }

  void bind();

  Rectangle2Di viewport() const { return _viewport; }
  AbstractFramebuffer& setViewport(const Rectangle2Di& viewport);

  AbstractFramebuffer& clear(FramebufferClearMask mask);
  AbstractFramebuffer& clearDepth(float depth);
  AbstractFramebuffer& clearStencil(int stencil);
  AbstractFramebuffer& clearDepthStencil(float depth, int stencil);

  void read(const Rectangle2Di& rec, const PixelFormat format, const PixelType type, byte* imageBuffer, glm::uvec2& size);

  // TODO cubemap, rectangle text
  void copyImage(const Rectangle2Di& rect, Texture1D& texture, int level, TextureFormat internalFormat);
  void copyImage(const Rectangle2Di& rect, Texture2D& texture, int level, TextureFormat internalFormat);

  void copySubImage(const Rectangle2Di& rect, Texture1D& texture, int level, int offset);
  void copySubImage(const Rectangle2Di& rect, Texture2D& texture, int level, const glm::uvec2& offset);
  void copySubImage(const Rectangle2Di& rect, Texture3D& texture, int level, const glm::uvec3& offset);

protected:
  constexpr explicit AbstractFramebuffer(GLuint id, const Rectangle2Di& viewport, ObjectFlags flags) noexcept : _id{id}, _viewport{viewport}, _flags{flags} {}
  ~AbstractFramebuffer() = default;
  AbstractFramebuffer(const AbstractFramebuffer&) = delete;

  AbstractFramebuffer(AbstractFramebuffer&& other) noexcept : _id{ other._id }, _viewport{ other._viewport }, _flags{ other._flags }
  {
    other._id = 0;
    other._viewport = {};
  }

  AbstractFramebuffer& operator=(const AbstractFramebuffer&) = delete;

  AbstractFramebuffer& operator=(AbstractFramebuffer&& other) noexcept 
  {
    std::swap(_id, other._id);
    std::swap(_viewport, other._viewport);
    std::swap(_flags, other._flags);
    return *this;
  }

  void bindInternal(FramebufferTarget target);
  void setViewportInternal();

  GLuint _id;
  Rectangle2Di _viewport;
  ObjectFlags _flags;

  GLenum checkStatus(FramebufferTarget target);
  void invalidate(const GLsizei count, const GLenum* attachments);
  void invalidate(const GLsizei count, const GLenum* attachments, const Rectangle2Di& rect);

  void cleari(const GLenum buffer, const GLint drawBuffer, const GLint* const value);
  void clearu(const GLenum buffer, const GLint drawBuffer, const GLuint* const value);
  void clearf(const GLenum buffer, const GLint drawBuffer, const GLfloat* const value);
  void clearfi(const GLenum buffer, const GLfloat depth, const GLint stencil);

  void drawBuffer(GLenum buffer);
  void drawBuffers(GLsizei count, const GLenum* buffers);

  void readBuffer(GLenum buffer);
};


ENUM_OPERATORS(FramebufferClearMask, GLbitfield)
ENUM_OPERATORS(FramebufferBlitMask, GLbitfield)
}