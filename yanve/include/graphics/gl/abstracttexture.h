#pragma once

#include <common.h>
#include <graphics/gl/globject.h>
#include <graphics/gl/sampler.h>
#include <graphics/gl/textureformat.h>
#include <graphics/gl/pixelformat.h>
#include <utils/nocreate.h>
#include <utils/dimension.h>


namespace yanve::gl
{
class YANVE_API AbstractTexture : public GLObject
{
  friend class AbstractFramebuffer;

public:
  AbstractTexture(const AbstractTexture&) = delete;
  AbstractTexture(AbstractTexture&& other) noexcept;
   
  ~AbstractTexture();

  AbstractTexture& operator=(const AbstractTexture&) = delete;
  AbstractTexture& operator=(AbstractTexture&& other) noexcept;

  void bind(GLuint textureUnit);
  void unbind(GLuint textureUnit);
  GLuint release();

  GLuint id() const { return _id; }

protected:
  template <size_t dimensionss> struct Helper {};

  explicit AbstractTexture(GLenum target);
  explicit AbstractTexture(NoCreateT, GLenum target) : _target{ target }, _id{ 0 }, _flags{ ObjectFlag::DestroyOnDestruction } {};
  explicit AbstractTexture(GLuint id, GLenum target, ObjectFlags flags) : _target{target}, _id{id}, _flags{flags} {}

  void bindImageInternal(GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);

  void setBaseLevel(GLint level);
  void setMaxLevel(GLint level);
  
  void setMagnificationFilter(SamplerFilter filter);
  void setMinificationFilter(SamplerFilter filter, SamplerMipmap mipmap);

  void setMinLevelOfDetail(GLfloat lod);
  void setMaxLevelOfDetail(GLfloat lod);

  void setLevelOfDetailBias(GLfloat bias);

  void setBorderColor(const glm::vec4& color);

  // void setMaxAnisotropy(GLfloat anisotropy);

  void setSrgbDecode(bool decode);

  void setSwizzle(const glm::ivec4& rgba) {
    setSwizzle(rgba.x, rgba.y, rgba.z, rgba.w);
  }
  void setSwizzle(GLint r, GLint g, GLint b, GLint a);

  void setCompareMode(SamplerCompareMode mode);
  void setCompareFunction(SamplerCompareFunction function);
  void setDepthStencilMode(SamplerDepthStencilMode mode);

  void bindInternal();
  void invalidateImage(GLint level);
  void generateMipmap();

   template <size_t dimensions> void image(GLint level, PixelFormat format, PixelType type, byte* imageBuffer, const utils::vecTypeFor<dimensions, uint> imageBufferSize);
  // template <size_t dimensions> void compressedImage(GLint level, byte* compressedImageBuffer, const utils::vecTypeFor<dimensions, uint> compressedImageBufferSize);
   template <size_t dimensions> void subImage(GLint level, PixelFormat format, PixelType type, const utils::vecTypeFor<dimensions, u32>& min, const utils::vecTypeFor<dimensions, u32>& max, byte* subImageBuffer, const utils::vecTypeFor<dimensions, uint> subImageBufferSize);
  // template <size_t dimensions> void compressedSubImage(GLint level, const utils::vecTypeFor<dimensions, u32>& min, const utils::vecTypeFor<dimensions, u32>& max, byte* compressedSubImageBuffer, const utils::vecTypeFor<dimensions, uint> compressedSubImageBufferSize);

private:
  void parameter(GLenum parameter, GLint value);
  void parameter(GLenum parameter, GLfloat value);
  void parameter(GLenum parameter, GLint *values);
  void parameter(GLenum parameter, const GLint* values);
  void parameter(GLenum parameter, GLfloat *values);
  void parameter(GLenum parameter, const GLfloat* values);
  void levelParameter(GLint level, GLenum paremeter, GLint* const values);
  void levelParameter(GLint level, GLenum parameter, GLfloat* const values);

  void storage(GLsizei levels, TextureFormat internalFormat, const utils::vecTypeFor<1, uint>& size);
  void storage(GLsizei levels, TextureFormat internalFormat, const utils::vecTypeFor<2, uint>& size);
  void storage(GLsizei levels, TextureFormat internalFormat, const utils::vecTypeFor<3, uint>& size);

