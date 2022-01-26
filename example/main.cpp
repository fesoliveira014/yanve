#include <iostream>

#define YANVE_NO_EXPORT
#include <yanve.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#ifdef __cplusplus
extern "C" {
#endif
  __declspec(dllexport) DWORD NvOptimusEnablement = 0;
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#ifdef __cplusplus
}
#endif

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
    //else LogInfo("OpenGL", "%s", message);

    if (severity == GL_DEBUG_SEVERITY_HIGH) {
      LogError("OpenGL", "This probably is breaking something.");
      //nomore = true;
    }
  }
}

class QuadShader : public yanve::gl::ShaderPipeline
{

public:
  typedef yanve::gl::Attribute<0, glm::vec3> Position;
  typedef yanve::gl::Attribute<3, glm::vec2> UV;

  QuadShader() : ShaderPipeline{}
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

class CubeMapShader : public yanve::gl::ShaderPipeline
{
public:
  typedef yanve::gl::Attribute<0, glm::vec3> Position;

  CubeMapShader() : ShaderPipeline()
  {
    yanve::gl::Shader vertex(yanve::gl::Shader::Type::Vertex);
    yanve::gl::Shader fragment(yanve::gl::Shader::Type::Fragment);

    vertex.addFile("res/shaders/cubemapvert.glsl");
    fragment.addFile("res/shaders/cubemapfrag.glsl");

    vertex.compile();
    fragment.compile();

    attachShaders({ vertex, fragment });

    link();

    _uniforms["view"] = getUniformLocation("view");
    _uniforms["projection"] = getUniformLocation("projection");
    _uniforms["skybox"] = getUniformLocation("skybox");
  }

  CubeMapShader& setViewMatrix(const glm::mat4& view)
  {
    setUniform("view", view);
    return *this;
  }

  CubeMapShader& setProjectionMatrix(const glm::mat4& projection)
  {
    setUniform("projection", projection);
    return *this;
  }

  CubeMapShader& bindTexture(yanve::gl::CubeMapTexture& texture)
  {
    texture.bind(TextureUnit);
    return *this;
  }

private:
  enum : GLuint { TextureUnit = 0 };
};

class TestShader : public yanve::gl::ShaderPipeline
{

public:
  typedef yanve::gl::Attribute<0, glm::vec3> Position;
  typedef yanve::gl::Attribute<1, glm::vec4> Color;

  TestShader() : ShaderPipeline()
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

class TestTextureShader : public yanve::gl::ShaderPipeline
{
public:
  typedef yanve::gl::Attribute<0, glm::vec3> Position;
  typedef yanve::gl::Attribute<1, glm::vec4> Color;
  typedef yanve::gl::Attribute<3, glm::vec2> UV;

  TestTextureShader() : ShaderPipeline()
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
    cubePositionBuffer{},
    cubeColorBuffer{},
    cubeTextureBuffer{},
    cubeIndexBuffer{yanve::gl::Buffer::Target::ElementArray},
    skyboxPositionBuffer{},
    quadPositionBuffer{},
    quadTextureBuffer{},
    mesh{ },
    skyboxMesh{ },
    quadMesh{ },
    cube2{ yanve::NoCreate },
    shaderProgram{},
    textureShaderProgram{},
    quadShaderProgram{},
    cubeMapShader{},
    texture{},
    cubeMapTexture{},
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
    init_renderer();
    init_geometry();
    init_textures();
    init_meshes();
    init_framebuffers();
    init_scene();
  }


  void init_renderer()
  {
    yanve::gl::Renderer::setFrontFace(yanve::gl::Renderer::FrontFace::CounterClockWise);
    yanve::gl::Renderer::setFaceCullingMode(yanve::gl::Renderer::PolygonFacing::Back);
    yanve::gl::Renderer::enable(yanve::gl::Renderer::Feature::FaceCulling);

    yanve::gl::Renderer::setDepthFunction(yanve::gl::Renderer::DepthFunction::Less);
    yanve::gl::Renderer::enable(yanve::gl::Renderer::Feature::DepthTest);

    yanve::gl::Renderer::disable(yanve::gl::Renderer::Feature::Blending);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglCallbackFunction, nullptr);
    glDebugMessageControl(
      GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true
    );
  }

