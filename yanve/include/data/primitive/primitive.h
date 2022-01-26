#pragma once

#include <common.h>
#include <data/geometrydata.h>
#include <utils/enumset.h>
#include <math/geometry.h>
#include <utils/logger.h>

namespace yanve
{
using namespace data;

enum class PrimitiveFlag : uint
{
  Position  = 1 << 0,
  Color     = 1 << 1,
  Normal    = 1 << 2,
  Texture   = 1 << 3,
  Tangent   = 1 << 4,
  Bitangent = 1 << 5,
  NoIndex   = 1 << 6,
};

typedef EnumSet <PrimitiveFlag> PrimitiveFlags;

class YANVE_API Primitive
{
public:
  static GeometryData cube(PrimitiveFlags flags = PrimitiveFlag::Position)
  {
    GeometryData cubeGD{};

    //front
    cubeGD.vertices.push_back(glm::vec3(0.0, 0.0, 0.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 1.0, 0.0));
    cubeGD.vertices.push_back(glm::vec3(0.0, 1.0, 0.0));
    cubeGD.vertices.push_back(glm::vec3(0.0, 0.0, 0.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 0.0, 0.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 1.0, 0.0));

    // back
    cubeGD.vertices.push_back(glm::vec3(1.0, 0.0, 1.0));
    cubeGD.vertices.push_back(glm::vec3(0.0, 1.0, 1.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 1.0, 1.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 0.0, 1.0));
    cubeGD.vertices.push_back(glm::vec3(0.0, 0.0, 1.0));
    cubeGD.vertices.push_back(glm::vec3(0.0, 1.0, 1.0));

    // left
    cubeGD.vertices.push_back(glm::vec3(0.0, 0.0, 1.0));
    cubeGD.vertices.push_back(glm::vec3(0.0, 1.0, 0.0));
    cubeGD.vertices.push_back(glm::vec3(0.0, 1.0, 1.0));
    cubeGD.vertices.push_back(glm::vec3(0.0, 0.0, 1.0));
    cubeGD.vertices.push_back(glm::vec3(0.0, 0.0, 0.0));
    cubeGD.vertices.push_back(glm::vec3(0.0, 1.0, 0.0));

    // right
    cubeGD.vertices.push_back(glm::vec3(1.0, 0.0, 0.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 1.0, 1.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 1.0, 0.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 0.0, 0.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 0.0, 1.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 1.0, 1.0));

    // bottom
    cubeGD.vertices.push_back(glm::vec3(0.0, 0.0, 1.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 0.0, 0.0));
    cubeGD.vertices.push_back(glm::vec3(0.0, 0.0, 0.0));
    cubeGD.vertices.push_back(glm::vec3(0.0, 0.0, 1.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 0.0, 1.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 0.0, 0.0));

    //top
    cubeGD.vertices.push_back(glm::vec3(0.0, 1.0, 0.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 1.0, 1.0));
    cubeGD.vertices.push_back(glm::vec3(0.0, 1.0, 1.0));
    cubeGD.vertices.push_back(glm::vec3(0.0, 1.0, 0.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 1.0, 0.0));
    cubeGD.vertices.push_back(glm::vec3(1.0, 1.0, 1.0));

    if (!(flags & PrimitiveFlag::NoIndex)) {
      for (int i = 0; i < cubeGD.vertices.size(); ++i) {
        cubeGD.indices.push_back(i);
      }
    }

    if (flags & PrimitiveFlag::Color) {
      for (int i = 0; i < cubeGD.vertices.size(); ++i) {
        cubeGD.colors.push_back(glm::vec4(cubeGD.vertices[i] + glm::vec3(0.5), 1.0));
      }
    }

    if (flags & PrimitiveFlag::Normal) {
      math::polygonNormals(cubeGD.vertices, cubeGD.indices, cubeGD.normals);
    }

    if (flags & PrimitiveFlag::Texture) {
      for (int i = 0; i < 6; ++i) {
        cubeGD.uvs.push_back(glm::vec2(0.0f, 0.0f));
        cubeGD.uvs.push_back(glm::vec2(1.0f, 1.0f));
        cubeGD.uvs.push_back(glm::vec2(0.0f, 1.0f));
        cubeGD.uvs.push_back(glm::vec2(0.0f, 0.0f));
        cubeGD.uvs.push_back(glm::vec2(1.0f, 0.0f));
        cubeGD.uvs.push_back(glm::vec2(1.0f, 1.0f));
      }
    }

    if (flags & PrimitiveFlag::Tangent) {
      LogWarning(__func__, "Tangent calculation not yet implemented.");
    }

    if (flags & PrimitiveFlag::Bitangent) {
      LogWarning(__func__, "Bitangent calculation not yet implemented.");
    }

    return cubeGD;
  }

