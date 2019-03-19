#pragma once

#include <common.h>
//#include <core/window.h>

namespace yanve
{
class YANVE_API Application
{
public:
  Application() {};
  virtual ~Application() {};

  virtual void initialize() = 0;
  virtual void update() = 0;
  virtual void updateGui() = 0;
  virtual void render() = 0;
  virtual int run() = 0;
  virtual void shutdown() = 0;

private:
};
}