#include <graphics/gl/state/meshstate.h>
#include <graphics/gl/state/state.h>
#include <graphics/gl/mesh.h>

namespace yanve::gl::state
{

MeshState::MeshState() : currentVAO{}
{
  
}

MeshState::~MeshState() {}

void MeshState::reset()
{
  currentVAO = State::DisengagedBinding;
}

void MeshState::resetVao()
{
  Mesh::bindVAOInternal(0);
}

}