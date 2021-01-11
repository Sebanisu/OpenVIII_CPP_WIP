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

#ifndef VIIIARCHIVE_COMPLEXSTRINGSECTION_HPP
#define VIIIARCHIVE_COMPLEXSTRINGSECTION_HPP
#include "open_viii/Strings/FF8String.hpp"
#include <array>
#include <cstdint>
#include <cstring>
#include <ostream>
#include <string_view>
namespace open_viii::menu_group {
struct ComplexStringSectionEntry
{
private:
  static constexpr auto UNK_COUNT = 6U;
  std::array<uint8_t, UNK_COUNT> m_unknown{};
  std::string_view m_buffer{};

  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_mngrp_complex_strings#String_Entry
public:
  explicit ComplexStringSectionEntry(const std::string_view &buffer)
  {
    uint16_t length{};
    std::memcpy(m_unknown.data(), buffer.data(), sizeof(m_unknown));
    std::memcpy(&length, buffer.data() + sizeof(m_unknown), sizeof(length));
    m_buffer =
      std::string_view{ buffer.data() + sizeof(m_unknown) + sizeof(length),
        length };
  }
  template<LangT langVal> std::ostream &out(std::ostream &os) const
  {
    const auto temp = FF8String<langVal>::decode(m_buffer);
    return os << tools::u8_to_sv(temp);
  }
};
struct ComplexStringSectionOffsets
{
  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_mngrp_complex_strings#Seek_Struct
private:
  std::uint16_t m_offset{};
  std::uint16_t m_index{};

public:
  [[nodiscard]] auto offset() const noexcept
  {
    return m_offset;
  }
  [[nodiscard]] auto index() const noexcept
  {
    return m_index;
  }
  friend std::ostream &operator<<(
    std::ostream &os, const ComplexStringSectionOffsets &item)
  {
    return os << '{' << item.index() << ", " << item.offset() << '}';
  }
};
struct ComplexStringSection
{
  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_mngrp_complex_strings
private:
  std::uint32_t m_count{};
  std::span<const char> m_buffer{};
  static constexpr auto SECTION_COUNT = 6U;
  std::array<std::string_view, SECTION_COUNT> m_data{};

public:
  ComplexStringSection([[maybe_unused]] const std::span<const char> &buffer,
    const std::array<std::string_view, SECTION_COUNT> &data)
    : m_data{ data }
  {
    if (std::ranges::size(buffer) > sizeof(m_count)) {
      std::memcpy(&m_count, std::ranges::data(buffer), sizeof(m_count));
      m_buffer = buffer.subspan(sizeof(m_count));
    }
  }
  [[nodiscard]] auto size() const
  {
    return m_count;
  }
  [[nodiscard]] ComplexStringSectionOffsets at(const size_t &id) const
  {
    ComplexStringSectionOffsets d{};
    if (id >= m_count || sizeof(d) * id >= std::ranges::size(m_buffer)) {
      return {};// throw?
    }
    memcpy(&d, m_buffer.data() + (sizeof(d) * id), sizeof(d));
    return d;
  }

  [[nodiscard]] auto at(const ComplexStringSectionOffsets &offsets) const
  {
    const auto temp = m_data.at(offsets.index());
    return ComplexStringSectionEntry{ std::string_view{
      temp.data() + offsets.offset(), temp.size() - offsets.offset() } };
  }
};
}// namespace open_viii::menu_group
#endif// VIIIARCHIVE_COMPLEXSTRINGSECTION_HPP