  void init_geometry()
  {
    // init geometry
    cubeGD =  yanve::Primitive::cube(
          yanve::PrimitiveFlag::Position |
          yanve::PrimitiveFlag::Color |
          yanve::PrimitiveFlag::Texture);

    skyboxGD = yanve::Primitive::skybox();
    
    quadGD = yanve::Primitive::plane(
      yanve::PrimitiveFlag::Position |
      yanve::PrimitiveFlag::Texture |
      yanve::PrimitiveFlag::NoIndex);
    
    // init buffers
    cubePositionBuffer.setData(cubeGD.vertices.data(), cubeGD.vertices.size() * sizeof(glm::vec3), yanve::gl::BufferUsage::StaticDraw);
    cubeColorBuffer.setData(cubeGD.colors.data(), cubeGD.colors.size() * sizeof(glm::vec4), yanve::gl::BufferUsage::StaticDraw);
    cubeTextureBuffer.setData(cubeGD.uvs.data(), cubeGD.uvs.size() * sizeof(glm::vec2), yanve::gl::BufferUsage::StaticDraw);
    cubeIndexBuffer.setData(cubeGD.indices.data(), cubeGD.indices.size() * sizeof(size_t), yanve::gl::BufferUsage::StaticDraw);
    skyboxPositionBuffer.setData(skyboxGD.vertices.data(), skyboxGD.vertices.size() * sizeof(glm::vec3), yanve::gl::BufferUsage::StaticDraw);
    quadPositionBuffer.setData(quadGD.vertices.data(), quadGD.vertices.size() * sizeof(glm::vec3), yanve::gl::BufferUsage::StaticDraw);
    quadTextureBuffer.setData(quadGD.uvs.data(), quadGD.uvs.size() * sizeof(glm::vec2), yanve::gl::BufferUsage::StaticDraw);

  }

  void init_textures()
  {
    // container texture loading and init
    {
      int width, height, channels;
      yanve::byte* data = stbi_load("res/textures/container.jpg", &width, &height, &channels, 0);

      if (data == nullptr) {
        throw std::runtime_error("Could not load texture");
      }

      texture.setWrapping({ yanve::gl::SamplerWrapping::Repeat, yanve::gl::SamplerWrapping::Repeat })
        .setMinificationFilter(yanve::gl::SamplerFilter::Linear)
        .setMagnificationFilter(yanve::gl::SamplerFilter::Linear)
        .setImage(0, yanve::gl::TextureFormat::RGB, yanve::gl::PixelFormat::RGB, yanve::gl::PixelType::UnsignedByte, data, { width, height })
        .generateMipMap();

      stbi_image_free(data);
    }

    // cubemap texture loading and init
    {
      int width, height, channels;
      std::vector<std::string> faces
      {
        "res/textures/skybox/right.jpg",
        "res/textures/skybox/left.jpg",
        "res/textures/skybox/top.jpg",
        "res/textures/skybox/bottom.jpg",
        "res/textures/skybox/front.jpg",
        "res/textures/skybox/back.jpg"
      };

      for (size_t i = 0; i < faces.size(); ++i) {
        yanve::byte* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        if (data == nullptr) {
          throw std::runtime_error("Could not load texture " + faces[i]);
        }
        switch (i) {
        case 0:
          cubeMapTexture.setImage(yanve::gl::CubeMapCoordinate::PositiveX, 0, yanve::gl::TextureFormat::RGB, yanve::gl::PixelFormat::RGB, yanve::gl::PixelType::UnsignedByte, data, { width, height });
          break;
        case 1:
          cubeMapTexture.setImage(yanve::gl::CubeMapCoordinate::NegativeX, 0, yanve::gl::TextureFormat::RGB, yanve::gl::PixelFormat::RGB, yanve::gl::PixelType::UnsignedByte, data, { width, height });
          break;
        case 2:
          cubeMapTexture.setImage(yanve::gl::CubeMapCoordinate::PositiveY, 0, yanve::gl::TextureFormat::RGB, yanve::gl::PixelFormat::RGB, yanve::gl::PixelType::UnsignedByte, data, { width, height });
          break;
        case 3:
          cubeMapTexture.setImage(yanve::gl::CubeMapCoordinate::NegativeY, 0, yanve::gl::TextureFormat::RGB, yanve::gl::PixelFormat::RGB, yanve::gl::PixelType::UnsignedByte, data, { width, height });
          break;
        case 4:
          cubeMapTexture.setImage(yanve::gl::CubeMapCoordinate::PositiveZ, 0, yanve::gl::TextureFormat::RGB, yanve::gl::PixelFormat::RGB, yanve::gl::PixelType::UnsignedByte, data, { width, height });
          break;
        case 5:
          cubeMapTexture.setImage(yanve::gl::CubeMapCoordinate::NegativeZ, 0, yanve::gl::TextureFormat::RGB, yanve::gl::PixelFormat::RGB, yanve::gl::PixelType::UnsignedByte, data, { width, height });
          break;
        }

        stbi_image_free(data);
      }

      cubeMapTexture.setMinificationFilter(yanve::gl::SamplerFilter::Linear)
        .setMagnificationFilter(yanve::gl::SamplerFilter::Linear)
        .setWrapping({ yanve::gl::SamplerWrapping::ClampToEdge, yanve::gl::SamplerWrapping::ClampToEdge , yanve::gl::SamplerWrapping::ClampToEdge });
    }

    // init screen quad
    {
      screenTexture.setWrapping({ yanve::gl::SamplerWrapping::ClampToEdge, yanve::gl::SamplerWrapping::ClampToEdge })
        .setMinificationFilter(yanve::gl::SamplerFilter::Linear)
        .setMagnificationFilter(yanve::gl::SamplerFilter::Linear)
        .setStorage(1, yanve::gl::TextureFormat::RGBA8, { 1024, 1024 });
    }
  }

