#pragma once

#if !defined(YANVE_TARGET_WINDOWS) || !defined(YANVE_BUILD_STATIC)
#error this file is only meant to be used in Windows non-static builds
#endif

/* Windows Weak Symbol workaround
 * This was adapted from Magnum as is, and serves as a workaround
 * for linking weak symbols in dynamic libraries in Windows platform.
 **/

namespace yanve::platform
{
void* windowsWeakSymbol(const char* name);
}