#pragma once

#include <common.h>
#include <graphics/gl/globject.h>

namespace yanve::gl
{

  class YANVE_API Shader : protected GLObject
  {
  public:
    enum class Type : GLenum
    {
      Vertex = GL_VERTEX_SHADER,
      Fragment = GL_FRAGMENT_SHADER,
      Geometry = GL_GEOMETRY_SHADER,
      TesselationControl = GL_TESS_CONTROL_SHADER,
      TesselationEvaluation = GL_TESS_EVALUATION_SHADER,
      Compute = GL_COMPUTE_SHADER
    };
    // This class describes a single shader, i.e. vertex shader, fragment shader, etc.
  protected:
    GLuint _id;
    Type _type;

    std::vector<std::string> _sources;

  public:
    Shader(Type type);
    Shader(const Shader&) = delete;
    Shader(Shader&& shader) noexcept;

    ~Shader();

    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&& other) noexcept;

    bool compile();

    Type type() { return _type; }
    GLuint id() { return _id; }

    Shader& addSource(std::string source);
    Shader& addFile(std::string filename);
  };
}