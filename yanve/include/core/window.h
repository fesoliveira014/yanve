#pragma once

#include <common.h>
#include <SDL2/SDL.h>

namespace yanve
{

class YANVE_API Window
{
public:
  Window(std::string title, int width, int height);
  ~Window();

  void clear();
  void update();
  void display();

  void setTitle(std::string title);
  std::string getTitle();

private:
  SDL_Window* _window;
  SDL_GLContext _context;

  std::string _title;
};

}