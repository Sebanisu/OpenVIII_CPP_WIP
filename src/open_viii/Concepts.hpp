// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef VIIIARCHIVE_CONCEPTS_HPP
#define VIIIARCHIVE_CONCEPTS_HPP
#include "CompressionTypeT.hpp"
#include <concepts>
namespace open_viii {

template<typename T> concept Number = std::floating_point<T> || std::integral<T>;

template<typename T> concept Color_R = requires(T a)
{
  {
    a.r()
  }
  ->std::convertible_to<std::uint8_t>;
};

template<typename T> concept Color_G = requires(T a)
{
  {
    a.g()
  }
  ->std::convertible_to<std::uint8_t>;
};
template<typename T> concept Color_B = requires(T a)
{
  {
    a.b()
  }
  ->std::convertible_to<std::uint8_t>;
};
template<typename T> concept Color_A = requires(T a)
{
  {
    a.a()
  }
  ->std::convertible_to<std::uint8_t>;
};

template<typename T> concept Color = Color_A<T> &&Color_B<T> &&Color_G<T> &&Color_R<T>;


template<typename T> concept FI_Like_UncompressedSize = requires(T a)
{
  {
    a.uncompressed_size()
  }
  ->std::integral;
};
template<typename T> concept FI_Like_Offset = requires(T a)
{
  {
    a.offset()
  }
  ->std::integral;
};

template<typename T> concept FI_Like_CompressionType = requires(T a)
{
  {
    a.compression_type()
  }
  ->std::convertible_to<CompressionTypeT>;
};

template<typename T> concept FI_Like = FI_Like_CompressionType<T> &&FI_Like_Offset<T> &&FI_Like_UncompressedSize<T>;
}// namespace open_viii
#endif// VIIIARCHIVE_CONCEPTS_HPP
