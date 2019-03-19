#include <core/gui.h>
#include <utils/logger.h>

namespace yanve
{
static const std::string LOG_TAG = "GuiManager::";

GuiManager::GuiManager() : _windowPtr{ nullptr } {}

GuiManager::~GuiManager()
{
  ImGui_ImplSdlGL3_Shutdown();
  ImGui::DestroyContext();
  _windowPtr = nullptr;
}

void GuiManager::setup(SDL_Window* windowPtr)
{
  instance()._setup(windowPtr);
}

void GuiManager::_setup(SDL_Window* windowPtr) 
{
  if (windowPtr == nullptr) {
    LogError(LOG_TAG + __func__, "Could not initialize gui because it received a null pointer.");
    return;
  }

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  ImGui_ImplSdlGL3_Init(windowPtr);
  ImGui::StyleColorsDark();

  _windowPtr = windowPtr;
}

void GuiManager::beginFrame()
{
  GuiManager::instance()._beginFrame();
}

void GuiManager::_beginFrame()
{
  if (_windowPtr) 
    ImGui_ImplSdlGL3_NewFrame(_windowPtr);
}

void GuiManager::endFrame()
{
  ImGui::Render();
  ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
}


}