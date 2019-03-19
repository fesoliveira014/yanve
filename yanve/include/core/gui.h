#pragma once

#include <common.h>

#include <external/imgui/imgui.h>
#include <external/imgui/imgui_impl_sdl_gl3.h>

struct SDL_Window;

namespace yanve
{
class GuiManager
{
public:
  static YANVE_API GuiManager& instance()
  {
    static GuiManager gui;
    return gui;
  }

  static void YANVE_API setup(SDL_Window* windowPtr);
  static void YANVE_API beginFrame();
  static void YANVE_API endFrame();

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