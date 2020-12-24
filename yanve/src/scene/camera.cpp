#pragma once

#include <scene/camera.h>
#include <math/angle.h>
#include <glm/gtx/quaternion.hpp>
#include <utils/logger.h>

namespace yanve::scene
{
static const std::string LOG_TAG = "Camera::";

Camera::Camera(const CameraData& data) : 
  SceneNode(data),
  _view{},
  _projection{},
  _absPosition{},
  _frustumLeft{}, _frustumRight{},
  _frustumTop{}, _frustumBottom{},
  _frustumNear{}, _frustumFar{},
  _fov{data.fov},
  _aspect{data.aspect},
  _near{data.nearPlane},
  _far{data.farPlane},
  _frustum{},
  _orthographic{ data.orthographic },
  _manualProjection{ false }
{
  _renderable = false;
  _type = SceneNodeType::Camera;
  LogInfo(LOG_TAG + __func__, "cameraData translation: {%.2f, %.2f, %.2f}", data.translation.x, data.translation.y, data.translation.z);
  LogInfo(LOG_TAG + __func__, "camera position: {%.2f, %.2f, %.2f}", _absPosition.x, _absPosition.y, _absPosition.z);
}

void Camera::setViewParameters(float fov, float aspect, float nearPlane, float farPlane)
{
  _fov = fov;
  _aspect = aspect;
  _near = nearPlane;
  _far = farPlane;

  float ymax = _near * glm::tan(math::radians(fov));
  float xmax = ymax * _aspect;

  _frustumLeft = -xmax;
  _frustumRight = xmax;
  _frustumBottom = -ymax;
  _frustumTop = ymax;
  _frustumNear = nearPlane;
  _frustumFar = farPlane;

  _manualProjection = false;

  markDirty();
}

void Camera::setProjectionMatrix(const glm::mat4& projection)
{
  _projection = projection;
  _manualProjection = true;

  markDirty();
}

void Camera::lookAt(const glm::vec3& position)
{
  _rotation = glm::quatLookAt(position, _rotation * glm::vec3(0, 1, 0));
  _relativeTransform = glm::toMat4(_rotation) * _relativeTransform;

  markDirty();
}

void Camera::onPostUpdate()
{
  _absPosition = glm::vec3(_absoluteTransform[3]);
  _view = glm::inverse(_absoluteTransform);

  if (!_manualProjection) {
    if (!_orthographic)
      _projection = glm::perspective(_fov, _aspect, _near, _far);
    else
      _projection = glm::ortho(_frustumLeft, _frustumFar, _frustumBottom, _frustumTop, _frustumNear, _frustumFar);
  }

  _frustum.buildViewFrustum(_view, _projection);
}

}