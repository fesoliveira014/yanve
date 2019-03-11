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
#include <sstream>
#include <algorithm>
#include <memory>

#include <glm/glm.hpp>