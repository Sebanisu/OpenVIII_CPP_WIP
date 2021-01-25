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

#ifndef VIIIARCHIVE_STATPERCENTINCREASEABILITIES_HPP
#define VIIIARCHIVE_STATPERCENTINCREASEABILITIES_HPP
#include "open_viii/strings/EncodedStringOffset.hpp"
namespace open_viii::kernel {
template<LangT langVal> struct StatPercentIncreaseAbilities
{
private:
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Stat-percentage-increasing-abilities
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	AP needed to learn the ability
   * 0x0005	1 byte	Stat to increase
   * 0x0006	1 byte	Increase value
   * 0x0007	1 byte	Unknown/Unused
   */
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  std::uint8_t m_ap_required{};
  std::uint8_t m_stat_to_increase{};
  std::uint8_t m_increased_value{};
  std::uint8_t m_unknown0{};

public:
  [[nodiscard]] auto &name_offset() const noexcept
  {
    return m_name_offset;
  }
  [[nodiscard]] auto &description_offset() const noexcept
  {
    return m_description_offset;
  }

  [[maybe_unused]] [[nodiscard]] auto ap_required() const noexcept
  {
    return m_ap_required;
  }
  [[maybe_unused]] [[nodiscard]] auto stat_to_increase() const noexcept
  {
    return m_stat_to_increase;
  }
  [[maybe_unused]] [[nodiscard]] auto increased_value() const noexcept
  {
    return m_increased_value;
  }
  [[nodiscard]] auto unknown0() const noexcept
  {
    return m_unknown0;
  }
  std::ostream &out(std::ostream &os, const std::span<const char> &buffer) const
  {
    auto name = m_name_offset.decoded_string<langVal>(buffer);
    auto description = m_description_offset.decoded_string<langVal>(buffer);
    if (!std::empty(name)) {
      os << tools::u8_to_sv(name);
    }
    if (!std::empty(description)) {
      os << ", " << tools::u8_to_sv(description);
    }
    return os << ", " << static_cast<std::uint32_t>(m_ap_required) << ", "
              << static_cast<std::uint32_t>(m_stat_to_increase) << ", "
              << static_cast<std::uint32_t>(m_increased_value) << ", "
              << static_cast<std::uint32_t>(m_unknown0);
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_STATPERCENTINCREASEABILITIES_HPP
