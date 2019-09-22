#pragma once

#include <common.h>

namespace yanve::gl
{

class YANVE_API Renderer
{
public:
  Renderer() = delete;

  enum class PolygonFacing : GLenum
  {
    Front = GL_FRONT,
    Back = GL_BACK,
    FrontAndBack = GL_FRONT_AND_BACK
  };

  enum class Feature : GLenum
  {
    BlendAdvancedCoherent = GL_BLEND_ADVANCED_COHERENT_KHR,
    Blending = GL_BLEND,
    DebugOutput = GL_DEBUG_OUTPUT,
    DebugOutputSynchronous = GL_DEBUG_OUTPUT_SYNCHRONOUS,
    DepthClamp = GL_DEPTH_CLAMP,
    DepthTest = GL_DEPTH_TEST,
    Dithering = GL_DITHER,
    FaceCulling = GL_CULL_FACE,
    FramebufferSrgb = GL_FRAMEBUFFER_SRGB,
    LogicOperation = GL_COLOR_LOGIC_OP,
    Multisampling = GL_MULTISAMPLE,
    PolygonOffsetFill = GL_POLYGON_OFFSET_FILL,
    PolygonOffsetLine = GL_POLYGON_OFFSET_LINE,
    PolygonOffsetPoint = GL_POLYGON_OFFSET_POINT,
    ProgramPointSize = GL_PROGRAM_POINT_SIZE,
    RasterizerDiscard = GL_RASTERIZER_DISCARD,
    SampleShading = GL_SAMPLE_SHADING,
    SeamlessCubeMapTexture = GL_TEXTURE_CUBE_MAP_SEAMLESS,
    ScissorTest = GL_SCISSOR_TEST,
    StencilTest = GL_STENCIL_TEST
  };

  static void enable(Feature feature);
  static void disable(Feature feature);
  static void setFeature(Feature feature, bool enabled);

  enum class Hint : GLenum 
  {
    FragmentShaderDerivative = GL_FRAGMENT_SHADER_DERIVATIVE_HINT,
    TextureCompression = GL_TEXTURE_COMPRESSION_HINT
  };

  enum class HintMode : GLenum 
  {
    Fastest = GL_FASTEST,
    Nicest = GL_NICEST,
    DontCare = GL_DONT_CARE
  };

  static void setHint(Hint target, HintMode mode);
  static void setClearColor(const glm::vec4& color);
  static void setClearDepth(GLfloat depth);
  static void setClearStencil(GLint stencil);

  enum class FrontFace : GLenum 
  {
    CounterClockWise = GL_CCW,
    ClockWise = GL_CW
  };

  static void setFrontFace(FrontFace mode);
  static void setFaceCullingMode(PolygonFacing mode);

  enum class PolygonMode : GLenum 
  {
    Fill = GL_FILL,
    Line = GL_LINE,
    Point = GL_POINT
  };

  static void setPolygonMode(PolygonMode mode);
  static void setPolygonOffset(float factor, float units);
  static void setLineWidth(float width);
  static void setPointSize(float size);
  static void setMinSampleShading(float value);
  static void setScissor(const glm::uvec2& min, const glm::uvec2& max);

  enum class StencilFunction : GLenum 
  {
    Never = GL_NEVER,
    Always = GL_ALWAYS,
    Less = GL_LESS,
    LessOrEqual = GL_LEQUAL,
    Equal = GL_EQUAL,
    NotEqual = GL_NOTEQUAL,
    GreaterOrEqual = GL_GEQUAL,
    Greater = GL_GREATER
  };

  enum class StencilOperation : GLenum 
  {
    Keep = GL_KEEP,
    Zero = GL_ZERO,
    Replace = GL_REPLACE,
    Increment = GL_INCR,
    IncrementWrap = GL_INCR_WRAP,
    Decrement = GL_DECR,
    DecrementWrap = GL_DECR_WRAP,
    Invert = GL_INVERT
  };

  static void setStencilFunction(PolygonFacing facing, StencilFunction function, int referenceValue, uint mask);
  static void setStencilFunction(StencilFunction function, int referenceValue, uint mask);
  static void setStencilOperation(PolygonFacing facing, StencilOperation stencilFail, StencilOperation depthFail, StencilOperation depthPass);
  static void setStencilOperation(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation depthPass);
  
