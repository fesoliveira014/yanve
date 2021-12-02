#pragma once

#include <common.h>
#include <graphics/gl/abstracttexture.h>
#include <graphics/gl/pixelformat.h>
#include <graphics/gl/sampler.h>
#include <utils/nocreate.h>

namespace yanve::gl
{
namespace impl
{
  template <size_t> constexpr GLenum textureTarget();
  template <> constexpr GLenum textureTarget<1>() { return GL_TEXTURE_1D; }
  template <> constexpr GLenum textureTarget<2>() { return GL_TEXTURE_2D; }
  template <> constexpr GLenum textureTarget<3>() { return GL_TEXTURE_3D; }
}

template <size_t dimensions>
class YANVE_API Texture : public AbstractTexture
{
public:
  enum : size_t {
    Dimensions = dimensions
  };

  static Texture<dimensions> wrap(GLuint id, ObjectFlags flags = {}) {
    return Texture<dimensions>{id, flags};
  }

  explicit Texture() : AbstractTexture{ impl::textureTarget<dimensions>() } {}
  explicit Texture(NoCreateT) noexcept : AbstractTexture{ NoCreate,
  impl::textureTarget<dimensions>() } {}

  Texture(const Texture<dimensions>&) = delete;
  Texture(Texture<dimensions>&&) = default;

  Texture<dimensions>& operator=(const Texture<dimensions>&) = delete;
  Texture<dimensions>& operator=(Texture<dimensions>&&) = default;

  template <size_t d = dimensions, class = typename std::enable_if<d == 1 || d == 2>::type>
  void bindImage(GLint imageUnit, GLint level, GLenum access, GLenum format) {
    bindImageInternal(imageUnit, level, false, 0, access, format);
  }

  template <size_t d = dimensions, class = typename std::enable_if<d == 3>::type>
  void bindImage(GLint imageUnit, GLint level, GLint layer, GLenum access, GLenum format) {
    bindImageInternal(imageUnit, level, true, layer, access, format);
  }

  Texture<dimensions>& setBaseLevel(GLint level)
  {
    AbstractTexture::setBaseLevel(level);
    return *this;
  }

  Texture<dimensions>& setMaxLevel(GLint level)
  {
    AbstractTexture::setMaxLevel(level);
    return *this;
  }

  Texture<dimensions>& setMinificationFilter(SamplerFilter filter, SamplerMipmap mipmap = SamplerMipmap::Base)
  {
    AbstractTexture::setMinificationFilter(filter, mipmap);
    return *this;
  }

  Texture<dimensions>& setMagnificationFilter(SamplerFilter filter)
  {
    AbstractTexture::setMagnificationFilter(filter);
    return *this;
  }

  Texture<dimensions>& setMinLod(GLfloat lod)
  {
    AbstractTexture::setMinLevelOfDetail(lod);
    return *this;
  }

  Texture<dimensions>& setMaxLod(GLfloat lod)
  {
    AbstractTexture::setMaxLevelOfDetail(lod);
    return *this;
  }

  Texture<dimensions>& setWrapping(const std::array<SamplerWrapping, dimensions>& wrapping)
  {
    Helper<dimensions>::setWrapping(*this, wrapping);
    return *this;
  }

  Texture<dimensions>& setBorderColor(const glm::vec4& color)
  {
    AbstractTexture::setBorderColor(color);
    return *this;
  }

  Texture<dimensions>& setSrgbDecode(bool decode)
  {
    AbstractTexture::setSrgbDecode(decode);
    return *this;
  }

  Texture<dimensions>& setSwizzle(const glm::vec4& rgba)
  {
    AbstractTexture::setSwizzle(rgba);
    return *this;
  }

  Texture<dimensions>& setCompareMode(SamplerCompareMode mode)
  {
    AbstractTexture::setCompareMode(mode);
    return *this;
  }

  Texture<dimensions>& setCompareFunction(SamplerCompareFunction function)
  {
    AbstractTexture::setCompareFunction(function);
    return *this;
  }

  Texture<dimensions>& setStencilDepthMode(SamplerDepthStencilMode mode)
  {
    AbstractTexture::setDepthStencilMode(mode);
    return *this;
  }

  void image(GLint level, PixelFormat format, PixelType type, byte* buffer, const utils::vecTypeFor<dimensions, uint>& size)
  {
    AbstractTexture::image<dimensions>(level, format, type, buffer, size);
  }

  void subImage(GLint level, PixelFormat format, PixelType type, const utils::vecTypeFor<dimensions, u32>& min, const utils::vecTypeFor<dimensions, u32>& max, byte* subImageBuffer, const utils::vecTypeFor<dimensions, uint> subImageBufferSize)
  {
    AbstractTexture::subImage<dimensions>(level, format, type, min, max, subImageBuffer, subImageBufferSize);
  }

  Texture<dimensions>& setImage(GLint level, TextureFormat internalFormat, PixelFormat format, PixelType type, byte* buffer, const utils::vecTypeFor<dimensions, uint>& size)
  {
    AbstractTexture::Helper<dimensions>::setImage(*this, level, internalFormat, format, type, buffer, size);
    return *this;
  }

  Texture<dimensions>& setSubImage(GLint level, const utils::vecTypeFor<dimensions, uint>& offset, const utils::vecTypeFor<dimensions, uint>& dims, PixelFormat format, PixelType type, byte* buffer, const utils::vecTypeFor<dimensions, uint>& size)
  {
    AbstractTexture::Helper<dimensions>::setSubImage(*this, level, offset, dims, format, type, buffer, size);
    return *this;
  }

  Texture<dimensions>& setStorage(GLint levels, TextureFormat internalFormat, const utils::vecTypeFor<dimensions, uint>& size)
  {
    AbstractTexture::Helper<dimensions>::setStorage(*this, levels, internalFormat, size);
    return *this;
  }

  Texture<dimensions>& generateMipMap()
  {
    AbstractTexture::generateMipmap();
    return *this;
  }

private:
  explicit Texture(GLuint id, ObjectFlags flags) noexcept :
    AbstractTexture{ id, impl::textureTarget<dimensions>(), flags } {}
};

typedef Texture<1> Texture1D;
typedef Texture<2> Texture2D;
typedef Texture<3> Texture3D;

}
