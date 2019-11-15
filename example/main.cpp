#include <yanve.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
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

class TestTextureShader : public yanve::gl::ShaderProgram
{
public:
  typedef yanve::gl::Attribute<0, glm::vec3> Position;
  typedef yanve::gl::Attribute<1, glm::vec4> Color;
  typedef yanve::gl::Attribute<2, glm::vec2> UV;

  TestTextureShader() : ShaderProgram()
  {
    yanve::gl::Shader vertex(yanve::gl::Shader::Type::Vertex);
    yanve::gl::Shader fragment(yanve::gl::Shader::Type::Fragment);

    vertex.addFile("res/shaders/texturevert.glsl");
    fragment.addFile("res/shaders/texturefrag.glsl");

    vertex.compile();
    fragment.compile();

    attachShaders({ vertex, fragment });

    link();

    _uniforms["model"] = getUniformLocation("model");
    _uniforms["texture1"] = getUniformLocation("texture1");

    setUniform("texture1", (int)TextureUnit);
  }

  TestTextureShader& setModelMatrix(const glm::mat4& model)
  {
    setUniform("model", model);
    return *this;
  }

  TestTextureShader& bindTexture(yanve::gl::Texture2D& texture)
  {
    texture.bind(TextureUnit);
    return *this;
  }
private:
  enum: GLuint {TextureUnit = 0};
};

class TestApp : public yanve::Application
{
  typedef typename yanve::gl::Attribute<0, glm::vec3> Position;

public:
  TestApp(std::string name, int width, int height) : 
    Application{name, width, height},
    running{ true }, 
    deltaTimer{},
    clock{},
    shaderProgram{},
    textureShaderProgram{},
    mesh{yanve::gl::MeshPrimitive::Triangles},
    texture{},
    framesPerSec{0},
    frames{0}
  { 
    
  }

  ~TestApp() {}

