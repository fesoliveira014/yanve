#pragma once

#include <common.h>
#include <scene/scenenode.h>

namespace yanve::scene
{

class YANVE_API Camera : public SceneNode
{
public:
  explicit Camera();
  explicit Camera(SceneNode* parent);

  bool isAttachable() override { return false; }

  void setViewParameters(float fov, float aspect, float nearPlane, float farPlane);
  void setProjectionMatrix(const glm::mat4& projection);
  void lookAt(const glm::vec3& position);

  const math::Frustum& getFrustum() const { return _frustum; }
  const glm::mat4& getViewMatrix() const { return _view; }
  const glm::mat4& getProjectionMatrix() const { return _projection; }
  const glm::vec3& getAbsolutePosition() const { return _absPosition; }

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