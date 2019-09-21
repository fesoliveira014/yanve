#pragma once

#ifdef _WIN32
#ifdef YANVE_EXPORTS
#define YANVE_API __declspec(dllexport)
#else
#define YANVE_API __declspec(dllimport)
#endif
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <sstream>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <optional>

#include <GL/glew.h>

//#ifndef GLEW_STATIC
//#define GLEW_STATIC
//#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/types.h>