#pragma once

#include <common.h>
#include <chrono>

namespace yanve::utils 
{

class Clock {
  typedef std::chrono::high_resolution_clock clock_t;
  typedef std::chrono::duration<double, std::ratio<1>> second_t;
  std::chrono::time_point<clock_t> _time;

public:
  YANVE_API Clock();

  void YANVE_API reset();
  double YANVE_API elapsed() const;
};

}