#pragma once

#include <common.h>
#include <random>
#include <limits>

namespace yanve::utils
{

class YANVE_API RandomGenerator
{
public:
  static float getNext() { return instance().getNextInternal(); }

  static i8  getNextInt8()  { return instance().getNextInt8Internal(); }
  static i16 getNextInt16() { return instance().getNextInt16Internal(); }
  static i32 getNextInt32() { return instance().getNextInt32Internal(); }
  static i64 getNextInt64() { return instance().getNextInt64Internal(); }

  static i8  getNextUInt8()  { return instance().getNextUInt8Internal(); }
  static i16 getNextUInt16() { return instance().getNextUInt16Internal(); }
  static i32 getNextUInt32() { return instance().getNextUInt32Internal(); }
  static i64 getNextUInt64() { return instance().getNextUInt64Internal(); }

  static float getNextFloat() { return instance().getNextFloat(); }
  static double getNextDouble() { return instance().getNextDouble(); }

  

private:
  explicit RandomGenerator() :
    _rd{},
    _engine{_rd()},
    _i16_dist{ std::numeric_limits<i16>::min(), std::numeric_limits<i16>::max() },
    _i32_dist{ std::numeric_limits<i32>::min(), std::numeric_limits<i32>::max() },
    _i64_dist{ std::numeric_limits<i64>::min(), std::numeric_limits<i64>::max() },
    _u16_dist{ std::numeric_limits<u16>::min(), std::numeric_limits<u16>::max() },
    _u32_dist{ std::numeric_limits<u32>::min(), std::numeric_limits<u32>::max() },
    _u64_dist{ std::numeric_limits<u64>::min(), std::numeric_limits<u64>::max() },
    _float_dist{ std::numeric_limits<float>::min(), std::numeric_limits<float>::max() },
    _double_dist{ std::numeric_limits<double>::min(), std::numeric_limits<double>::max() },
    _fraction_dist{ 0.0f, 1.0f }
  {
    
  }

  static RandomGenerator& instance()
  {
    static RandomGenerator generator;
    return generator;
  }

  float getNextInternal() { return _fraction_dist(_engine); }

  i8  getNextInt8Internal() { return (i8)_i16_dist(_engine); }
  i16 getNextInt16Internal() { return _i16_dist(_engine); }
  i32 getNextInt32Internal() { return _i32_dist(_engine); }
  i64 getNextInt64Internal() { return _i64_dist(_engine); }

  u8  getNextUInt8Internal() { return (u8)_u16_dist(_engine); }
  u16 getNextUInt16Internal() { return _u16_dist(_engine); }
  u32 getNextUInt32Internal() { return _u32_dist(_engine); }
  u64 getNextUInt64Internal() { return _u64_dist(_engine); }

  float getNextFloatInternal() { return _float_dist(_engine); }
  double getNextDoubleInternal() { return _double_dist(_engine); }

  std::random_device _rd;
  std::default_random_engine _engine;
  std::uniform_int_distribution<i16> _i16_dist;
  std::uniform_int_distribution<i32> _i32_dist;
  std::uniform_int_distribution<i64> _i64_dist;

  std::uniform_int_distribution<u16> _u16_dist;
  std::uniform_int_distribution<u32> _u32_dist;
  std::uniform_int_distribution<u64> _u64_dist;

  std::uniform_real_distribution<float> _float_dist;
  std::uniform_real_distribution<double> _double_dist;

  std::uniform_real_distribution<float> _fraction_dist;
};

}