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
#ifndef VIIIARCHIVE_ZELLDUELLIMITBREAK_HPP
#define VIIIARCHIVE_ZELLDUELLIMITBREAK_HPP
#include "AttackFlagsT.hpp"
#include "AttackTypeT.hpp"
#include "BattleOnlyStatusesT.hpp"
#include "CommonKernel.hpp"
#include "ElementT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include "PersistentStatusesT.hpp"
#include "TargetT.hpp"
#include "ZellDuelButtonT.hpp"
#include <array>
namespace open_viii::kernel {
/**
 * Offset	Length	Description
 * 0x0000	2 bytes	Offset to limit name
 * 0x0002	2 bytes	Offset to limit description
 * 0x0004	2 bytes	Magic ID
 * 0x0006	1 byte	Attack type
 * 0x0007	1 byte	Attack power
 * 0x0008	1 byte	Attack flags
 * 0x0009	1 byte	Unknown
 * 0x000A	1 byte	Target Info
 * 0x000B	1 bytes	Unknown
 * 0x000C	1 byte	Hit count
 * 0x000D	1 byte	Element Attack
 * 0x000E	1 byte	Element Attack %
 * 0x000F	1 byte	Status Attack Enabler
 * 0x0010	2 bytes	Sequence Button 1
 * 0x0012	2 bytes	Sequence Button 2
 * 0x0014	2 bytes	Sequence Button 3
 * 0x0016	2 bytes	Sequence Button 4
 * 0x0018	2 bytes	Sequence Button 5
 * 0x001A	2 bytes	status_0; //statuses 0-7
 * 0x001C	4 bytes	status_1; //statuses 8-39
 * Buttons is
 * finisher = 0x0001
 * up = 0x0010
 * -> = 0x0020
 * do = 0x0040
 * <- = 0x0080
 * L2 = 0x0100
 * R2 = 0x0200
 * L1 = 0x0400
 * R1 = 0x0800
 * /\ = 0x1000
 * O = 0x2000
 * X = 0x4000
 * |_|= 0x8000
 * None = 0xFFFF
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Duel-(Zell-limit-break)
 */
struct ZellDuelLimitBreak_impl
{
protected:
  static constexpr std::size_t MAX_NUMBER_OF_BUTTONS = { 5U };
  static constexpr std::size_t EXPECTED_SIZE         = { 32U };
  constexpr ZellDuelLimitBreak_impl()                = default;
  EncodedStringOffset m_name_offset                  = {};
  EncodedStringOffset m_description_offset           = {};
  std::uint16_t       m_magic_id                     = {};
  AttackTypeT         m_attack_type                  = {};
  std::uint8_t        m_attack_power                 = {};
  AttackFlagsT        m_attack_flags                 = {};
  std::uint8_t        m_unknown0                     = {};
  TargetT             m_target                       = {};
  std::uint8_t        m_unknown1                     = {};
  std::uint8_t        m_hit_count                    = {};
  ElementT            m_element                      = {};
  uint8_t             m_element_attack_percent       = {};
  std::uint8_t        m_status_attack_enabler        = {};
  std::array<ZellDuelButtonT, MAX_NUMBER_OF_BUTTONS> m_button_sequence     = {};
  PersistentStatusesT                                m_persistent_statuses = {};
  BattleOnlyStatusesT m_battle_only_statuses                               = {};

public:
  constexpr auto
    operator<=>(const ZellDuelLimitBreak_impl &right) const noexcept = default;
};
using ZellDuelLimitBreak = CommonKernel<ZellDuelLimitBreak_impl>;
static_assert(ZellDuelLimitBreak::EXPECTED_SIZE == sizeof(ZellDuelLimitBreak));
static_assert(has_name_offset<ZellDuelLimitBreak>);
static_assert(has_description_offset<ZellDuelLimitBreak>);
static_assert(has_magic_id<ZellDuelLimitBreak>);
static_assert(has_attack_type<ZellDuelLimitBreak>);
static_assert(has_attack_power<ZellDuelLimitBreak>);
static_assert(has_attack_flags<ZellDuelLimitBreak>);
static_assert(has_unknown0<ZellDuelLimitBreak>);
static_assert(has_target<ZellDuelLimitBreak>);
static_assert(has_unknown1<ZellDuelLimitBreak>);
static_assert(has_hit_count<ZellDuelLimitBreak>);
static_assert(has_element<ZellDuelLimitBreak>);
static_assert(has_element_attack_percent<ZellDuelLimitBreak>);
static_assert(has_status_attack_enabler<ZellDuelLimitBreak>);
static_assert(has_button_sequence<ZellDuelLimitBreak>);
static_assert(has_persistent_statuses<ZellDuelLimitBreak>);
static_assert(has_battle_only_statuses<ZellDuelLimitBreak>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_ZELLDUELLIMITBREAK_HPP
