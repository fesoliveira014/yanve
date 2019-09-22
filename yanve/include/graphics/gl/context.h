#pragma once

#include <common.h>

namespace yanve::gl
{

class Context
{
public:
  static YANVE_API Context& instance();
};

}