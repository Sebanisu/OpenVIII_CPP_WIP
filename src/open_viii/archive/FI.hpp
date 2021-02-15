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
#ifndef VIIIARCHIVE_FI_HPP
#define VIIIARCHIVE_FI_HPP
#include "open_viii/CompressionTypeT.hpp"
#include "open_viii/tools/Tools.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <span>
#include <vector>
namespace open_viii::archive {
/**
 * FI is the file index for the FL and FS files.
 */
struct FI
{
  // changed to int because libraries require casting to int anyway.
private:
  /**
   * Uncompressed Size of entry
   */
  std::uint32_t m_uncompressed_size{};
  /**
   * Offset to entry
   */
  std::uint32_t m_offset{};
  /**
   * Compression Type
   */
  CompressionTypeT m_compression_type{};
public:
  constexpr static const std::size_t SIZE = 12U;
  constexpr static const auto        EXT  = std::string_view(".FI");
  [[nodiscard]] constexpr auto       uncompressed_size() const noexcept
  {
    return m_uncompressed_size;
  }
  [[nodiscard]] constexpr auto offset() const noexcept
  {
    return m_offset;
  }
  [[nodiscard]] constexpr auto compression_type() const noexcept
  {
    return m_compression_type;
  }
  constexpr FI() noexcept = default;
  template<FI_Like fiT>
  requires(!std::is_same_v<fiT, FI>) constexpr explicit FI(
    const fiT &fi) noexcept
    : m_uncompressed_size{ static_cast<decltype(m_uncompressed_size)>(
      fi.uncompressed_size()) },
      m_offset{ static_cast<decltype(m_offset)>(fi.offset()) },
      m_compression_type{ static_cast<decltype(m_compression_type)>(
        fi.compression_type()) }
  {}
  constexpr FI(
    const unsigned int &    uncompressed_size,
    const unsigned int &    offset,
    const CompressionTypeT &compression_type = CompressionTypeT::none) noexcept
    : m_uncompressed_size{ uncompressed_size },
      m_offset{ offset },
      m_compression_type{ compression_type }
  {}
  [[nodiscard]] constexpr static std::size_t
    get_count(const std::size_t &file_size) noexcept
  {
    return file_size / SIZE;
  }
  // GetCount which is fileSize/Size if file doesn't exist return 0;
  [[maybe_unused]] [[nodiscard]] std::size_t static get_count(
    const std::filesystem::path &path)
  {
    if (std::filesystem::exists(path)) {
      return get_count(std::filesystem::file_size(path));
    }
    return {};
  }
  [[nodiscard]] friend std::ostream &operator<<(std::ostream &os,
                                                const FI &    data)
  {
    os << '{' << data.m_uncompressed_size << ", " << data.m_offset << ", "
       << static_cast<unsigned int>(data.m_compression_type) << '}';
    return os;
  }
  /**
   * gets member variables
   * @note required to structured binding support
   */
  template<std::size_t I>
  requires(I < 3) [[nodiscard]] constexpr auto get() const noexcept
  {
    if constexpr (I == 0) {
      return m_uncompressed_size;
    } else if constexpr (I == 1) {
      return m_offset;
    } else if constexpr (I == 2) {
      return m_compression_type;
    }
  }
};
static_assert(sizeof(FI) == FI::SIZE);
/**
 * find the offset in file where FI entry is located
 * @param id, count from 0 to N of FI entries to skip.
 * @param offset, the number of bytes to start of first FI entry
 * @return id * 12 + offset
 */
[[nodiscard]] constexpr static std::size_t
  get_fi_entry_offset(const std::size_t &id, const std::size_t &offset = 0U)
{
  return (id * sizeof(FI)) + offset;
}
}// namespace open_viii::archive
namespace std {
/**
 * define number of arguments
 * @note required to structured binding support
 */
template<>
struct tuple_size<open_viii::archive::FI> : std::integral_constant<size_t, 3>
{
};
/**
 * type of 1st argument
 * @note required to structured binding support
 */
template<> struct tuple_element<0, open_viii::archive::FI>
{
  using type = std::uint32_t;
};
/**
 * type of 2nd argument
 * @note required to structured binding support
 */
template<> struct tuple_element<1, open_viii::archive::FI>
{
  using type = std::uint32_t;
};
/**
 * type of 3rd argument
 * @note required to structured binding support
 */
template<> struct tuple_element<2, open_viii::archive::FI>
{
  using type = open_viii::CompressionTypeT;
};
}// namespace std
#endif// !VIIIARCHIVE_FI_HPP