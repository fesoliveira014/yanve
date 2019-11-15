#include <graphics/gl/state/texturestate.h>
#include <graphics/gl/state/state.h>

namespace yanve::gl::state
{
  TextureState::TextureState() /*:
    maxSize{}, max3DSize{}, maxCubeMapSize{}, maxRectangleSize{},
    maxBufferSize{}, maxLodBias{}, maxColorSamples{}, maxDepthSamples{},
    maxIntegerSamples{}*/
  {
    // TODO: add some other mechanism instead of exception
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    if (maxTextureUnits <= 0) throw std::runtime_error("Could not get number of texture units");

    bindings.resize(maxTextureUnits);

    GLint maxImageUnits;
    glGetIntegerv(GL_MAX_IMAGE_UNITS, &maxImageUnits);
    imageBindings.resize(maxImageUnits);

    currentTextureUnit = 0;
    bufferOffsetAlignment = 0;
  }

  TextureState::~TextureState() = default;

  void TextureState::reset()
  {
    std::fill_n(bindings.begin(), bindings.size(), std::pair<GLenum, GLuint>{ {}, State::DisengagedBinding});
    std::fill_n(imageBindings.begin(), imageBindings.size(), std::tuple<GLuint, GLint, GLboolean, GLint, GLenum>{State::DisengagedBinding, 0, false, 0, 0});
  }




}