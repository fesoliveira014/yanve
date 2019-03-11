#include <core/window.h>

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <gl/glew.h>
#include <core/inputmanager.h>

namespace yanve
{
Window::Window(std::string title, int width, int height)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    throw std::exception("SDL can't be initialized.");
  }

  _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  
}
}