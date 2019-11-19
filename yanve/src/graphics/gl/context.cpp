#include <graphics/gl/context.h>
#include <graphics/gl/extensions.h>
#include <graphics/gl/renderer.h>
#include <graphics/gl/defaultframebuffer.h>

#include <graphics/gl/state/state.h>
#include <graphics/gl/state/bufferstate.h>
#include <graphics/gl/state/framebufferstate.h>
#include <graphics/gl/state/rendererstate.h>
#include <graphics/gl/state/meshstate.h>
#include <graphics/gl/state/shaderstate.h>
#include <graphics/gl/state/texturestate.h>


#include <utils/logger.h>

#if defined(YANVE_TARGET_WINDOWS) && defined(YANVE_BUILD_STATIC)
#error huh
#include <platform/windowsweaksymbol.h>
#endif

namespace yanve::gl
{

namespace {
#define _extension(vendor, extension)                           \
    {extensions::vendor::extension::Index, extensions::vendor::extension::requiredVersion(), extensions::vendor::extension::coreVersion(), extensions::vendor::extension::string()}

constexpr Extension ExtensionList[]{
    _extension(AMD,transform_feedback3_lines_triangles),
    _extension(AMD,vertex_shader_layer),
    _extension(AMD,shader_trinary_minmax),
    _extension(ARB,robustness),
    _extension(ARB,robustness_isolation),
    _extension(ARB,robustness_application_isolation),
    _extension(ARB,robustness_share_group_isolation),
    _extension(ARB,bindless_texture),
    _extension(ARB,compute_variable_group_size),
    _extension(ARB,seamless_cubemap_per_texture),
    _extension(ARB,sparse_texture),
    _extension(ARB,sparse_buffer),
    _extension(ARB,ES3_2_compatibility),
    _extension(ATI,texture_mirror_once),
    _extension(EXT,texture_filter_anisotropic),
    _extension(EXT,texture_compression_s3tc),
    _extension(EXT,texture_mirror_clamp),
    _extension(EXT,texture_compression_dxt1),
    _extension(EXT,texture_sRGB_decode),
    _extension(EXT,shader_integer_mix),
    _extension(EXT,debug_label),
    _extension(EXT,debug_marker),
    _extension(EXT,texture_sRGB_R8),
    _extension(GREMEDY,string_marker),
    _extension(KHR,texture_compression_astc_ldr),
    _extension(KHR,texture_compression_astc_hdr),
    _extension(KHR,blend_equation_advanced),
    _extension(KHR,blend_equation_advanced_coherent),
    _extension(KHR,texture_compression_astc_sliced_3d) };
constexpr Extension ExtensionList300[]{
    _extension(ARB,map_buffer_range),
    _extension(ARB,color_buffer_float),
    _extension(ARB,half_float_pixel),
    _extension(ARB,texture_float),
    _extension(ARB,depth_buffer_float),
    _extension(ARB,texture_rg),
    _extension(ARB,vertex_array_object),
    _extension(ARB,framebuffer_object),
    _extension(ARB,framebuffer_sRGB),
    _extension(ARB,half_float_vertex),
    _extension(EXT,gpu_shader4),
    _extension(EXT,packed_float),
    _extension(EXT,texture_array),
    _extension(EXT,texture_compression_rgtc),
    _extension(EXT,texture_shared_exponent),
    _extension(EXT,draw_buffers2),
    _extension(EXT,texture_integer),
    _extension(EXT,transform_feedback),
    _extension(NV,depth_buffer_float),
    _extension(NV,conditional_render) };
constexpr Extension ExtensionList310[]{
    _extension(ARB,texture_rectangle),
    _extension(ARB,draw_instanced),
    _extension(ARB,texture_buffer_object),
    _extension(ARB,uniform_buffer_object),
    _extension(ARB,copy_buffer),
    _extension(EXT,texture_snorm),
    _extension(NV,primitive_restart) };
constexpr Extension ExtensionList320[]{
    _extension(ARB,geometry_shader4),
    _extension(ARB,depth_clamp),
    _extension(ARB,draw_elements_base_vertex),
    _extension(ARB,fragment_coord_conventions),
    _extension(ARB,provoking_vertex),
    _extension(ARB,seamless_cube_map),
    _extension(ARB,sync),
    _extension(ARB,texture_multisample),
    _extension(ARB,vertex_array_bgra) };
constexpr Extension ExtensionList330[]{
    _extension(ARB,instanced_arrays),
    _extension(ARB,blend_func_extended),
    _extension(ARB,explicit_attrib_location),
    _extension(ARB,occlusion_query2),
    _extension(ARB,sampler_objects),
    _extension(ARB,shader_bit_encoding),
    _extension(ARB,texture_rgb10_a2ui),
    _extension(ARB,texture_swizzle),
    _extension(ARB,timer_query),
    _extension(ARB,vertex_type_2_10_10_10_rev) };
constexpr Extension ExtensionList400[]{
    _extension(ARB,draw_buffers_blend),
    _extension(ARB,sample_shading),
    _extension(ARB,texture_cube_map_array),
    _extension(ARB,texture_gather),
    _extension(ARB,texture_query_lod),
    _extension(ARB,draw_indirect),
    _extension(ARB,gpu_shader5),
    _extension(ARB,gpu_shader_fp64),
    _extension(ARB,shader_subroutine),
    _extension(ARB,tessellation_shader),
    _extension(ARB,texture_buffer_object_rgb32),
    _extension(ARB,transform_feedback2),
    _extension(ARB,transform_feedback3) };
constexpr Extension ExtensionList410[]{
    _extension(ARB,ES2_compatibility),
    _extension(ARB,get_program_binary),
    _extension(ARB,separate_shader_objects),
    _extension(ARB,shader_precision),
    _extension(ARB,vertex_attrib_64bit),
    _extension(ARB,viewport_array) };
constexpr Extension ExtensionList420[]{
    _extension(ARB,texture_compression_bptc),
    _extension(ARB,base_instance),
    _extension(ARB,shading_language_420pack),
    _extension(ARB,transform_feedback_instanced),
    _extension(ARB,compressed_texture_pixel_storage),
    _extension(ARB,conservative_depth),
    _extension(ARB,internalformat_query),
    _extension(ARB,map_buffer_alignment),
    _extension(ARB,shader_atomic_counters),
    _extension(ARB,shader_image_load_store),
    /* Mentioned in GLSL 4.20 specs as newly added */
    _extension(ARB,shading_language_packing),
    _extension(ARB,texture_storage) };
constexpr Extension ExtensionList430[]{
    _extension(ARB,arrays_of_arrays),
    _extension(ARB,ES3_compatibility),
    _extension(ARB,clear_buffer_object),
    _extension(ARB,compute_shader),
    _extension(ARB,copy_image),
    _extension(ARB,explicit_uniform_location),
    _extension(ARB,fragment_layer_viewport),
    _extension(ARB,framebuffer_no_attachments),
    _extension(ARB,internalformat_query2),
    _extension(ARB,invalidate_subdata),
    _extension(ARB,multi_draw_indirect),
    _extension(ARB,program_interface_query),
    _extension(ARB,robust_buffer_access_behavior),
    _extension(ARB,shader_image_size),
    _extension(ARB,shader_storage_buffer_object),
    _extension(ARB,stencil_texturing),
    _extension(ARB,texture_buffer_range),
    _extension(ARB,texture_query_levels),
    _extension(ARB,texture_storage_multisample),
    _extension(ARB,texture_view),
    _extension(ARB,vertex_attrib_binding),
    _extension(KHR,debug) };
constexpr Extension ExtensionList440[]{
    _extension(ARB,buffer_storage),
    _extension(ARB,clear_texture),
    _extension(ARB,enhanced_layouts),
    _extension(ARB,multi_bind),
    _extension(ARB,query_buffer_object),
    _extension(ARB,texture_mirror_clamp_to_edge),
    _extension(ARB,texture_stencil8),
    _extension(ARB,vertex_type_10f_11f_11f_rev) };
constexpr Extension ExtensionList450[]{
    _extension(ARB,ES3_1_compatibility),
    _extension(ARB,clip_control),
    _extension(ARB,conditional_render_inverted),
    _extension(ARB,cull_distance),
    _extension(ARB,derivative_control),
    _extension(ARB,direct_state_access),
    _extension(ARB,get_texture_sub_image),
    _extension(ARB,shader_texture_image_samples),
    _extension(ARB,texture_barrier),
    _extension(KHR,context_flush_control),
    _extension(KHR,robustness) };
constexpr Extension ExtensionList460[]{
    _extension(ARB,indirect_parameters),
    _extension(ARB,shader_draw_parameters),
    _extension(ARB,shader_group_vote),
    _extension(ARB,pipeline_statistics_query),
    _extension(ARB,transform_feedback_overflow_query),
    _extension(ARB,shader_atomic_counter_ops),
    _extension(ARB,gl_spirv),
    _extension(ARB,polygon_offset_clamp),
    _extension(ARB,texture_filter_anisotropic),
    _extension(KHR,no_error) };

}

std::vector<Extension> Extension::extensions(Version version)
{
  switch (version) {
  case Version::None:  return std::vector<Extension>(ExtensionList, ExtensionList + sizeof(ExtensionList) / sizeof(Extension));
  case Version::GL300: return std::vector<Extension>(ExtensionList300, ExtensionList300 + sizeof(ExtensionList300) / sizeof(Extension));
  case Version::GL310: return std::vector<Extension>(ExtensionList310, ExtensionList310 + sizeof(ExtensionList310) / sizeof(Extension));
  case Version::GL320: return std::vector<Extension>(ExtensionList320, ExtensionList320 + sizeof(ExtensionList320) / sizeof(Extension));
  case Version::GL330: return std::vector<Extension>(ExtensionList330, ExtensionList330 + sizeof(ExtensionList330) / sizeof(Extension));
  case Version::GL400: return std::vector<Extension>(ExtensionList400, ExtensionList400 + sizeof(ExtensionList400) / sizeof(Extension));
  case Version::GL410: return std::vector<Extension>(ExtensionList410, ExtensionList410 + sizeof(ExtensionList410) / sizeof(Extension));
  case Version::GL420: return std::vector<Extension>(ExtensionList420, ExtensionList420 + sizeof(ExtensionList420) / sizeof(Extension));
  case Version::GL430: return std::vector<Extension>(ExtensionList430, ExtensionList430 + sizeof(ExtensionList430) / sizeof(Extension));
  case Version::GL440: return std::vector<Extension>(ExtensionList440, ExtensionList440 + sizeof(ExtensionList440) / sizeof(Extension));
  case Version::GL450: return std::vector<Extension>(ExtensionList450, ExtensionList450 + sizeof(ExtensionList450) / sizeof(Extension));
  case Version::GL460: return std::vector<Extension>(ExtensionList460, ExtensionList460 + sizeof(ExtensionList460) / sizeof(Extension));
  default: return {};
  }
}

#if !defined(YANVE_BUILD_STATIC) || defined(YANVE_TARGET_WINDOWS)
namespace {
#endif

YANVE_THREAD_LOCAL
#if defined(YANVE_BUILD_STATIC) && !defined(YANVE_TARGET_WINDOWS) // TODO: we are not doing static builds here
YANVE_API
  #ifdef __GNUC__
  __attribute__((weak))
  #endif
#endif
Context* currentContext = nullptr;

#if !defined(YANVE_BUILD_STATIC) || defined(YANVE_TARGET_WINDOWS)
}
#endif

#if defined(YANVE_TARGET_WINDOWS) && defined(YANVE_BUILD_STATIC)
extern "C" YANVE_API Context*& yanveUniqueCurrentContext()
{
  return currentContext;
}

namespace 
{

Context*& windowsCurrentContext()
{
  static Context*& (*const uniqueGlobals)() = reinterpret_cast<Context * &(*)()>(platform::windowsWeakSymbol("yanveUniqueCurrentContext"));
  return uniqueGlobals();
}

}
#

#define currentContext windowsCurrentContext();
#endif

Context::Context(DefaultFramebuffer& defaultFramebuffer) : Context{NoCreate}
{
  if (!create(defaultFramebuffer)) {
    LogError("Context", "Count not create GL Context!");
    throw std::runtime_error("Could not create GL context!");
  }
}

Context::Context(NoCreateT) : _version{gl::Version::None}, _flags{}
{

}

Context::Context(Context&& other) noexcept:
  _version{other._version},
  _flags{other._flags},
  _extensionStatus{other._extensionStatus},
  _supportedExtensions{other._supportedExtensions},
  _state{std::move(other._state)}
{
  if (currentContext == &other) currentContext = this;
}

Context::~Context()
{
  if (currentContext == this) currentContext = nullptr;
}

bool Context::hasCurrent() 
{ 
  return currentContext; 
}

Context& Context::current()
{
  if (!currentContext) {
    LogError("GL::Context::current", "no current context");
    std::abort();
  }
  return *currentContext;
}

void Context::makeCurrent(Context* context)
{
  currentContext = context;
}

bool Context::create(DefaultFramebuffer& defaultFramebuffer)
{
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    LogError("Context::create", "Could not initialize GLAD.");
    return false;
  }

