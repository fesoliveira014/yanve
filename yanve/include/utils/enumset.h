#pragma once

#include <common.h>
#include <utils/nocreate.h>

namespace yanve
{
template<class T, typename std::underlying_type<T>::type fullValue = typename std::underlying_type<T>::type(~0)> class EnumSet;

template <class T, typename std::underlying_type<T>::type fullValue>
class EnumSet
{
public:
  typedef T type;
  typedef typename std::underlying_type<T>::type underlying_type;

  enum: underlying_type
  {
    FullValue = fullValue
  };

  constexpr EnumSet() : _value{ /*static_cast<underlying_type>(0)*/ } {}
  constexpr EnumSet(T value):
    _value{static_cast<underlying_type>(value)} {}

  explicit EnumSet(NoCreateT) {}

  constexpr bool operator==(EnumSet<T, fullValue> other) const {
    return _value == other._value;
  }

  constexpr bool operator!=(EnumSet<T, fullValue> other) const {
    return !operator==(other);
  }

  constexpr bool operator>=(EnumSet<T, fullValue> other) const {
    return (*this & other) == other;
  }

  constexpr bool operator<=(EnumSet<T, fullValue> other) const {
    return (*this & other) == *this;
  }

  constexpr EnumSet<T, fullValue> operator|(EnumSet<T, fullValue> other) const {
    return EnumSet<T, fullValue>(_value | other._value);
  }

  EnumSet<T, fullValue>& operator|=(EnumSet<T, fullValue> other) {
    _value |= other._value;
    return *this;
  }

  constexpr EnumSet<T, fullValue> operator&(EnumSet<T, fullValue> other) const {
    return EnumSet<T, fullValue>(_value & other._value);
  }

  EnumSet<T, fullValue>& operator&=(EnumSet<T, fullValue> other) {
    _value &= other._value;
    return *this;
  }

  constexpr EnumSet<T, fullValue> operator^(EnumSet<T, fullValue> other) const {
    return EnumSet<T, fullValue>(_value ^ other._value);
  }

  EnumSet<T, fullValue>& operator^=(EnumSet<T, fullValue> other) {
    _value ^= other._value;
    return *this;
  }

  constexpr EnumSet<T, fullValue> operator~() const {
    return EnumSet<T, fullValue>(fullValue & ~_value);
  }

  constexpr explicit operator bool() const {
    return _value != 0;
  }

  constexpr explicit operator underlying_type() const {
    return _value;
  }

private:
  constexpr explicit EnumSet(underlying_type type) : _value{ type } {}
  underlying_type _value;
};

template<class T, class = typename std::enable_if<std::is_enum<T>::value>::type> 
constexpr typename std::underlying_type<T>::type enumCastUnderlyingType(T value) 
{
  return typename std::underlying_type<T>::type(value);
}

template<class T, typename std::underlying_type<T>::type fullValue> 
constexpr typename std::underlying_type<T>::type enumCastUnderlyingType(EnumSet<T, fullValue> value) 
{
  return typename std::underlying_type<T>::type(value);
}

#define ENUMSET_OPERATORS(class)                                            \
    constexpr bool operator==(class::type a, class b) {                     \
        return class(a) == b;                                               \
    }                                                                       \
    constexpr bool operator!=(class::type a, class b) {                     \
        return class(a) != b;                                               \
    }                                                                       \
    constexpr bool operator>=(class::type a, class b) {                     \
        return class(a) >= b;                                               \
    }                                                                       \
    constexpr bool operator<=(class::type a, class b) {                     \
        return class(a) <= b;                                               \
    }                                                                       \
    constexpr class operator|(class::type a, class b) {                     \
        return b | a;                                                       \
    }                                                                       \
    constexpr class operator&(class::type a, class b) {                     \
        return b & a;                                                       \
    }                                                                       \
    constexpr class operator^(class::type a, class b) {                     \
        return b ^ a;                                                       \
    }                                                                       \
    constexpr class operator~(class::type a) {                              \
        return ~class(a);                                                   \
    }

#define ENUMSET_FRIEND_OPERATORS(class)                                     \
    friend constexpr bool operator==(typename class::type a, class b) {     \
        return class(a) == b;                                               \
    }                                                                       \
    friend constexpr bool operator!=(typename class::type a, class b) {     \
        return class(a) != b;                                               \
    }                                                                       \
    friend constexpr bool operator>=(typename class::type a, class b) {     \
        return class(a) >= b;                                               \
    }                                                                       \
    friend constexpr bool operator<=(typename class::type a, class b) {     \
        return class(a) <= b;                                               \
    }                                                                       \
    friend constexpr class operator|(typename class::type a, class b) {     \
        return b | a;                                                       \
    }                                                                       \
    friend constexpr class operator&(typename class::type a, class b) {     \
        return b & a;                                                       \
    }                                                                       \
    friend constexpr class operator^(typename class::type a, class b) {     \
        return b ^ a;                                                       \
    }                                                                       \
    friend constexpr class operator~(typename class::type a) {              \
        return ~class(a);                                                   \
    }


}