#pragma once

#include <common.h>
#include <utils/enumset.h>

// todo: fix this
namespace yanve
{
class AssetManager;
}

namespace yanve::asset
{

using AssetId = uint;

enum class AssetType : uint
{
  Undefined = 0,
  Mesh,
  Texture,
  Shader,
  Material,
};

enum class AssetFlag : uint
{
  TexRenderable = 1 << 0,
  TexDepthBuffer = 1 << 1,
};

typedef EnumSet<AssetFlag> AssetFlags;

class Asset
{
  template <class T> friend class AssetPtr;
  friend class yanve::AssetManager;
public:
  Asset(std::string name, AssetType type, AssetFlags flags);
  virtual ~Asset();
  
  virtual bool load(const std::string& path) = 0;
  virtual void unload() = 0;

  std::string& name() { return _name; }
  AssetType type() { return _type; }
  AssetId id() { return _id; }

protected:
  void addReference() { _refCount++; }
  void removeReference() { _refCount--; }
  size_t refCount() { return _refCount; }
  
  virtual Asset* clone() = 0;

private:
  std::string _name;
  AssetType _type;
  AssetId _id;

  size_t _refCount;
};

template <class T>
class AssetPtr
{
public:
  AssetPtr() : _ptr{ nullptr } {}
  AssetPtr(Asset* ptr) : _ptr{ ptr } {}
  AssetPtr(const AssetPtr& other) : _ptr{ other._ptr } { _ptr->addReference(); }
  AssetPtr(AssetPtr&&) = delete; // no move operation allowed for smart pointer
  ~AssetPtr() { cleanup(); }

  AssetPtr& operator=(const AssetPtr& other)
  {
    if (this->_ptr != other._ptr) {
      this->cleanup();
      this->_ptr = other._ptr;
      this->_ptr->addReference();
    }
    return *this;
  }

  AssetPtr& operator=(AssetPtr&&) = delete;

  Asset* operator->()
  {
    return this->_ptr;
  }

  Asset& operator*()
  {
    return *(this->_ptr);
  }

protected:
  void cleanup() {
    if (_ptr != nullptr) {
      if (_ptr->refCount()) {
        _ptr->removeReference();
      }

      if (_ptr->refCount() == 0) {
        delete _ptr;
      }
    }
  }

private:
  T* _ptr{ nullptr };
};

ENUMSET_OPERATORS(AssetFlags);

}