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

static const std::string LOG_TAG = "SceneNode::";

SceneNode::SceneNode(const SceneNodeData& data) :
  _translation{data.translation},
  _rotation{ data.rotation },
  _scale{ data.scale },
  _name{ data.name },
  _type{ SceneNodeType::Undefined },
  _parent{ nullptr },
  _dirty{ false },
  _transformed{ false }
{
  LogInfo(LOG_TAG + __func__, "SceneNode \"%s\" {", _name);
  LogInfo(LOG_TAG + __func__, "  translation: {%.02f, %.02f, %.02f},", _translation.x, _translation.y, _translation.z);
  LogInfo(LOG_TAG + __func__, "  rotation:    {%.02f, %.02f, %.02f, %.02f},", _rotation.x, _rotation.y, _rotation.z, _rotation.w);
  LogInfo(LOG_TAG + __func__, "  scale:       {%.02f, %.02f, %.02f},", _translation.x, _translation.y, _translation.z);
  LogInfo(LOG_TAG + __func__, "}", _name);

  doTransform();
}

SceneNode::~SceneNode() 
{
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
  _rotation = math::eulerToQuat(angles);

  markDirty();

  return *this;
}

SceneNode& SceneNode::rotate(float angle, const glm::vec3& basis)
{
  glm::vec3 axis = glm::normalize(basis) * glm::sin(angle * 0.5f);
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
  glm::mat4 translation = glm::translate(glm::mat4{1}, _translation);
  glm::mat4 rotation = glm::toMat4(_rotation);
  glm::mat4 scaling = glm::scale(glm::mat4{1.0}, _scale);

  //glm::mat3 rotation = glm::toMat3(_rotation);
  //glm::mat4 rotationTranslation{ glm::vec4{ rotation[0], 0}, 
  //                               glm::vec4{ rotation[1], 0}, 
  //                               glm::vec4{ rotation[2], 0}, 
  //                               glm::vec4{_translation, 1} };

  _relativeTransform = scaling;
  _relativeTransform = rotation * _relativeTransform;
  _relativeTransform = translation * _relativeTransform;
  //_relativeTransform = rotationTranslation * _relativeTransform;
  _transformed = true;
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

std::string nodeTypeStr(SceneNodeType type)
{
  std::string typeStr = "";
  switch (type) {
  case SceneNodeType::Undefined:
    typeStr = "Undefined";
    break;
  case SceneNodeType::Mesh:
    typeStr = "Mesh";
    break;
  //case SceneNodeType::Model:
    //typeStr = "Model";
    //break;
  //case SceneNodeType::Light:
    //typeStr = "Light";
    //break;
  case SceneNodeType::Camera:
    typeStr = "Camera";
    break;
  //case SceneNodeType::Emmiter:
    //typeStr = "Emmiter";
    //break;
  //case SceneNodeType::Compute:
    //typeStr = "Compute";
    //break;
  default:
    LogError(__func__, "non-existent scene node type %d", (int)type);
    break;
  }

  return typeStr;
}

}