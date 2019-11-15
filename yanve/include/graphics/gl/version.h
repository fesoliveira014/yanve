#pragma once

#include <common.h>

namespace yanve::gl
{
enum class Version : i32
{
  None = 0xFFFF,                  /**< Unspecified */
  GL210 = 210,                    /**< OpenGL 2.1 / GLSL 1.20 */
  GL300 = 300,                    /**< OpenGL 3.0 / GLSL 1.30 */
  GL310 = 310,                    /**< OpenGL 3.1 / GLSL 1.40 */
  GL320 = 320,                    /**< OpenGL 3.2 / GLSL 1.50 */
  GL330 = 330,                    /**< OpenGL 3.3, GLSL 3.30 */
  GL400 = 400,                    /**< OpenGL 4.0, GLSL 4.00 */
  GL410 = 410,                    /**< OpenGL 4.1, GLSL 4.10 */
  GL420 = 420,                    /**< OpenGL 4.2, GLSL 4.20 */
  GL430 = 430,                    /**< OpenGL 4.3, GLSL 4.30 */
  GL440 = 440,                    /**< OpenGL 4.4, GLSL 4.40 */
  GL450 = 450,                    /**< OpenGL 4.5, GLSL 4.50 */
  GL460 = 460,                    /**< OpenGL 4.6, GLSL 4.60 */
};

constexpr Version version(i32 major, i32 minor)
{
  return Version(100 * major + 10 * minor);
}

inline std::pair<i32, i32> version(Version version)
{
  const i32 v = i32(version);
  return { v / 100, (v % 100) / 10 };
}
}