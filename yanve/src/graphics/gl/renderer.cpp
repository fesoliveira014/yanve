#include <graphics/gl/renderer.h>

namespace yanve::gl
{

void Renderer::enable(Feature feature)
{
  glEnable(GLenum(feature));
}

void Renderer::disable(Feature feature)
{
  glDisable(GLenum(feature));
}

void Renderer::setFeature(Feature feature, bool enabled)
{
  enabled ? enable(feature) : disable(feature);
}

void Renderer::setHint(Hint target, HintMode mode)
{
  glHint(GLenum(target), GLenum(mode));
}

void Renderer::setClearColor(const glm::vec4& color)
{
  glClearColor(color.x, color.y, color.z, color.w);
}

void Renderer::setClearDepth(GLfloat depth)
{
  glClearDepth(depth);
}

void Renderer::setClearStencil(GLint stencil)
{
  glClearStencil(stencil);
}

void Renderer::setFrontFace(FrontFace mode)
{
  glFrontFace(GLenum(mode));
}

void Renderer::setFaceCullingMode(PolygonFacing mode)
{
  glCullFace(GLenum(mode));
}

void Renderer::setPolygonMode(PolygonMode mode)
{
  glPolygonMode(GL_FRONT_AND_BACK, GLenum(mode));
}

void Renderer::setPolygonOffset(float factor, float units)
{
  glPolygonOffset(factor, units);
}

void Renderer::setLineWidth(float width)
{
  glLineWidth(width);
}

void Renderer::setPointSize(float size)
{
  glPointSize(size);
}

void Renderer::setMinSampleShading(float value)
{
  glMinSampleShading(value);
}

void Renderer::setScissor(const glm::uvec2& min, const glm::uvec2& max)
{
  glm::uvec2 dims = max - min;
  glScissor(min.x, min.y, dims.x, dims.y);
}

void Renderer::setStencilFunction(PolygonFacing facing, StencilFunction function, int referenceValue, uint mask)
{
  glStencilFuncSeparate(GLenum(facing), GLenum(function), referenceValue, mask);
}

void Renderer::setStencilFunction(StencilFunction function, int referenceValue, uint mask)
{
  glStencilFunc(GLenum(function), referenceValue, mask);
}

void Renderer::setStencilOperation(PolygonFacing facing, StencilOperation stencilFail, StencilOperation depthFail, StencilOperation depthPass)
{
  glStencilOpSeparate(GLenum(facing), GLenum(stencilFail), GLenum(depthFail), GLenum(depthPass));
}

void Renderer::setStencilOperation(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation depthPass)
{
  glStencilOp(GLenum(stencilFail), GLenum(depthFail), GLenum(depthPass));
}

void Renderer::setDepthFunction(DepthFunction function)
{
  glDepthFunc(GLenum(function));
}

void Renderer::setColorMask(GLboolean allowRed, GLboolean allowGreen, GLboolean allowBlue, GLboolean allowAlpha)
{
  glColorMask(allowRed, allowGreen, allowBlue, allowAlpha);
}

void Renderer::setDepthMask(GLboolean allow)
{
  glDepthMask(allow);
}

void Renderer::setStencilMask(PolygonFacing facing, uint allowBits)
{
  glStencilMaskSeparate(GLenum(facing), allowBits);
}

void Renderer::setStencilMask(uint allowBits)
{
  glStencilMask(allowBits);
}

void Renderer::setBlendEquation(BlendEquation equation)
{
  glBlendEquation(GLenum(equation));
}

void Renderer::setBlendEquation(BlendEquation rgb, BlendEquation alpha)
{
  glBlendEquationSeparate(GLenum(rgb), GLenum(alpha));
}

void Renderer::setBlendFunction(BlendFunction source, BlendFunction destination)
{
  glBlendFunc(GLenum(source), GLenum(destination));
}

void Renderer::setBlendFunction(BlendFunction sourceRgb, BlendFunction destinationRgb, BlendFunction sourceAlpha, BlendFunction destinationAlpha)
{
  glBlendFuncSeparate(GLenum(sourceRgb), GLenum(destinationRgb), GLenum(sourceAlpha), GLenum(destinationAlpha));
}

void Renderer::setBlendColor(const glm::vec4& color)
{
  glBlendColor(color.x, color.y, color.z, color.w);
}

void Renderer::initializeContext()
{
  setClearColor({.945f, .945f, .945f, 1.0f});
}

}