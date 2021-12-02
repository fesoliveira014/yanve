#include <graphics/gl/cubemaptexture.h>

namespace yanve::gl
{

CubeMapTexture& CubeMapTexture::setSubImage(const CubeMapCoordinate coordinate, const GLint level, const glm::vec2& offset, const PixelFormat format, const PixelType type, const GLvoid* const data, const glm::vec2& size)
{
  glTextureSubImage3D(_id, level, offset.x, offset.y, GLenum(coordinate) - GL_TEXTURE_CUBE_MAP_POSITIVE_X, size.x, size.y, 1, GLenum(format), GLenum(type), data);
  return *this;
}

}