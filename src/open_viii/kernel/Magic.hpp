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
#ifndef VIIIARCHIVE_MAGIC_HPP
#define VIIIARCHIVE_MAGIC_HPP
#include "AttackFlagsT.hpp"
#include "AttackTypeT.hpp"
#include "BattleOnlyStatusesT.hpp"
#include "CommonKernel.hpp"
#include "ElementT.hpp"
#include "GFGroup.hpp"
#include "JunctionStatusesT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include "PersistentStatusesT.hpp"
#include "StatGroup.hpp"
#include "TargetT.hpp"
namespace open_viii::kernel {
struct Magic_impl
{
  /**
   * Offset	Length	Description
   * 	2 bytes	Offset to spell name
   * 	2 bytes	Offset to spell description
   * 	2 bytes	[[Magic ID
   * 	1 byte	Unknown
   * 	1 byte	Attack type
   * 	1 byte	Spell power (used in damage formula)
   * 	1 byte	Unknown
   * 	1 byte	Default target
   * 	1 byte	Attack Flags
   * 	1 byte	Draw resist (how hard is the magic to draw)
   * 	1 byte	Hit count (works with meteor animation, not sure about others)
   * 	1 byte	[[Element
   * 	1 byte	Unknown
   * 	4 bytes	[[Statuses 1
   * 	2 bytes	[[Statuses 0
   * 	1 byte	Status attack enabler
   * 	1 byte	Characters HP junction value
   * 	1 byte	Characters STR junction value
   * 	1 byte	Characters VIT junction value
   * 	1 byte	Characters MAG junction value
   * 	1 byte	Characters SPR junction value
   * 	1 byte	Characters SPD junction value
   * 	1 byte	Characters EVA junction value
   * 	1 byte	Characters HIT junction value
   * 	1 byte	Characters LUCK junction value
   * 	1 byte	[[Characters J-Elem attack
   * 	1 byte	Characters J-Elem attack value
   * 	1 byte	[[Characters J-Elem defense
   * 	1 byte	Characters J-Elem defense value
   * 	1 byte	Characters J-Status attack value
   * 	1 byte	Characters J-Status defense value
   * 	2 bytes	[[Characters J-Statuses Attack
   * 	2 bytes	[[Characters J-Statuses Defend
   * 	1 byte	Quezacolt compatibility
   * 	1 byte	Shiva compatibility
   * 	1 byte	Ifrit compatibility
   * 	1 byte	Siren compatibility
   * 	1 byte	Brothers compatibility
   * 	1 byte	Diablos compatibility
   * 	1 byte	Carbuncle compatibility
   * 	1 byte	Leviathan compatibility
   * 	1 byte	Pandemona compatibility
   * 	1 byte	Cerberus compatibility
   * 	1 byte	Alexander compatibility
   * 	1 byte	Doomtrain compatibility
   * 	1 byte	Bahamut compatibility
   * 	1 byte	Cactuar compatibility
   * 	1 byte	Tonberry compatibility
   * 	1 byte	Eden compatibility
   * 	2 bytes	Unknown
   * 	@see https://github.com/DarkShinryu/doomtrain/wiki/Magic-data
   */
protected:
  EncodedStringOffset     m_name_offset            = {};
  EncodedStringOffset     m_description_offset     = {};
  std::uint16_t           m_magic_id               = {};
  std::uint8_t            m_unknown0               = {};
  AttackTypeT             m_attack_type            = {};
  std::uint8_t            m_spell_power            = {};
  std::uint8_t            m_unknown1               = {};
  TargetT                 m_target                 = {};
  AttackFlagsT            m_attack_flags           = {};
  std::uint8_t            m_draw_resist            = {};
  std::uint8_t            m_hit_count              = {};
  ElementT                m_element                = {};
  std::uint8_t            m_unknown2               = {};
  BattleOnlyStatusesT     m_battle_only_statuses   = {};
  PersistentStatusesT     m_persistent_statuses    = {};
  std::uint8_t            m_status_attack          = {};
  StatGroup<std::uint8_t> m_junction_stats         = {};
  ElementT                m_j_elem_attack_flag     = {};
  std::uint8_t            m_j_elem_attack_value    = {};
  ElementT                m_j_elem_defense_flag    = {};
  std::uint8_t            m_j_elem_defense_value   = {};
  std::uint8_t            m_j_status_attack_value  = {};
  std::uint8_t            m_j_status_defense_value = {};
  JunctionStatusesT       m_j_statuses_attack_flag = {};
  JunctionStatusesT       m_j_statuses_defend_flag = {};
  GFGroup<std::uint8_t>   m_compatibility          = {};
  std::uint16_t           m_unknown3               = {};
  constexpr Magic_impl()                           = default;
  static constexpr std::size_t EXPECTED_SIZE       = 60U;

public:
  constexpr auto
    operator<=>(const Magic_impl &right) const noexcept
    = default;
};
using Magic = CommonKernel<Magic_impl>;
static_assert(Magic::EXPECTED_SIZE == sizeof(Magic));
static_assert(has_name_offset<Magic>);
static_assert(has_description_offset<Magic>);
static_assert(has_magic_id<Magic>);
static_assert(has_unknown0<Magic>);
static_assert(has_attack_type<Magic>);
static_assert(has_spell_power<Magic>);
static_assert(has_unknown1<Magic>);
static_assert(has_target<Magic>);
static_assert(has_attack_flags<Magic>);
static_assert(has_draw_resist<Magic>);
static_assert(has_hit_count<Magic>);
static_assert(has_element<Magic>);
static_assert(has_unknown2<Magic>);
static_assert(has_battle_only_statuses<Magic>);
static_assert(has_persistent_statuses<Magic>);
static_assert(has_status_attack<Magic>);
static_assert(has_junction_stats<Magic>);
static_assert(has_j_elem_attack_flag<Magic>);
static_assert(has_j_elem_attack_value<Magic>);
static_assert(has_j_elem_defense_flag<Magic>);
static_assert(has_j_elem_defense_value<Magic>);
static_assert(has_j_status_attack_value<Magic>);
static_assert(has_j_status_defense_value<Magic>);
static_assert(has_j_statuses_attack_flag<Magic>);
static_assert(has_j_statuses_defend_flag<Magic>);
static_assert(has_compatibility<Magic>);
static_assert(has_unknown3<Magic>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_MAGIC_HPP
