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
  _translation{glm::vec3(0)},
  _rotation{ glm::quat(1, 0, 0, 0) },
  _scale{ glm::vec3{1, 1, 1} },
  _relativeTransform{ glm::mat4(1) },
  _absoluteTransform{ glm::mat4(1) },
  _name{},
  _parent{ nullptr },
  _children{},
  _dirty{ false },
  _transformed{ false }
{

}

SceneNode::SceneNode(SceneNode* parent) :
  SceneNode()
{
  _parent = parent;
  _parent->addChild(this);
}

SceneNode::~SceneNode() 
{
  for (auto& child : _children) {
    delete child;
  }

  _children.clear();
  _parent = nullptr;
}

SceneNode& SceneNode::translate(const glm::vec3& translation)
{
  _translation = translation;
  //glm::mat4 translationM = glm::translate(glm::mat4{ 1 }, translation);
  //_relativeTransform = translationM * _relativeTransform;
  
  markDirty();

  return *this;
}

SceneNode& SceneNode::rotate(const glm::vec3& angles) 
{
  glm::vec3 euler = angles * math::pi_over_180<float>;
  glm::quat roll{ cosf(euler.x * 0.5f), sinf(euler.x * 0.5f), 0, 0 };
  glm::quat pitch{ cosf(euler.y * 0.5f), 0, sinf(euler.y * 0.5f), 0 };
  glm::quat yaw{ cosf(euler.z * 0.5f), 0, 0, sinf(euler.z * 0.5f) };

  _rotation = pitch * roll * yaw;

  markDirty();

  return *this;
}

SceneNode& SceneNode::rotate(float angle, const glm::vec3& basis)
{
  glm::vec3 axis = basis * glm::sin(angle * 0.5f);
  _rotation = glm::quat(glm::cos(angle * 0.5f), axis);

  //_relativeTransform = glm::toMat4(_rotation) * _relativeTransform;
  
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
  //glm::mat4 scaleM = glm::scale(glm::mat4{ 1 }, _scale);
  //_relativeTransform = scaleM * _relativeTransform;

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
  glm::mat4 scaling = glm::scale(glm::mat4{1.0}, _scale);

  glm::mat3 rotation = glm::toMat3(_rotation);
  glm::mat4 rotationTranslation{ glm::vec4{ rotation[0], 0}, 
                                 glm::vec4{ rotation[1], 0}, 
                                 glm::vec4{ rotation[2], 0}, 
                                 glm::vec4{_translation, 1} };

  _relativeTransform = scaling;
  _relativeTransform = rotationTranslation * _relativeTransform;
  _transformed = true;
}

void SceneNode::addChild(SceneNode* node)
{
  if (!isAttachable())
    return;

  SceneNode* newNode{ node };
  newNode->_id = utils::RandomGenerator::getNextUInt64();
  _children.push_back(std::move(newNode));
}

bool SceneNode::removeChild(const SceneNode* node)
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

void SceneNode::update()
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
      child->update();
    else
      LogError("SceneNode::update", "null child found, this should be unreachable");
  }
  onFinishedUpdate();
}

void SceneNode::markDirty()
{
  _dirty = true;
  //_transformed = true;

  SceneNode* node = _parent;
  while (node != nullptr) {
    node->_dirty = true;
    //node->_transformed = true;
    node = node->_parent;
  }

  markChildrenDirty();
}

void SceneNode::markChildrenDirty()
{
  for (auto& child : _children) {
    if (!child->_dirty) {
      child->_dirty = true;
      child->_transformed = true;
      child->markChildrenDirty();
    }
  }
}



}