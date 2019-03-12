#include <core/inputmanager.h>
#include <SDL2/SDL.h>

namespace yanve
{
//InputManagerPtr InputManager::ptr = nullptr;

InputManager& InputManager::instance()
{
  static InputManager manager;
  return manager;
}

InputManager::InputManager()
{
  quit = false;
}

void InputManager::update()
{
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      quit = true;
      break;
    case SDL_WINDOWEVENT:
      if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
        resizeEvent(e.window.data1, e.window.data2);
      }
      else if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
        focusEvent(true);
      }
      else if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
        focusEvent(false);
      }
      else if (e.window.event == SDL_WINDOWEVENT_MINIMIZED) {
        minimizedEvent(true);
      }
      else if (e.window.event == SDL_WINDOWEVENT_MAXIMIZED) {
        minimizedEvent(false);
      }
      break;
    case SDL_KEYDOWN:
      keyPressedEvent(e.key.keysym.scancode, SDL_KEYDOWN);
      break;
    case SDL_KEYUP:
      keyPressedEvent(e.key.keysym.scancode, SDL_KEYUP);
      break;
    case SDL_MOUSEBUTTONDOWN:
      mousePressedEvent(e.button.button, SDL_MOUSEBUTTONDOWN);
      break;
    case SDL_MOUSEBUTTONUP:
      mousePressedEvent(e.button.button, SDL_MOUSEBUTTONUP);
      break;
    case SDL_MOUSEMOTION:
      mouseMovedEvent(e.motion.x, e.motion.y);
      break;
    case SDL_MOUSEWHEEL:
      mouseScrolledEvent(e.wheel.x, e.wheel.y);
      break;
    default:
      break;
    }
  }
}

void InputManager::keyPressedEvent(int key, int action)
{
}

void InputManager::mousePressedEvent(int button, int action)
{
}

void InputManager::mouseMovedEvent(int x, int y)
{
}

void InputManager::mouseScrolledEvent(int xOffset, int yOffset)
{
}

void InputManager::resizeEvent(int width, int height)
{
}

void InputManager::focusEvent(bool focus)
{
}

void InputManager::minimizedEvent(bool minimized)
{
}

}