  GLint majorVersion, minorVersion;

  glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);

  const auto versionError = Renderer::error();
  if (versionError == Renderer::Error::NoError) {
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
  }
  else {
    if (versionError == Renderer::Error::InvalidEnum) {
      LogError("Context::create", "cannot retrieve OpenGL version");
      return false;
    }
  }

  _version = gl::version(majorVersion, minorVersion);

  if (isVersionSupported(Version::GL300))
    glGetIntegerv(GL_CONTEXT_FLAGS, reinterpret_cast<GLint*>(&_flags));
  else {
    LogError("Context::create", "context must at least support OpenGL 3.0");
    return false;
  }

  std::vector<Version> versions{
    Version::GL300,
    Version::GL310,
    Version::GL320,
    Version::GL330,
    Version::GL400,
    Version::GL410,
    Version::GL420,
    Version::GL430,
    Version::GL440,
    Version::GL450,
    Version::GL460,
    Version::None
  };

  /*std::size_t future = 0;
  while (versions[future] != Version::None && isVersionSupported(versions[future]))
    ++future;

  _extensionStatus.resize(state::ExtensionCount);
    
  for (std::size_t i = 0; i != future; ++i) {
    for (const Extension& extension : Extension::extensions(versions[i]))
      _extensionStatus[extension.index()] = true;
  }

  std::unordered_map<std::string, Extension> futureExtensions;
  for (std::size_t i = future; i != versions.size(); ++i) {
    for (const Extension& extension : Extension::extensions(versions[i]))
      futureExtensions.emplace(extension.string(), extension);
  }

  const std::vector<std::string> extensions = extensionStrings();
  for (const auto& extension : extensions) {
    const auto found = futureExtensions.find(extension);
    if (found != futureExtensions.end()) {
      _supportedExtensions.push_back(found->second);
      _extensionStatus[found->second.index()] = true;
    }

  }*/
  
  _state = std::make_unique<detail::State>(*this);

  auto& state = *_state->framebuffer;

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
  defaultFramebuffer._viewport = state.viewport = Rectangle2Di{ {viewport[0], viewport[1]}, {viewport[2], viewport[3]} };

  Renderer::initializeContext();

  return true;
}

