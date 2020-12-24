#include <scene/scenemanager.h>
#include <scene/scene.h>
#include <scene/meshnode.h>
#include <scene/camera.h>

#include <utils/logger.h>

namespace yanve::scene
{

static const std::string LOG_TAG = "SceneManager::";

SceneManager& SceneManager::getInstance()
{
  static SceneManager instance;
  return instance;
}

SceneManager::SceneManager()
{
  Scene* root = new Scene(SceneData{ "root" });
  root->_id = RootNode;
  
  root->_children.reserve(100); // todo: move this to some constant definition
  _nodes.reserve(100);

  _nodes.push_back(root);
}

SceneManager::~SceneManager()
{
  for (auto& node : _nodes) {
    delete node;
    node = nullptr;
  }
}

void SceneManager::updateNodes()
{
  getRootNode().update();
}

void SceneManager::updateQueues()
{
  _renderQueue.clear();

  for (auto node : _nodes) {
    if (node->isRenderable()) {
      _renderQueue.push_back({ node, 0 });
    }
  }
}

size_t SceneManager::addNode(SceneNode* node, SceneNode& parent) 
{
  // todo: change isAttachable to canAttach and check if a 
  // given node type can attach to that parent node type
  // or maybe change it to receive node type as a parameter?
  if (!parent.isAttachable()) {
    LogError(LOG_TAG + __func__, "cant attach to node of type %s to parent of type %s", nodeTypeStr(node->type()), nodeTypeStr(parent.type()));
    delete node; node = nullptr;
    return 0;
  }

  node->_parent = &parent;
  parent._children.push_back(node);
  node->onAttach(parent);
  node->markDirty();

  if (!_freeSlots.empty()) {
    size_t slot = _freeSlots.back();
    if (_nodes[slot] != nullptr) {
      LogError(LOG_TAG + __func__, "can't add node to freeslot %ud, slot %ud is already taken", slot, slot);
      delete node; node = nullptr;
      return 0;
    }

    _freeSlots.pop_back();
    node->_id = slot + 1;
    _nodes[slot] = node;
    return slot + 1;
  }
  else {
    _nodes.push_back(node);
    node->_id = _nodes.size();
    return node->_id;
  }
}

void SceneManager::removeNode(SceneNode& node)
{
  SceneNode* parent = node._parent;
  SceneNode* nodePtr = &node;

  removeNodeRec(node);

  if (parent != nullptr) {
    for (size_t i = 0; i < parent->_children.size(); ++i) {
      if (parent->_children[i] = nodePtr) {
        parent->_children.erase(parent->_children.begin() + i);
        break;
      }
    }
    parent->markDirty();
  }
  else {
    node._children.clear();
    node.markDirty();
  }
}

void SceneManager::removeNodeRec(SceneNode& node)
{
  size_t id = node._id;
  
  if (id != RootNode) node.onDetach(*node._parent);

  for (size_t i = 0; i < node._children.size(); ++i) {
    removeNodeRec(*node._children[i]);
  }

  if (id != RootNode) {
    _freeSlots.push_back(id - 1);
  }
}

bool SceneManager::relocateNode(SceneNode& node, SceneNode& parent)
{
  if (node._id == RootNode) return false;

  if (!parent.isAttachable()) {
    LogError(LOG_TAG + __func__, "cant attach to node of type %s to parent of type %s", nodeTypeStr(node.type()), nodeTypeStr(parent.type()));
    return false;
  }

  node.onDetach(*node._parent);
  for (size_t i = 0; i < node._parent->_children.size(); ++i) {
    if (node._parent->_children[i] == &node) {
      node._parent->_children.erase(node._parent->_children.begin() + i);
      break;
    }
  }

  node._parent->markDirty();

  parent._children.push_back(&node);
  node._parent = &parent;
  node.onAttach(parent);

  parent.markDirty();
}

size_t SceneManager::newNode(const SceneNodeData& data, SceneNode* parent)
{
  if (parent == nullptr) return 0;

  SceneNode* node = nullptr;

  switch (node->_type) {
  case SceneNodeType::Mesh:
    node = new MeshNode(*(MeshNodeData*)&data);
    break;
  case SceneNodeType::Camera:
    node = new Camera(*(CameraData*)&data);
    break;
  default:
    break;
  }

  if (node == nullptr) return 0;

  
}


}