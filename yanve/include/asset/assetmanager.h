#pragma once

#include <common.h>
#include <asset/asset.h>


namespace yanve
{

using namespace yanve::asset;

class AssetManager
{
  
  AssetId loadAsset(const std::string& path, AssetType type, const std::string& name, AssetFlags flags);
  AssetId addAsset(AssetType type, const std::string& name, AssetFlags flags);
  AssetId cloneAsset(Asset& source, const std::string& name);
  bool removeAsset(AssetId id);

  template <class T> T& getAsset(AssetId id);
  template <class T> T& findAsset(AssetType type, const std::string& name);
  std::vector<std::unique_ptr<Asset>>& getAssets() { return _assets; }

  static AssetManager& instance();

private:
  AssetManager();
  ~AssetManager();
  AssetManager(const AssetManager&) = delete;
  AssetManager(AssetManager&&) = delete;

  AssetManager& operator=(const AssetManager&) = delete;
  AssetManager& operator=(AssetManager&&) = delete;

private:
  std::vector<std::unique_ptr<Asset>> _assets;
  std::vector<size_t> _freeSlots;
};

}