  typedef StencilFunction DepthFunction;
  static void setDepthFunction(DepthFunction function);
  static void setColorMask(GLboolean allowRed, GLboolean allowGreen, GLboolean allowBlue, GLboolean allowAlpha);
  static void setDepthMask(GLboolean allow);
  static void setStencilMask(PolygonFacing facing, uint allowBits);
  static void setStencilMask(uint allowBits);

  enum class BlendEquation : GLenum 
  {
    Add = GL_FUNC_ADD,
    Subtract = GL_FUNC_SUBTRACT,
    ReverseSubtract = GL_FUNC_REVERSE_SUBTRACT,
    Min = GL_MIN,
    Max = GL_MAX,
    Multiply = GL_MULTIPLY_KHR,
    Screen = GL_SCREEN_KHR,
    Overlay = GL_OVERLAY_KHR,
    Darken = GL_DARKEN_KHR,
    Lighten = GL_LIGHTEN_KHR,
    ColorDodge = GL_COLORDODGE_KHR,
    ColorBurn = GL_COLORBURN_KHR,
    HardLight = GL_HARDLIGHT_KHR,
    SoftLight = GL_SOFTLIGHT_KHR,
    Difference = GL_DIFFERENCE_KHR,
    Exclusion = GL_EXCLUSION_KHR,
    HslHue = GL_HSL_HUE_KHR,
    HslSaturation = GL_HSL_SATURATION_KHR,
    HslColor = GL_HSL_COLOR_KHR,
    HslLuminosity = GL_HSL_LUMINOSITY_KHR
  };

  enum class BlendFunction : GLenum 
  {
    Zero = GL_ZERO,
    One = GL_ONE,
    ConstantColor = GL_CONSTANT_COLOR,
    OneMinusConstantColor = GL_ONE_MINUS_CONSTANT_COLOR,
    ConstantAlpha = GL_CONSTANT_ALPHA,
    OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA,
    SourceColor = GL_SRC_COLOR,
    SecondSourceColor = GL_SRC1_COLOR,
    OneMinusSourceColor = GL_ONE_MINUS_SRC_COLOR,
    OneMinusSecondSourceColor = GL_ONE_MINUS_SRC1_COLOR,
    SourceAlpha = GL_SRC_ALPHA,
    SourceAlphaSaturate = GL_SRC_ALPHA_SATURATE,
    SecondSourceAlpha = GL_SRC1_ALPHA,
    OneMinusSourceAlpha = GL_ONE_MINUS_SRC_ALPHA,
    OneMinusSecondSourceAlpha = GL_ONE_MINUS_SRC1_ALPHA,
    DestinationColor = GL_DST_COLOR,
    OneMinusDestinationColor = GL_ONE_MINUS_DST_COLOR,
    DestinationAlpha = GL_DST_ALPHA,
    OneMinusDestinationAlpha = GL_ONE_MINUS_DST_ALPHA
  };

  static void setBlendEquation(BlendEquation equation);
  static void setBlendEquation(BlendEquation rgb, BlendEquation alpha);
  static void setBlendFunction(BlendFunction source, BlendFunction destination);
  static void setBlendFunction(BlendFunction sourceRgb, BlendFunction destinationRgb, BlendFunction sourceAlpha, BlendFunction destinationAlpha);
  static void setBlendColor(const glm::vec4& color);

  enum class Error : GLenum 
  {
    NoError = GL_NO_ERROR,
    InvalidEnum = GL_INVALID_ENUM,
    InvalidValue = GL_INVALID_VALUE,
    InvalidOperation = GL_INVALID_OPERATION,
    InvalidFramebufferOperation = GL_INVALID_FRAMEBUFFER_OPERATION,
    OutOfMemory = GL_OUT_OF_MEMORY,
    StackUnderflow = GL_STACK_UNDERFLOW,
    StackOverflow = GL_STACK_OVERFLOW
  };

  static Error error() { return static_cast<Error>(/*glGetError()*/GL_NO_ERROR); }
};

}