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
  _quit = false;
}

void InputManager::update()
{
  for (auto& key : _keyboardState) {
    key.first = false;
    key.last = false;
  }

  for (auto& button : _mouseButtonState) {
    button.first = false;
    button.last = false;
  }

  _mouseCursorState.scroll = glm::vec2(0.0);
  _windowState.resized = false;

  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      _quit = true;
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
      if (!_windowState.minimized && _windowState.focus)
        keyPressedEvent(e.key.keysym.scancode, Action::Pressed);
      break;
    case SDL_KEYUP:
      if (!_windowState.minimized && _windowState.focus)
        keyPressedEvent(e.key.keysym.scancode, Action::Released);
      break;
    case SDL_MOUSEBUTTONDOWN:
      if (!_windowState.minimized && _windowState.focus)
        mousePressedEvent(e.button.button, Action::Pressed);
      break;
    case SDL_MOUSEBUTTONUP:
      if (!_windowState.minimized && _windowState.focus)
        mousePressedEvent(e.button.button, Action::Released);
      break;
    case SDL_MOUSEMOTION:
      if (!_windowState.minimized && _windowState.focus)
        mouseMovedEvent(e.motion.x, e.motion.y);
      break;
    case SDL_MOUSEWHEEL:
      if (!_windowState.minimized && _windowState.focus)
        mouseScrolledEvent(e.wheel.x, e.wheel.y);
      break;
    default:
      break;
    }
  }
}

void InputManager::keyPressedEvent(int key, int action)
{
  if (action == Action::Pressed) {
    _keyboardState[key].pressed = true;
    _keyboardState[key].first = true;
    _keyboardState[key].last = false;
  }
  else if (action == Action::Released) {
    _keyboardState[key].pressed = false;
    _keyboardState[key].first = false;
    _keyboardState[key].last = true;
  }
}

void InputManager::mousePressedEvent(int button, int action)
{
  if (action == Action::Pressed) {
    _mouseButtonState[button].pressed = true;
    _mouseButtonState[button].first = true;
    _mouseButtonState[button].last = false;
    _mouseButtonState[button].x0 = _mouseCursorState.x;
    _mouseButtonState[button].y0 = _mouseCursorState.y;
  }
  else if (action == Action::Released) {
    _mouseButtonState[button].pressed = false;
    _mouseButtonState[button].first = false;
    _mouseButtonState[button].last = true;
    _mouseButtonState[button].x0 = _mouseCursorState.x;
    _mouseButtonState[button].y0 = _mouseCursorState.y;
  }
}

void InputManager::mouseMovedEvent(int x, int y)
{
  _mouseCursorState.x = x;
  _mouseCursorState.y = y;
}

void InputManager::mouseScrolledEvent(int xOffset, int yOffset)
{
  _mouseCursorState.scroll = glm::vec2{ xOffset, yOffset };
}

void InputManager::resizeEvent(int width, int height)
{
  _windowState.width = width > 0 ? width : 1;
  _windowState.height = height > 0 ? height : 1;
  _windowState.resized = true;
}

void InputManager::focusEvent(bool focus)
{
  
  _windowState.focus = focus;
}

void InputManager::minimizedEvent(bool minimized)
{
  _windowState.minimized = minimized;
}

}