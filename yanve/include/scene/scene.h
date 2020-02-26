#pragma once

#include <common.h>
#include <scene/scenenode.h>

namespace yanve::scene {

class Scene : public SceneNode
{
public:
  explicit Scene() = default;

  ~Scene();

  bool isScene() override { return true; }
};

}