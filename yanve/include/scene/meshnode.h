#pragma once

#include <common.h>
#include <scene/scenenode.h>

#include <math/aabb.h>
#include <graphics/gl/mesh.h>

namespace yanve::scene
{

struct YANVE_API MeshNodeData : public SceneNodeData
{
  gl::Mesh* mesh; // todo: pointer for now as this resource is not owned by this class
  math::AABB bBox;

  MeshNodeData(const std::string& name, gl::Mesh* mesh, const math::AABB& bBox) :
    SceneNodeData(SceneNodeType::Mesh, name),
    mesh{mesh},
    bBox{bBox}
  {}
};

class YANVE_API MeshNode : public SceneNode
{
public:
  MeshNode(const MeshNodeData& data);
  ~MeshNode();

  bool isAttachable() override { return true; }

  void onAttach(SceneNode& parent) override;
  void onDetach(SceneNode& parent) override;
  void onPostUpdate() override;

  gl::Mesh& mesh() { return *_mesh; } // temporary, just to test

protected:
  gl::Mesh* _mesh; // todo: pointer for now as this resource is not owned by this class
  math::AABB _localBBox;
};

}