  void getImage(GLint level, GLenum format, GLenum type, byte* imageBuffer, std::size_t imageBufferSize);
  // void getCompressedImage(GLint level, GLenum format, GLenum type, byte* compressedImageBuffer, const std::size_t compressedImageBufferSize);
  void getSubImage(GLint level, const glm::uvec3 offset, const glm::uvec3 dims, GLenum format, GLenum type, const std::size_t subImageBufferSize, byte* subImageBuffer);
  // void getCompressedSubImage(GLint level, const std::size_t min, const std::size_t max, byte* subImageBuffer, const std::size_t subImageBufferSize);

protected:
  GLuint _id;
  GLenum _target;
  ObjectFlags _flags;

};

template <> struct YANVE_API AbstractTexture::Helper<1>
{
  static size_t imageSize(AbstractTexture& texture, GLint level);

  static void setWrapping(AbstractTexture& texture, const std::array<SamplerWrapping, 1>& wrapping);
  static void setStorage(AbstractTexture& texture, GLsizei levels, TextureFormat internalFormat, GLsizei size);
  
  static void setImage(AbstractTexture& texture, GLint level, TextureFormat internalFormat, PixelFormat format, PixelType type, byte* imageBuffer, size_t size) {
    setImage(texture, texture._target, level, internalFormat, format, type, imageBuffer, size);
  }
  static void setImage(AbstractTexture& texture, GLenum target, GLint level, TextureFormat internalFormat, PixelFormat format, PixelType type, byte* imageBuffer, size_t size);
  //static void setCompressedImage(AbstractTexture& texture, GLint level, byte* imageBuffer, size_t size);
  
  static void setSubImage(AbstractTexture& texture, GLint level, const size_t& offset, const size_t& width, PixelFormat format, PixelType type, byte* imageBuffer, size_t size);
  //static void setCompressedSubImage(AbstractTexture& texture, GLint level, size_t offset, byte* imageBuffer, size_t size);

  // invalidateSubImage?
};

template <> struct YANVE_API AbstractTexture::Helper<2>
{
  static glm::uvec2 imageSize(AbstractTexture& texture, GLint level);

  static void setWrapping(AbstractTexture& texture, const std::array<SamplerWrapping, 2> & wrapping);
  static void setStorage(AbstractTexture& texture, GLsizei levels, TextureFormat internalFormat, const glm::uvec2& size);

  /*static void setImage(AbstractTexture& texture, GLint level, TextureFormat internalFormat, PixelFormat format, PixelType type, byte* imageBuffer, const glm::uvec2& size) {
    setImage(texture, texture._target, level, internalFormat, format, type, imageBuffer, size);
  }*/
  static void setImage(AbstractTexture& texture, GLint level, TextureFormat internalFormat, PixelFormat format, PixelType type, byte* imageBuffer, const glm::uvec2& size) {
    setImage(texture, texture._target, level, internalFormat, format, type, imageBuffer, size);
  }
  static void setImage(AbstractTexture& texture, GLenum target, GLint level, TextureFormat internalFormat, PixelFormat format, PixelType type, byte* imageBuffer, const glm::uvec2& size);
  //static void setCompressedImage(AbstractTexture& texture, GLint level, byte* imageBuffer, const glm::uvec2& size) {
  //  setCompressedImage(texture, texture._target, imageBuffer, size);
  //}
  //static void setCompressedImage(AbstractTexture& texture, GLenum target, GLint level, byte* imageBuffer, const glm::uvec2& size);
  
  static void setSubImage(AbstractTexture& texture, GLint level, const glm::uvec2& offset, const glm::uvec2& dims, PixelFormat format, PixelType type, byte* imageBuffer, const glm::uvec2& size);
  //static void setCompressedSubImage(AbstractTexture& texture, GLint level, size_t offset, byte* imageBuffer, const glm::uvec2& size);

  // invalidateSubImage?
};

template <> struct YANVE_API AbstractTexture::Helper<3>
{
  static glm::uvec3 imageSize(AbstractTexture& texture, GLint level);

  static void setWrapping(AbstractTexture& texture, const std::array<SamplerWrapping, 3>& wrapping);
  static void setStorage(AbstractTexture& texture, GLsizei levels, TextureFormat internalFormat, const glm::uvec3& size);

  static void setImage(AbstractTexture& texture, GLint level, TextureFormat internalFormat, PixelFormat format, PixelType type, byte* imageBuffer, const glm::uvec3& size);
  //static void setCompressedImage(AbstractTexture& texture, GLenum target, GLint level, byte* imageBuffer, const glm::uvec3& size);

  static void setSubImage(AbstractTexture& texture, GLint level, const glm::uvec3& offset, const glm::uvec3& dims, PixelFormat format, PixelType type, byte* imageBuffer, const glm::uvec3& size);
  //static void setCompressedSubImage(AbstractTexture& texture, GLint level, size_t offset, byte* imageBuffer, const glm::uvec3& size);

  // invalidateSubImage?
};

inline AbstractTexture::AbstractTexture(AbstractTexture&& other) noexcept :
  _target{ other._target }, _id{ other._id }, _flags{ other._flags }
{
  other._id = 0;
}

inline AbstractTexture& AbstractTexture::operator =(AbstractTexture&& other) noexcept
{
  std::swap(_target, other._target);
  std::swap(_id, other._id);
  std::swap(_flags, other._flags);
  other._id = 0;
  return *this;
}

inline GLuint AbstractTexture::release()
{
  const GLuint id = _id;
  _id = 0;
  return id;
}

}