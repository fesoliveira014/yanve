#pragma once

#include <common.h>

namespace yanve::gl
{

class Context;

namespace state 
{
struct BufferState;
struct MeshState;
struct TextureState;
struct RendererState;
struct FramebufferState;
struct ShaderState;

struct State
{
  enum : u32 {
    DisengagedBinding = 0
  };

  explicit State(Context& context);
  ~State();

  std::unique_ptr<BufferState>      buffer;
  std::unique_ptr<MeshState>        mesh;
  std::unique_ptr<TextureState>     texture;
  std::unique_ptr<FramebufferState> framebuffer;
  std::unique_ptr<ShaderState>      shader;
  //std::unique_ptr<RendererState>   renderer;
};

}
}