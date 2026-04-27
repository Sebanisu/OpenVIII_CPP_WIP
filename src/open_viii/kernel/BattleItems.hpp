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
#ifndef VIIIARCHIVE_BATTLEITEMS_HPP
#define VIIIARCHIVE_BATTLEITEMS_HPP
#include "AttackFlagsT.hpp"
#include "AttackTypeT.hpp"
#include "BattleOnlyStatusesT.hpp"
#include "CommonKernel.hpp"
#include "ElementT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include "PersistentStatusesT.hpp"
#include "TargetT.hpp"
namespace open_viii::kernel {
struct BattleItems_impl
{
  /**
   * Offset	Length	Description
   * 0x0000	2 bytes	Offset to item name
   * 0x0002	2 bytes	Offset to item description
   * 0x0004	2 bytes	Magic ID
   * 0x0006	1 byte	Attack type
   * 0x0007	1 byte	Attack power
   * 0x0008	1 byte	Unknown
   * 0x0009	1 byte	Target info
   * 0x000A	1 byte	Unknown
   * 0x000B	1 byte	Attack flags
   * 0x000C	1 bytes	Unknown
   * 0x000D	1 byte	Status Attack Enabler
   * 0x000E	2 bytes	status_0; //statuses 0-7
   * 0x0010	4 bytes	status_1; //statuses 8-39
   * 0x0014	1 byte	Attack Param
   * 0x0015	1 byte	Unknown
   * 0x0016	1 bytes	Hit Count
   * 0x0017	1 bytes	Element
   * @see https://github.com/DarkShinryu/doomtrain/wiki/Battle-items
   */
protected:
  EncodedStringOffset          m_name_offset           = {};
  EncodedStringOffset          m_description_offset    = {};
  std::uint16_t                m_magic_id              = {};
  AttackTypeT                  m_attack_type           = {};
  std::uint8_t                 m_attack_power          = {};
  std::uint8_t                 m_unknown0              = {};
  TargetT                      m_target                = {};
  std::uint8_t                 m_unknown1              = {};
  AttackFlagsT                 m_attack_flags          = {};
  std::uint8_t                 m_unknown2              = {};
  std::uint8_t                 m_status_attack_enabler = {};
  PersistentStatusesT          m_persistent_statuses   = {};// statuses 0-7
  BattleOnlyStatusesT          m_battle_only_statuses  = {};// statuses 8-39
  std::uint8_t                 m_attack_param          = {};
  std::uint8_t                 m_unknown3              = {};
  std::uint8_t                 m_hit_count             = {};
  ElementT                     m_element               = {};
  static constexpr std::size_t EXPECTED_SIZE           = 24U;
  constexpr BattleItems_impl()                         = default;

public:
  constexpr auto
    operator<=>(const BattleItems_impl &right) const noexcept = default;
};
using BattleItems = CommonKernel<BattleItems_impl>;
static_assert(sizeof(BattleItems) == BattleItems::EXPECTED_SIZE);
static_assert(has_name_offset<BattleItems>);
static_assert(has_description_offset<BattleItems>);
static_assert(has_magic_id<BattleItems>);
static_assert(has_attack_type<BattleItems>);
static_assert(has_attack_power<BattleItems>);
static_assert(has_unknown0<BattleItems>);
static_assert(has_target<BattleItems>);
static_assert(has_unknown1<BattleItems>);
static_assert(has_attack_flags<BattleItems>);
static_assert(has_unknown2<BattleItems>);
static_assert(has_status_attack_enabler<BattleItems>);
static_assert(has_persistent_statuses<BattleItems>); // statuses 0-7
static_assert(has_battle_only_statuses<BattleItems>);// statuses 8-39
static_assert(has_attack_param<BattleItems>);
static_assert(has_unknown3<BattleItems>);
static_assert(has_hit_count<BattleItems>);
static_assert(has_element<BattleItems>);
static_assert(BattleItems().target() == TargetT::none);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_BATTLEITEMS_HPP
