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
#ifndef VIIIARCHIVE_NONJUNCTIONABLEGFS_HPP
#define VIIIARCHIVE_NONJUNCTIONABLEGFS_HPP
#include "AttackTypeT.hpp"
#include "BattleOnlyStatusesT.hpp"
#include "ElementT.hpp"
#include "PersistentStatusesT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <compare>
namespace open_viii::kernel {
/**
 * Offset	Length	Description
 * 0x0000	2 bytes	Offset to GF attack name
 * 0x0002	2 bytes	Magic ID (decides what animation to play)
 * 0x0004	1 byte	Attack type
 * 0x0005	1 byte	GF power (used in damage formula)
 * 0x0006	1 byte	Status attack enabler
 * 0x0007	1 byte	Unknown
 * 0x0008	1 byte	Status flags
 * 0x0009	2 bytes	Unknown
 * 0x000B	1 byte	Element
 *  0x00 - Non-Elemental
 *  0x01 - Fire
 *  0x02 - Ice
 *  0x04 - Thunder
 *  0x08 - Earth
 *  0x10 - Poison
 *  0x20 - Wind
 *  0x40 - Water
 *  0x80 - Holy
 * 0x000C	1 byte	Status 1
 *  0x00 - None
 *  0x01 - Sleep
 *  0x02 - Haste
 *  0x04 - Slow
 *  0x08 - Stop
 *  0x10 - Regen
 *  0x20 - Protect
 *  0x40 - Shell
 *  0x80 - Reflect
 * 0x000D	1 byte	Status 2
 *  0x00 - None
 *  0x01 - Aura
 *  0x02 - Curse
 *  0x04 - Doom
 *  0x08 - Invincible
 *  0x10 - Petrifying
 *  0x20 - Float
 *  0x40 - Confusion
 *  0x80 - Drain
 * 0x000E	1 byte	Status 3
 *  0x00 - None
 *  0x01 - Eject
 *  0x02 - Double
 *  0x04 - Triple
 *  0x08 - Defend
 *  0x10 - ???
 *  0x20 - ???
 *  0x40 - ???
 *  0x80 - ???
 * 0x000F	1 byte	Status 4
 *  0x00 - None
 *  0x01 - Vit0
 *  0x02 - ???
 *  0x04 - ???
 *  0x08 - ???
 *  0x10 - ???
 *  0x20 - ???
 *  0x40 - ???
 *  0x80 - ???
 * 0x0010	1 byte	Status 5
 *  0x00 - None
 *  0x01 - Death
 *  0x02 - Poison
 *  0x04 - Petrify
 *  0x08 - Darkness
 *  0x10 - Silence
 *  0x20 - Berserk
 *  0x40 - Zombie
 *  0x80 - ???
 * 0x0011	1 byte	Unknown
 * 0x0012	1 byte	Power Mod (used in damage formula)
 * 0x0013	1 byte	Level Mod (used in damage formula)
 * @see
 * https://github.com/DarkShinryu/doomtrain/wiki/Non-junctionable-GF-attacks
 */
struct NonJunctionableGFs
{
private:
  EncodedStringOffset m_name_offset{};
  std::uint16_t       m_magic_id{};
  AttackTypeT         m_attack_type{};
  std::uint8_t        m_gf_power{};// (used in damage formula)
  std::uint8_t        m_status_attack_enabler{};
  std::uint8_t        m_unknown0{};
  std::uint8_t        m_status_flags{};
  std::uint8_t        m_unknown1{};
  std::uint8_t        m_unknown2{};
  ElementT            m_element{};
  BattleOnlyStatusesT m_battle_only_statuses{};
  PersistentStatusesT m_persistent_statuses{};
  // std::uint8_t Unknown3_{}; // assuming this is part of persistent statuses.
  std::uint8_t m_power_mod{};// (used in damage formula)
  std::uint8_t m_level_mod{};// (used in damage formula)
public:
  constexpr auto
    operator<=>(const NonJunctionableGFs &right) const noexcept = default;
  [[nodiscard]] constexpr auto name_offset() const noexcept
  {
    return m_name_offset;
  }
  [[nodiscard]] constexpr auto magic_id() const noexcept
  {
    return m_magic_id;
  }
  [[nodiscard]] constexpr auto attack_type() const noexcept
  {
    return m_attack_type;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto gf_power() const noexcept
  {
    return m_gf_power;
  }// (used in damage formula)
  [[nodiscard]] constexpr auto status_attack_enabler() const noexcept
  {
    return m_status_attack_enabler;
  }
  [[nodiscard]] constexpr auto unknown0() const noexcept
  {
    return m_unknown0;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto status_flags() const noexcept
  {
    return m_status_flags;
  }
  [[nodiscard]] constexpr auto unknown1() const noexcept
  {
    return m_unknown1;
  }
  [[nodiscard]] constexpr auto unknown2() const noexcept
  {
    return m_unknown2;
  }
  [[nodiscard]] constexpr auto element() const noexcept
  {
    return m_element;
  }
  [[nodiscard]] constexpr auto battle_only_statuses() const noexcept
  {
    return m_battle_only_statuses;
  }
  [[nodiscard]] constexpr auto persistent_statuses() const noexcept
  {
    return m_persistent_statuses;
  }
  // [[nodiscard]] constexpr auto  Unknown3() const noexcept { return Unknown3_;
  // } // assuming this is part of persistent statuses.
  [[nodiscard]] constexpr auto power_mod() const noexcept
  {
    return m_power_mod;
  }// (used in damage formula)
  [[maybe_unused]] [[nodiscard]] constexpr auto level_mod() const noexcept
  {
    return m_level_mod;
  }// (used in damage formula)
  std::ostream &out(std::ostream &                                os,
                    [[maybe_unused]] const std::span<const char> &buffer) const
  {
    return os
           << ", " << static_cast<std::uint32_t>(m_magic_id) << ", "
           << static_cast<std::uint32_t>(m_attack_type) << ", "
           << static_cast<std::uint32_t>(m_gf_power)// (used in damage formula)
           << ", " << static_cast<std::uint32_t>(m_status_attack_enabler)
           << ", " << static_cast<std::uint32_t>(m_unknown0) << ", "
           << static_cast<std::uint32_t>(m_status_flags) << ", "
           << static_cast<std::uint32_t>(m_unknown1) << ", "
           << static_cast<std::uint32_t>(m_unknown2) << ", "
           << static_cast<std::uint32_t>(m_element) << ", "
           << static_cast<std::uint32_t>(m_battle_only_statuses) << ", "
           << static_cast<std::uint32_t>(m_persistent_statuses) << ", "
           << static_cast<std::uint32_t>(m_power_mod)// (used in damage formula)
           << ", "
           << static_cast<std::uint32_t>(m_level_mod)// (used in damage formula)
      ;
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_NONJUNCTIONABLEGFS_HPP
