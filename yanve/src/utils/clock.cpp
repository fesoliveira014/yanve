#include <utils/clock.h>

namespace yanve::utils
{

Clock::Clock() :
  _time{clock_t::now()}
{

}

void Clock::reset()
{
  _time = clock_t::now();
}

double Clock::elapsed() const
{
  return std::chrono::duration_cast<second_t>(clock_t::now() - _time).count();
}

}