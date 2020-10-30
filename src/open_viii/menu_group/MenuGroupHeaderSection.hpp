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

#ifndef VIIIARCHIVE_MENUGROUPHEADERSECTION_HPP
#define VIIIARCHIVE_MENUGROUPHEADERSECTION_HPP
#include <cstdint>
#include <cstring>
#include <string_view>
namespace open_viii::menu_group {
struct MenuGroupHeaderSection
{
  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_mngrphd_bin
  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_mngrp_bin
private:
  static constexpr std::uint32_t INVALID_SIZE = 0U;
  static constexpr std::uint32_t INVALID_FILE_OFFSET = 0xFFFFFFFFU;
  static constexpr std::uint8_t PERM_OFFSET{ 1U };
  std::uint32_t m_file_offset{};
  std::uint32_t m_size{};

public:
  [[nodiscard]] auto file_offset() const noexcept
  {
    return m_file_offset - PERM_OFFSET;
  }
  [[nodiscard]] const auto &size() const noexcept
  {
    return m_size;
  }
  template<typename charContainer = std::string_view>
  [[nodiscard]] std::string_view get_section_buffer(const charContainer &file_buffer) const
  {
    const uint32_t adj_offset = file_offset();
    if (m_size == INVALID_SIZE || m_file_offset == INVALID_FILE_OFFSET
        || static_cast<std::size_t>(adj_offset) + static_cast<std::size_t>(m_size) > std::ranges::size(file_buffer)) {
      return {};// returns empty buffer if invalid or error.
    }
    std::cout << '<' << adj_offset << ',' << m_size << ">\n";
    return { std::data(file_buffer) + adj_offset, m_size };
  }
};
}// namespace open_viii::menu_group
#endif// VIIIARCHIVE_MENUGROUPHEADERSECTION_HPP
