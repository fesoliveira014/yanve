#include <scene/scenenode.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/gtx/quaternion.hpp>
#include <utils/logger.h>
#include <math/angle.h>
#include <utils/random.h>

namespace yanve::scene
{

SceneNode::SceneNode() :
  _translation{},
  _rotation{},
  _scale{},
  _relativeTransform{},
  _absoluteTransform{},
  _name{},
  _parent{ nullptr },
  _children{},
  _dirty{ false },
  _transformed{ false }
{

}

SceneNode::SceneNode(SceneNodePtr parent) :
  SceneNode()
{
  _parent = parent;
}

SceneNode::~SceneNode() 
{
  _children.clear();
  _parent = nullptr;
}

SceneNode& SceneNode::translate(const glm::vec3& translation)
{
  _translation = translation;
  
  markDirty();

  return *this;
}

SceneNode& SceneNode::rotate(float angle, const glm::vec3& basis)
{
  glm::vec3 axis = basis * glm::sin(angle * 0.5f);
  _rotation = glm::quat(glm::cos(angle * 0.5f), axis);
  
  markDirty();
  
  return *this;
}

SceneNode& SceneNode::rotateX(float angle) 
{
  return rotate(angle, glm::vec3(1, 0, 0));
}

SceneNode& SceneNode::rotateY(float angle) 
{
  return rotate(angle, glm::vec3(0, 1, 0));
}

SceneNode& SceneNode::rotateZ(float angle) 
{
  return rotate(angle, glm::vec3(0, 0, 1));
}

SceneNode& SceneNode::scale(const glm::vec3& factor)
{
  _scale = factor;
  
  markDirty();

  return *this;
}

//SceneNode& SceneNode::setTransformation(const glm::mat4& transformation)
//{
//  glm::vec3 skew;
//  glm::vec4 pers;
//  glm::decompose(transformation, _scale, _rotation, _translation, skew, pers);
//
//  markDirty();
//
//  return *this;
//}

void SceneNode::doTransform()
{
  //glm::mat4 translation = glm::translate(glm::mat4{}, _translation);
  //glm::mat4 rotation = glm::toMat4(_rotation);
  glm::mat4 scaling = glm::scale(glm::mat4{}, _scale);

  glm::mat3 rotation = glm::toMat3(_rotation);
  glm::mat4 rotationTranslation{ glm::vec4{ rotation[0], 0}, 
                                 glm::vec4{ rotation[1], 0}, 
                                 glm::vec4{ rotation[2], 0}, 
                                 glm::vec4{_translation, 1} };

  _relativeTransform = rotationTranslation * scaling;
  _transformed = true;
}

void SceneNode::addChild(SceneNode* node)
{
  if (!isAttachable())
    return;

  SceneNodePtr newNode{ node };
  newNode->_id = utils::RandomGenerator::getNextUInt64();
  _children.push_back(std::move(newNode));
}

bool SceneNode::removeChild(const SceneNodePtr node)
{
  bool removed = false;
  auto it = std::find(_children.begin(), _children.end(), node);
  if (it != _children.end()) {
    _children.erase(it);
    removed = true;
  }
  else {
    for (auto& child : _children) {
      if (child->removeChild(node)) {
        removed = true;
        break;
      }
    }
  }
  return removed;
}

void SceneNode::updateTree()
{
  if (!_dirty) return;

  if (!_transformed)
    doTransform();

  if (_parent != nullptr) {
    auto& parent = *_parent;
    _absoluteTransform = parent._absoluteTransform * _relativeTransform;
  }
  else {
    _absoluteTransform = _relativeTransform;
  }

  onPostUpdate();

  _dirty = false;
  _transformed = false;

  for (auto& child : _children) {
    if (child != nullptr)
      child->updateTree();
    else
      LogError("SceneNode::updateTree", "null child found, this should be unreachable");
  }
  onFinishedUpdate();
}

void SceneNode::markDirty()
{
  _dirty = true;

  std::optional<SceneNodeRef> nodeRef = _parent;
  while (nodeRef.has_value()) {
    auto& node = nodeRef.value().get();
    node._dirty = true;
    nodeRef = node._parent;
  }

  markChildrenDirty();
}

void SceneNode::markChildrenDirty()
{
  for (auto& childRef : _children) {
    auto& child = childRef.get();
    if (!child._dirty) {
      child._dirty = true;
      child.markChildrenDirty();
    }
  }
}



}