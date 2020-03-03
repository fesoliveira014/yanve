#include <core/inputmanager.h>
#include <utils/logger.h>

#include <external/imgui/imgui.h>
#include <external/imgui/imgui_impl_sdl_gl3.h>
#include <SDL2/SDL.h>

namespace yanve
{
//InputManagerPtr InputManager::ptr = nullptr;
  static const std::string LOG_TAG = "InputManager::";

InputManager& InputManager::instance()
{
  static InputManager manager;
  return manager;
}

InputManager::InputManager()
{
  for (auto& key : _keyboardState) {
    key.first = false;
    key.last = false;
    key.pressed = false;
  }

  for (auto& button : _mouseButtonState) {
    button.first = false;
    button.last = false;
    button.pressed = false;
    button.x0 = 0;
    button.y0 = 0;
    button.x1 = 0;
    button.y1 = 0;
  }

  _mouseCursorState.x = 0;
  _mouseCursorState.y = 0;
  _mouseCursorState.dx = 0;
  _mouseCursorState.dy = 0;
  _mouseCursorState.scroll = glm::vec2(0.0);

  _windowState.width = 0;
  _windowState.height = 0;
  _windowState.resized = false;
  _windowState.minimized = false;
  _windowState.focus= true;

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

  _mouseCursorState.dx = 0;
  _mouseCursorState.dy = 0;
  _mouseCursorState.scroll = glm::vec2(0.0);
  _windowState.resized = false;

  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    ImGui_ImplSdlGL3_ProcessEvent(&e);
    switch (e.type) {
    case SDL_QUIT:
      _quit = true;
      LogVerbose(LOG_TAG + __func__, "Quit event");
      break;
    case SDL_WINDOWEVENT:
      if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
        resizeEvent(e.window.data1, e.window.data2);
        LogVerbose(LOG_TAG + __func__, "Resize event: (%d, %d)", e.window.data1, e.window.data2);
      }
      else if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
        focusEvent(true);
        LogVerbose(LOG_TAG + __func__, "Gained focus event");
      }
      else if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
        focusEvent(false);
        LogVerbose(LOG_TAG + __func__, "Lost focus event");
      }
      else if (e.window.event == SDL_WINDOWEVENT_MINIMIZED) {
        minimizedEvent(true);
        LogVerbose(LOG_TAG + __func__, "Minimized event");
      }
      else if (e.window.event == SDL_WINDOWEVENT_MAXIMIZED) {
        minimizedEvent(false);
        LogVerbose(LOG_TAG + __func__, "Maximized event");
      }
      break;
    case SDL_KEYDOWN:
      if (!_windowState.minimized && _windowState.focus) {
        keyPressedEvent(e.key.keysym.scancode, Action::Pressed);
        LogVerbose(LOG_TAG + __func__, "Key down event: {scancode: %d}", e.key.keysym.scancode);
      }
      break;
    case SDL_KEYUP:
      if (!_windowState.minimized && _windowState.focus) {
        keyPressedEvent(e.key.keysym.scancode, Action::Released);
        LogVerbose(LOG_TAG + __func__, "Key up event: {scancode: %d}", e.key.keysym.scancode);
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      if (!_windowState.minimized && _windowState.focus) {
        mousePressedEvent(e.button.button, Action::Pressed);
        LogVerbose(LOG_TAG + __func__, "Button down event: {botton: %d}", e.button.button);
      }
      break;
    case SDL_MOUSEBUTTONUP:
      if (!_windowState.minimized && _windowState.focus) {
        mousePressedEvent(e.button.button, Action::Released);
        LogVerbose(LOG_TAG + __func__, "Button up event: {botton: %d}", e.button.button);
      }
      break;
    case SDL_MOUSEMOTION:
      if (!_windowState.minimized && _windowState.focus) {
        mouseMovedEvent(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
        LogVerbose(LOG_TAG + __func__, "Mouse motion event: (%d,%d)", e.motion.x, e.motion.y);
      }
      break;
    case SDL_MOUSEWHEEL:
      if (!_windowState.minimized && _windowState.focus) {
        mouseScrolledEvent(e.wheel.x, e.wheel.y);
        LogVerbose(LOG_TAG + __func__, "Mouse scroll event: (%d,%d)", e.wheel.x, e.wheel.y);
      }
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
  _mouseCursorState.dx = _mouseCursorState.x - x;
  _mouseCursorState.dy = _mouseCursorState.y - y;
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