  void init_meshes()
  {
    // init meshes
    mesh.addBuffer(cubePositionBuffer, 0, TestShader::Position{})
      .addBuffer(cubeColorBuffer, 0, TestShader::Color{})
      .addBuffer(cubeTextureBuffer, 0, TestTextureShader::UV{})
      .setCount(cubeGD.vertices.size())
      .setIndexBuffer(cubeIndexBuffer, 0, yanve::gl::Mesh::MeshIndexType::UnsignedInt);

    skyboxMesh.addBuffer(skyboxPositionBuffer, 0, CubeMapShader::Position{})
      .setCount(skyboxGD.vertices.size());

    quadMesh.addBuffer(quadPositionBuffer, 0, QuadShader::Position{})
      .addBuffer(quadTextureBuffer, 0, QuadShader::UV{})
      .setCount(quadGD.vertices.size());

    cube2 = yanve::gl::MeshView(mesh);
    cube2.setCount(cubeGD.vertices.size());    
  }

  void init_framebuffers()
  {
    // init framebuffer
    yanve::gl::Renderbuffer rb{};
    rb.setStorage(yanve::gl::RenderbufferFormat::Depth24Stencil8, { 1024, 1024 });

    renderbuffer = std::move(rb);

    yanve::gl::Framebuffer fb{ {{0, 0}, { 1024, 1024 }} };
    fb.attachTexture(yanve::gl::Framebuffer::ColorAttachment{ 0 }, screenTexture, 0)
      .attachRenderbuffer(yanve::gl::Framebuffer::BufferAttachment::DepthStencil, renderbuffer)
      .mapForDraw({ yanve::gl::Framebuffer::ColorAttachment{0} });

    auto status = fb.checkStatus(yanve::gl::FramebufferTarget::Draw);
    if (status != yanve::gl::Framebuffer::Status::Complete) {
      LogError("TestApp::TestApp", "Framebuffer has bad status: %x", status);
      std::abort();
    }

    screenFramebuffer = std::move(fb);
    yanve::gl::defaultFramebuffer.setViewport({ {}, window.size() });
  }

