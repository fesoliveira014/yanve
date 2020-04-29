#include <iostream>

#define YANVE_NO_EXPORT
#include <yanve.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

static void openglCallbackFunction(
  GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar* message,
  const void* userParam
) {
  static bool nomore = false;
  (void)source; (void)type; (void)id;
  (void)severity; (void)length; (void)userParam;
  if (!nomore) {
    if (severity == GL_DEBUG_SEVERITY_HIGH) LogError("OpenGL", "%s", message);
    else LogInfo("OpenGL", "%s", message);

    if (severity == GL_DEBUG_SEVERITY_HIGH) {
      LogError("OpenGL", "This probably is breaking something.");
      //nomore = true;
    }
  }
}

class QuadShader : public yanve::gl::ShaderProgram
{

public:
  typedef yanve::gl::Attribute<0, glm::vec3> Position;
  typedef yanve::gl::Attribute<1, glm::vec2> UV;

  QuadShader() : ShaderProgram{}
  {
    yanve::gl::Shader vertex(yanve::gl::Shader::Type::Vertex);
    yanve::gl::Shader fragment(yanve::gl::Shader::Type::Fragment);

    vertex.addFile("res/shaders/quadvert.glsl");
    fragment.addFile("res/shaders/quadfrag.glsl");

    vertex.compile();
    fragment.compile();

    attachShaders({ vertex, fragment });

    link();

    setUniform("screenTexture", (int)TextureUnit);
  }

