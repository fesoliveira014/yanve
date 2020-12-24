#pragma once

#include <common.h>
#include <glm/gtc/quaternion.hpp>

#include <math/aabb.h>
#include <math/frustum.h>

namespace yanve::scene
{

enum class YANVE_API SceneNodeType
{
  Undefined = 0,
  Mesh,
  //Model,
  //Light,
  Camera,
  //Emmiter,
  //Compute
};

struct YANVE_API SceneNodeData
{
  glm::vec3 translation = glm::vec3{ 0 };
  glm::quat rotation = glm::quat{ 1, 0, 0, 0 };
  glm::vec3 scale;

  std::string name;
  SceneNodeType type;

  //std::vector<SceneNodeData*> children;

  SceneNodeData(SceneNodeType type, const std::string& name) :
    type{type},
    name{ name },
    scale{ glm::vec3{1,1,1} }
  {

  }

  virtual ~SceneNodeData()
  {
    /*for (int i = 0; i < children.size(); ++i) {
      if (children[i] != nullptr) delete children[i];
    }*/
  }
};

class YANVE_API SceneNode
{
public:
  //using SceneNode* = std::shared_ptr<SceneNode>;

  /*explicit SceneNode();
  explicit SceneNode(SceneNode* parent);*/
  SceneNode(const SceneNodeData& data);

  SceneNode(const SceneNode&) = delete;
  SceneNode(SceneNode&&) = delete;

  virtual ~SceneNode();

  SceneNode& translate(const glm::vec3& translation);
  SceneNode& rotate(const glm::vec3& euler);
  SceneNode& rotate(float angle, const glm::vec3& basis);
  SceneNode& rotateX(float angle);
  SceneNode& rotateY(float angle);
  SceneNode& rotateZ(float angle);
  SceneNode& scale(const glm::vec3& factor);
  //SceneNode& setTransformation(const glm::mat4& transformation);

  glm::mat4 transform() { if (!_transformed) doTransform(); return _relativeTransform; }
  glm::mat4 transform() const { return _relativeTransform; } 
  glm::mat4 absTransform() const { return _absoluteTransform; }
  glm::vec3 translation() const { return _translation; }
  glm::quat rotation() const { return _rotation; }
  glm::vec3 scale() const { return _scale; }

  std::string name() const { return _name; }
  SceneNode& setName(std::string name) { _name = name; return *this; }

  SceneNode* parent() const { return _parent; }
  std::vector<SceneNode*>& children() { return _children; }

  SceneNodeType type() const { return _type; }

  void update();

  virtual bool isScene() { return false; }
  virtual bool isAttachable() { return true; }
  virtual bool isRenderable() { return _renderable; }
  
  void markDirty();
  void markChildrenDirty();

protected:
  SceneNode() = delete;
  void doTransform();
  

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
  SceneNodeType _type;

  SceneNode* _parent;
  std::vector<SceneNode*> _children;

  math::AABB _bBox;

  bool _dirty;
  bool _transformed;
  bool _renderable;

  size_t _id;

  friend class SceneManager;
};

YANVE_API std::string nodeTypeStr(SceneNodeType type);

}