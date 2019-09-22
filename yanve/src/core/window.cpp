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

  glViewport(0, 0, GLsizei(width), GLsizei(height));
  glClearColor(0.0, 0.0, 0.0, 1.0);

  // first time input manager is initialized (dunno if this is good or not, need to think harder about dependencies
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

void Window::clearColor(const glm::vec4& color)
{
  glClearColor(color.r, color.g, color.b, color.a);
}

// TODO: this should not be part of the window class, move it either to
// the renderer or to a default framebuffer class
void Window::clear(GLbitfield bitfield)
{
  glClear(bitfield);
}


void Window::update()
{
  auto& input = InputManager::instance();

  if (input.windowResized()) {
    auto size = input.windowSize();
    glViewport(0, 0, GLsizei(size.x), GLsizei(size.y));
  }
}

void Window::display()
{
  SDL_GL_SwapWindow(_window);
}

}