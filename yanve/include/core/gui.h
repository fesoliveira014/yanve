#pragma once

#include <common.h>

#include <external/imgui/imgui.h>
#include <external/imgui/imgui_impl_sdl_gl3.h>

struct SDL_Window;

namespace yanve
{
class YANVE_API GuiManager
{
public:
  static GuiManager& instance()
  {
    static GuiManager gui;
    return gui;
  }

  static void setup(SDL_Window* windowPtr);
  static void beginFrame();
  static void endFrame();

private:
  GuiManager();
  GuiManager(const GuiManager&) = delete;
  GuiManager(GuiManager&&) = delete;

  void _setup(SDL_Window* windowPtr);
  void _beginFrame();

  ~GuiManager();
  
  SDL_Window* _windowPtr;
};
}