#include <yanve.h>
#include <iostream>

class TestApp : public yanve::Application
{
  typedef typename yanve::gl::Attribute<0, glm::vec3> Position;

public:
  TestApp(std::string name) : 
    running{ true }, 
    window{ name, 1024, 768 },
    deltaTimer{},
    clock{},
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

    //vertices.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));  // bottom left
    //vertices.push_back(glm::vec3( 0.5f, -0.5f, 0.0f));  // bottom right
    //vertices.push_back(glm::vec3( 0.5f,  0.5f, 0.0f));  // top right
    //vertices.push_back(glm::vec3(-0.5f,  0.5f, 0.0f));   // top left 

    for (int i = 0; i < vertices.size(); ++i) {
      colors.push_back(glm::vec4(vertices[i] + glm::vec3(0.5), 1.0));
    }

    for (int i = 0; i < vertices.size(); ++i) {
      indices.push_back(i);
    }

    //indices.push_back(0); indices.push_back(1); indices.push_back(2);
    //indices.push_back(0); indices.push_back(2); indices.push_back(3);

    //glFrontFace(GL_CCW);

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    yanve::gl::Buffer vBuf{};
    vBuf.setData(vertices.data(), vertices.size() * sizeof(glm::vec3), yanve::gl::BufferUsage::StaticDraw);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    yanve::gl::Buffer iBuf{};
    iBuf.setData(indices.data(), indices.size() * sizeof(size_t), yanve::gl::BufferUsage::StaticDraw);

    glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    vertexBuffer = std::move(vBuf);
    indexBuffer = std::move(iBuf);

    const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 model;"
    "void main()\n"
    "{\n"
    "   gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\0";

    const char* fragmentShaderSource = 
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\n\0";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    modelMatrix = glm::mat4(1.0f);
    angle = 0.0f;

    glUseProgram(shaderProgram);
    modelUniformPos = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelUniformPos, 1, GL_FALSE, glm::value_ptr(modelMatrix));
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
    glUniformMatrix4fv(modelUniformPos, 1, GL_FALSE, glm::value_ptr(transform));
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
    glUseProgram(shaderProgram);
    glBindVertexArray(vertexArrayObject); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    
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
  std::vector<size_t> indices;

  yanve::gl::Buffer vertexBuffer;
  yanve::gl::Buffer colorBuffer;
  yanve::gl::Buffer indexBuffer;
  GLuint vertexArrayObject;

  GLuint shaderProgram;
  GLuint modelUniformPos;

  glm::mat4 modelMatrix;
  float angle = 0.0f;
};

int main(int argc, char* argv[])
{
  TestApp app{"test"};
  return app.run();
}