  void init_scene()
  {
    auto& input = yanve::InputManager::instance();
    glm::vec2 windowSize = glm::vec2(input.windowState().width, input.windowState().height);

    yanve::math::AABB bBox{ glm::vec3{std::numeric_limits<float>::max()},
                          glm::vec3{-std::numeric_limits<float>::max()} };

    for (const glm::vec3& v : cubeGD.vertices) {
      if (v.x < bBox.min.x) bBox.min.x = v.x;
      if (v.y < bBox.min.y) bBox.min.y = v.y;
      if (v.z < bBox.min.z) bBox.min.z = v.z;
      if (v.x > bBox.max.x) bBox.min.x = v.x;
      if (v.y > bBox.max.y) bBox.min.y = v.y;
      if (v.z > bBox.max.z) bBox.min.z = v.z;
    }

    yanve::scene::CameraData cameraData{ "camera" };
    cameraData.fov = 45.0f;
    cameraData.aspect = windowSize.x / windowSize.y;
    cameraData.nearPlane = 0.1f;
    cameraData.farPlane = 1000.f;
    cameraData.translation = glm::vec3{ 0, 0, 10 };
    cameraData.rotation = yanve::math::eulerToQuat({ 0, 0, 0 });
    yanve::scene::Camera* camera = new yanve::scene::Camera(cameraData);

    yanve::scene::MeshNodeData meshData{ "cube", &mesh,  bBox };
    yanve::scene::MeshNode* meshNode1 = new yanve::scene::MeshNode(meshData);

    meshData.name = "cube2";
    meshData.translation = glm::vec3{ 0, 3, 0 };
    yanve::scene::MeshNode* meshNode2 = new yanve::scene::MeshNode(meshData);

    //meshData.name = "skybox";
    //meshData.mesh = &skyboxMesh;
    //meshData.bBox = yanve::math::AABB{ {-1.0f, -1.0, -1.0f}, {1.0f, 1.0f, 1.0f} };
    //meshData.translation = glm::vec3{ 0 };
    //yanve::scene::MeshNode* skyboxNode = new yanve::scene::MeshNode(meshData);

    auto& sm = yanve::scene::SceneManager::getInstance();
    cameraId = sm.addNode(camera, sm.getRootNode());
    mesh1Id = sm.addNode(meshNode1, sm.getRootNode());
    mesh2Id = sm.addNode(meshNode2, sm.getRootNode());
    //skyboxId = sm.addNode(skyboxNode, sm.getRootNode());

    // todo: improve this
    cameraUp = ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->up();
    cameraForward = ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->forward();
    cameraRight = ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->right();
    LogInfo("TestApp::TestApp", "after update: camera position: {%.2f, %.2f, %.2f}", camera->translation().x, camera->translation().y, camera->translation().z);

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

    auto& sm = yanve::scene::SceneManager::getInstance();

    // input processing
    {
      auto& input = yanve::InputManager::instance();
      input.update();

      if (input.windowState().resized) {
        glm::vec2 windowSize{ input.windowState().width, input.windowState().height };
        screenFramebuffer.setViewport({ {}, windowSize });
        yanve::gl::defaultFramebuffer.setViewport({ {}, windowSize });
        window.resize(windowSize);
        ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->setViewParameters(45.0, windowSize.x / windowSize.y, 0.1f, 1000.f);
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

      cameraPosition = ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->absolutePosition();

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

      running = !input.quit();
    }

    ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->translate(cameraPosition);
    ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->rotate(cameraRotation);

    ((yanve::scene::MeshNode*)sm.resolveNodeID(mesh1Id))->rotate(angle, { 1,1,0 });
    ((yanve::scene::MeshNode*)sm.resolveNodeID(mesh2Id))->rotate(-angle, { 1,1,0 });

    sm.updateNodes();
    sm.updateQueues();

    cameraUp = ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->up();
    cameraForward = ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->forward();
    cameraRight = ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->right();
    
    //cameraRotation = glm::eulerAngles(camera->rotation());

    if (enableDepthTest != depthTestState) {
      depthTestState = enableDepthTest;
      yanve::gl::Renderer::setFeature(yanve::gl::Renderer::Feature::DepthTest, depthTestState);
    }

    if (enableFaceCulling != faceCullingState) {
      faceCullingState = enableFaceCulling;
      yanve::gl::Renderer::setFeature(yanve::gl::Renderer::Feature::FaceCulling, faceCullingState);
    }

    //glm::mat4 transform = glm::rotate(modelMatrix, angle, glm::vec3(1.0, 1.0, 0.0));
    glm::mat4 view = ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->view();
    glm::mat4 projection = ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->projection();

    //shaderProgram.setModelMatrix(transform);
    shaderProgram.setViewMatrix(view);
    shaderProgram.setProjectionMatrix(projection);

    //textureShaderProgram.setModelMatrix(transform);
    textureShaderProgram.setViewMatrix(view);
    textureShaderProgram.setProjectionMatrix(projection);

    cubeMapShader.setViewMatrix(glm::mat4(glm::mat3(view)));
    cubeMapShader.setProjectionMatrix(projection);

    angle += 0.0001f;
    if (angle >= 360.0f) angle = 0.0f;

    if (angle > 2.0f * glm::pi<float>())
      angle = 0.0f;

    frames++;

    yanve::GuiManager::beginFrame();
    updateGui();
  }

  void updateBufferDebugInfo(const yanve::gl::Buffer& b, const std::string& name = "")
  {
    ImGui::PushID(&b);
    std::ostringstream oss;
    oss << "buffer " << b.id();
    std::string bufferName = oss.str();
    if (name != "") {
      bufferName = name;
    }
    if (ImGui::TreeNode(bufferName.data())) {
      ImGui::Text("id: %d", b.id());
      ImGui::Text("target: %u", (unsigned int)b.target());
      ImGui::Text("size: %lu", b.size());
      ImGui::Text("flags: %u", (uint32_t)b.flags());
      ImGui::TreePop();
    }
    ImGui::PopID();
  }

  void updateMeshAttributeDebugInfo(const yanve::gl::Mesh::AttributeLayout& a) 
  {
    ImGui::PushID(&a);
    std::string attrName = "attribute " + std::to_string(a.buffer.id());
    if (ImGui::TreeNode(attrName.data())) {
      updateBufferDebugInfo(a.buffer);
      ImGui::Text("location: %d", a.location);
      ImGui::Text("size: %lu", a.size);
      ImGui::Text("normalized: %d", a.normalized);
      ImGui::Text("type: %u", a.type);
      ImGui::Text("offset: %lu", a.offset);
      ImGui::Text("stride: %lu", a.stride);
      ImGui::TreePop();
    }
    ImGui::PopID();
  }

  void updateMeshDebugInfo(yanve::gl::Mesh& m, const std::string& name)
  {
    ImGui::PushID(&m);
    if (ImGui::TreeNode(name.data())) {
      ImGui::Text("primitive: %d", m.primitive());
      ImGui::Text("base instance: %u", m.baseInstance());
      ImGui::Text("base vertex: %u", m.baseVertex());
      ImGui::Text("count: %lu", m.count());
      ImGui::Text("index type: %u", (unsigned int)m.indexType());
      ImGui::Text("index type size: %lu", m.indexTypeSize());
      ImGui::Text("instance count: %lu", m.instanceCount());
      ImGui::Text("is indexed: %d", (int)m.isIndexed());
      if (m.attributes().size() > 0) {
        for (auto& attribute : m.attributes()) {
          updateMeshAttributeDebugInfo(attribute);
        }
      }
      updateBufferDebugInfo(m.indexBuffer(), "index buffer");
      ImGui::TreePop();
      ImGui::Separator();
    }
    ImGui::PopID();
  }

  void updateSceneGui(yanve::scene::SceneNode& node)
  {
    if (&node != nullptr) {
      std::string nodeName = yanve::scene::nodeTypeStr(node.type());
      ImGui::PushID(&node);
      if (ImGui::TreeNode(nodeName.data())) {
        ImGui::Text("name: \"%s\"", node.name());
        ImGui::Text("translation: {%.02f, %.02f, %.02f}", node.translation().x, node.translation().y, node.translation().z);
        ImGui::Text("rotation:    {%.02f, %.02f, %.02f, %.02f}", node.rotation().x, node.rotation().y, node.rotation().z, node.rotation().w);
        ImGui::Text("scale:       {%.02f, %.02f, %.02f}", node.scale().x, node.scale().y, node.scale().z);
        if (node.children().size() > 0) {
          ImGui::Text("children (%d):", node.children().size());
          for (auto child : node.children()) {
            updateSceneGui(*child);
          }
        }
        ImGui::TreePop();
        ImGui::Separator();
      }
      ImGui::PopID();
    }
  }
  
  void updateGui() override
  {
    auto& sm = yanve::scene::SceneManager::getInstance();

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
        ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->translate(glm::vec3{ 0, 0, 10 });
        ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->rotate(glm::vec3{ 0 });
        sm.updateNodes();
        sm.updateQueues();
        cameraUp = ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->up();
        cameraForward = ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->forward();
        cameraRight = ((yanve::scene::Camera*)sm.resolveNodeID(cameraId))->right();
      }
    }

