#include <core/window.h>

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <gl/glew.h>
#include <core/inputmanager.h>
#include <utils/logger.h>
#include <core/inputmanager.h>

const std::string LOG_TAG = "Window";

namespace yanve
{
Window::Window(std::string title, int width, int height)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    LogError(LOG_TAG + "::Window", "Could not initialize SDL2.");
    throw std::exception("SDL can't be initialized.");
  }

  _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (_window == nullptr) {
    LogError(LOG_TAG + "::Window", "Could not initialize SDL window.");
    throw std::exception("Window can't be initialized.");
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

  _context = SDL_GL_CreateContext(_window);

  SDL_GL_SetSwapInterval(0);

  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    LogError(LOG_TAG + "::Window", "Could not initialize GLEW.");
    throw std::exception("GLEW can't be initalized.");
  }
  // first time input manager is initialized (dunno if this is good or not, need to think harder about dependencies
  InputManager::instance().resizeEvent(width, height); 
  _title = title;
}
}