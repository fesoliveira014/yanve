#pragma once

#include <cstdint>

namespace yanve
{

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

using byte = u8;
using uint = u32;
using ulong = u64;
using real = f32;

using Rectangle2D = std::pair<glm::vec2, glm::vec2>;
using Rectangle2Di = std::pair<glm::ivec2, glm::ivec2>;
using Rectangle2Du = std::pair<glm::uvec2, glm::uvec2>;

}