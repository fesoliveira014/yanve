#pragma once

#include <common.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

namespace yanve
{

class YANVE_API Window
{
  friend class Application;

public:
  Window(std::string title, int width, int height);
  ~Window();

  void swapBuffers();

  glm::ivec2 size() const { return _size; }
  void resize(const glm::ivec2& size);

  void setTitle(std::string title) { _title = title; }
  std::string getTitle() { return _title; }
  
protected:
  SDL_Window* handle() { return _window; }
  SDL_GLContext* context() { return &_context; }

  SDL_Window* _window;
  SDL_GLContext _context;
  glm::ivec2 _size;

  std::string _title;
};

}