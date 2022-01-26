#pragma once

#include <common.h>
#include <cmath>
#include <math/constants.h>

namespace yanve::math
{
YANVE_API inline glm::vec3 faceNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
  glm::vec3 v1 = a - b;
  glm::vec3 v2 = c - a;

  return glm::cross(v2, v1);
}

YANVE_API inline void polygonNormals(
  const std::vector<glm::vec3>& vertices,
  const std::vector<uint>& indices,
  std::vector<glm::vec3>& normals)
{
  for (uint i = 0; i < vertices.size(); ++i) {
    normals.push_back(glm::vec3(0.0));
  }

  for (uint i = 0; i < indices.size() / 3; ++i) {
    uint a = indices[i * 3];
    uint b = indices[i * 3 + 1];
    uint c = indices[i * 3 + 2];

    glm::vec3 normal = faceNormal(vertices[a], vertices[b], vertices[c]);

    normals[a] += normal;
    normals[b] += normal;
    normals[c] += normal;
  }

  for (auto& normal : normals) {
    normal = glm::normalize(normal);
  }
}

}