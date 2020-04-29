#include <math/frustum.h>
#include <math/angle.h>

namespace yanve::math
{

void Frustum::buildViewFrustum(const glm::mat4& transformation, float fov, float aspect, float nearPlane, float farPlane)
{
  float ymax = nearPlane * glm::tan(radians<float>(fov / 2));
  float xmax = ymax * aspect;

  buildViewFrustum(transformation, -xmax, xmax, -ymax, ymax, nearPlane, farPlane);
}

void Frustum::buildViewFrustum(const glm::mat4& transformation, float left, float right, float top, float bottom, float nearPlane, float farPlane)
{
  float left_f = left * farPlane / nearPlane;
  float right_f = right * farPlane / nearPlane;
  float bottom_f = bottom * farPlane / nearPlane;
  float top_f = top * farPlane / nearPlane;

  _corners[0] = glm::vec3(left, bottom, -nearPlane);
  _corners[1] = glm::vec3(right, bottom, -nearPlane);
  _corners[2] = glm::vec3(right, top, -nearPlane);
  _corners[3] = glm::vec3(left, top, -nearPlane);

  _corners[4] = glm::vec3(left_f, bottom_f, -farPlane);
  _corners[5] = glm::vec3(right_f, bottom_f, -farPlane);
  _corners[6] = glm::vec3(right_f, top_f, -farPlane);
  _corners[7] = glm::vec3(left_f, top_f, -farPlane);

  glm::mat3 trans{ transformation };

  _origin = trans * glm::vec3(0);
  for (u32 i = 0; i < 8; ++i)
    _corners[i] = trans *_corners[i];

  _planes[0] = Plane(_origin, _corners[3], _corners[0]);		// Left
  _planes[1] = Plane(_origin, _corners[1], _corners[2]);		// Right
  _planes[2] = Plane(_origin, _corners[0], _corners[1]);		// Bottom
  _planes[3] = Plane(_origin, _corners[2], _corners[3]);		// Top
  _planes[4] = Plane(_corners[0], _corners[1], _corners[2]);	// Near
  _planes[5] = Plane(_corners[5], _corners[4], _corners[7]);	// Far
}

void Frustum::buildViewFrustum(const glm::mat4& view, const glm::mat4& projection)
{
  glm::mat4 m = projection * view;

  _planes[0] = Plane(-(m[0][3] + m[0][0]), -(m[1][3] + m[1][0]),
    -(m[2][3] + m[2][0]), -(m[3][3] + m[3][0]));	// Left
  _planes[1] = Plane(-(m[0][3] - m[0][0]), -(m[1][3] - m[1][0]),
    -(m[2][3] - m[2][0]), -(m[3][3] - m[3][0]));	// Right
  _planes[2] = Plane(-(m[0][3] + m[0][1]), -(m[1][3] + m[1][1]),
    -(m[2][3] + m[2][1]), -(m[3][3] + m[3][1]));	// Bottom
  _planes[3] = Plane(-(m[0][3] - m[0][1]), -(m[1][3] - m[1][1]),
    -(m[2][3] - m[2][1]), -(m[3][3] - m[3][1]));	// Top
  _planes[4] = Plane(-(m[0][3] + m[0][2]), -(m[1][3] + m[1][2]),
    -(m[2][3] + m[2][2]), -(m[3][3] + m[3][2]));	// Near
  _planes[5] = Plane(-(m[0][3] - m[0][2]), -(m[1][3] - m[1][2]),
    -(m[2][3] - m[2][2]), -(m[3][3] - m[3][2]));	// Far

  _origin = glm::inverse(view) * glm::vec4(0,0,0,1);

  // Calculate corners
  glm::mat4 mm = glm::inverse(m);
  glm::vec4 corner = mm * glm::vec4(-1, -1, -1, 1);
  _corners[0] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
  corner = mm * glm::vec4(1, -1, -1, 1);
  _corners[1] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
  corner = mm * glm::vec4(1, 1, -1, 1);
  _corners[2] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
  corner = mm * glm::vec4(-1, 1, -1, 1);
  _corners[3] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
  corner = mm * glm::vec4(-1, -1, 1, 1);
  _corners[4] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
  corner = mm * glm::vec4(1, -1, 1, 1);
  _corners[5] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
  corner = mm * glm::vec4(1, 1, 1, 1);
  _corners[6] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
  corner = mm * glm::vec4(-1, 1, 1, 1);
  _corners[7] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
}

void Frustum::buildBoxFrustum(const glm::mat4& transformation, float left, float right, float top, float bottom, float front, float back)
{
  // Get points on front plane
  _corners[0] = glm::vec3(left, bottom, front);
  _corners[1] = glm::vec3(right, bottom, front);
  _corners[2] = glm::vec3(right, top, front);
  _corners[3] = glm::vec3(left, top, front);

  // Get points on far plane
  _corners[4] = glm::vec3(left, bottom, back);
  _corners[5] = glm::vec3(right, bottom, back);
  _corners[6] = glm::vec3(right, top, back);
  _corners[7] = glm::vec3(left, top, back);

  glm::mat3 trans{ transformation };

  // Transform points to fit camera position and rotation
  _origin = trans * glm::vec3(0, 0, 0);
  for (u32 i = 0; i < 8; ++i)
    _corners[i] = trans * _corners[i];

  // Build planes
  _planes[0] = Plane(_corners[0], _corners[3], _corners[7]);	// Left
  _planes[1] = Plane(_corners[2], _corners[1], _corners[6]);	// Right
  _planes[2] = Plane(_corners[0], _corners[4], _corners[5]);	// Bottom
  _planes[3] = Plane(_corners[3], _corners[2], _corners[6]);	// Top
  _planes[4] = Plane(_corners[0], _corners[1], _corners[2]);	// Front
  _planes[5] = Plane(_corners[4], _corners[7], _corners[6]);	// Back
}

bool Frustum::cullSphere(glm::vec3 origin, float radius) const
{
  return false;
}

bool Frustum::cullBox(const AABB& box) const
{
  return false;
}

bool Frustum::cullFrustum(const Frustum& frustum)
{
  return false;
}

AABB Frustum::calculateAABB() const
{
  return AABB();
}



}