  static GeometryData skybox(PrimitiveFlags flags = PrimitiveFlag::Position)
  {
    GeometryData gd{};

    gd.vertices.push_back({ -1.0f,  1.0f, -1.0f });
    gd.vertices.push_back({ -1.0f, -1.0f, -1.0f });
    gd.vertices.push_back({ 1.0f, -1.0f, -1.0f });
    gd.vertices.push_back({ 1.0f, -1.0f, -1.0f });
    gd.vertices.push_back({ 1.0f,  1.0f, -1.0f });
    gd.vertices.push_back({ -1.0f,  1.0f, -1.0f });

    gd.vertices.push_back({ -1.0f, -1.0f,  1.0f });
    gd.vertices.push_back({ -1.0f, -1.0f, -1.0f });
    gd.vertices.push_back({ -1.0f,  1.0f, -1.0f });
    gd.vertices.push_back({ -1.0f,  1.0f, -1.0f });
    gd.vertices.push_back({ -1.0f,  1.0f,  1.0f });
    gd.vertices.push_back({ -1.0f, -1.0f,  1.0f });

    gd.vertices.push_back({ 1.0f, -1.0f, -1.0f });
    gd.vertices.push_back({ 1.0f, -1.0f,  1.0f });
    gd.vertices.push_back({ 1.0f,  1.0f,  1.0f });
    gd.vertices.push_back({ 1.0f,  1.0f,  1.0f });
    gd.vertices.push_back({ 1.0f,  1.0f, -1.0f });
    gd.vertices.push_back({ 1.0f, -1.0f, -1.0f });

    gd.vertices.push_back({ -1.0f, -1.0f,  1.0f });
    gd.vertices.push_back({ -1.0f,  1.0f,  1.0f });
    gd.vertices.push_back({ 1.0f,  1.0f,  1.0f });
    gd.vertices.push_back({ 1.0f,  1.0f,  1.0f });
    gd.vertices.push_back({ 1.0f, -1.0f,  1.0f });
    gd.vertices.push_back({ -1.0f, -1.0f,  1.0f });

    gd.vertices.push_back({ -1.0f,  1.0f, -1.0f });
    gd.vertices.push_back({ 1.0f,  1.0f, -1.0f });
    gd.vertices.push_back({ 1.0f,  1.0f,  1.0f });
    gd.vertices.push_back({ 1.0f,  1.0f,  1.0f });
    gd.vertices.push_back({ -1.0f,  1.0f,  1.0f });
    gd.vertices.push_back({ -1.0f,  1.0f, -1.0f });

    gd.vertices.push_back({ -1.0f, -1.0f, -1.0f });
    gd.vertices.push_back({ -1.0f, -1.0f,  1.0f });
    gd.vertices.push_back({ 1.0f, -1.0f, -1.0f });
    gd.vertices.push_back({ 1.0f, -1.0f, -1.0f });
    gd.vertices.push_back({ -1.0f, -1.0f,  1.0f });
    gd.vertices.push_back({ 1.0f, -1.0f,  1.0f });

    if (!(flags & PrimitiveFlag::NoIndex)) {
      LogWarning(__func__, "Skybox has no indices by default.");
    }

    if (flags & PrimitiveFlag::Color) {
      LogWarning(__func__, "Color buffer not supported for skybox.");
    }

    if (flags & PrimitiveFlag::Normal) {
      LogWarning(__func__, "Normal buffer not supported for skybox.");
    }

    if (flags & PrimitiveFlag::Texture) {
      LogWarning(__func__, "UV buffer not supported for skybox.");
    }

    if (flags & PrimitiveFlag::Tangent) {
      LogWarning(__func__, "Tangent calculation not yet implemented.");
    }

    if (flags & PrimitiveFlag::Bitangent) {
      LogWarning(__func__, "Bitangent calculation not yet implemented.");
    }

    return gd;
  }

