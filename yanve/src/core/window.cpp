#include <core/window.h>

#include <core/gui.h>
#include <core/inputmanager.h>
#include <utils/logger.h>

namespace yanve
{
static const std::string LOG_TAG = "Window";

Window::Window(std::string title, int width, int height)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    LogError(LOG_TAG + __func__, "Could not initialize SDL2.");
    throw std::exception("SDL can't be initialized.");
  }

  _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (_window == nullptr) {
    LogError(LOG_TAG + __func__, "Could not initialize SDL window.");
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
    LogError(LOG_TAG + __func__, "Could not initialize GLAD.");
    throw std::exception("GLAD can't be initalized.");
  }

  // TODO: this has to be moved to context
  glViewport(0, 0, GLsizei(width), GLsizei(height));
  glClearColor(0.0, 0.0, 0.0, 1.0);

  // move to abstract application contructor when possible
  InputManager::instance().resizeEvent(width, height);
  GuiManager::setup(_window);

  _title = title;
}

Window::~Window()
{
  SDL_GL_DeleteContext(_context);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}

void Window::resize(const glm::ivec2& size)
{
  _size = size;
}

void Window::swapBuffers()
{
  SDL_GL_SwapWindow(_window);
}

}