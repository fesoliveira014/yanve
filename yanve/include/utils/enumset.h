//#pragma once
//
//#include <common.h>
//
//namespace yanve
//{
//
//template <class T, typename std::underlying_type<T>::type fullValue>
//class EnumSet
//{
//public:
//  typedef T type;
//  typedef std::underlying_type<T>::type underlying_type;
//
//  enum FullValue
//  {
//    FullValue = fullValue
//  };
//
//
//
//  constexpr EnumSet() : value{} {}
//
//private:
//  underlying_type value;
//};
//}