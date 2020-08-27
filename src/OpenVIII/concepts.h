//
// Created by pcvii on 8/26/2020.
//

#ifndef VIIIARCHIVE_CONCEPTS_H
#define VIIIARCHIVE_CONCEPTS_H
#include <concepts>
namespace OpenVIII {

template<typename T> concept Number = std::floating_point<T> || std::integral<T>;

template<typename T> concept Color_R = requires(T a)
{
  {
    a.R()
  }
  ->std::convertible_to<std::uint8_t>;
};

template<typename T> concept Color_G = requires(T a)
{
  {
    a.G()
  }
  ->std::convertible_to<std::uint8_t>;
};
template<typename T> concept Color_B = requires(T a)
{
  {
    a.B()
  }
  ->std::convertible_to<std::uint8_t>;
};
template<typename T> concept Color_A = requires(T a)
{
  {
    a.A()
  }
  ->std::convertible_to<std::uint8_t>;
};

template<typename T> concept Color = Color_A<T> &&Color_B<T> &&Color_G<T> &&Color_R<T>;


}// namespace OpenVIII
#endif// VIIIARCHIVE_CONCEPTS_H
