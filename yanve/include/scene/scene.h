#pragma once

#include <common.h>
#include <scene/scenenode.h>

namespace yanve::scene {

struct YANVE_API SceneData : SceneNodeData
{
  SceneData(const std::string& name) : SceneNodeData(SceneNodeType::Undefined, name) {}
};

class Scene : public SceneNode
{
public:
  explicit Scene(const SceneData& data) : SceneNode { data } {}

  ~Scene() {};

  bool isScene() override { return true; }
  bool isRenderable() override { return false; }

private:
};

}