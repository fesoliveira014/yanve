#include <graphics/gl/shaderprogram.h>
#include <graphics/gl/shader.h>
#include <utils/logger.h>

namespace yanve::gl
{

ShaderProgram::ShaderProgram() : 
  _id{glCreateProgram()}
{
  
}

ShaderProgram::ShaderProgram(ShaderProgram&& shader) noexcept : 
  _id{ shader._id }
{
  shader._id = 0;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
  std::swap(_id, other._id);
  return *this;
}

ShaderProgram::~ShaderProgram() {
  if (_id) glDeleteProgram(_id);
}

void ShaderProgram::use()
{
  glUseProgram(_id);
}

void ShaderProgram::attachShader(Shader& shader)
{
  glAttachShader(_id, shader.id());
}

void ShaderProgram::attachShaders(std::initializer_list<std::reference_wrapper<Shader>> shaders)
{
  for (auto& shader : shaders) attachShader(shader);
}

bool ShaderProgram::link()
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

bool ShaderProgram::hasUniform(std::string name)
{
  if (_uniforms.find(name) == _uniforms.end())
    return false;

  return true;
}

GLint ShaderProgram::getUniformLocation(std::string name)
{
  if (!hasUniform(name)) {
    GLuint location = glGetUniformLocation(_id, name.data());
    if (location == 0xffffffff)
      return 0;
    _uniforms[name] = location;
  }
  return _uniforms[name];
}

void ShaderProgram::setUniform(std::string name, bool value)
{
  use();
  glUniform1i(_uniforms[name], static_cast<GLint>(value));
}

void ShaderProgram::setUniform(std::string name, int value)
{
  use();
  glUniform1i(_uniforms[name], value);
}

void ShaderProgram::setUniform(std::string name, float value)
{
  use();
  glUniform1f(_uniforms[name], value);
}

void ShaderProgram::setUniform(std::string name, const glm::vec2& value)
{
  use();
  glUniform2f(_uniforms[name], value.x, value.y);
}

void ShaderProgram::setUniform(std::string name, const glm::vec3& value)
{
  use();
  glUniform3f(_uniforms[name], value.x, value.y, value.z);
}

void ShaderProgram::setUniform(std::string name, const glm::vec4& value)
{
  use();
  glUniform4f(_uniforms[name], value.x, value.y, value.z, value.w);
}

void ShaderProgram::setUniform(std::string name, const glm::mat2& value)
{
  use();
  glUniformMatrix2fv(_uniforms[name], 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setUniform(std::string name, const glm::mat3& value)
{
  use();
  glUniformMatrix3fv(_uniforms[name], 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setUniform(std::string name, const glm::mat4& value)
{
  use();
  glUniformMatrix4fv(_uniforms[name], 1, GL_FALSE, glm::value_ptr(value));
}


}
