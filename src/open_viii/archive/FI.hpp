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
  std::uint32_t m_uncompressed_size{};
  std::uint32_t m_offset{};
  CompressionTypeT m_compression_type{};

  [[nodiscard]] constexpr static size_t get_start_offset(const unsigned int &id, const size_t &offset = 0U)
  {
    return (id * SIZE) + offset;
  }

public:
  constexpr static const size_t SIZE = 12U;

  constexpr static const auto EXT = std::string_view(".FI");

  [[nodiscard]] constexpr auto uncompressed_size() const noexcept
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
  constexpr explicit FI(const fiT &fi)
    : m_uncompressed_size{ static_cast<decltype(m_uncompressed_size)>(fi.uncompressed_size()) },
      m_offset{ static_cast<decltype(m_offset)>(fi.offset()) },
      m_compression_type{ static_cast<decltype(m_compression_type)>(fi.compression_type()) }
  {}
  constexpr FI(const unsigned int &uncompressed_size,
    const unsigned int &offset,
    const CompressionTypeT &compression_type = CompressionTypeT::none) noexcept
    : m_uncompressed_size{ uncompressed_size }, m_offset{ offset }, m_compression_type{ compression_type }
  {}

  explicit FI(std::ifstream &&fp, const long &start_offset = 0, bool close = false)
  {
    // unsure if this is correct but passing from ifstream is an rvalue
    // so umm it won't let me use a normal reference unless it's const.
    if (!fp.is_open()) {
      return;
    }
    if (start_offset < 0) {
      return;// shouldn't be less than 0;
    }
    fp.seekg(start_offset);

    Tools::read_val(fp, m_uncompressed_size);
    if (m_uncompressed_size > 0) {// if size is 0 than no point in reading more.
      Tools::read_val(fp, m_offset);
      Tools::read_val(fp, m_compression_type);
    }
    if (close) {
      fp.close();
    }
  }

  FI(const std::filesystem::path &path, const unsigned int &id, const size_t &offset)
    : FI(std::ifstream(path, std::ios::in | std::ios::binary), static_cast<long>(get_start_offset(id, offset)), true)
  {}

  explicit FI(std::span<const char> buffer, const size_t &start_offset = 0U)
  {
    if (start_offset + SIZE > std::ranges::size(buffer)) {
      return;
    }
    buffer = buffer.subspan(start_offset, SIZE);

    std::memcpy(&m_uncompressed_size, std::ranges::data(buffer), sizeof(m_uncompressed_size));
    if (m_uncompressed_size > 0) {// if size is 0 than no point in reading more.
      buffer = buffer.subspan(sizeof(m_uncompressed_size));
      std::memcpy(&m_offset, std::ranges::data(buffer), sizeof(m_offset));
      buffer = buffer.subspan(sizeof(m_offset));
      std::memcpy(&m_compression_type, std::ranges::data(buffer), sizeof(m_compression_type));
    }
  }

  FI(const std::vector<char> &buffer, const unsigned int &id, const size_t &offset)
    : FI(buffer, get_start_offset(id, offset))
  {}

  [[nodiscard]] constexpr static size_t get_count(const std::size_t &file_size) noexcept
  {
    return file_size / SIZE;
  }
  // GetCount which is fileSize/Size if file doesn't exist return 0;
  [[maybe_unused]] [[nodiscard]] size_t static get_count(const std::filesystem::path &path)
  {
    if (std::filesystem::exists(path)) {
      return get_count(std::filesystem::file_size(path));
    }
    return {};
  }

  [[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const FI &data)
  {
    os << '{' << data.m_uncompressed_size << ", " << data.m_offset << ", "
       << static_cast<unsigned int>(data.m_compression_type) << '}';
    return os;
  }
};
}// namespace open_viii::archive

#endif// !VIIIARCHIVE_FI_HPP