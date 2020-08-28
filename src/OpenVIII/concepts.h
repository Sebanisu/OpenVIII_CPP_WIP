//
// Created by pcvii on 8/26/2020.
//

#ifndef VIIIARCHIVE_CONCEPTS_H
#define VIIIARCHIVE_CONCEPTS_H
#include <concepts>
#include "CompressionTypeT.h"
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


template<typename T> concept FI_Like_UncompressedSize = requires(T a)
{
  {
    a.UncompressedSize()
  }
  ->std::integral;
};
template<typename T> concept FI_Like_Offset = requires(T a)
{
  {
    a.Offset()
  }
  ->std::integral;
};

template<typename T> concept FI_Like_CompressionType = requires(T a)
{
  {
    a.CompressionType()
  }
  ->std::convertible_to<CompressionTypeT>;
};

template<typename T> concept FI_Like = FI_Like_CompressionType<T> &&FI_Like_Offset<T> &&FI_Like_UncompressedSize<T>;
}// namespace OpenVIII
#endif// VIIIARCHIVE_CONCEPTS_H