std::string Context::vendorString() const
{
  return std::string{ reinterpret_cast<const char*>(glGetString(GL_VENDOR)) };
}

std::string Context::rendererString() const
{
  return std::string{ reinterpret_cast<const char*>(glGetString(GL_RENDERER)) };
}

std::string Context::versionString() const
{
  return std::string{ reinterpret_cast<const char*>(glGetString(GL_VERSION)) };
}

std::string Context::shadingLanguageVersionString() const
{
  return std::string{ reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)) };
}

std::vector<std::string> Context::shadingLanguageVersionStrings() const
{
  GLint versionCount = 0;
  glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &versionCount);

  if (!versionCount)
    return { shadingLanguageVersionString() };

  std::vector<std::string> versions;
  versions.reserve(versionCount);
  for (GLint i = 0; i < versionCount; ++i)
    versions.emplace_back(reinterpret_cast<const char*>(glGetStringi(GL_SHADING_LANGUAGE_VERSION, i)));
  
  return versions;
}

bool Context::isCoreProfile()
{
  GLint value = 0;
  glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &value);
  return value & GL_CONTEXT_CORE_PROFILE_BIT;
}

bool Context::isVersionSupported(Version version) const
{
  return _version >= version;
}

Version Context::supportedVersion(std::initializer_list<Version> versions) const
{
  for (auto version : versions)
    if (isVersionSupported(version)) return version;

  return Version::GL210;
}

void Context::resetState(const States states)
{
  if (!(States::Buffers & states)) 
    _state->buffer->reset();
  if (!(States::Framebuffers & states)) 
    _state->framebuffer->reset();
  if (!(States::Meshes & states))
    _state->mesh->reset();
  if (!(States::MeshVao & states))
    _state->mesh->resetVao();
  /*if (!(States::PixelStorage & states)) {
    _state->renderer->unpackPixelStorage.reset();
    _state->renderer->packPixelStorage.reset();
  }*/
  if (!(States::Shaders & states))
    _state->shader->reset();
  if (!(States::Textures & states))
    _state->texture->reset();
  /*if (!(States::TransformFeedback & states))
    _state->transformFeedback->reset();*/
}

}