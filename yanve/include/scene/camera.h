#pragma once

#include <common.h>
#include <scene/scenenode.h>

namespace yanve::scene
{

struct YANVE_API CameraData : SceneNodeData
{
  float fov, aspect, nearPlane, farPlane;
  bool orthographic;

  CameraData(const std::string& name) :
    SceneNodeData{SceneNodeType::Camera, name},
    fov{45.0}, aspect{4.0f/3.0f}, nearPlane{0.1f}, farPlane{100.0f}, orthographic{ false } 
  {
  }
};

class YANVE_API Camera : public SceneNode
{
public:
  explicit Camera(const CameraData& data);

  bool isAttachable() override { return false; }

  void setViewParameters(float fov, float aspect, float nearPlane, float farPlane);
  void setProjectionMatrix(const glm::mat4& projection);
  void lookAt(const glm::vec3& position);

  const math::Frustum& frustum() const { return _frustum; }
  const glm::mat4& view() const { return _view; }
  const glm::mat4& projection() const { return _projection; }
  const glm::vec3& absolutePosition() const { return _absPosition; }
  
  const glm::vec3& right() const { 
    return glm::conjugate(_rotation) * glm::vec3(1.0f, 0.0f, 0.0f);
  }
  
  const glm::vec3& up() const { 
    return glm::conjugate(_rotation) * glm::vec3(0.0f, 1.0f, 0.0f);
  }

  const glm::vec3& forward() const { 
    return glm::conjugate(_rotation) * glm::vec3(0.0, 0.0f, -1.0f);
  }

protected:
  void onPostUpdate() override;

  glm::mat4 _view, _projection;
  glm::vec3 _absPosition;

  float _frustumLeft, _frustumRight, _frustumTop,
        _frustumBottom, _frustumNear, _frustumFar;

  float _fov, _aspect, _near, _far;

  math::Frustum _frustum;

  bool _orthographic = false;
  bool _manualProjection;
};

}