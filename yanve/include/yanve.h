// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include <platform/targetver.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// reference additional headers your program requires here
#include <common.h>
#include <core/inputmanager.h>
#include <core/window.h>
#include <core/application.h>
#include <core/gui.h>

#include <graphics/gl/attribute.h>
#include <graphics/gl/buffer.h>
#include <graphics/gl/shader.h>
#include <graphics/gl/shaderprogram.h>
#include <graphics/gl/mesh.h>
#include <graphics/gl/pixelformat.h>
#include <graphics/gl/sampler.h>
#include <graphics/gl/textureformat.h>
#include <graphics/gl/abstracttexture.h>
#include <graphics/gl/texture.h>

#include <utils/logger.h>
#include <utils/clock.h>

