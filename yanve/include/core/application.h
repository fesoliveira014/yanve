#pragma once

#include <common.h>

namespace yanve
{
class Application
{
public:
  Application() {};
  virtual ~Application() {};

  virtual void initialize() = 0;
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void shutdown() = 0;
};
}