  void initialize() override 
  {
    yanve::gl::Renderer::setFrontFace(yanve::gl::Renderer::FrontFace::CounterClockWise);
    yanve::gl::Renderer::setFaceCullingMode(yanve::gl::Renderer::PolygonFacing::Back);
    yanve::gl::Renderer::enable(yanve::gl::Renderer::Feature::FaceCulling);

    yanve::gl::Renderer::setDepthFunction(yanve::gl::Renderer::DepthFunction::Less);
    yanve::gl::Renderer::enable(yanve::gl::Renderer::Feature::DepthTest);

    //front
    vertices.push_back(glm::vec3(-0.5, -0.5, -0.5)); uvs.push_back(glm::vec2(0.0f, 0.0f));
    vertices.push_back(glm::vec3( 0.5,  0.5, -0.5)); uvs.push_back(glm::vec2(1.0f, 1.0f));
    vertices.push_back(glm::vec3(-0.5,  0.5, -0.5)); uvs.push_back(glm::vec2(0.0f, 1.0f));
    vertices.push_back(glm::vec3(-0.5, -0.5, -0.5)); uvs.push_back(glm::vec2(0.0f, 0.0f));
    vertices.push_back(glm::vec3( 0.5, -0.5, -0.5)); uvs.push_back(glm::vec2(1.0f, 0.0f));
    vertices.push_back(glm::vec3( 0.5,  0.5, -0.5)); uvs.push_back(glm::vec2(1.0f, 1.0f));

    // back
    vertices.push_back(glm::vec3( 0.5, -0.5,  0.5)); uvs.push_back(glm::vec2(0.0f, 0.0f));
    vertices.push_back(glm::vec3(-0.5,  0.5,  0.5)); uvs.push_back(glm::vec2(1.0f, 1.0f));
    vertices.push_back(glm::vec3( 0.5,  0.5,  0.5)); uvs.push_back(glm::vec2(0.0f, 1.0f));
    vertices.push_back(glm::vec3( 0.5, -0.5,  0.5)); uvs.push_back(glm::vec2(0.0f, 0.0f));
    vertices.push_back(glm::vec3(-0.5, -0.5,  0.5)); uvs.push_back(glm::vec2(1.0f, 0.0f));
    vertices.push_back(glm::vec3(-0.5,  0.5,  0.5)); uvs.push_back(glm::vec2(1.0f, 1.0f));

    // left
    vertices.push_back(glm::vec3(-0.5, -0.5,  0.5)); uvs.push_back(glm::vec2(0.0f, 0.0f));
    vertices.push_back(glm::vec3(-0.5,  0.5, -0.5)); uvs.push_back(glm::vec2(1.0f, 1.0f));
    vertices.push_back(glm::vec3(-0.5,  0.5,  0.5)); uvs.push_back(glm::vec2(0.0f, 1.0f));
    vertices.push_back(glm::vec3(-0.5, -0.5,  0.5)); uvs.push_back(glm::vec2(0.0f, 0.0f));
    vertices.push_back(glm::vec3(-0.5, -0.5, -0.5)); uvs.push_back(glm::vec2(1.0f, 0.0f));
    vertices.push_back(glm::vec3(-0.5,  0.5, -0.5)); uvs.push_back(glm::vec2(1.0f, 1.0f));

    // right
    vertices.push_back(glm::vec3( 0.5, -0.5, -0.5)); uvs.push_back(glm::vec2(0.0f, 0.0f));
    vertices.push_back(glm::vec3( 0.5,  0.5,  0.5)); uvs.push_back(glm::vec2(1.0f, 1.0f));
    vertices.push_back(glm::vec3( 0.5,  0.5, -0.5)); uvs.push_back(glm::vec2(0.0f, 1.0f));
    vertices.push_back(glm::vec3( 0.5, -0.5, -0.5)); uvs.push_back(glm::vec2(0.0f, 0.0f));
    vertices.push_back(glm::vec3( 0.5, -0.5,  0.5)); uvs.push_back(glm::vec2(1.0f, 0.0f));
    vertices.push_back(glm::vec3( 0.5,  0.5,  0.5)); uvs.push_back(glm::vec2(1.0f, 1.0f));
    
    // bottom
    vertices.push_back(glm::vec3(-0.5, -0.5,  0.5)); uvs.push_back(glm::vec2(0.0f, 0.0f));
    vertices.push_back(glm::vec3( 0.5, -0.5, -0.5)); uvs.push_back(glm::vec2(1.0f, 1.0f));
    vertices.push_back(glm::vec3(-0.5, -0.5, -0.5)); uvs.push_back(glm::vec2(0.0f, 1.0f));
    vertices.push_back(glm::vec3(-0.5, -0.5,  0.5)); uvs.push_back(glm::vec2(0.0f, 0.0f));
    vertices.push_back(glm::vec3( 0.5, -0.5,  0.5)); uvs.push_back(glm::vec2(1.0f, 0.0f));
    vertices.push_back(glm::vec3( 0.5, -0.5, -0.5)); uvs.push_back(glm::vec2(1.0f, 1.0f));

    //top
    vertices.push_back(glm::vec3(-0.5,  0.5, -0.5)); uvs.push_back(glm::vec2(0.0f, 0.0f));
    vertices.push_back(glm::vec3( 0.5,  0.5,  0.5)); uvs.push_back(glm::vec2(1.0f, 1.0f)); 
    vertices.push_back(glm::vec3(-0.5,  0.5,  0.5)); uvs.push_back(glm::vec2(0.0f, 1.0f)); 
    vertices.push_back(glm::vec3(-0.5,  0.5, -0.5)); uvs.push_back(glm::vec2(0.0f, 0.0f));
    vertices.push_back(glm::vec3( 0.5,  0.5, -0.5)); uvs.push_back(glm::vec2(1.0f, 0.0f));
    vertices.push_back(glm::vec3( 0.5,  0.5,  0.5)); uvs.push_back(glm::vec2(1.0f, 1.0f));

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

    yanve::gl::Buffer tBuf{};
    tBuf.setData(uvs.data(), uvs.size() * sizeof(glm::vec2), yanve::gl::BufferUsage::StaticDraw);

    yanve::gl::Buffer iBuf{yanve::gl::Buffer::Target::ElementArray};
    iBuf.setData(indices.data(), indices.size() * sizeof(size_t), yanve::gl::BufferUsage::StaticDraw);

    vertexBuffer = std::move(vBuf);
    colorBuffer = std::move(cBuf);
    texBuffer = std::move(tBuf);
    indexBuffer = std::move(iBuf);

    int width, height, channels;
    //stbi_set_flip_vertically_on_load(true);
    yanve::byte *data = stbi_load("res/textures/container.jpg", &width, &height, &channels, 0);

    if (data == nullptr) {
      throw std::runtime_error("Could not load texture");
    }

    texture.setWrapping({ yanve::gl::SamplerWrapping::Repeat, yanve::gl::SamplerWrapping::Repeat })
      .setMinificationFilter(yanve::gl::SamplerFilter::Linear)
      .setMagnificationFilter(yanve::gl::SamplerFilter::Linear)
      .setImage(0, yanve::gl::TextureFormat::RGB, yanve::gl::PixelFormat::RGB, yanve::gl::PixelType::UnsignedByte, data, glm::uvec2{ width, height })
      .generateMipMap();

    stbi_image_free(data);
    
    mesh.addBuffer(vertexBuffer, 0, TestShader::Position{})
      .addBuffer(colorBuffer, 0, TestShader::Color{})
      .addBuffer(texBuffer, 0, TestTextureShader::UV{})
      .setCount(vertices.size())
      .setIndexBuffer(indexBuffer, 0, yanve::gl::Mesh::MeshIndexType::UnsignedInt);

    modelMatrix = glm::mat4(1.0f);
    angle = 0.0f;

    shaderProgram.setModelMatrix(modelMatrix);
    textureShaderProgram.setModelMatrix(modelMatrix);
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

    if (input.windowResized()) {
      window.resize(input.windowSize());
    }

    yanve::gl::defaultFramebuffer.clear(yanve::gl::FramebufferClearMask::Color | yanve::gl::FramebufferClearMask::Depth);
    yanve::GuiManager::beginFrame();

    if (enableDepthTest != depthTestState) {
      depthTestState = enableDepthTest;
      yanve::gl::Renderer::setFeature(yanve::gl::Renderer::Feature::DepthTest, depthTestState);
    }

    if (enableFaceCulling != faceCullingState) {
      faceCullingState = enableFaceCulling;
      yanve::gl::Renderer::setFeature(yanve::gl::Renderer::Feature::FaceCulling, faceCullingState);
    }

    glm::mat4 transform = glm::rotate(modelMatrix, angle, glm::vec3(1.0, 1.0, 0.0));
    shaderProgram.setModelMatrix(transform);
    textureShaderProgram.setModelMatrix(transform);
    
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
      ImGui::Checkbox("Use texture", &useTexture);
      ImGui::Checkbox("Enable depth test", &enableDepthTest);
      ImGui::Checkbox("Enable face culling", &enableFaceCulling);
      
      ImGui::End();
    }
  }

  void render() override
  {
    if (useTexture) {
      textureShaderProgram.bindTexture(texture);
      mesh.draw(textureShaderProgram);
    }
    else {
      mesh.draw(shaderProgram);
    }

    updateGui();
    yanve::GuiManager::endFrame();

    window.swapBuffers();
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
  bool running;

  yanve::utils::Clock deltaTimer;
  yanve::utils::Clock clock;

  long framesPerSec;
  long frames;

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec4> colors;
  std::vector<glm::vec2> uvs;
  std::vector<GLuint> indices;

  yanve::gl::Buffer vertexBuffer;
  yanve::gl::Buffer colorBuffer;
  yanve::gl::Buffer texBuffer;
  yanve::gl::Buffer indexBuffer;

  yanve::gl::Mesh mesh;
  yanve::gl::Texture2D texture;

  //GLuint shaderProgram;
  TestShader shaderProgram;
  TestTextureShader textureShaderProgram;

  glm::mat4 modelMatrix;
  float angle = 0.0f;

  bool useTexture = false;
  bool enableDepthTest = false, depthTestState = false;
  bool enableFaceCulling = false, faceCullingState = false;
};

int main(int argc, char* argv[])
{
  TestApp app{"test", 1024, 268};
  return app.run();
}