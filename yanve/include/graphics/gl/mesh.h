#pragma once

#include <common.h>
#include <graphics/gl/buffer.h>
#include <graphics/gl/attribute.h>

namespace yanve::gl
{

enum class MeshPrimitive
{
  Points = GL_POINTS,
  Lines = GL_LINES,
  LineLoop = GL_LINE_LOOP,
  LineStrip = GL_LINE_STRIP,
  Triangles = GL_TRIANGLES,
  TriangleFan = GL_TRIANGLE_FAN,
  TriangleStrip = GL_TRIANGLE_STRIP,
  Patches = GL_PATCHES,
  
  // GL > 3.2
  LinesAdjacency = GL_LINES_ADJACENCY,
  LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
  TriangleAdjacency = GL_TRIANGLES_ADJACENCY,
  TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY
};

class Mesh
{
public:
  explicit Mesh(MeshPrimitive primitive = MeshPrimitive::Triangles);
  
  Mesh& draw();

  template <class ... T>
  Mesh& addBuffer(const Buffer& buffer, GLintptr offset, const T&... attributes); // some overloads to this
  
};

}