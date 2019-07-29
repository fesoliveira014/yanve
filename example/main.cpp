#include <yanve.h>
#include <iostream>

class TestShader : public yanve::gl::ShaderProgram
{

public:
  typedef yanve::gl::Attribute<0, glm::vec3> Position;
  typedef yanve::gl::Attribute<1, glm::vec4> Color;

  TestShader() : ShaderProgram()
  {
    yanve::gl::Shader vertex(yanve::gl::Shader::Type::Vertex);
    yanve::gl::Shader fragment(yanve::gl::Shader::Type::Fragment);

    vertex.addFile("res/shaders/simplevertex.glsl");
    fragment.addFile("res/shaders/simplefrag.glsl");

    vertex.compile();
    fragment.compile();

    attachShaders({ vertex, fragment });

    link();

    _uniforms["model"] = getUniformLocation("model");
  }

  TestShader& setModelMatrix(const glm::mat4& model)
  {
    setUniform("model", model);
    return *this;
  }
};

class TestApp : public yanve::Application
{
  typedef typename yanve::gl::Attribute<0, glm::vec3> Position;

public:
  TestApp(std::string name) : 
    running{ true }, 
    window{ name, 1024, 768 },
    deltaTimer{},
    clock{},
    shaderProgram{},
    mesh{yanve::gl::MeshPrimitive::Triangles},
    framesPerSec{0},
    frames{0}
  { 
    
  }

  ~TestApp() {}

  void initialize() override 
  {
    //front
    vertices.push_back(glm::vec3(-0.5, -0.5, -0.5));
    vertices.push_back(glm::vec3( 0.5,  0.5, -0.5));
    vertices.push_back(glm::vec3(-0.5,  0.5, -0.5));
    vertices.push_back(glm::vec3(-0.5, -0.5, -0.5));
    vertices.push_back(glm::vec3( 0.5, -0.5, -0.5));
    vertices.push_back(glm::vec3( 0.5,  0.5, -0.5));

    // back
    vertices.push_back(glm::vec3( 0.5, -0.5,  0.5));
    vertices.push_back(glm::vec3(-0.5,  0.5,  0.5));
    vertices.push_back(glm::vec3( 0.5,  0.5,  0.5));
    vertices.push_back(glm::vec3( 0.5, -0.5,  0.5));
    vertices.push_back(glm::vec3(-0.5, -0.5,  0.5));
    vertices.push_back(glm::vec3(-0.5,  0.5,  0.5));

    // left
    vertices.push_back(glm::vec3(-0.5, -0.5,  0.5));
    vertices.push_back(glm::vec3(-0.5,  0.5, -0.5));
    vertices.push_back(glm::vec3(-0.5,  0.5,  0.5));
    vertices.push_back(glm::vec3(-0.5, -0.5,  0.5));
    vertices.push_back(glm::vec3(-0.5, -0.5, -0.5));
    vertices.push_back(glm::vec3(-0.5,  0.5, -0.5));

    // right
    vertices.push_back(glm::vec3( 0.5, -0.5, -0.5));
    vertices.push_back(glm::vec3( 0.5,  0.5,  0.5));
    vertices.push_back(glm::vec3( 0.5,  0.5, -0.5));
    vertices.push_back(glm::vec3( 0.5, -0.5, -0.5));
    vertices.push_back(glm::vec3( 0.5, -0.5,  0.5));
    vertices.push_back(glm::vec3( 0.5,  0.5,  0.5));
    
    // bottom
    vertices.push_back(glm::vec3(-0.5, -0.5,  0.5));
    vertices.push_back(glm::vec3( 0.5, -0.5, -0.5));
    vertices.push_back(glm::vec3(-0.5, -0.5, -0.5));
    vertices.push_back(glm::vec3(-0.5, -0.5,  0.5));
    vertices.push_back(glm::vec3( 0.5, -0.5,  0.5));
    vertices.push_back(glm::vec3( 0.5, -0.5, -0.5));

    //top
    vertices.push_back(glm::vec3(-0.5,  0.5, -0.5));
    vertices.push_back(glm::vec3( 0.5,  0.5,  0.5)); 
    vertices.push_back(glm::vec3(-0.5,  0.5,  0.5)); 
    vertices.push_back(glm::vec3(-0.5,  0.5, -0.5));
    vertices.push_back(glm::vec3( 0.5,  0.5, -0.5));
    vertices.push_back(glm::vec3( 0.5,  0.5,  0.5));

    for (int i = 0; i < vertices.size(); ++i) {
      colors.push_back(glm::vec4(vertices[i] + glm::vec3(0.5), 1.0));
    }

    for (int i = 0; i < vertices.size(); ++i) {
      indices.push_back(i);
    }

    yanve::gl::Buffer vBuf{};
    vBuf.setData(vertices.data(), vertices.size() * sizeof(glm::vec3), yanve::gl::BufferUsage::StaticDraw);

    yanve::gl::Buffer cBuf{};
    cBuf.setData(colors.data(), colors.size() * sizeof(glm::vec4), yanve::gl::BufferUsage::StaticDraw);

    yanve::gl::Buffer iBuf{yanve::gl::Buffer::Target::ElementArray};
    iBuf.setData(indices.data(), indices.size() * sizeof(size_t), yanve::gl::BufferUsage::StaticDraw);

    vertexBuffer = std::move(vBuf);
    colorBuffer = std::move(cBuf);
    indexBuffer = std::move(iBuf);
    
    mesh.addBuffer(vertexBuffer, 0, TestShader::Position{})
      .addBuffer(colorBuffer, 0, TestShader::Color{})
      .setCount(vertices.size())
      .setIndexBuffer(indexBuffer, 0, yanve::gl::Mesh::MeshIndexType::UnsignedInt);

    modelMatrix = glm::mat4(1.0f);
    angle = 0.0f;

    shaderProgram.setModelMatrix(modelMatrix);
  }

