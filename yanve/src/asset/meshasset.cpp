#include <asset/meshasset.h>

namespace yanve::asset
{

MeshAsset::MeshAsset(std::string name, AssetFlags flags) :
  Asset(name, AssetType::Mesh, flags),
  _mesh{NoCreate}
{

}

MeshAsset::~MeshAsset()
{
  
}


}