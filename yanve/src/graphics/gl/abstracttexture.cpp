#pragma once

#include <graphics/gl/abstracttexture.h>
#include <graphics/gl/context.h>
#include <graphics/gl/state/state.h>
#include <graphics/gl/state/texturestate.h>
#include <utils\logger.h>

namespace yanve::gl
{
AbstractTexture::AbstractTexture(GLenum target) :
  _flags{ObjectFlags::DestroyOnDestruction},
  _target{target}
{
  //glGenTextures(1, &_id);
  glCreateTextures(target, 1, &_id);
  _flags |= ObjectFlags::Created;
}

AbstractTexture::~AbstractTexture()
{
  if (!_id || !(_flags & ObjectFlags::DestroyOnDestruction)) return;

  // TODO: update global state to reflect deletion
  auto& state = *Context::current().state().texture;

  for (auto& binding : state.bindings) {
    if (binding.second == _id) binding = {};
  }

  for (auto& binding : state.imageBindings) {
    if (std::get<0>(binding) == _id) binding = {};
  }

  glDeleteTextures(1, &_id);
}

void AbstractTexture::unbind(GLuint textureUnit)
{
  auto& state = *Context::current().state().texture;

  if (state.bindings[textureUnit].first == 0) return;

  if (state.currentTextureUnit != textureUnit)
    glActiveTexture(GL_TEXTURE0 + (state.currentTextureUnit = textureUnit));

  if (state.bindings[textureUnit].first == 0) { // sanity check
    LogError("AbstractTexture::unbind", "Bad texture state");
    throw std::runtime_error("AbstractTexture::unbind: Bad texture state");
  }

  glBindTexture(state.bindings[textureUnit].first, 0);

  state.bindings[textureUnit] = {};
}

void AbstractTexture::bind(GLuint textureUnit)
{
  auto& state = *Context::current().state().texture;

  if (state.bindings[textureUnit].second == _id) return;

  state.bindings[textureUnit] = { _target, _id };

  if (state.currentTextureUnit != textureUnit)
    glActiveTexture(GL_TEXTURE0 + (state.currentTextureUnit = textureUnit));
  
  glBindTexture(_target, _id);
}

void AbstractTexture::setBaseLevel(GLint level)
{
  parameter(GL_TEXTURE_BASE_LEVEL, level);
}

void AbstractTexture::setMaxLevel(GLint level)
{
  parameter(GL_TEXTURE_MAX_LEVEL, level);
}

void AbstractTexture::setMagnificationFilter(SamplerFilter filter)
{
  parameter(GL_TEXTURE_MAG_FILTER, GLint(filter));
}

void AbstractTexture::setMinificationFilter(SamplerFilter filter, SamplerMipmap mipmap)
{
  parameter(GL_TEXTURE_MIN_FILTER, GLint(filter) | GLint(mipmap));
}

void AbstractTexture::setMinLevelOfDetail(GLfloat lod)
{
  parameter(GL_TEXTURE_MIN_LOD, lod);
}

void AbstractTexture::setMaxLevelOfDetail(GLfloat lod)
{
  parameter(GL_TEXTURE_MAX_LOD, lod);
}

void AbstractTexture::setLevelOfDetailBias(GLfloat bias)
{
  parameter(GL_TEXTURE_LOD_BIAS, bias);
}

void AbstractTexture::setBorderColor(const glm::vec4& color)
{
  parameter(GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color));
}

void AbstractTexture::setSrgbDecode(bool decode)
{
  parameter(GL_TEXTURE_SRGB_DECODE_EXT, decode ? GL_DECODE_EXT : GL_SKIP_DECODE_EXT);
}

void AbstractTexture::setSwizzle(GLint r, GLint g, GLint b, GLint a)
{
  const GLint rgba[] = { r, g, b, a };
  parameter(GL_TEXTURE_SWIZZLE_RGBA, rgba);
}

void AbstractTexture::setCompareMode(SamplerCompareMode mode)
{
  parameter(GL_TEXTURE_COMPARE_MODE, GLint(mode));
}

void AbstractTexture::setCompareFunction(SamplerCompareFunction function)
{
  parameter(GL_TEXTURE_COMPARE_FUNC, GLint(function));
}

void AbstractTexture::setDepthStencilMode(SamplerDepthStencilMode mode)
{
  parameter(GL_DEPTH_STENCIL_TEXTURE_MODE, GLint(mode));
}

void AbstractTexture::bindInternal()
{
  // TODO: check if bound and change state if not
  glBindTexture(_target, _id);
}

void AbstractTexture::invalidateImage(GLint level)
{
  bindInternal();
  glInvalidateTexImage(_id, level);
}

void AbstractTexture::generateMipmap()
{
  glGenerateTextureMipmap(_id);
}

template <>
void AbstractTexture::image<1>(GLint level, PixelFormat format, PixelType type, byte* imageBuffer, const utils::vecTypeFor<1, uint> imageBufferSize)
{
  getImage(level, GLenum(format), GLenum(type), imageBuffer, imageBufferSize);
}

