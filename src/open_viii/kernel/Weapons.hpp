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
#include "CommonKernel.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include "RenzokukenFinishersT.hpp"
namespace open_viii::kernel {
struct Weapons_impl
{
  /**
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
   * @see https://github.com/DarkShinryu/doomtrain/wiki/Weapons
   */
protected:
  EncodedStringOffset  m_name_offset          = {};
  RenzokukenFinishersT m_renzokuken_finishers = {};
  std::uint8_t         m_unknown              = {};
  CharactersT          m_character_id         = {};
  AttackTypeT          m_attack_type          = {};
  std::uint8_t         m_attack_power         = {};
  std::uint8_t         m_attack_parameter     = {};
  std::uint8_t         m_str_bonus            = {};
  std::uint8_t         m_weapon_tier          = {};
  std::uint8_t         m_critical_bonus       = {};
  std::uint8_t         m_melee_weapon         = {};
  constexpr Weapons_impl()                    = default;
  static constexpr std::size_t EXPECTED_SIZE  = 12U;

public:
  constexpr auto
    operator<=>(const Weapons_impl &right) const noexcept = default;
};
using Weapons = CommonKernel<Weapons_impl>;
static_assert(Weapons::EXPECTED_SIZE == sizeof(Weapons));
static_assert(has_name_offset<Weapons>);
static_assert(has_renzokuken_finishers<Weapons>);
static_assert(has_unknown<Weapons>);
static_assert(has_character_id<Weapons>);
static_assert(has_attack_type<Weapons>);
static_assert(has_attack_power<Weapons>);
static_assert(has_attack_parameter<Weapons>);
static_assert(has_str_bonus<Weapons>);
static_assert(has_weapon_tier<Weapons>);
static_assert(has_critical_bonus<Weapons>);
static_assert(has_melee_weapon<Weapons>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_WEAPONS_HPP
