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
#include "CommonKernel.hpp"
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
struct NonJunctionableGFs_impl
{
protected:
  EncodedStringOffset          m_name_offset           = {};
  std::uint16_t                m_magic_id              = {};
  AttackTypeT                  m_attack_type           = {};
  /** Used in damage formula */
  std::uint8_t                 m_gf_power              = {};
  std::uint8_t                 m_status_attack_enabler = {};
  std::uint8_t                 m_unknown0              = {};
  std::uint8_t                 m_status_flags          = {};
  std::uint8_t                 m_unknown1              = {};
  std::uint8_t                 m_unknown2              = {};
  ElementT                     m_element               = {};
  BattleOnlyStatusesT          m_battle_only_statuses  = {};
  PersistentStatusesT          m_persistent_statuses   = {};
  // std::uint8_t Unknown3_={}; // assuming this is part of persistent statuses.
  /** Used in damage formula */
  std::uint8_t                 m_power_mod             = {};
  /** Used in damage formula */
  std::uint8_t                 m_level_mod             = {};
  static constexpr std::size_t EXPECTED_SIZE           = 20U;
  constexpr NonJunctionableGFs_impl()                  = default;

public:
  constexpr auto
    operator<=>(const NonJunctionableGFs_impl &right) const noexcept = default;
};
using NonJunctionableGFs = CommonKernel<NonJunctionableGFs_impl>;
static_assert(sizeof(NonJunctionableGFs) == NonJunctionableGFs::EXPECTED_SIZE);
static_assert(has_name_offset<NonJunctionableGFs>);
static_assert(has_magic_id<NonJunctionableGFs>);
static_assert(has_attack_type<NonJunctionableGFs>);
static_assert(has_gf_power<NonJunctionableGFs>);
static_assert(has_status_attack_enabler<NonJunctionableGFs>);
static_assert(has_unknown0<NonJunctionableGFs>);
static_assert(has_status_flags<NonJunctionableGFs>);
static_assert(has_unknown1<NonJunctionableGFs>);
static_assert(has_unknown2<NonJunctionableGFs>);
static_assert(has_element<NonJunctionableGFs>);
static_assert(has_battle_only_statuses<NonJunctionableGFs>);
static_assert(has_persistent_statuses<NonJunctionableGFs>);
static_assert(has_power_mod<NonJunctionableGFs>);
static_assert(has_level_mod<NonJunctionableGFs>);

}// namespace open_viii::kernel
#endif// VIIIARCHIVE_NONJUNCTIONABLEGFS_HPP
