#include <graphics/gl/state/shaderstate.h>
#include <graphics/gl/state/state.h>

namespace yanve::gl::detail
{

ShaderState::ShaderState() : current{} {}

ShaderState::~ShaderState() = default;

void ShaderState::reset()
{
  current = State::DisengagedBinding;
}

}