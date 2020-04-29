#pragma once

#include <scene/camera.h>
#include <math/angle.h>
#include <glm/gtx/quaternion.hpp>

namespace yanve::scene
{
Camera::Camera() : 
  SceneNode(),
  _view{},
  _projection{},
  _absPosition{},
  _frustumLeft{}, _frustumRight{},
  _frustumTop{}, _frustumBottom{},
  _frustumNear{}, _frustumFar{},
  _frustum{},
  _orthographic{ false },
  _manualProjection{ false }
{
  _name = "camera";
}

Camera::Camera(SceneNode* parent) : 
  SceneNode(parent),
  _view{},
  _projection{},
  _absPosition{},
  _frustumLeft{}, _frustumRight{},
  _frustumTop{}, _frustumBottom{},
  _frustumNear{}, _frustumFar{},
  _frustum{},
  _orthographic{ false },
  _manualProjection{ false }
{
  _name = "camera";
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