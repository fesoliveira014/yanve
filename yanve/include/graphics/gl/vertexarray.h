#pragma once

#include <common.h>
#include <graphics/gl/attribute.h>

namespace yanve::gl
{

class Buffer;

class VertexArray
{
public:
  explicit VertexArray();
  VertexArray(const VertexArray&) = delete;
  VertexArray(VertexArray&& other);

  ~VertexArray();

  template<class ...T>
  VertexArray& addBuffer(Buffer& buffer, size_t offset, const T&... attributes);

private:
  GLuint _handle;
};

}