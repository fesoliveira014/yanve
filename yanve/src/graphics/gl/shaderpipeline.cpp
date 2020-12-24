#include <graphics/gl/shaderpipeline.h>
#include <graphics/gl/shader.h>
#include <utils/logger.h>

#include <graphics/gl/context.h>
#include <graphics/gl/state/state.h>
#include <graphics/gl/state/shaderstate.h>

namespace yanve::gl
{

ShaderPipeline::ShaderPipeline() : 
  _id{glCreateProgram()}
{
  
}

ShaderPipeline::ShaderPipeline(ShaderPipeline&& shader) noexcept : 
  _id{ shader._id }
{
  shader._id = 0;
}

ShaderPipeline& ShaderPipeline::operator=(ShaderPipeline&& other) noexcept
{
  std::swap(_id, other._id);
  return *this;
}

ShaderPipeline::~ShaderPipeline() {
  auto& state = *Context::current().state().shader;

  if (state.current == _id)
    glUseProgram(state.current = 0);

  if (_id) glDeleteProgram(_id);
  _id = 0;
}

void ShaderPipeline::use()
{
  auto& state = *Context::current().state().shader;

  if (state.current == _id) return;

  glUseProgram(state.current = _id);
}

void ShaderPipeline::attachShader(Shader& shader)
{
  glAttachShader(_id, shader.id());
}

void ShaderPipeline::attachShaders(std::initializer_list<std::reference_wrapper<Shader>> shaders)
{
  for (auto& shader : shaders) attachShader(shader);
}

bool ShaderPipeline::link()
{
  GLint success;

  glLinkProgram(_id);
  glGetProgramiv(_id, GL_LINK_STATUS, &success);

  if(!success) {
    GLchar errorLog[1024];
    glGetProgramInfoLog(_id, sizeof(errorLog), nullptr, errorLog);
    LogError(__func__, "Error linking program: %s", errorLog);
    return false;
  }

  return true;
}

bool ShaderPipeline::hasUniform(std::string name)
{
  if (_uniforms.find(name) == _uniforms.end())
    return false;

  return true;
}

GLint ShaderPipeline::getUniformLocation(std::string name)
{
  if (!hasUniform(name)) {
    GLuint location = glGetUniformLocation(_id, name.data());
    if (location == 0xffffffff)
      return 0;
    _uniforms[name] = location;
  }
  return _uniforms[name];
}

void ShaderPipeline::setUniform(std::string name, bool value)
{
  use();
  glUniform1i(_uniforms[name], static_cast<GLint>(value));
}

void ShaderPipeline::setUniform(std::string name, int value)
{
  use();
  glUniform1i(_uniforms[name], value);
}

void ShaderPipeline::setUniform(std::string name, float value)
{
  use();
  glUniform1f(_uniforms[name], value);
}

void ShaderPipeline::setUniform(std::string name, const glm::vec2& value)
{
  use();
  glUniform2f(_uniforms[name], value.x, value.y);
}

void ShaderPipeline::setUniform(std::string name, const glm::vec3& value)
{
  use();
  glUniform3f(_uniforms[name], value.x, value.y, value.z);
}

void ShaderPipeline::setUniform(std::string name, const glm::vec4& value)
{
  use();
  glUniform4f(_uniforms[name], value.x, value.y, value.z, value.w);
}

void ShaderPipeline::setUniform(std::string name, const glm::mat2& value)
{
  use();
  glUniformMatrix2fv(_uniforms[name], 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderPipeline::setUniform(std::string name, const glm::mat3& value)
{
  use();
  glUniformMatrix3fv(_uniforms[name], 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderPipeline::setUniform(std::string name, const glm::mat4& value)
{
  use();
  glUniformMatrix4fv(_uniforms[name], 1, GL_FALSE, glm::value_ptr(value));
}


}
