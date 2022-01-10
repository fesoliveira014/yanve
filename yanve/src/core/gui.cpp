#include <core/gui.h>
#include <utils/logger.h>

namespace yanve
{
static const std::string LOG_TAG = "GuiManager::";

GuiManager::GuiManager() : _windowPtr{ nullptr } {}

GuiManager::~GuiManager()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  _windowPtr = nullptr;
}

void GuiManager::setup(SDL_Window* windowPtr, void* context)
{
  instance()._setup(windowPtr, context);
}

void GuiManager::_setup(SDL_Window* windowPtr, void* context)
{
  if (windowPtr == nullptr) {
    LogError(LOG_TAG + __func__, "Could not initialize gui because it received a null pointer.");
    return;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForOpenGL(windowPtr, context);
  ImGui_ImplOpenGL3_Init();
  io.Fonts->AddFontDefault();

  _windowPtr = windowPtr;
}

void GuiManager::beginFrame()
{
  GuiManager::instance()._beginFrame();
}

void GuiManager::_beginFrame()
{
  if (_windowPtr) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(_windowPtr);
    ImGui::NewFrame();
  }
}

void GuiManager::render()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}