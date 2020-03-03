#pragma once

#include <common.h>
#include <scene/scenenode.h>

namespace yanve::scene
{

struct RenderQueueItem
{
  SceneNode* node;
  int sortKey;
};

class SceneManager
{
//public:
//  SceneManager();
//  ~SceneManager();
//
//  void updateNodes();
//  void updateQueues();
//
//  size_t addNode(SceneNode* node, SceneNode& parent);
//  bool removeNode(SceneNode& node);
//  bool relocateNode(SceneNode& node, SceneNode& parent);
//
//  SceneNode* resolveNodeID(size_t id) const
//  {
//    return (id != 0 && id - 1 < _nodes.size() ? _nodes[id - 1] : nullptr);
//  }
//
//protected:
//  std::vector<SceneNode*> _nodes;
//  std::vector<size_t> _freeSlots;
//  std::vector<SceneNode*> _renderableNodes;
//  std::vector<size_t> _freeRenderableSlots;
//  std::vector<SceneNode*> _lightQueue;
//  std::vector<RenderQueueItem> _renderQueue;
};
}