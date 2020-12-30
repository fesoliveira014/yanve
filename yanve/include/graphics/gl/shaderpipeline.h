#pragma once

#include <common.h>
#include <graphics/gl/globject.h>
#include <graphics/gl/mesh.h>
#include <graphics/gl/meshview.h>

namespace yanve::gl 
{
  class Shader;

  class YANVE_API ShaderPipeline : public GLObject
  {
  protected:
    GLuint _id;
    std::unordered_map<std::string, GLuint> _uniforms;
    ObjectFlags _flags;

  public:
    ShaderPipeline();
    ShaderPipeline(const ShaderPipeline&) = delete;
    ShaderPipeline(ShaderPipeline&& shader) noexcept;

    ShaderPipeline& operator=(const Shader&) = delete;
    ShaderPipeline& operator=(ShaderPipeline&& other) noexcept;

    ~ShaderPipeline();

    void use();

    void draw(Mesh& mesh);
    void draw(MeshView& meshView);

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