  static GeometryData plane(PrimitiveFlags flags = PrimitiveFlag::Position)
  {
    GeometryData quadGD{};

    quadGD.vertices.push_back(glm::vec3(-1.0, 1.0, 0.0));
    quadGD.vertices.push_back(glm::vec3(-1.0, -1.0, 0.0));
    quadGD.vertices.push_back(glm::vec3(1.0, 1.0, 0.0));

    quadGD.vertices.push_back(glm::vec3(1.0, 1.0, 0.0));
    quadGD.vertices.push_back(glm::vec3(-1.0, -1.0, 0.0));
    quadGD.vertices.push_back(glm::vec3(1.0, -1.0, 0.0));

    if (!(flags & PrimitiveFlag::NoIndex)) {
      for (int i = 0; i < quadGD.vertices.size(); ++i) {
        quadGD.indices.push_back(i);
      }
    }

    if (flags & PrimitiveFlag::Color) {
      for (int i = 0; i < quadGD.vertices.size(); ++i) {
        quadGD.colors.push_back(glm::vec4(quadGD.vertices[i] + glm::vec3(0.5), 1.0));
      }
    }

    if (flags & PrimitiveFlag::Normal) {
      math::polygonNormals(quadGD.vertices, quadGD.indices, quadGD.normals);
    }

    if (flags & PrimitiveFlag::Texture) {
      quadGD.uvs.push_back(glm::vec2(0.0, 1.0));
      quadGD.uvs.push_back(glm::vec2(0.0, 0.0));
      quadGD.uvs.push_back(glm::vec2(1.0, 1.0));
      quadGD.uvs.push_back(glm::vec2(1.0, 1.0));
      quadGD.uvs.push_back(glm::vec2(0.0, 0.0));
      quadGD.uvs.push_back(glm::vec2(1.0, 0.0));
    }

    if (flags & PrimitiveFlag::Tangent) {
      LogWarning(__func__, "Tangent calculation not yet implemented.");
    }

    if (flags & PrimitiveFlag::Bitangent) {
      LogWarning(__func__, "Bitangent calculation not yet implemented.");
    }

    return quadGD;
  }

/*
  static GeometryData&& prim(PrimitiveFlags flags)
  {
    GeometryData gd{};

    // add gd.vertices here

    for (int i = 0; i < gd.vertices.size(); ++i) {
      gd.indices.push_back(i);
    }

    if (flags & PrimitiveFlag::Color) {
      for (int i = 0; i < gd.vertices.size(); ++i) {
        gd.colors.push_back(glm::vec4(gd.vertices[i] + glm::vec3(0.5), 1.0));
      }
    }

    if (flags & PrimitiveFlag::Normal) {
      math::polygonNormals(gd.vertices, gd.indices, gd.normals);
    }

    if (flags & PrimitiveFlag::Texture) {

    }

    if (flags & PrimitiveFlag::Tangent) {
      LogWarning(__func__, "Tangent calculation not yet implemented.");
    }

    if (flags & PrimitiveFlag::Bitangent) {
      LogWarning(__func__, "Bitangent calculation not yet implemented.");
    }

    return std::move(gd);
  }
*/

};

ENUMSET_OPERATORS(PrimitiveFlags)
}