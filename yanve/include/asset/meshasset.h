#pragma once

#include <common.h>
#include <asset/asset.h>
#include <data/geometrydata.h>
#include <graphics/gl/mesh.h>

namespace yanve::asset
{

class MeshAsset : public Asset
{
  MeshAsset(std::string name, AssetFlags flags);
  ~MeshAsset();

  bool load(const std::string& path) { return false; };
  //bool load(const byte *data, int size);
  bool load(const data::GeometryData& data);
  bool load(gl::Mesh&& mesh);

  void unload();

  gl::Mesh& mesh() { return _mesh; }

protected:
  Asset* clone();

private:
  gl::Mesh _mesh;
};

}