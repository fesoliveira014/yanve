#pragma once

#include <common.h>
#include <scene/scenenode.h>

namespace yanve::scene
{

const int RootNode = 1;

struct YANVE_API RenderQueueItem
{
  SceneNode* node;
  int sortKey;
};

class  SceneManager
{
public:
  ~SceneManager();

  void YANVE_API updateNodes();
  void YANVE_API updateQueues();

  size_t YANVE_API addNode(SceneNode* node, SceneNode& parent);
  void YANVE_API removeNode(SceneNode& node);
  bool YANVE_API relocateNode(SceneNode& node, SceneNode& parent);

  YANVE_API SceneNode* resolveNodeID(size_t id) const
  {
    return (id != 0 && id - 1 < _nodes.size() ? _nodes[id - 1] : nullptr);
  }

  YANVE_API SceneNode& getRootNode() { return *_nodes[0]; }
  YANVE_API std::vector<RenderQueueItem>& getRenderQueue() { return _renderQueue; }

public:
  YANVE_API static SceneManager& getInstance();

protected:
  SceneManager();

  size_t newNode(const SceneNodeData& data, SceneNode* parent);
  void removeNodeRec(SceneNode& node);

protected:
  std::vector<SceneNode*> _nodes;
  std::vector<size_t> _freeSlots;
  std::vector<SceneNode*> _renderableNodes;
  std::vector<size_t> _freeRenderableSlots;
  std::vector<SceneNode*> _lightQueue;
  std::vector<RenderQueueItem> _renderQueue;
};
}