#include <core/application.h>
#include <core/gui.h>
#include <core/window.h>
#include <core/inputmanager.h>

namespace yanve
{

Application::Application(std::string title, int width, int height) :
  window{title, width, height},
  context{}
{
  gl::Context::makeCurrent(&context);
  InputManager::instance().resizeEvent(width, height);
  GuiManager::setup(window.handle());
}

}