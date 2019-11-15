#include <graphics/gl/state/framebufferstate.h>
#include <graphics/gl/state/state.h>

namespace yanve::gl::state
{

FramebufferState::FramebufferState() : readBinding{}, writeBinding{}, renderbufferBinding{}, viewport{FramebufferState::DisengagedViewport}
{}

void FramebufferState::reset()
{
  readBinding = writeBinding = renderbufferBinding = State::DisengagedBinding;
  viewport = DisengagedViewport;
}

}