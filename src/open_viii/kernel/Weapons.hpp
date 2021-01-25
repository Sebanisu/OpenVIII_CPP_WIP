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

#ifndef VIIIARCHIVE_WEAPONS_HPP
#define VIIIARCHIVE_WEAPONS_HPP

#include "AttackTypeT.hpp"
#include "CharactersT.hpp"
#include "RenzokukenFinishersT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <cstdint>

namespace open_viii::kernel {
template<LangT langVal> struct Weapons
{
  /* https://github.com/DarkShinryu/doomtrain/wiki/Weapons
   * Section Structure
   * Offset	Length	Description
   * 0x0000	2 bytes	Offset to weapon name
   * 0x0002	1 byte	Renzokuken finishers
   *    0x01 = Rough Divide
   *    0x02 = Fated Circle
   *    0x04 = Blasting Zone
   *    0x08 = Lion Heart
   * 0x0003	1 byte	Unknown
   * 0x0004	1 byte	Character ID
   *    0x00 - Squall
   *    0x01 - Zell
   *    0x02 - Irvine
   *    0x03 - Quistis
   *    0x04 - Rinoa
   *    0x05 - Selphie
   *    0x06 - Seifer
   *    0x07 - Edea
   *    0x08 - Laguna
   *    0x09 - Kiros
   *    0x0A - Ward
   * 0x0005	1 bytes	Attack Type
   * 0x0006	1 byte	Attack Power
   * 0x0007	1 byte	Attack Parameter
   * 0x0008	1 byte	STR Bonus
   * 0x0009	1 byte	Weapon Tier
   * 0x000A	1 byte	Crit Bonus
   * 0x000B	1 byte	Melee Weapon?
   */
private:
  EncodedStringOffset m_name_offset{};
  RenzokukenFinishersT m_renzokuken_finishers{};
  std::uint8_t m_unknown{};
  CharactersT m_character_id{};
  AttackTypeT m_attack_type{};
  std::uint8_t m_attack_power{};
  std::uint8_t m_attack_parameter{};
  std::uint8_t m_str_bonus{};
  std::uint8_t m_weapon_tier{};
  std::uint8_t m_critical_bonus{};
  std::uint8_t m_melee_weapon{};

public:
  [[nodiscard]] const auto &name_offset() const noexcept
  {
    return m_name_offset;
  }
  [[maybe_unused]] [[nodiscard]] const auto &
    renzokuken_finishers() const noexcept
  {
    return m_renzokuken_finishers;
  }
  [[nodiscard]] const auto &unknown() const noexcept
  {
    return m_unknown;
  }
  [[maybe_unused]] [[nodiscard]] const auto &character_id() const noexcept
  {
    return m_character_id;
  }
  [[nodiscard]] const auto &attack_type() const noexcept
  {
    return m_attack_type;
  }
  [[maybe_unused]] [[nodiscard]] const auto &attack_power() const noexcept
  {
    return m_attack_power;
  }
  [[maybe_unused]] [[nodiscard]] const auto &attack_parameter() const noexcept
  {
    return m_attack_parameter;
  }
  [[maybe_unused]] [[nodiscard]] const auto &str_bonus() const noexcept
  {
    return m_str_bonus;
  }
  [[maybe_unused]] [[nodiscard]] const auto &weapon_tier() const noexcept
  {
    return m_weapon_tier;
  }
  [[maybe_unused]] [[nodiscard]] const auto &critical_bonus() const noexcept
  {
    return m_critical_bonus;
  }
  [[maybe_unused]] [[nodiscard]] const auto &melee_weapon() const noexcept
  {
    return m_melee_weapon != 0;
  }
  std::ostream &out(std::ostream &os, const std::span<const char> &buffer) const
  {
    auto name = m_name_offset.decoded_string<langVal>(buffer);
    if (!std::empty(name)) {
      os << tools::u8_to_sv(name);
    }
    return os << ", " << static_cast<std::uint32_t>(m_renzokuken_finishers)
              << ", " << static_cast<std::uint32_t>(m_unknown) << ", "
              << static_cast<std::uint32_t>(m_character_id) << ", "
              << static_cast<std::uint32_t>(m_attack_type) << ", "
              << static_cast<std::uint32_t>(m_attack_power) << ", "
              << static_cast<std::uint32_t>(m_attack_parameter) << ", "
              << static_cast<std::uint32_t>(m_str_bonus) << ", "
              << static_cast<std::uint32_t>(m_weapon_tier) << ", "
              << static_cast<std::uint32_t>(m_critical_bonus) << ", "
              << static_cast<std::uint32_t>(m_melee_weapon);
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_WEAPONS_HPP