template <>
void AbstractTexture::image<2>(GLint level, PixelFormat format, PixelType type, byte * imageBuffer, const utils::vecTypeFor<2, uint> imageBufferSize)
{
  getImage(level, GLenum(format), GLenum(type), imageBuffer, (size_t)imageBufferSize.x * (size_t)imageBufferSize.y);
}

template <>
void AbstractTexture::image<3>(GLint level, PixelFormat format, PixelType type, byte * imageBuffer, const utils::vecTypeFor<3, uint> imageBufferSize)
{
  getImage(level, GLenum(format), GLenum(type), imageBuffer, (size_t)imageBufferSize.x * (size_t)imageBufferSize.y * (size_t)imageBufferSize.z);
}

template <>
void AbstractTexture::subImage<1>(GLint level, PixelFormat format, PixelType type, const utils::vecTypeFor<1, u32> & min, const utils::vecTypeFor<1, u32> & max, byte * subImageBuffer, const utils::vecTypeFor<1, uint> subImageBufferSize)
{
  glm::uvec3 offset{ min, 0, 0 };
  glm::uvec3 dims{ max - min, 0, 0 };

  getSubImage(level, offset, dims, GLenum(format), GLenum(type), subImageBufferSize, subImageBuffer);
}

template <>
void AbstractTexture::subImage<2>(GLint level, PixelFormat format, PixelType type, const utils::vecTypeFor<2, u32> & min, const utils::vecTypeFor<2, u32> & max, byte * subImageBuffer, const utils::vecTypeFor<2, uint> subImageBufferSize)
{
  glm::uvec3 offset{ min.x, min.y, 0 };
  glm::uvec3 dims{ max.x - min.x, max.y - min.y, 0 };

  getSubImage(level, offset, dims, GLenum(format), GLenum(type), (size_t)subImageBufferSize.x * (size_t)subImageBufferSize.y, subImageBuffer);
}

template <>
void AbstractTexture::subImage<3>(GLint level, PixelFormat format, PixelType type, const utils::vecTypeFor<3, u32> & min, const utils::vecTypeFor<3, u32> & max, byte * subImageBuffer, const utils::vecTypeFor<3, uint> subImageBufferSize)
{
  glm::uvec3 offset{ min };
  glm::uvec3 dims{ max - min };

  getSubImage(level, offset, dims, GLenum(format), GLenum(type), (size_t)subImageBufferSize.x * (size_t)subImageBufferSize.y * (size_t)subImageBufferSize.z, subImageBuffer);
}

