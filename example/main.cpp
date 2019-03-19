#include <yanve.h>
#include <iostream>

class TestApp : public yanve::Application
{
public:
  TestApp(std::string name) : window{name, 1024, 800}, running { true } {}
  ~TestApp() {}

  void initialize() override {}

  void update() override
  {
    auto& input = yanve::InputManager::instance();
    input.update();
    window.clear();
    window.update();

    updateGui();

    running = !input.quit();
  }
  
  void updateGui() override
  {
    yanve::GuiManager::beginFrame();
    {
      ImGui::Begin("hello");
      ImGui::Text("Hello, world!");
      ImGui::End();
    }
    yanve::GuiManager::endFrame();
  }

  void render() override
  {
    window.display();
  }

  int run() override
  {
    initialize();
    while (running) {
      update();
      render();
    }
    shutdown();
    return 0; // add error code variable maybe?
  }

  void shutdown() override {}
  bool isRunning() { return running; }

protected:
  yanve::Window window;
  bool running;
};

int main(int argc, char* argv[])
{
  TestApp app{"test"};
  return app.run();
}