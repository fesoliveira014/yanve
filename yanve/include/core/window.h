#pragma once

#include <common.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

namespace yanve
{

class Window
{
public:
  YANVE_API Window(std::string title, int width, int height);
  YANVE_API ~Window();

  void YANVE_API clear(const glm::vec4& color = glm::vec4(0.0, 0.0, 0.0, 1.0));
  void YANVE_API update();
  void YANVE_API display();

  void YANVE_API setTitle(std::string title) { _title = title; }
  std::string YANVE_API getTitle() { return _title; }

private:
  SDL_Window* _window;
  SDL_GLContext _context;

  std::string _title;
  bool _open;
};

}