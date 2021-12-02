#pragma once

#include <common.h>
#include <utils/logger.h>

/**
  * This code was inspired (copied?) from Magnum:
  * https://github.com/mosra/magnum/blob/master/src/Magnum/MeshTools/Interleave.h
  */

namespace yanve::utils
{
namespace implementation
{
template <typename T, typename = int>
struct HasType : std::false_type {};

template <typename T>
struct HasType <T, decltype((void)T::Type, 0)> : std::true_type {};

struct AttributeCount
{
  template<class T, class ...U> typename std::enable_if<!std::is_convertible<T, std::size_t>::value, std::size_t>::type operator()(const T& first, const U& ... next) const 
  {
    if (!(sizeof...(next) == 0 || AttributeCount{}(next) == first.size() || AttributeCount{}(next) == ~std::size_t{ 0 })) {
      LogError("AttributeCount", "Attribute arrays don't have the same length, expected %d but got %d", first.size(), AttributeCount{}(next...));
      exit(1); // todo: add graceful exit
    }

    return first.size();
  }

  template<class T, class... U> std::size_t operator()(std::size_t, const T& first, const U& ... next) const {
    return AttributeCount{}(first, next...);
  }

  constexpr std::size_t operator()(std::size_t) const { return ~std::size_t(0); }
  constexpr std::size_t operator()() const { return 0; }
};

template <typename T> 
constexpr std::size_t typeSize() 
{
  if constexpr (HasType<T>::value) {
    return sizeof(T::Type);
  }
  else {
    return sizeof(T::value_type);
  }
}

struct Stride {
  template<class T, class ...U> typename std::enable_if<!std::is_convertible<T, std::size_t>::value, std::size_t>::type operator()(const T&, const U& ... next) const {
    return typeSize<T>() + Stride{}(next...);
  }
  template<class... T> std::size_t operator()(std::size_t gap, const T& ... next) const {
    return gap + Stride{}(next...);
  }
  constexpr std::size_t operator()() const { return 0; }
};

/* Copy data to the buffer */
template<class T> typename std::enable_if<!std::is_convertible<T, std::size_t>::value, std::size_t>::type writeOneInterleaved(std::size_t stride, char* startingOffset, const T& attributeList) {
  auto it = attributeList.begin();
  for (std::size_t i = 0; i != attributeList.size(); ++i, ++it)
    std::memcpy(startingOffset + i * stride, reinterpret_cast<const char*>(&*it), typeSize<T>());

  return typeSize<T>();
}

/* Skip gap */
constexpr std::size_t writeOneInterleaved(std::size_t, char*, std::size_t gap) { return gap; }

/* Write interleaved data */
inline void writeInterleaved(std::size_t, char*) {}
template<class T, class ...U> void writeInterleaved(std::size_t stride, char* startingOffset, const T& first, const U& ... next) {
  writeInterleaved(stride, startingOffset + writeOneInterleaved(stride, startingOffset, first), next...);
}

}

template <typename T, typename ...U> 
std::optional<std::vector<uint8_t>> interleave(const T& first, const U&... next)
{
  const std::size_t count = implementation::AttributeCount{}(first, next...);
  const std::size_t stride = implementation::Stride{}(first, next...);

  if (count && count != ~std::size_t{ 0 }) {
    std::vector<uint8_t> data;
    data.resize(count * stride);
    implementation::writeInterleaved(stride, data.begin(), first, next...);
    return data;
  }

  LogError(__func__, "Could not interleave attributes.");

  return {};
}

}