  QuadShader& bindTexture(yanve::gl::Texture2D& texture)
  {
    texture.bind(TextureUnit);
    return *this;
  }

private:
  enum : GLuint { TextureUnit = 0 };

};

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
    _uniforms["view"] = getUniformLocation("view");
    _uniforms["projection"] = getUniformLocation("projection");
  }

  TestShader& setModelMatrix(const glm::mat4& model)
  {
    setUniform("model", model);
    return *this;
  }

  TestShader& setViewMatrix(const glm::mat4& view)
  {
    setUniform("view", view);
    return *this;
  }

  TestShader& setProjectionMatrix(const glm::mat4& projection)
  {
    setUniform("projection", projection);
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
    _uniforms["view"] = getUniformLocation("view");
    _uniforms["projection"] = getUniformLocation("projection");
    _uniforms["texture1"] = getUniformLocation("texture1");

    setUniform("texture1", (int)TextureUnit);
  }

  TestTextureShader& setModelMatrix(const glm::mat4& model)
  {
    setUniform("model", model);
    return *this;
  }

  TestTextureShader& setViewMatrix(const glm::mat4& view)
  {
    setUniform("view", view);
    return *this;
  }

  TestTextureShader& setProjectionMatrix(const glm::mat4& projection)
  {
    setUniform("projection", projection);
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
public:
  TestApp(std::string name, int width, int height) : 
    Application{name, width, height},
    running{ true }, 
    deltaTimer{},
    clock{},
    mesh{yanve::gl::MeshPrimitive::Triangles},
    cube2{ yanve::gl::MeshPrimitive::Triangles },
    shaderProgram{},
    textureShaderProgram{},
    quadShaderProgram{},
    texture{},
    screenTexture{},
    screenFramebuffer{yanve::NoCreate},
    framesPerSec{ 0 },
    frames{ 0 },
    cameraPosition{ 0 },
    cameraRotation{ 0 },
    cameraForward{ 0 },
    cameraUp{ 0 },
    cameraRight{ 0 }
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

    yanve::gl::Renderer::disable(yanve::gl::Renderer::Feature::Blending);

    /*glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglCallbackFunction, nullptr);
    glDebugMessageControl(
      GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true
    );*/

    quadVertices.push_back(glm::vec3(-1.0,  1.0, 0.0)); quadUV.push_back(glm::vec2(0.0, 1.0));
    quadVertices.push_back(glm::vec3(-1.0, -1.0, 0.0)); quadUV.push_back(glm::vec2(0.0, 0.0));
    quadVertices.push_back(glm::vec3( 1.0,  1.0, 0.0)); quadUV.push_back(glm::vec2(1.0, 1.0));

    quadVertices.push_back(glm::vec3( 1.0,  1.0, 0.0)); quadUV.push_back(glm::vec2(1.0, 1.0));
    quadVertices.push_back(glm::vec3(-1.0, -1.0, 0.0)); quadUV.push_back(glm::vec2(0.0, 0.0));
    quadVertices.push_back(glm::vec3( 1.0, -1.0, 0.0)); quadUV.push_back(glm::vec2(1.0, 0.0));

    yanve::gl::Buffer qVB{};
    qVB.setData(quadVertices.data(), quadVertices.size() * sizeof(glm::vec3), yanve::gl::BufferUsage::StaticDraw);

    yanve::gl::Buffer qTB{};
    qTB.setData(quadUV.data(), quadUV.size() * sizeof(glm::vec2), yanve::gl::BufferUsage::StaticDraw);

    quadVertexBuffer = std::move(qVB);
    quadTexBuffer = std::move(qTB);

    quadMesh.addBuffer(quadVertexBuffer, 0, QuadShader::Position{})
      .addBuffer(quadTexBuffer, 0, QuadShader::UV{})
      .setCount(quadVertices.size());

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
    yanve::byte *data = stbi_load("res/textures/container.jpg", &width, &height, &channels, 0);

    if (data == nullptr) {
      throw std::runtime_error("Could not load texture");
    }

    texture.setWrapping({ yanve::gl::SamplerWrapping::Repeat, yanve::gl::SamplerWrapping::Repeat })
      .setMinificationFilter(yanve::gl::SamplerFilter::Linear)
      .setMagnificationFilter(yanve::gl::SamplerFilter::Linear)
      .setImage(0, yanve::gl::TextureFormat::RGB, yanve::gl::PixelFormat::RGB, yanve::gl::PixelType::UnsignedByte, data, { width, height })
      .generateMipMap();

    stbi_image_free(data);
    
    mesh.addBuffer(vertexBuffer, 0, TestShader::Position{})
      .addBuffer(colorBuffer, 0, TestShader::Color{})
      .addBuffer(texBuffer, 0, TestTextureShader::UV{})
      .setCount(vertices.size())
      .setIndexBuffer(indexBuffer, 0, yanve::gl::Mesh::MeshIndexType::UnsignedInt);

    cube2.addBuffer(vertexBuffer, 0, TestShader::Position{})
      .addBuffer(colorBuffer, 0, TestShader::Color{})
      .addBuffer(texBuffer, 0, TestTextureShader::UV{})
      .setCount(vertices.size())
      .setIndexBuffer(indexBuffer, 0, yanve::gl::Mesh::MeshIndexType::UnsignedInt);

    screenTexture.setWrapping({ yanve::gl::SamplerWrapping::ClampToEdge, yanve::gl::SamplerWrapping::ClampToEdge })
      .setMinificationFilter(yanve::gl::SamplerFilter::Linear)
      .setMagnificationFilter(yanve::gl::SamplerFilter::Linear)
      .setStorage(1, yanve::gl::TextureFormat::RGBA8, { 1024, 1024 });

    yanve::gl::Renderbuffer rb{};
    rb.setStorage(yanve::gl::RenderbufferFormat::Depth24Stencil8, { 1024, 1024 });

    renderbuffer = std::move(rb);

    yanve::gl::Framebuffer fb{ {{0, 0}, { 1024, 1024 }} };
    fb.attachTexture(yanve::gl::Framebuffer::ColorAttachment{0}, screenTexture, 0)
      .attachRenderbuffer(yanve::gl::Framebuffer::BufferAttachment::DepthStencil, renderbuffer)
      .mapForDraw({ yanve::gl::Framebuffer::ColorAttachment{0} });

    auto status = fb.checkStatus(yanve::gl::FramebufferTarget::Draw);
    if (status != yanve::gl::Framebuffer::Status::Complete) {
      LogError("TestApp::TestApp", "Framebuffer has bad status: %x", status);
      std::abort();
    }

    screenFramebuffer = std::move(fb);

    auto& input = yanve::InputManager::instance();
    glm::vec2 windowSize = glm::vec2(input.windowState().width, input.windowState().height);

    scene = new yanve::scene::Scene();

    camera = new yanve::scene::Camera(scene);
    camera->setViewParameters(45.0,  windowSize.x / windowSize.y, 0.1f, 1000.f);
    camera->translate({ 0, 0, 10 });
    camera->rotate({ 0, 0, 0 });

    scene->update();

    cameraUp = camera->up();
    cameraForward = camera->forward();
    cameraRight = camera->right();
    cameraPosition = camera->absolutePosition();
    //cameraRotation = glm::eulerAngles(camera->rotation());
    
    modelMatrix = glm::mat4(1.0f);
    angle = 0.0f;

    shaderProgram.setModelMatrix(modelMatrix);
    textureShaderProgram.setModelMatrix(modelMatrix);

    yanve::gl::defaultFramebuffer.setViewport({ {}, window.size() });
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

    if (input.windowState().resized) {
      glm::vec2 windowSize{ input.windowState().width, input.windowState().height };
      screenFramebuffer.setViewport({ {}, windowSize });
      yanve::gl::defaultFramebuffer.setViewport({ {}, windowSize });
      window.resize(windowSize);
      camera->setViewParameters(45.0, windowSize.x / windowSize.y, 0.1f, 1000.f);
    }

    if (input.mouseButtonState(yanve::InputManager::MouseButtom::buttonRight).pressed && 
        input.mouseCursorState().moved) {
      glm::vec2 dmouse{ input.mouseCursorState().dx, input.mouseCursorState().dy };

      // Look left/right
      cameraRotation.y += dmouse.x * cameraRotationSpeed;

      // Loop up/down but only in a limited range
      cameraRotation.x -= dmouse.y * cameraRotationSpeed;
      if (cameraRotation.x > 90)  cameraRotation.x = 90;
      if (cameraRotation.x < -90) cameraRotation.x = -90;
    }

    float speed = cameraSpeed / framesPerSec;
    float pitch = yanve::math::radians(cameraRotation.x);
    float yaw = yanve::math::radians(cameraRotation.y);

    // forward
    if (input.keyState(yanve::InputManager::Key::keyW).pressed) {
      cameraPosition.x -= sinf(yaw) * cosf(-pitch) * speed;
      cameraPosition.y -= sinf(-pitch) * speed;
      cameraPosition.z -= cosf(yaw) * cosf(-pitch) * speed;
    }
    
    // back
    if (input.keyState(yanve::InputManager::Key::keyS).pressed) {
      cameraPosition.x += sinf(yaw) * cosf(-pitch) * speed;
      cameraPosition.y += sinf(-pitch) * speed;
      cameraPosition.z += cosf(yaw) * cosf(-pitch) * speed;
    }

    // rotate left
    if (input.keyState(yanve::InputManager::Key::keyA).pressed) {
      cameraPosition.x += sinf(yanve::math::radians(cameraRotation.y - 90)) * speed;
      cameraPosition.z += cosf(yanve::math::radians(cameraRotation.y - 90)) * speed;
    }

    // rotate right
    if (input.keyState(yanve::InputManager::Key::keyD).pressed) {
      cameraPosition.x += sinf(yanve::math::radians(cameraRotation.y + 90)) * speed;
      cameraPosition.z += cosf(yanve::math::radians(cameraRotation.y + 90)) * speed;
    }

    // strafe left
    if (input.keyState(yanve::InputManager::Key::keyQ).pressed) {
      
    }
     
    // strafe right
    if (input.keyState(yanve::InputManager::Key::keyE).pressed) {
      
    }

    camera->translate(cameraPosition);
    camera->rotate(cameraRotation);

    scene->update();

    cameraUp = camera->up();
    cameraForward = camera->forward();
    cameraRight = camera->right();
    cameraPosition = camera->absolutePosition();
    //cameraRotation = glm::eulerAngles(camera->rotation());

    if (enableDepthTest != depthTestState) {
      depthTestState = enableDepthTest;
      yanve::gl::Renderer::setFeature(yanve::gl::Renderer::Feature::DepthTest, depthTestState);
    }

    if (enableFaceCulling != faceCullingState) {
      faceCullingState = enableFaceCulling;
      yanve::gl::Renderer::setFeature(yanve::gl::Renderer::Feature::FaceCulling, faceCullingState);
    }

    glm::mat4 transform = glm::rotate(modelMatrix, angle, glm::vec3(1.0, 1.0, 0.0));
    glm::mat4 view = camera->view();
    glm::mat4 projection = camera->projection();

    shaderProgram.setModelMatrix(transform);
    shaderProgram.setViewMatrix(view);
    shaderProgram.setProjectionMatrix(projection);

    textureShaderProgram.setModelMatrix(transform);
    textureShaderProgram.setViewMatrix(view);
    textureShaderProgram.setProjectionMatrix(projection);

    angle += 0.0001f;

    if (angle > 2.0f * glm::pi<float>())
      angle = 0.0f;

    running = !input.quit();
    frames++;

    yanve::GuiManager::beginFrame();
  }
  
  void updateGui() override
  {
    {
      ImGui::Begin("Test Box");

      ImGui::Text("FPS: %d", framesPerSec);
      ImGui::Checkbox("Use texture", &useTexture);
      ImGui::Checkbox("Enable depth test", &enableDepthTest);
      ImGui::Checkbox("Enable face culling", &enableFaceCulling);
      ImGui::Checkbox("Quad polygon mode", &polygonMode);

      if (ImGui::CollapsingHeader("Camera")) {
        ImGui::Text("Camera pos: (%.2f, %.2f, %.2f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);
        ImGui::Text("Camera angles: (%.2f, %.2f, %.2f)", cameraRotation.x, cameraRotation.y, cameraRotation.z);
        ImGui::SliderFloat("Camera speed", &cameraSpeed, 5.0f, 20.0f, "%.2f", 0.5f);
        ImGui::SliderFloat("Camera rotation speed", &cameraRotationSpeed, 0.001f, 0.1f, "%.4f", 0.005f);
        if (ImGui::Button("Reset Camera")) {
          cameraPosition = glm::vec3{ 0, 0, 10 };
          cameraRotation = glm::vec3{ 0 };
        }
      }
      
      ImGui::End();
    }
  }

  void render() override
  {
    yanve::gl::Renderer::enable(yanve::gl::Renderer::Feature::DepthTest);
    screenFramebuffer
      .clear(yanve::gl::FramebufferClearMask::Color | yanve::gl::FramebufferClearMask::Depth)
      .bind();
    if (useTexture) {
      textureShaderProgram.bindTexture(texture);
      mesh.draw(textureShaderProgram);
    }
    else {
      mesh.draw(shaderProgram);
    }

    yanve::gl::Renderer::disable(yanve::gl::Renderer::Feature::DepthTest);

    yanve::gl::defaultFramebuffer
      .clear(yanve::gl::FramebufferClearMask::Color)
      .bind();
    quadShaderProgram.bindTexture(screenTexture);
    quadMesh.draw(quadShaderProgram);

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

  std::vector<glm::vec3> quadVertices;
  std::vector<glm::vec2> quadUV;

  yanve::gl::Buffer vertexBuffer;
  yanve::gl::Buffer colorBuffer;
  yanve::gl::Buffer texBuffer;
  yanve::gl::Buffer indexBuffer;

  yanve::gl::Buffer quadVertexBuffer;
  yanve::gl::Buffer quadTexBuffer;

  yanve::gl::Mesh mesh;
  yanve::gl::Mesh cube2;
  yanve::gl::Mesh quadMesh;
  yanve::gl::Texture2D texture;
  yanve::gl::Texture2D screenTexture;
  
  yanve::gl::Renderbuffer renderbuffer;
  yanve::gl::Framebuffer screenFramebuffer;

  yanve::scene::Scene* scene;
  yanve::scene::Camera* camera;

  TestShader shaderProgram;
  TestTextureShader textureShaderProgram;
  QuadShader quadShaderProgram;

  glm::mat4 modelMatrix;
  float angle = 0.0f;

  bool useTexture = false;
  bool enableDepthTest = false, depthTestState = true;
  bool enableFaceCulling = false, faceCullingState = true;
  bool polygonMode = false;

  float cameraSpeed = 10.f;
  float cameraRotationSpeed = .3f;
  glm::vec3 cameraPosition;
  glm::vec3 cameraRotation;
  glm::vec3 cameraUp;
  glm::vec3 cameraForward;
  glm::vec3 cameraRight;
  bool resetCamera = false;
};

int main(int argc, char* argv[])
{
  TestApp app{"test", 1000, 1000};
  return app.run();
}