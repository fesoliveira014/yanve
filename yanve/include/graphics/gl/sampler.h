#pragma once

#include <common.h>

namespace yanve::gl
{

enum class SamplerFilter : GLint
{
  Nearest = GL_NEAREST,
  Linear = GL_LINEAR
};

enum class SamplerMipmap : GLint
{
  Base = GL_NEAREST & ~GL_NEAREST,
  Nearest = GL_NEAREST_MIPMAP_LINEAR & ~GL_NEAREST,
  Linear = GL_LINEAR_MIPMAP_LINEAR & ~GL_NEAREST
};

enum class SamplerWrapping : GLint
{
  Repeat = GL_REPEAT,
  MirroedRepeat = GL_MIRRORED_REPEAT,
  ClampToEdge = GL_CLAMP_TO_EDGE,
  ClampToBorder = GL_CLAMP_TO_BORDER,
  MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
};

enum class SamplerCompareMode : GLint
{
  None = GL_NONE,
  CompareRefToTexture = GL_COMPARE_REF_TO_TEXTURE
};

enum class SamplerCompareFunction : GLint
{
  Never = GL_NEVER,
  Always = GL_ALWAYS,
  Less = GL_LESS,
  LessOrEqual = GL_LEQUAL,
  Equal = GL_EQUAL,
  NotEqual = GL_NOTEQUAL,
  GreaterOrEqual = GL_GEQUAL,
  Greater = GL_GREATER
};

enum class SamplerDepthStencilMode : GLint
{
  DepthComponent = GL_DEPTH_COMPONENT,
  StencilIndex = GL_STENCIL_INDEX
};

}