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
#ifndef VIIIARCHIVE_DEVOUR_HPP
#define VIIIARCHIVE_DEVOUR_HPP
#include "BattleOnlyStatusesT.hpp"
#include "DevourStatFlagT.hpp"
#include "PercentQuantityT.hpp"
#include "PersistentStatusesT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <compare>
#include <ratio>
namespace open_viii::kernel {
/**
 * Offset	Length	Description
 * 0x0000	2 bytes	Offset to devour description
 * 0x0002	1 byte	Damage or heal HP and Status
 *   0x1E - Cure
 *   0x1F - Damage
 * 0x0003	1 byte	HP Heal/DMG Quantity Flag
 *   0x00 - 0%
 *   0x01 - 6.25%
 *   0x02 - 12.50%
 *   0x04 - 25%
 *   0x08 - 50%
 *   0x10 - 100%
 * 0x0004	4 bytes	status_1; //statuses 8-39
 * 0x0008	2 bytes	status_0; //statuses 0-7
 * 0x000A	1 byte	Raised Stat Flag
 *   0x00 - None
 *   0x01 - STR
 *   0x02 - VIT
 *   0x04 - MAG
 *   0x08 - SPR
 *   0x10 - SPD
 * 0x000B	1 byte	Raised Stat HP Quantity
 * @see
 * https://github.com/DarkShinryu/doomtrain/wiki/Devour
 * */
struct Devour_impl
{
private:
  static constexpr auto full      = 1.0F;
  static constexpr auto half      = 1.0F / 2.0F;
  static constexpr auto quarter   = 1.0F / 4.0F;
  static constexpr auto eighth    = 1.0F / 8.0F;
  static constexpr auto sixteenth = 1.0F / 16.0F;

protected:
  EncodedStringOffset          m_description_offset      = {};
  /**
   * HP and Status; If last on right bit is set heal, and if not dmg. Rest looks
   * like 0b‭00011110‬.
   */
  std::uint8_t                 m_damage_or_heal          = {};
  PercentQuantityT             m_percent_quantity        = {};
  BattleOnlyStatusesT          m_battle_only_statuses    = {};// statuses 8-39
  PersistentStatusesT          m_persistent_statuses     = {};// statuses 0-7
  DevourStatFlagT              m_devour_stat_flag        = {};
  std::uint8_t                 m_raised_stat_hp_quantity = {};
  static constexpr std::size_t EXPECTED_SIZE             = 10U;
  Devour_impl()                                          = default;
  [[nodiscard]] constexpr auto
    damage_or_heal_impl() const noexcept
  {
    return (m_damage_or_heal & 0x1U) == 0;
  }
  [[nodiscard]] constexpr auto
    percent_quantity_impl() const noexcept
  {
    const auto flag_set = [this](const PercentQuantityT &flag) {
      return (static_cast<uint8_t>(m_percent_quantity)
              & static_cast<uint8_t>(flag))
          != 0;
    };
    float out{};
    if (flag_set(PercentQuantityT::full)) {
      out += full;
    }
    if (flag_set(PercentQuantityT::half)) {
      out += half;
    }
    if (flag_set(PercentQuantityT::quarter)) {
      out += quarter;
    }
    if (flag_set(PercentQuantityT::eighth)) {
      out += eighth;
    }
    if (flag_set(PercentQuantityT::sixteenth)) {
      out += sixteenth;
    }
    return out;
  }

public:
  constexpr auto
    operator<=>(const Devour_impl &right) const noexcept = default;
};
using Devour = CommonKernel<Devour_impl>;
static_assert(has_description_offset<Devour>);
static_assert(has_damage_or_heal<Devour>);
static_assert(has_percent_quantity<Devour>);
static_assert(has_battle_only_statuses<Devour>);
static_assert(has_persistent_statuses<Devour>);
static_assert(has_devour_stat_flag<Devour>);
static_assert(has_raised_stat_hp_quantity<Devour>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_DEVOUR_HPP
