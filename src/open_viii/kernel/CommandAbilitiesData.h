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
#ifndef VIIIARCHIVE_COMMANDABILITIESDATA_H
#define VIIIARCHIVE_COMMANDABILITIESDATA_H
#include "AttackFlagsT.hpp"
#include "AttackTypeT.hpp"
#include "BattleOnlyStatusesT.hpp"
#include "CommonKernel.hpp"
#include "ElementT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include "PersistentStatusesT.hpp"
namespace open_viii::kernel {
/**
 * 0x0000	2 bytes	Magic ID
 * 0x0002	2 bytes	Unknown
 * 0x0004	1 byte	Attack type
 * 0x0005	1 byte	Attack power
 * 0x0006	1 byte	Attack flags
 * 0x0007	1 byte	Hit Count
 * 0x0008	1 byte	Element
 * 0x0009	1 byte	Status attack enabler
 * 0x000A	6 bytes	Statuses
 *  public PersistentStatuses Statuses0 { get;  }
 *  public BattleOnlyStatuses Statuses1 { get;  }
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Command-ability-data
 */
struct CommandAbilitiesData_impl
{
protected:
  std::uint16_t       m_magic_id              = {};
  std::uint16_t       m_unknown               = {};
  AttackTypeT         m_attack_type           = {};
  std::uint8_t        m_attack_power          = {};
  AttackFlagsT        m_attack_flags          = {};
  std::uint8_t        m_hit_count             = {};
  ElementT            m_element               = {};
  std::uint8_t        m_status_attack_enabler = {};
  PersistentStatusesT m_persistent_statuses   = {};
  BattleOnlyStatusesT m_battle_only_statuses  = {};
  constexpr CommandAbilitiesData_impl()       = default;
  static constexpr std::size_t EXPECTED_SIZE  = 16U;

public:
  constexpr auto
    operator<=>(
      const CommandAbilitiesData_impl &right) const noexcept = default;
};
using CommandAbilitiesData = CommonKernel<CommandAbilitiesData_impl>;
static_assert(
  sizeof(CommandAbilitiesData) == CommandAbilitiesData::EXPECTED_SIZE);
static_assert(has_magic_id<CommandAbilitiesData>);
static_assert(has_unknown<CommandAbilitiesData>);
static_assert(has_attack_type<CommandAbilitiesData>);
static_assert(has_attack_power<CommandAbilitiesData>);
static_assert(has_attack_flags<CommandAbilitiesData>);
static_assert(has_hit_count<CommandAbilitiesData>);
static_assert(has_element<CommandAbilitiesData>);
static_assert(has_status_attack_enabler<CommandAbilitiesData>);
static_assert(has_persistent_statuses<CommandAbilitiesData>);
static_assert(has_battle_only_statuses<CommandAbilitiesData>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_COMMANDABILITIESDATA_H
