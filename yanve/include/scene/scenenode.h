#pragma once

#include <common.h>
#include <glm/gtc/quaternion.hpp>

#include <math/aabb.h>
#include <math/frustum.h>

namespace yanve::scene
{
class SceneNode
{
public:
  using SceneNodePtr = std::shared_ptr<SceneNode>;

  explicit SceneNode();
  explicit SceneNode(SceneNodePtr parent);

  SceneNode(const SceneNode&) = delete;
  SceneNode(SceneNode&&) = delete;

  virtual ~SceneNode();

  SceneNode& translate(const glm::vec3& translation);
  SceneNode& rotate(float angle, const glm::vec3& basis);
  SceneNode& rotateX(float angle);
  SceneNode& rotateY(float angle);
  SceneNode& rotateZ(float angle);
  SceneNode& scale(const glm::vec3& factor);
  //SceneNode& setTransformation(const glm::mat4& transformation);

  glm::mat4 transform() { if (_transformed) doTransform(); return _relativeTransform; }
  // whenever node is const, the node should not have been modified, so no update needed
  glm::mat4 transform() const { return _relativeTransform; } 
  glm::vec3 translation() const { return _translation; }
  glm::quat rotation() const { return _rotation; }
  glm::vec3 scale() const { return _scale; }

  std::string name() const { return _name; }
  SceneNode& setName(std::string name) { _name = name; }

  SceneNodePtr parent() const { return _parent; }
  std::list<SceneNodePtr>& children() { return _children; }

  void addChild(SceneNode* node);
  bool removeChild(const SceneNodePtr child);

  void updateTree();

  virtual bool isScene() { return false; }
  virtual bool isAttachable() { return true; }

protected:
  void doTransform();
  void markDirty();
  void markChildrenDirty();

  virtual void onPostUpdate() {}
  virtual void onFinishedUpdate() {}
  virtual void onAttach(SceneNode& parent) {}
  virtual void onDetach(SceneNode& parent) {}
  virtual bool canAttach(SceneNode& parent) const { return true; }

protected:
  glm::vec3 _translation;
  glm::quat _rotation;
  glm::vec3 _scale;
  glm::mat4 _relativeTransform, _absoluteTransform;

  std::string _name;

  SceneNodePtr _parent;
  std::list<SceneNodePtr> _children;

  bool _dirty;
  bool _transformed;

  size_t _id;
};

}