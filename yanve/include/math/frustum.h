#pragma once

#include <common.h>
#include <math/plane.h>
#include <math/aabb.h>

namespace yanve::math
{

class YANVE_API Frustum
{
public:
  glm::vec3 getCorner(int corner) const { return _corners[corner]; }
  glm::vec3 getOrigin() const { return _origin; }

  void buildViewFrustum(const glm::mat4& transformation, float fov, float aspect, float nearPlane, float farPlane);
  void buildViewFrustum(const glm::mat4& transformation, float left, float right, float top, float bottom, float nearPlane, float farPlane);
  void buildViewFrustum(const glm::mat4& view, const glm::mat4& projection);
  
  void buildBoxFrustum(const glm::mat4& transformation, float left, float right, float top, float bottom, float front, float back);

  bool cullSphere(glm::vec3 origin, float radius) const;
  bool cullBox(const AABB& box) const;
  bool cullFrustum(const Frustum& frustum);

  AABB calculateAABB() const;

protected:
  Plane _planes[6];
  glm::vec3 _origin;
  glm::vec3 _corners[8];
};

}