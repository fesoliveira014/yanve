#pragma once

#include <common.h>
#include <core/window.h>
#include <graphics/gl/context.h>
#include <graphics/gl/defaultframebuffer.h>

namespace yanve
{

class YANVE_API Application
{
public:
  // TODO: create "config" class to encapsulate initial state
  Application(std::string title, int width, int height);
  virtual ~Application() {};

  virtual void initialize() = 0;
  virtual void update() = 0;
  virtual void updateGui() = 0;
  virtual void render() = 0;
  virtual int run() = 0;
  virtual void shutdown() = 0;

protected:
  Window window;
  gl::DefaultFramebuffer defaultFramebuffer;

private:
  gl::Context context;
};
}