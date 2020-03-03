#pragma once

#include <common.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

namespace yanve
{

class InputManager
{
public:
  enum YANVE_API Action {
    Pressed = 0,
    Released,
    NumActions
  };

  enum YANVE_API Key
  {
    keyUnknown = SDL_SCANCODE_UNKNOWN,
    keyA = SDL_SCANCODE_A,
    keyB = SDL_SCANCODE_B,
    keyC = SDL_SCANCODE_C,
    keyD = SDL_SCANCODE_D,
    keyE = SDL_SCANCODE_E,
    keyF = SDL_SCANCODE_F,
    keyG = SDL_SCANCODE_G,
    keyH = SDL_SCANCODE_H,
    keyI = SDL_SCANCODE_I,
    keyJ = SDL_SCANCODE_J,
    keyK = SDL_SCANCODE_K,
    keyL = SDL_SCANCODE_L,
    keyM = SDL_SCANCODE_M,
    keyN = SDL_SCANCODE_N,
    keyO = SDL_SCANCODE_O,
    keyP = SDL_SCANCODE_P,
    keyQ = SDL_SCANCODE_Q,
    keyR = SDL_SCANCODE_R,
    keyS = SDL_SCANCODE_S,
    keyT = SDL_SCANCODE_T,
    keyU = SDL_SCANCODE_U,
    keyV = SDL_SCANCODE_V,
    keyW = SDL_SCANCODE_W,
    keyX = SDL_SCANCODE_X,
    keyY = SDL_SCANCODE_Y,
    keyZ = SDL_SCANCODE_Z,
    key1 = SDL_SCANCODE_1,
    key2 = SDL_SCANCODE_2,
    key3 = SDL_SCANCODE_3,
    key4 = SDL_SCANCODE_4,
    key5 = SDL_SCANCODE_5,
    key6 = SDL_SCANCODE_6,
    key7 = SDL_SCANCODE_7,
    key8 = SDL_SCANCODE_8,
    key9 = SDL_SCANCODE_9,
    key0 = SDL_SCANCODE_0,
    keyReturn = SDL_SCANCODE_RETURN,
    keyEscape = SDL_SCANCODE_ESCAPE,
    keyBackspace = SDL_SCANCODE_BACKSPACE,
    keyTab = SDL_SCANCODE_TAB,
    keySpace = SDL_SCANCODE_SPACE,
    keyMinus = SDL_SCANCODE_MINUS,
    keyEquals = SDL_SCANCODE_EQUALS,
    keyLeftBracket = SDL_SCANCODE_LEFTBRACKET,
    keyRrightBracket = SDL_SCANCODE_RIGHTBRACKET,
    keyBackSlash = SDL_SCANCODE_BACKSLASH,
    keyNonusash = SDL_SCANCODE_NONUSHASH,
    keySemicolon = SDL_SCANCODE_SEMICOLON,
    keyApostrophe = SDL_SCANCODE_APOSTROPHE,
    keyGrave = SDL_SCANCODE_GRAVE,
    keyComma = SDL_SCANCODE_COMMA,
    keyPeriod = SDL_SCANCODE_PERIOD,
    keySlash = SDL_SCANCODE_SLASH,
    keyCapslock = SDL_SCANCODE_CAPSLOCK,
    keyF1 = SDL_SCANCODE_F1,
    keyF2 = SDL_SCANCODE_F2,
    keyF3 = SDL_SCANCODE_F3,
    keyF4 = SDL_SCANCODE_F4,
    keyF5 = SDL_SCANCODE_F5,
    keyF6 = SDL_SCANCODE_F6,
    keyF7 = SDL_SCANCODE_F7,
    keyF8 = SDL_SCANCODE_F8,
    keyF9 = SDL_SCANCODE_F9,
    keyF10 = SDL_SCANCODE_F10,
    keyF11 = SDL_SCANCODE_F11,
    keyF12 = SDL_SCANCODE_F12,
    keyPrintScreen = SDL_SCANCODE_PRINTSCREEN,
    keyScrollLock = SDL_SCANCODE_SCROLLLOCK,
    keyPause = SDL_SCANCODE_PAUSE,
    keyInsert = SDL_SCANCODE_INSERT,
    keyHome = SDL_SCANCODE_HOME,
    keyPageUp = SDL_SCANCODE_PAGEUP,
    keyDelete = SDL_SCANCODE_DELETE,
    keyEnd = SDL_SCANCODE_END,
    keyPageDown = SDL_SCANCODE_PAGEDOWN,
    keyRight = SDL_SCANCODE_RIGHT,
    keyLeft = SDL_SCANCODE_LEFT,
    keyDown = SDL_SCANCODE_DOWN,
    keyUp = SDL_SCANCODE_UP,
    keyNumLockClear = SDL_SCANCODE_NUMLOCKCLEAR,
    numKeys
  };

