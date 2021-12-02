#pragma once

#include <common.h>

namespace yanve
{

template <typename T>
class optional_ref 
{
  T* _ptr = nullptr;
public:
  optional_ref() noexcept = default;
  optional_ref(T& reference)
    : _ptr{std::addressof(reference)} {}
  optional_ref(std::nullopt_t) noexcept
    : _ptr{nullptr} {}

  explicit operator bool() const noexcept { return _ptr != nullptr; }

  T& operator*() noexcept 
  {
    assert(_ptr != nullptr && "Dereferencing null optional_ref");
    return *_ptr;
  }

  const T& operator*() const noexcept
  {
    assert(_ptr != nullptr && "Dereferencing null optional_ref");
    return *_ptr;
  }

  T* operator->() noexcept
  {
    assert(_ptr != nullptr && "Dereferencing null optional_ref");
    return _ptr;
  }
};

}