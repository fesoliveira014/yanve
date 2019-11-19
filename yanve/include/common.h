#pragma once

#ifdef YANVE_NO_EXPORT
#define YANVE_API __declspec(dllimport)
#elif defined(_WIN32)
#define YANVE_API __declspec(dllexport)
#define YANVE_IMPORT __declspec(dllimport)
#define YANVE_LOCAL 
#define YANVE_THREAD_LOCAL __declspec(thread)
#else
#define YANVE_API __attribute__((visibility("default")))
#define YANVE_IMPORT __attribute__((visibility("default")))
#define YANVE_LOCAL __attribute__((visibility("hidden")))
#define YANVE_THREAD_LOCAL _thread
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
