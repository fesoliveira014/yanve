#include <graphics/gl/state/state.h>

#include <graphics/gl/state/texturestate.h>
#include <graphics/gl/state/framebufferstate.h>
#include <graphics/gl/state/bufferstate.h>
#include <graphics/gl/state/meshstate.h>
#include <graphics/gl/state/shaderstate.h>

#include <graphics/gl/context.h>

namespace yanve::gl::detail
{

State::State(Context& context) :
  buffer{ new BufferState() },
  mesh{new MeshState()},
  texture{new TextureState()},
  framebuffer{new FramebufferState()},
  shader{new ShaderState()}
{
  static_cast<void>(context);
}

State::~State() = default;

}