  void update() override
  {
    if (clock.elapsed() >= 1.0) {
      clock.reset();
      framesPerSec = frames;
      frames = 0;
    }

    auto& input = yanve::InputManager::instance();
    input.update();
    window.clear();
    window.update();
    yanve::GuiManager::beginFrame();

    glm::mat4 transform = glm::rotate(modelMatrix, angle, glm::vec3(1.0, 1.0, 0.0));
    shaderProgram.setModelMatrix(transform);
    angle += 0.0001f;

    if (angle > 2.0f * glm::pi<float>())
      angle = 0.0f;

    running = !input.quit();
    frames++;
  }
  
  void updateGui() override
  {
    {
      ImGui::Begin("Test Box");

      ImGui::Text("FPS: %d", framesPerSec);
      
      ImGui::End();
    }
  }

  void render() override
  {
    mesh.draw(shaderProgram);

    updateGui();
    yanve::GuiManager::endFrame();

    window.display();
  }

  int run() override
  {
    initialize();
    while (running) {
      update();
      render();
    }
    shutdown();
    return 0; // add error code variable maybe?
  }

  void shutdown() override {}
  bool isRunning() { return running; }

protected:
  yanve::Window window;
  bool running;

  yanve::utils::Clock deltaTimer;
  yanve::utils::Clock clock;

  long framesPerSec;
  long frames;

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec4> colors;
  std::vector<GLuint> indices;

  yanve::gl::Buffer vertexBuffer;
  yanve::gl::Buffer colorBuffer;
  yanve::gl::Buffer indexBuffer;

  yanve::gl::Mesh mesh;

  //GLuint shaderProgram;
  TestShader shaderProgram;

  glm::mat4 modelMatrix;
  float angle = 0.0f;
};

int main(int argc, char* argv[])
{
  TestApp app{"test"};
  return app.run();
}