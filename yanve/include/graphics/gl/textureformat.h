#pragma once

#include <common.h>

namespace yanve::gl
{

enum class TextureFormat : GLint
{
  Red = GL_RED,
  RG = GL_RG,
  RGB = GL_RGB,
  RGBA = GL_RGBA,
  R8 = GL_R8,
  RG8 = GL_RG8,
  RGB8 = GL_RGB8,
  RGBA8 = GL_RGBA8,
  R16 = GL_R16,
  RG16 = GL_RG16,
  RGB16 = GL_RGB16,
  RGBA16 = GL_RGBA16,
  R32I = GL_R32I,
  RG32I = GL_RG32I,
  RGB32I = GL_RGB32I,
  RGBA32I = GL_RGBA32I,
  R32F = GL_R32F,
  RG32F = GL_RG32F,
  RGB32F = GL_RGB32F,
  RGBA32F = GL_RGBA32F,
  Luminance = GL_LUMINANCE,
  LuminanceAlpha = GL_LUMINANCE_ALPHA,
  CompressedRed = GL_COMPRESSED_RED,
  CompressedRG = GL_COMPRESSED_RG,
  CompressedRGB = GL_COMPRESSED_RGB,
  CompressedRGBA = GL_COMPRESSED_RGBA,
  DepthComponent = GL_DEPTH_COMPONENT,
  DepthComponent16 = GL_DEPTH_COMPONENT16,
  DepthComponent32 = GL_DEPTH_COMPONENT32,
  DepthComponent32F = GL_DEPTH_COMPONENT32F,
  StencilIndex8 = GL_STENCIL_INDEX8,
  DepthStencil = GL_DEPTH_STENCIL,
  Depth24Stencil8 = GL_DEPTH24_STENCIL8,
  Depth32FStencil8 = GL_DEPTH32F_STENCIL8
};

}