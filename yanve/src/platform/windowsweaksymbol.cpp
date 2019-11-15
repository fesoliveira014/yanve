#include <platform/windowsweaksymbol.h>

#define WIN32_LEAN_AND_MEAN 1
#define VC_EXTRALEAN
#include <windows.h>

namespace yanve::platform
{

void* windowsWeakSymbol(const char* name) 
{
  /* FARPROC?! I want either a function pointer or a variable pointer */
  return reinterpret_cast<void*>(GetProcAddress(GetModuleHandleA(nullptr), name));
}

}