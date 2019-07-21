#include <graphics/gl/shader.h>
#include <utils/misc.h>
#include <utils/logger.h>

namespace yanve::gl
{

Shader::Shader(Type type) :
  _type{type},
  _sources{}
{
  _id = glCreateShader(static_cast<GLenum>(_type));
}

Shader::Shader(Shader&& shader) noexcept :
  _type{ shader._type },
  _id{ shader._id },
  _sources{ std::move(shader._sources) }
{
  shader._id = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
  std::swap(_type, other._type);
  std::swap(_id, other._id);
  std::swap(_sources, other._sources);
  return *this;
}

Shader::~Shader()
{
  if (_id != 0)
    glDeleteShader(_id);
}

Shader& Shader::addSource(std::string source)
{
  _sources.push_back(source);
  return *this;
}

Shader& Shader::addFile(std::string filename)
{
  std::string source;
  if (!utils::read_file(filename.c_str(), source)) {
    LogError(__func__, "Could not load shader %s", filename);
  }

  addSource(source);

  return *this;
}

bool Shader::compile()
{
  std::vector<char*> pointers{};
  std::vector<GLint> sizes{};

  pointers.resize(_sources.size());
  sizes.resize(_sources.size());

  for (std::size_t i = 0; i < _sources.size(); ++i) {
    pointers[i] = static_cast<GLchar*>(_sources[i].data());
    sizes[i] = _sources[i].size();
  }

  glShaderSource(_id, _sources.size(), pointers.data(), sizes.data());
  glCompileShader(_id);

  GLint success;
  glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar errorLog[1024];
    glGetShaderInfoLog(_id, sizeof(errorLog), nullptr, errorLog);
    LogError(__func__, "Error compiling shader of type %d", _type);
    LogError(__func__, "Error log: %s", errorLog);
    return false;
  }

  return true;
}

}