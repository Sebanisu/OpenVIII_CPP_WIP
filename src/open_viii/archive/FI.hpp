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
#include "tl/read.hpp"
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
  /**
   * Expected size of the FI struct, used for a static assert after.
   */
  constexpr static const std::size_t SIZE = 12U;
  /**
   * File extension for FI
   */
  constexpr static const auto EXT = std::string_view(".FI", 3U);
  /**
   * Get uncompressed size for this entry.
   */
  [[nodiscard]] constexpr auto
    uncompressed_size() const noexcept
  {
    return m_uncompressed_size;
  }
  /**
   * Get offset for this entry.
   */
  [[nodiscard]] constexpr auto
    offset() const noexcept
  {
    return m_offset;
  }
  /**
   * Get Compression type for this entry.
   */
  [[nodiscard]] constexpr auto
    compression_type() const noexcept
  {
    return m_compression_type;
  }
  /**
   * Enable default constructor.
   */
  constexpr FI() noexcept = default;
  /**
   * Constructor takes a FI like object and copies the data out of it.
   * @tparam fiT Fi Like object type
   * @param fi Fi Like object
   * @note There may be data loss. This object stores things as std::uint32_t.
   * FileData object uses std::uint64_t. Also FileData has a string for file
   * path. This doesn't store the path.
   */
  template<FI_Like fiT>
  requires(!std::is_same_v<std::decay_t<fiT>, FI>) constexpr explicit FI(
    const fiT &fi) noexcept
    : m_uncompressed_size{ static_cast<decltype(m_uncompressed_size)>(
      fi.uncompressed_size()) },
      m_offset{ static_cast<decltype(m_offset)>(fi.offset()) },
      m_compression_type{ static_cast<decltype(m_compression_type)>(
        fi.compression_type()) }
  {}
  /**
   * Piecemeal constructor takes the 3 values stored in this object.
   * @param uncompressed_size bytes of the entry after being uncompressed.
   * @param offset bytes from start of FS file.
   * @param compression_type compressed or not. And which compression it's
   * using.
   */
  constexpr FI(
    const unsigned int &    uncompressed_size,
    const unsigned int &    offset,
    const CompressionTypeT &compression_type = CompressionTypeT::none) noexcept
    : m_uncompressed_size{ uncompressed_size },
      m_offset{ offset },
      m_compression_type{ compression_type }
  {}
  /**
   * Read FI from a span at offset.
   * @param data span containing all the FI entries
   * @param offset location from start where the desired entry is.
   */
  FI(std::span<const char> data, std::intmax_t offset)
    : FI(tl::read::input(&data, true)
           .seek(offset, std::ios::beg)
           .template output<FI>())
  {}
  /**
   * Read FI[id] from a span at offset.
   * @param data span containing all the FI entries
   * @param id is the FI entry number starting at 0.
   * @param offset to entry 0.
   */
  FI(std::span<const char> data, std::size_t id, std::size_t offset)
    : FI(data, static_cast<std::intmax_t>(get_fi_entry_offset(id, offset)))
  {}
  /**
   * Read FI from a path at offset.
   * @param path containing all the FI entries
   * @param offset location from start where the desired entry is.
   */
  FI(const std::filesystem::path &path, std::intmax_t offset)
    : FI(tl::read::from_file<FI>(offset, path))
  {}
  /**
   * Read FI[id] from a path at offset.
   * @param path containing all the FI entries
   * @param id is the FI entry number starting at 0.
   * @param offset to entry 0.
   */
  FI(const std::filesystem::path &path, std::size_t id, std::size_t offset)
    : FI(path, static_cast<std::intmax_t>(get_fi_entry_offset(id, offset)))
  {}
  /**
   * Get count of possible entries based on file_size
   * @param file_size total bytes in a file.
   */
  [[nodiscard]] constexpr static std::size_t
    get_count(const std::size_t &file_size) noexcept
  {
    return file_size / SIZE;
  }
  /**
   * Get count of possible entries based on file_size
   * @param path to file containing the FI entries.
   * @return If path doesn't exist returns 0.
   */
  [[maybe_unused]] [[nodiscard]] std::size_t static get_count(
    const std::filesystem::path &path)
  {
    if (std::filesystem::exists(path)) {
      return get_count(std::filesystem::file_size(path));
    }
    return {};
  }
  /**
   * Dump values to ostream.
   * @param os out stream value
   * @param data FI entry
   */
  [[nodiscard]] friend std::ostream &
    operator<<(std::ostream &os, const FI &data)
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
  /**
   * find the offset in file where FI entry is located
   * @param id, count from 0 to N of FI entries to skip.
   * @param offset, the number of bytes to start of first FI entry
   * @return id * 12 + offset
   */
  [[nodiscard]] constexpr static std::size_t
    get_fi_entry_offset(const std::size_t &id, const std::size_t &offset = 0U)
  {
    return (id * FI::SIZE) + offset;
  }
};
static_assert(sizeof(FI) == FI::SIZE);
}// namespace open_viii::archive
namespace std {
/**
 * define number of arguments
 * @note required to structured binding support
 */
template<>
struct [[maybe_unused]] tuple_size<open_viii::archive::FI> : std::integral_constant<size_t, 3>
{
};
/**
 * type of 1st argument
 * @note required to structured binding support
 */
template<> struct [[maybe_unused]] tuple_element<0, open_viii::archive::FI>
{
  using type = std::uint32_t;
};
/**
 * type of 2nd argument
 * @note required to structured binding support
 */
template<> struct [[maybe_unused]] tuple_element<1, open_viii::archive::FI>
{
  using type = std::uint32_t;
};
/**
 * type of 3rd argument
 * @note required to structured binding support
 */
template<> struct [[maybe_unused]] tuple_element<2, open_viii::archive::FI>
{
  using type = open_viii::CompressionTypeT;
};
}// namespace std
#endif// !VIIIARCHIVE_FI_HPP