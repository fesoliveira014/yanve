#pragma once

#include <common.h>

#include <external/imgui/imgui.h>
#include <external/imgui/imgui_impl_sdl.h>
#include <external/imgui/imgui_impl_opengl3.h>

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

  static void setup(SDL_Window* windowPtr, void* context);
  static void beginFrame();
  static void render();

private:
  GuiManager();
  GuiManager(const GuiManager&) = delete;
  GuiManager(GuiManager&&) = delete;

  void _setup(SDL_Window* windowPtr, void* context);
  void _beginFrame();

  ~GuiManager();
  
  SDL_Window* _windowPtr;
};
}