  enum YANVE_API Mouse
  {
    buttonLeft = SDL_BUTTON_LEFT,
    buttonMiddle = SDL_BUTTON_MIDDLE,
    buttonRight = SDL_BUTTON_RIGHT,
    numButtons
  };

private:
  void keyPressedEvent(int key, int action);
  void mousePressedEvent(int button, int action);
  void mouseMovedEvent(int x, int y);
  void mouseScrolledEvent(int xOffset, int yOffset);
  void resizeEvent(int width, int height);
  void focusEvent(bool focus);
  void minimizedEvent(bool minimized);

  friend class Application;

public:
  void YANVE_API update();

  bool YANVE_API windowResized() { return _windowState.resized; }
  bool YANVE_API windowMinimized() { return _windowState.minimized; }
  bool YANVE_API windowFocus() { return _windowState.focus; }
  glm::vec2 YANVE_API windowSize() const { return glm::vec2{ _windowState.width, _windowState.height }; }
  bool YANVE_API pressed(const Key& keyboardKey) { return _keyboardState[keyboardKey].pressed; }
  bool YANVE_API pressed(const Mouse& button) { return _mouseButtonState[button].pressed; }
  bool YANVE_API mouseMoved() { return _mouseCursorState.dx == _mouseCursorState.dy == 0 ? false : true; }
  glm::vec2 YANVE_API mousePosition() const { return glm::vec2{ _mouseCursorState.x, _mouseCursorState.y }; }
  glm::vec2 YANVE_API mouseDisplacement(const Mouse& button) const { return glm::vec2{ _mouseButtonState[button].x1 - _mouseButtonState[button].x0, _mouseButtonState[button].y1 - _mouseButtonState[button].y0 }; }
  glm::vec2 YANVE_API scroll() const { return _mouseCursorState.scroll; }
  bool YANVE_API quit() { return _quit; }

private:
  struct KeyState
  {
    bool pressed;
    bool first;
    bool last;
  };

  struct MouseButtonState
  {
    int x0;
    int y0;
    int x1;
    int y1;
    bool pressed;
    bool first;
    bool last;
  };

  struct MouseCursorState
  {
    int x;
    int y;
    int dx;
    int dy;
    glm::vec2 scroll = glm::vec2{ 0.0 };
  };

  struct WindowState
  {
    int width;
    int height;
    bool resized;
    bool minimized;
    bool focus;
  };
  
  KeyState _keyboardState[Key::numKeys];
  MouseButtonState _mouseButtonState[Mouse::numButtons];
  MouseCursorState _mouseCursorState;
  WindowState _windowState;
  bool _quit;

public:
  static YANVE_API InputManager& instance();

private:
  InputManager();
  ~InputManager() {};
  InputManager(const InputManager&) = delete;
  InputManager(InputManager&&) = delete;
  InputManager& operator= (const InputManager&) = delete;

  //static InputManagerPtr ptr;
};
}