    if (ImGui::CollapsingHeader("Scene")) {
      updateSceneGui(sm.getRootNode());
    }

    if (ImGui::CollapsingHeader("Mesh")) {
      updateMeshDebugInfo(mesh, "cube");
      updateMeshDebugInfo(skyboxMesh, "skybox");
      updateMeshDebugInfo(quadMesh, "quad");
    }
      
    ImGui::End();
    
  }

  void render() override
  {
    auto& sm = yanve::scene::SceneManager::getInstance();

    // clear and bind screen framebuffer
    screenFramebuffer
      .clear(yanve::gl::FramebufferClear::Color | yanve::gl::FramebufferClear::Depth)
      .bind();

    // render cubemap
    yanve::gl::Renderer::setDepthMask(false);
    cubeMapShader.bindTexture(cubeMapTexture);
    cubeMapShader.draw(skyboxMesh);
    yanve::gl::Renderer::setDepthMask(true);
    
    // render scene
    yanve::gl::Renderer::enable(yanve::gl::Renderer::Feature::DepthTest);

    for (auto item : sm.getRenderQueue()) {
      if (item.node->isRenderable()) {
        if (useTexture) {
          textureShaderProgram.setModelMatrix(item.node->absTransform());
          textureShaderProgram.bindTexture(texture);
          dynamic_cast<yanve::scene::MeshNode*>(item.node)->mesh().draw(textureShaderProgram);
        }
        else {
          shaderProgram.setModelMatrix(item.node->absTransform());
          dynamic_cast<yanve::scene::MeshNode*>(item.node)->mesh().draw(shaderProgram);
        }
      }
    }

    yanve::gl::Renderer::disable(yanve::gl::Renderer::Feature::DepthTest);

    // clear and bind default framebuffer
    yanve::gl::defaultFramebuffer
      .clear(yanve::gl::FramebufferClear::Color)
      .bind();

    // render scene texture to quad
    quadShaderProgram.bindTexture(screenTexture);
    quadMesh.draw(quadShaderProgram); // this is not part of the scene, as we rendered the scene to a texture, 
                                      // and now the texture on a quad

    // render gui
    yanve::GuiManager::render();

    // swap buffers
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

  void shutdown() override 
  {
    // this is currently wrong, it should be moved to a scene manager or similar system
    //if (scene != nullptr) delete scene;
    //if (camera != nullptr) delete camera;
    //if (meshNode1 != nullptr) delete meshNode1;
    //if (meshNode2 != nullptr) delete meshNode2;
  }
  
  bool isRunning() { return running; }

protected:
  bool running;

  yanve::utils::Clock deltaTimer;
  yanve::utils::Clock clock;

  long framesPerSec;
  long frames;

  yanve::data::GeometryData cubeGD;
  yanve::data::GeometryData skyboxGD;
  yanve::data::GeometryData quadGD;

  yanve::gl::Buffer cubePositionBuffer;
  yanve::gl::Buffer cubeColorBuffer;
  yanve::gl::Buffer cubeTextureBuffer;
  yanve::gl::Buffer cubeIndexBuffer;
  yanve::gl::Buffer skyboxPositionBuffer;
  yanve::gl::Buffer quadPositionBuffer;
  yanve::gl::Buffer quadTextureBuffer;

  yanve::gl::Mesh mesh;
  yanve::gl::MeshView cube2;
  yanve::gl::Mesh quadMesh;
  yanve::gl::Mesh skyboxMesh;
  yanve::gl::Texture2D texture;
  yanve::gl::Texture2D screenTexture;
  yanve::gl::CubeMapTexture cubeMapTexture;
  
  yanve::gl::Renderbuffer renderbuffer;
  yanve::gl::Framebuffer screenFramebuffer;

  size_t cameraId, mesh1Id, mesh2Id, skyboxId;

  TestShader shaderProgram;
  TestTextureShader textureShaderProgram;
  QuadShader quadShaderProgram;
  CubeMapShader cubeMapShader;

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
  TestApp app{"test", 800, 600};
  return app.run();
}