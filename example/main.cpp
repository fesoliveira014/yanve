#include <yanve.h>
#include <iostream>

int main(int argc, char* argv[])
{
  yanve::Window window{ "hello", 1024, 800 };

  auto& input = yanve::InputManager::instance();
  while (!input.quit()) {
    input.update();
    window.update();

    window.clear();
    window.display();
  }

  return 0;
}