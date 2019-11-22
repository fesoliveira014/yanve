#pragma once

#include <common.h>
#include <utils/nocreate.h>
#include <utils/enumoperators.h>

#include <graphics/gl/version.h>

namespace yanve::gl
{

class DefaultFramebuffer;

namespace detail 
{ 
  struct ContextState;
  struct State;

  enum size_t { ExtensionCount = 192 };
}

class YANVE_API Extension
{
public:
  static std::vector<Extension> extensions(Version version);

  constexpr size_t index() const { return _index; }
  constexpr Version requiredVersion() const { return _requiredVersion; }
  constexpr Version coreVersion() const { return _coreVersion; }
  constexpr const char* string() const { return _string; }

  constexpr Extension(std::size_t index, Version requiredVersion, Version coreVersion, const char* string) : _index{ index }, _requiredVersion{ requiredVersion }, _coreVersion{ coreVersion }, _string{ string } {}

private:
  size_t _index;
  Version _requiredVersion;
  Version _coreVersion;
  const char* _string;
};

class YANVE_API Context
{
public:
  enum class Flag : GLint
  {
    Debug = GL_CONTEXT_FLAG_DEBUG_BIT,
    ForwardCompatible = GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT,
    NoError = GL_CONTEXT_FLAG_NO_ERROR_BIT,
    RobustAccess = GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT
  };

  enum class States : uint {
    Buffers = 1 << 0,
    Framebuffers = 1 << 1,
    Meshes = 1 << 2,
    MeshVao = 1 << 3,
    //BindScratchVao = 1 << 4,
    PixelStorage = 1 << 5,
    Renderer = 1 << 6,
    Shaders = 1 << 7,
    Textures = 1 << 8,
    TransformFeedback = 1 << 9,
    EnterExternal = MeshVao,
    ExitExternal = Buffers | Framebuffers | Meshes | MeshVao | PixelStorage | Renderer | Shaders | Textures | TransformFeedback
  };

  enum class DetectedDriver : uint{
    Amd = 1 << 0,
    Angle = 1 << 1,
    IntelWindows = 1 << 2,
    Mesa = 1 << 3,
    NVidia = 1 << 4,
    Svga3D = 1 << 5,
    SwiftShader = 1 << 6,
    ArmMali = 1 << 7
  };

  explicit Context(/*DefaultFramebuffer& defaultFramebuffer*/);
  explicit Context(NoCreateT);
  //explicit Context(NoCreateT, int argc, char** argv);
  //explicit Context(NoCreateT, std::vector<std::string> args);

  Context(const Context&) = delete;
  Context(Context&& other) noexcept;

  ~Context();

  static bool hasCurrent();
  static Context& current();
  static void makeCurrent(Context* context);

  bool create(DefaultFramebuffer& defaultFramebuffer);

  Version version() const { return _version; }
  
  std::string vendorString() const;
  std::string rendererString() const;
  std::string versionString() const;
  std::string shadingLanguageVersionString() const;
  std::vector<std::string> shadingLanguageVersionStrings() const;
  //std::vector<std::string> extensionStrings() const;

  Flag flags() const { return _flags; }
  detail::State& state() const { return *_state; }

  const std::vector<Extension>& supportedExtensions() const {
    return _supportedExtensions;
  }

  bool isCoreProfile();
  bool isVersionSupported(Version version) const;
  Version supportedVersion(std::initializer_list<Version> versions) const;

  void resetState(const States states);

  enum class InternalFlag : uint {
    DisplayInitializationLog = 1 << 0,
    DisplayVerboseInitializationLog = DisplayInitializationLog | (1 << 1),
    GpuValidation = 1 << 2
  };

protected:
  Context& operator=(const Context&) = delete;
  Context& operator=(Context&&) = delete;
  
  std::unique_ptr<detail::State> _state;
  Version _version;
  Flag _flags;

  std::vector<bool> _extensionStatus;

  std::vector<Extension> _supportedExtensions;
  std::optional<DetectedDriver> _detectedDrivers;
  std::vector<std::string> _disabledExtensions;
};

ENUM_OPERATORS(Context::Flag, GLint)
ENUM_OPERATORS(Context::States, uint)

}