void AbstractTexture::bindImageInternal(GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
{
  // TODO: check if image is already bound
  glBindImageTexture(unit, _id, level, layered, layer, GLenum(access), GLenum(format));
}

void AbstractTexture::parameter(GLenum parameter, GLint value)
{
  glTextureParameteri(_id, parameter, value);
}

void AbstractTexture::parameter(GLenum parameter, GLfloat value)
{
  glTextureParameterf(_id, parameter, value);
}

void AbstractTexture::parameter(GLenum parameter, GLint *values)
{
  glTextureParameteriv(_id, parameter, values);
}

void AbstractTexture::parameter(GLenum parameter, const GLint* values)
{
  glTextureParameteriv(_id, parameter, values);
}

void AbstractTexture::parameter(GLenum parameter, GLfloat* values)
{
  glTextureParameterfv(_id, parameter, values);
}

void AbstractTexture::parameter(GLenum parameter, const GLfloat* values)
{
  glTextureParameterfv(_id, parameter, values);
}

void AbstractTexture::levelParameter(GLint level, GLenum parameter, GLint* const values)
{
  glGetTextureLevelParameteriv(_id, level, parameter, values);
}

void AbstractTexture::levelParameter(GLint level, GLenum parameter, GLfloat* const values)
{
  glGetTextureLevelParameterfv(_id, level, parameter, values);
}

void AbstractTexture::storage(GLsizei levels, TextureFormat internalFormat, const utils::vecTypeFor<1, uint>& size)
{
  glTextureStorage1D(_id, levels, GLenum(internalFormat), size);
}

void AbstractTexture::storage(GLsizei levels, TextureFormat internalFormat, const utils::vecTypeFor<2, uint>& size)
{
  glTextureStorage2D(_id, levels, GLenum(internalFormat), size.x, size.y);
}

void AbstractTexture::storage(GLsizei levels, TextureFormat internalFormat, const utils::vecTypeFor<3, uint>& size)
{
  glTextureStorage3D(_id, levels, GLenum(internalFormat), size.x, size.y, size.z);
}

void AbstractTexture::getImage(GLint level, GLenum format, GLenum type, byte* imageBuffer, std::size_t imageBufferSize)
{
  glGetTextureImage(_id, level, format, type, imageBufferSize, imageBuffer);
}

void AbstractTexture::getSubImage(GLint level, const glm::uvec3 offset, const glm::uvec3 dims, GLenum format, GLenum type, const std::size_t subImageBufferSize, byte* subImageBuffer)
{
  glGetTextureSubImage(_id, level, offset.x, offset.y, offset.z, dims.x, dims.y, dims.z, format, type, subImageBufferSize, subImageBuffer);
}

size_t AbstractTexture::Helper<1>::imageSize(AbstractTexture & texture, GLint level)
{
  GLint value;
  texture.levelParameter(level, GL_TEXTURE_WIDTH, &value);

  return (size_t)value;
}

void AbstractTexture::Helper<1>::setWrapping(AbstractTexture & texture, const std::array<SamplerWrapping, 1>& wrapping)
{
  texture.parameter(GL_TEXTURE_WRAP_S, GLint(wrapping[0]));
}

void AbstractTexture::Helper<1>::setStorage(AbstractTexture& texture, GLsizei levels, TextureFormat internalFormat, GLsizei size)
{
  texture.storage(levels, internalFormat, size);
}

void AbstractTexture::Helper<1>::setImage(AbstractTexture & texture, GLint level, TextureFormat internalFormat, PixelFormat format, PixelType type, byte * imageBuffer, size_t size)
{
  texture.bindInternal();
  glTexImage1D(texture._target, level, GLint(internalFormat), size, 0, GLenum(format), GLenum(type), imageBuffer);
}

void AbstractTexture::Helper<1>::setSubImage(AbstractTexture & texture, GLint level, const size_t& offset, const size_t& width, PixelFormat format, PixelType type, byte * imageBuffer, size_t size)
{
  texture.bindInternal();
  glTexSubImage1D(texture._target, level, offset, width, GLenum(format), GLenum(type), imageBuffer);
}

glm::uvec2 AbstractTexture::Helper<2>::imageSize(AbstractTexture & texture, GLint level)
{
  glm::ivec2 value;
  texture.levelParameter(level, GL_TEXTURE_WIDTH, &value[0]);
  texture.levelParameter(level, GL_TEXTURE_HEIGHT, &value[1]);

  return glm::uvec2(value);
}

void AbstractTexture::Helper<2>::setWrapping(AbstractTexture & texture, const std::array<SamplerWrapping, 2>& wrapping)
{
  texture.parameter(GL_TEXTURE_WRAP_S, GLint(wrapping[0]));
  texture.parameter(GL_TEXTURE_WRAP_T, GLint(wrapping[1]));
}

void AbstractTexture::Helper<2>::setStorage(AbstractTexture & texture, GLsizei levels, TextureFormat internalFormat, const glm::uvec2 & size)
{
  texture.storage(levels, internalFormat, size);
}

void AbstractTexture::Helper<2>::setImage(AbstractTexture & texture, GLint level, TextureFormat internalFormat, PixelFormat format, PixelType type, byte * imageBuffer, const glm::uvec2 & size)
{
  texture.bindInternal();
  glTexImage2D(texture._target, level, GLint(internalFormat), size.x, size.y, 0, GLenum(format), GLenum(type), imageBuffer);
}

void AbstractTexture::Helper<2>::setSubImage(AbstractTexture & texture, GLint level, const glm::uvec2 & offset, const glm::uvec2 & dims, PixelFormat format, PixelType type, byte * imageBuffer, const glm::uvec2 & size)
{
  texture.bindInternal();
  glTexSubImage2D(texture._target, level, offset.x, offset.y, dims.x, dims.y, GLenum(format), GLenum(type), imageBuffer);
}

glm::uvec3 AbstractTexture::Helper<3>::imageSize(AbstractTexture & texture, GLint level)
{
  glm::ivec3 value;
  texture.levelParameter(level, GL_TEXTURE_WIDTH, &value[0]);
  texture.levelParameter(level, GL_TEXTURE_HEIGHT, &value[1]);
  texture.levelParameter(level, GL_TEXTURE_DEPTH, &value[2]);

  return glm::uvec3(value);
}

void AbstractTexture::Helper<3>::setWrapping(AbstractTexture & texture, const std::array<SamplerWrapping, 3> & wrapping)
{
  texture.parameter(GL_TEXTURE_WRAP_S, GLint(wrapping[0]));
  texture.parameter(GL_TEXTURE_WRAP_T, GLint(wrapping[1]));
  texture.parameter(GL_TEXTURE_WRAP_R, GLint(wrapping[2]));
}

void AbstractTexture::Helper<3>::setStorage(AbstractTexture & texture, GLsizei levels, TextureFormat internalFormat, const glm::uvec3 & size)
{
  texture.storage(levels, internalFormat, size);
}

void AbstractTexture::Helper<3>::setImage(AbstractTexture & texture, GLint level, TextureFormat internalFormat, PixelFormat format, PixelType type, byte * imageBuffer, const glm::uvec3 & size)
{
  texture.bindInternal();
  glTexImage3D(texture._target, level, GLint(internalFormat), size.x, size.y, size.z, 0, GLenum(format), GLenum(type), imageBuffer);
}

void AbstractTexture::Helper<3>::setSubImage(AbstractTexture & texture, GLint level, const glm::uvec3 & offset, const glm::uvec3 & dims, PixelFormat format, PixelType type, byte * imageBuffer, const glm::uvec3 & size)
{
  texture.bindInternal();
  glTexSubImage3D(texture._target, level, offset.x, offset.y, offset.z, dims.x, dims.y, dims.z, GLenum(format), GLenum(type), imageBuffer);
}

}