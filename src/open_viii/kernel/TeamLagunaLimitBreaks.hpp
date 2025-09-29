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
#ifndef VIIIARCHIVE_TEAMLAGUNALIMITBREAKS_HPP
#define VIIIARCHIVE_TEAMLAGUNALIMITBREAKS_HPP
#include "AttackFlagsT.hpp"
#include "AttackTypeT.hpp"
#include "BattleOnlyStatusesT.hpp"
#include "CommonKernel.hpp"
#include "ElementT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include "PersistentStatusesT.hpp"
#include "TargetT.hpp"
namespace open_viii::kernel {
/**
 * Offset	Length	Description
 * 0x0000	2 bytes	Offset to limit name
 * 0x0002	2 bytes	Offset to limit description
 * 0x0004	2 bytes	Magic ID
 * 0x0006	1 byte	Attack Type
 * 0x0007	1 byte	Attack Power
 * 0x0008	2 bytes	Unknown
 * 0x000A	1 byte	Target Info
 * 0x000B	1 byte	Attack Flags
 * 0x000C	1 byte	Hit Count
 * 0x000D	1 byte	Element Attack
 * 0x000E	1 byte	Element Attack %
 * 0x000F	1 byte	Status Attack Enabler
 * 0x0010	2 bytes	status_0; //statuses 0-7
 * 0x0012	2 bytes	Unknown
 * 0x0014	4 bytes	status_1; //statuses 8-39
 * @see
 * https://github.com/DarkShinryu/doomtrain/wiki/Temporary-character-limit-breaks
 */
struct TeamLagunaLimitBreaks_impl
{
protected:
  EncodedStringOffset m_name_offset            = {};
  EncodedStringOffset m_description_offset     = {};
  std::uint16_t       m_magic_id               = {};
  AttackTypeT         m_attack_type            = {};
  std::uint8_t        m_attack_power           = {};
  std::uint8_t        m_unknown0               = {};
  std::uint8_t        m_unknown1               = {};
  TargetT             m_target                 = {};
  AttackFlagsT        m_attack_flags           = {};
  std::uint8_t        m_hit_count              = {};
  ElementT            m_element                = {};
  std::uint8_t        m_element_attack_percent = {};
  std::uint8_t        m_status_attack_enabler  = {};
  PersistentStatusesT m_persistent_statuses    = {};// statuses 0-7
  std::uint8_t        m_unknown2               = {};
  std::uint8_t        m_unknown3               = {};
  BattleOnlyStatusesT m_battle_only_statuses   = {};// statuses 8-39
  constexpr TeamLagunaLimitBreaks_impl()       = default;
  static constexpr std::size_t EXPECTED_SIZE   = 24U;

public:
  constexpr auto
    operator<=>(const TeamLagunaLimitBreaks_impl &right) const noexcept
    = default;
};
using TeamLagunaLimitBreaks = CommonKernel<TeamLagunaLimitBreaks_impl>;
static_assert(
  TeamLagunaLimitBreaks::EXPECTED_SIZE == sizeof(TeamLagunaLimitBreaks));
static_assert(has_name_offset<TeamLagunaLimitBreaks>);
static_assert(has_description_offset<TeamLagunaLimitBreaks>);
static_assert(has_magic_id<TeamLagunaLimitBreaks>);
static_assert(has_attack_type<TeamLagunaLimitBreaks>);
static_assert(has_attack_power<TeamLagunaLimitBreaks>);
static_assert(has_unknown0<TeamLagunaLimitBreaks>);
static_assert(has_unknown1<TeamLagunaLimitBreaks>);
static_assert(has_target<TeamLagunaLimitBreaks>);
static_assert(has_attack_flags<TeamLagunaLimitBreaks>);
static_assert(has_hit_count<TeamLagunaLimitBreaks>);
static_assert(has_element<TeamLagunaLimitBreaks>);
static_assert(has_element_attack_percent<TeamLagunaLimitBreaks>);
static_assert(has_status_attack_enabler<TeamLagunaLimitBreaks>);
static_assert(has_persistent_statuses<TeamLagunaLimitBreaks>);// statuses 0-7
static_assert(has_unknown2<TeamLagunaLimitBreaks>);
static_assert(has_unknown3<TeamLagunaLimitBreaks>);
static_assert(has_battle_only_statuses<TeamLagunaLimitBreaks>);// statuses 8-39
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_TEAMLAGUNALIMITBREAKS_HPP
