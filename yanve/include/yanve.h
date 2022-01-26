// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include <platform/targetver.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#endif

// reference additional headers your program requires here
#include <common.h>
#include <core/inputmanager.h>
#include <core/window.h>
#include <core/application.h>
#include <core/gui.h>

#include <graphics/gl/attribute.h>
#include <graphics/gl/buffer.h>
#include <graphics/gl/shader.h>
#include <graphics/gl/shaderpipeline.h>
#include <graphics/gl/mesh.h>
#include <graphics/gl/pixelformat.h>
#include <graphics/gl/sampler.h>
#include <graphics/gl/textureformat.h>
#include <graphics/gl/abstracttexture.h>
#include <graphics/gl/texture.h>
#include <graphics/gl/cubemaptexture.h>
#include <graphics/gl/abstractframebuffer.h>
#include <graphics/gl/defaultframebuffer.h>
#include <graphics/gl/framebuffer.h>
#include <graphics/gl/renderbuffer.h>
#include <graphics/gl/renderbufferformat.h>
#include <graphics/gl/renderer.h>
#include <graphics/gl/extensions.h>
#include <graphics/gl/version.h>
#include <graphics/gl/context.h>

#include <data/geometrydata.h>
#include <data/primitive/primitive.h>

#include <math/constants.h>
#include <math/angle.h>
#include <math/aabb.h>
#include <math/plane.h>
#include <math/frustum.h>
#include <math/geometry.h>

#include <scene/scenemanager.h>
#include <scene/scenenode.h>
#include <scene/scene.h>
#include <scene/camera.h>
#include <scene/meshnode.h>

#include <utils/logger.h>
#include <utils/clock.h>
#include <utils/nocreate.h>
#include <utils/random.h>

#ifdef __cplusplus
extern "C" {
#endif
  __declspec(dllexport) extern DWORD NvOptimusEnablement;
  __declspec(dllexport) extern int AmdPowerXpressRequestHighPerformance;
#ifdef __cplusplus
}
#endif
