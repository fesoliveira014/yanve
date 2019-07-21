#pragma once

#include <common.h>
#include <graphics/gl/globject.h>

namespace yanve::gl 
{
  class Shader;

  class YANVE_API ShaderProgram : public GLObject
  {
  protected:
    GLuint _id;
    std::unordered_map<std::string, GLuint> _uniforms;

  public:
    ShaderProgram();
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram(ShaderProgram&& shader) noexcept;

    ShaderProgram& operator=(const Shader&) = delete;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    ~ShaderProgram();

    void use();

    void attachShader(Shader& shader);
    void attachShaders(std::initializer_list<std::reference_wrapper<Shader>> shaders);
    bool link();

  protected:
    bool hasUniform(std::string name);
    GLint getUniformLocation(std::string name);

    void setUniform(std::string name, bool value);
    void setUniform(std::string name, int value);
    void setUniform(std::string name, float value);
    void setUniform(std::string name, const glm::vec2& value);
    void setUniform(std::string name, const glm::vec3& value);
    void setUniform(std::string name, const glm::vec4& value);
    void setUniform(std::string name, const glm::mat2& value);
    void setUniform(std::string name, const glm::mat3& value);
    void setUniform(std::string name, const glm::mat4& value);
  };
}
