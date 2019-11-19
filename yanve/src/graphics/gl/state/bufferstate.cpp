#include <graphics/gl/state/bufferstate.h>
#include <utils/logger.h>

namespace yanve::gl::detail
{

Buffer::Target BufferState::targetForIndex[TargetCount-1] = {
  Buffer::Target::Array,
  Buffer::Target::ElementArray,
  Buffer::Target::CopyWrite,
  Buffer::Target::DrawIndirect,
  Buffer::Target::PixelPack,
  Buffer::Target::PixelUnpack,
  Buffer::Target::TransformFeedback,
  Buffer::Target::CopyRead,
  Buffer::Target::Uniform,
  Buffer::Target::Texture,
  Buffer::Target::AtomicCounter,
  Buffer::Target::DispatchIndirect,
  Buffer::Target::ShaderStorage
};

std::size_t BufferState::indexForTarget(Buffer::Target target)
{
  switch (target) {
  case Buffer::Target::Array:                 return 1;
  case Buffer::Target::ElementArray:          return 2;
  case Buffer::Target::CopyWrite:             return 3;
  case Buffer::Target::DrawIndirect:          return 4;
  case Buffer::Target::PixelPack:             return 5;
  case Buffer::Target::PixelUnpack:           return 6;
  case Buffer::Target::TransformFeedback:     return 7;
  case Buffer::Target::CopyRead:              return 8;
  case Buffer::Target::Uniform:               return 9;
  case Buffer::Target::Texture:               return 10;
  case Buffer::Target::AtomicCounter:         return 11;
  case Buffer::Target::DispatchIndirect:      return 12;
  case Buffer::Target::ShaderStorage:         return 13;
  default:
    LogError("BufferState::indexForTarget", "Reached unreachable code :(");
    return 0;
  }
}

BufferState::BufferState() : bindings{}
{

}

void BufferState::reset()
{
  std::fill_n(bindings, size_t(TargetCount), 0);
}

}