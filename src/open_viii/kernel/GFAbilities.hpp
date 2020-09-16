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

#ifndef VIIIARCHIVE_GFABILITIES_HPP
#define VIIIARCHIVE_GFABILITIES_HPP

#include "open_viii/Strings/EncodedStringOffset.hpp"
#include "StatT.hpp"
namespace open_viii::kernel {
template<LangT langVal> struct GFAbilities
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/GF-abilities
   * Offset	Length	Description
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	AP Required to learn ability
   * 0x0005	1 byte	Enable Boost
   * 0x0006	1 byte	Stat to increase
   * 0x0007	1 byte	Increase value
   */
private:
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  uint8_t m_ap_required{};
  uint8_t m_enable_boost{};
  StatT m_stat_to_increase{};
  uint8_t m_increase_value{};

public:
  [[nodiscard]] const auto &name_offset() const noexcept { return m_name_offset; }
  [[nodiscard]] const auto &description_offset() const noexcept { return m_description_offset; }
  [[maybe_unused]] [[nodiscard]] auto ap_required() const noexcept { return m_ap_required; }
  [[maybe_unused]] [[nodiscard]] auto enable_boost() const noexcept { return m_enable_boost != 0; }
  [[maybe_unused]] [[nodiscard]] auto stat_to_increase() const noexcept { return m_stat_to_increase; }
  [[maybe_unused]] [[nodiscard]] auto increase_value() const noexcept { return m_increase_value; }
  std::ostream &out(std::ostream &os, const std::span<const char> &buffer) const
  {
    auto name = m_name_offset.decoded_string<langVal>(buffer);
    auto description = m_description_offset.decoded_string<langVal>(buffer);
    if (!std::empty(name)) {
      os << Tools::u8_to_sv(name);
    }
    if (!std::empty(description)) {
      os << ", " << Tools::u8_to_sv(description);
    }
    os << ", " << static_cast<int>(m_ap_required);
    os << ", " << static_cast<int>(m_enable_boost);
    os << ", " << static_cast<int>(m_stat_to_increase);
    os << ", " << static_cast<int>(m_increase_value);
    return os;
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_GFABILITIES_HPP
