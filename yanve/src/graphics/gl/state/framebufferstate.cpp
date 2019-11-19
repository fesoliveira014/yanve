#include <graphics/gl/state/framebufferstate.h>
#include <graphics/gl/state/state.h>

namespace yanve::gl::detail
{

  FramebufferState::FramebufferState() : readBinding{}, writeBinding{}, renderbufferBinding{}, viewport{ FramebufferState::DisengagedViewport }, maxColorAttachments{}
{}

void FramebufferState::reset()
{
  readBinding = writeBinding = renderbufferBinding = State::DisengagedBinding;
  viewport = DisengagedViewport;
}

}