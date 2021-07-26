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
#ifndef VIIIARCHIVE_RINOALIMITBREAKPART2_HPP
#define VIIIARCHIVE_RINOALIMITBREAKPART2_HPP
#include "AttackFlagsT.hpp"
#include "AttackTypeT.hpp"
#include "BattleOnlyStatusesT.hpp"
#include "CommonKernel.hpp"
#include "ElementT.hpp"
#include "PersistentStatusesT.hpp"
#include "TargetT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <compare>
namespace open_viii::kernel {
/**
 * Offset	Length	Description
 * 0x0000	2 bytes	Offset to limit name
 * 0x0002	2 bytes	Magic ID
 * 0x0004	1 byte	Attack type
 * 0x0005	1 byte	Attack power
 * 0x0006	1 byte	Attack flags
 * 0x0007	1 byte	Unknown
 * 0x0008	1 byte	Target info
 * 0x0009	1 byte	Unknown
 * 0x000A	1 byte	Hit Count
 * 0x000B	1 byte	Element Attack
 * 0x000C	1 byte	Element Attack %
 * 0x000D	1 byte	Status Attack Enabler
 * 0x000E	2 bytes	status_0; //statuses 0-7
 * 0x0010	4 bytes	status_1; //statuses 8-39
 * @see
 * https://github.com/DarkShinryu/doomtrain/wiki/Rinoa-limit-breaks-(part-2)
 */
struct RinoaLimitBreakPart2_impl
{
protected:
  EncodedStringOffset m_name_offset            = {};
  std::uint16_t       m_magic_id               = {};
  AttackTypeT         m_attack_type            = {};
  std::uint8_t        m_attack_power           = {};
  AttackFlagsT        m_attack_flags           = {};
  std::uint8_t        m_unknown0               = {};
  TargetT             m_target                 = {};
  std::uint8_t        m_unknown1               = {};
  std::uint8_t        m_hit_count              = {};
  ElementT            m_element                = {};
  std::uint8_t        m_element_attack_percent = {};
  std::uint8_t        m_status_attack_enabler  = {};
  PersistentStatusesT m_persistent_statuses    = {};// statuses 0-7
  BattleOnlyStatusesT m_battle_only_statuses   = {};// statuses 8-39
  constexpr RinoaLimitBreakPart2_impl()        = default;
  static constexpr std::size_t EXPECTED_SIZE   = 20U;

public:
  constexpr auto
    operator<=>(
      const RinoaLimitBreakPart2_impl &right) const noexcept = default;
};
using RinoaLimitBreakPart2 = CommonKernel<RinoaLimitBreakPart2_impl>;
static_assert(RinoaLimitBreakPart2::EXPECTED_SIZE
              == sizeof(RinoaLimitBreakPart2));
static_assert(has_name_offset<RinoaLimitBreakPart2>);
static_assert(has_magic_id<RinoaLimitBreakPart2>);
static_assert(has_attack_type<RinoaLimitBreakPart2>);
static_assert(has_attack_power<RinoaLimitBreakPart2>);
static_assert(has_attack_flags<RinoaLimitBreakPart2>);
static_assert(has_unknown0<RinoaLimitBreakPart2>);
static_assert(has_target<RinoaLimitBreakPart2>);
static_assert(has_unknown1<RinoaLimitBreakPart2>);
static_assert(has_hit_count<RinoaLimitBreakPart2>);
static_assert(has_element<RinoaLimitBreakPart2>);
static_assert(has_element_attack_percent<RinoaLimitBreakPart2>);
static_assert(has_status_attack_enabler<RinoaLimitBreakPart2>);
static_assert(has_persistent_statuses<RinoaLimitBreakPart2>); // statuses 0-7
static_assert(has_battle_only_statuses<RinoaLimitBreakPart2>);// statuses 8-39
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_RINOALIMITBREAKPART2_HPP
