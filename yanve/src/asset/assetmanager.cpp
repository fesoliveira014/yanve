#include <asset/assetmanager.h>
#include <asset/meshasset.h>
#include <utils/logger.h>

namespace yanve
{
using namespace asset;

static std::string TAG = "AssetManager::";

AssetManager::AssetManager() :
  _assets{100},
  _freeSlots{}
{
  
}

AssetManager::~AssetManager()
{
  _assets.clear();
  _freeSlots.clear();
}

AssetManager& AssetManager::instance()
{
  static AssetManager instance;
  return instance;
}

AssetId AssetManager::loadAsset(const std::string& path, AssetType type, const std::string& name, AssetFlags flags)
{
  switch (type) {
  case AssetType::Mesh:
    //AssetId id = MeshAsset::load(path, name, flags);
    break;
  }
}

AssetId AssetManager::addAsset(AssetType type, const std::string& name, AssetFlags flags)
{
  for (auto& asset : _assets) {
    if (asset->name() == name) {
      return asset->id();
    }
  }

  std::unique_ptr<Asset> asset;

  switch (type) {
  case AssetType::Mesh:
    asset = std::make_unique<MeshAsset>(name, flags);
    break;
  case AssetType::Texture:
    //asset = std::make_unique<TextureAsset>(name, flags);
    break;
  case AssetType::Shader:
    //asset = std::make_unique<ShaderAsset>(name, flags);
    break;
  case AssetType::Material:
    //asset = std::make_unique<MaterialAsset>(name, flags);
    break;
  case AssetType::Undefined:
  default:
    break;
  }

  if (asset != nullptr) {
    AssetId id;
    if (_freeSlots.size()) {
      id = _freeSlots.back();
      _freeSlots.pop_back();
      _assets[id] = std::move(asset);
    }
    else {
      id = _assets.size();
      _assets.push_back(std::move(asset));
    }

    return id;
  }

  return 0;
}

AssetId AssetManager::cloneAsset(Asset& source, const std::string& name)
{
  if (name != "") {
    for (size_t i = 0; i < _assets.size(); ++i) {
      if (_assets[i] != nullptr && _assets[i]->name() == name) {
        LogInfo(TAG + __func__, "an asset named %s already exists", name);
        return 0;
      }
    }
  }

  Asset* newAsset = source.clone();
  if (newAsset == nullptr) return 0;

  newAsset->_name = name;
  
}

bool AssetManager::removeAsset(AssetId id)
{
  if (id >= _assets.size())
    return false;

  if (_assets[id] != nullptr) {
    _assets[id].reset();
    _freeSlots.push_back(id);
  } 
  else {
    LogWarning(TAG + __func__, "asset %d already removed", id);
  }
  
  return true;
}

template <class T>
T& AssetManager::getAsset(AssetId id) 
{
  // sanity check
  assert(id < _assets.size());
  return *((T*)_assets[id].get()); //probably not correct?
}

}