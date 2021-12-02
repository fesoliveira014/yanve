#pragma once

#include <common.h>
#include <graphics/gl/abstracttexture.h>
#include <graphics/gl/pixelformat.h>
#include <graphics/gl/sampler.h>
#include <utils/nocreate.h>

namespace yanve::gl
{

enum class CubeMapCoordinate : GLenum 
{
  PositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
  NegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
  PositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
  NegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
  PositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
  NegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
};

class YANVE_API CubeMapTexture : public AbstractTexture
{
public:
  static CubeMapTexture wrap(GLuint id, ObjectFlags flags = {})
  {
    return CubeMapTexture{ id, flags };
  }

  CubeMapTexture() : AbstractTexture{GL_TEXTURE_CUBE_MAP} {}

  explicit CubeMapTexture(NoCreateT) noexcept : AbstractTexture{NoCreate, GL_TEXTURE_CUBE_MAP} {}
  
  CubeMapTexture(const CubeMapTexture&) = delete;
  CubeMapTexture(CubeMapTexture&&) noexcept = default;

  CubeMapTexture& operator=(const CubeMapTexture&) = delete;
  CubeMapTexture& operator=(CubeMapTexture&&) noexcept = default;

  void bindImage(GLint imageUnit, GLint level, CubeMapCoordinate coordinate, GLenum access, GLenum format) {
    bindImageInternal(imageUnit, level, false, GLenum(coordinate) - GL_TEXTURE_CUBE_MAP_POSITIVE_X, access, format);
  }

  void bindImageLayered(GLint imageUnit, GLint level, CubeMapCoordinate coordinate, GLenum access, GLenum format) {
    bindImageInternal(imageUnit, level, true, 0, access, format);
  }

  CubeMapTexture& setBaseLevel(GLint level)
  {
    AbstractTexture::setBaseLevel(level);
    return *this;
  }

  CubeMapTexture& setMaxLevel(GLint level)
  {
    AbstractTexture::setMaxLevel(level);
    return *this;
  }

  CubeMapTexture& setMinificationFilter(SamplerFilter filter, SamplerMipmap mipmap = SamplerMipmap::Base)
  {
    AbstractTexture::setMinificationFilter(filter, mipmap);
    return *this;
  }

  CubeMapTexture& setMagnificationFilter(SamplerFilter filter)
  {
    AbstractTexture::setMagnificationFilter(filter);
    return *this;
  }

  CubeMapTexture& setMinLod(GLfloat lod)
  {
    AbstractTexture::setMinLevelOfDetail(lod);
    return *this;
  }

  CubeMapTexture& setMaxLod(GLfloat lod)
  {
    AbstractTexture::setMaxLevelOfDetail(lod);
    return *this;
  }

  CubeMapTexture& setWrapping(const std::array<SamplerWrapping, 2>& wrapping)
  {
    Helper<2>::setWrapping(*this, wrapping);
    return *this;
  }

  CubeMapTexture& setWrapping(const std::array<SamplerWrapping, 3>& wrapping)
  {
    Helper<3>::setWrapping(*this, wrapping);
    return *this;
  }

  CubeMapTexture& setBorderColor(const glm::vec4& color)
  {
    AbstractTexture::setBorderColor(color);
    return *this;
  }

  CubeMapTexture& setSrgbDecode(bool decode)
  {
    AbstractTexture::setSrgbDecode(decode);
    return *this;
  }

  CubeMapTexture& setSwizzle(const glm::vec4& rgba)
  {
    AbstractTexture::setSwizzle(rgba);
    return *this;
  }

  CubeMapTexture& setCompareMode(SamplerCompareMode mode)
  {
    AbstractTexture::setCompareMode(mode);
    return *this;
  }

  CubeMapTexture& setCompareFunction(SamplerCompareFunction function)
  {
    AbstractTexture::setCompareFunction(function);
    return *this;
  }

  CubeMapTexture& setStencilDepthMode(SamplerDepthStencilMode mode)
  {
    AbstractTexture::setDepthStencilMode(mode);
    return *this;
  }

  CubeMapTexture& setStorage(GLint levels, TextureFormat internalFormat, const glm::vec2& size)
  {
    AbstractTexture::Helper<2>::setStorage(*this, levels, internalFormat, size);
    return *this;
  }


  //void image(const CubeMapCoordinate coordinate, GLint level, PixelFormat format, PixelType type, byte* buffer, const glm::vec3& size);
  //void subImage(const CubeMapCoordinate coordinate, GLint level, const glm::vec2& offset, const glm::vec2& size, const PixelFormat format, const PixelType type, const GLvoid* const data);
  
  CubeMapTexture& setImage(CubeMapCoordinate coordinate, GLint level, TextureFormat internalFormat, PixelFormat format, PixelType type, byte* buffer, const glm::vec2& size)
  {
    AbstractTexture::Helper<2>::setImage(*this, GLenum(coordinate), level, internalFormat, format, type, buffer, size);
    return *this;
  }

  CubeMapTexture& setSubImage(const CubeMapCoordinate coordinate, const GLint level, const glm::vec2& offset, const PixelFormat format, const PixelType type, const GLvoid* const data, const glm::vec2& size);

private:
  explicit CubeMapTexture(GLuint id, ObjectFlags flags) noexcept : 
    AbstractTexture{ id, GL_TEXTURE_CUBE_MAP, flags } {}
};

}