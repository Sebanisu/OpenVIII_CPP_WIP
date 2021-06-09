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
#ifndef VIIIARCHIVE_MENUMESSAGES_HPP
#define VIIIARCHIVE_MENUMESSAGES_HPP
#include "MenuMessagesSection.hpp"
#include <array>
#include <cstdint>
#include <cstring>
#include <vector>
namespace open_viii::menu_group {
struct MenuMessages
{
  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_tkmnmes
private:
  static constexpr std::uint16_t                DEFAULT_SIZE{ 16U };
  std::uint16_t                                 m_size{ DEFAULT_SIZE };
  std::array<std::uint16_t, DEFAULT_SIZE>       m_sections{};
  std::array<MenuMessagesSection, DEFAULT_SIZE> m_sub_sections{};

public:
  [[nodiscard]] auto
    size() const noexcept
  {
    return m_size;
  }
  [[nodiscard]] auto
    sections() const noexcept
  {
    return m_sections;
  }
  [[maybe_unused]] [[nodiscard]] auto
    sub_sections() const noexcept
  {
    return m_sub_sections;
  }
  [[nodiscard]] auto
    begin() const noexcept
  {
    return m_sub_sections.begin();
  }
  [[nodiscard]] auto
    end() const noexcept
  {
    return m_sub_sections.end();
  }
  template<typename T = std::vector<char>>
  explicit MenuMessages(const T &buffer)
  {
    // TODO remove pointer; use span instead of template.
    auto *ptr = buffer.data();
    std::memcpy(&m_size, ptr, sizeof(m_size));
    if (m_size != DEFAULT_SIZE) {
      exit(1);
    }
    ptr += sizeof(m_size);
    std::memcpy(m_sections.data(),
                ptr,
                std::ranges::size(m_sections) * sizeof(std::uint16_t));
    ptr += sizeof(m_sections);
    for (size_t i = 0; i < std::ranges::size(m_sections); i++) {
      //[Count of Subsections] = [Start of file] + [Section value]
      if (m_sections.at(i) == 0U) {
        continue;
      }
      ptr = buffer.data() + m_sections.at(i);
      std::uint16_t subSectionCount{};
      std::memcpy(&subSectionCount, ptr, sizeof(subSectionCount));
      std::string_view b{ ptr + sizeof(std::uint16_t),
                          subSectionCount * sizeof(std::uint16_t) };
      m_sub_sections.at(i).set_data(b, subSectionCount);
      //      ptr += sizeof(subSectionCount);
      //      m_sub_sections.at(i).resize(subSectionCount);
      //      std::memcpy(m_sub_sections.at(i).data(), ptr,
      //      m_sub_sections.at(i).size() * sizeof(EncodedStringOffset));
      //[Start of string location] = [Start of file] + [Section value] +
      //[Subsection value]
    }
  }
};
}// namespace open_viii::menu_group
#endif// VIIIARCHIVE_MENUMESSAGES_HPP
