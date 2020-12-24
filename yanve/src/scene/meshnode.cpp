#pragma once

#include <scene/meshnode.h>

namespace yanve::scene
{
  MeshNode::MeshNode(const MeshNodeData& data) :
    SceneNode{ data },
    _mesh{ data.mesh },
    _localBBox{data.bBox}
  {
    _renderable = true;
    _type = SceneNodeType::Mesh;
  }

  MeshNode::~MeshNode() 
  {
    if (_mesh) _mesh = nullptr; // this is owned by this resource
  }

  void MeshNode::onAttach(SceneNode& parent) 
  {
    SceneNode* node = &parent;
    //while (node->type != SceneNodeType::Model) {}
    _parent = node;
    _parent->markDirty();
  }

  void MeshNode::onDetach(SceneNode& /*parent*/)
  {
    _parent->markDirty();
  }

  void MeshNode::onPostUpdate()
  {
    _bBox = _localBBox;
    _bBox.transform( _absoluteTransform );
  }
}