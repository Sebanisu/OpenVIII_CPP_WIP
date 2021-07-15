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
#ifndef VIIIARCHIVE_ENEMYATTACKS_HPP
#define VIIIARCHIVE_ENEMYATTACKS_HPP
#include "AttackFlagsT.hpp"
#include "AttackTypeT.hpp"
#include "BattleOnlyStatusesT.hpp"
#include "CameraChange.hpp"
#include "CommonKernel.hpp"
#include "ElementT.hpp"
#include "PersistentStatusesT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <compare>
namespace open_viii::kernel {
struct EnemyAttacks_impl
{
  /**
   * Offset	Length	Description
   * 0x00	2 bytes	Offset to attack name
   * 0x02	2 bytes	Magic ID
   * 0x04	1 byte	Camera Change
   * 0x05	1 byte	Unknown
   * 0x06	1 byte	Attack type
   * 0x07	1 byte	Attack power
   * 0x08	1 byte	Attack flags
   * 0x09	1 byte	Unknown
   * 0x0A	1 byte	Element
   * 0x0B	1 byte	Unknown
   * 0x0C	1 byte	Status attack enabler
   * 0x0D	1 byte	Attack Parameter
   * 0x0E	2 bytes	status_0; //statuses 0-7
   * 0x10	4 bytes	status_1; //statuses 8-31
   * @see https://github.com/DarkShinryu/doomtrain/wiki/Enemy-attacks
   */
protected:
  EncodedStringOffset m_name_offset           = {};
  uint16_t            m_magic_id              = {};
  CameraChange        m_camera_change         = {};
  uint8_t             m_unknown0              = {};
  AttackTypeT         m_attack_type           = {};
  uint8_t             m_attack_power          = {};
  AttackFlagsT        m_attack_flags          = {};
  uint8_t             m_unknown1              = {};
  ElementT            m_element               = {};
  uint8_t             m_unknown2              = {};
  uint8_t             m_status_attack_enabler = {};
  uint8_t             m_attack_parameter      = {};
  PersistentStatusesT m_persistent_statuses   = {};// statuses 0-7
  BattleOnlyStatusesT m_battle_only_statuses  = {};// statuses 8-31
  constexpr EnemyAttacks_impl()               = default;
  static constexpr std::size_t EXPECTED_SIZE  = 20U;

public:
  constexpr auto
    operator<=>(const EnemyAttacks_impl &right) const noexcept = default;
};
using EnemyAttacks = CommonKernel<EnemyAttacks_impl>;
static_assert(sizeof(EnemyAttacks) == EnemyAttacks::EXPECTED_SIZE);
static_assert(has_name_offset<EnemyAttacks>);
static_assert(has_magic_id<EnemyAttacks>);
static_assert(has_camera_change<EnemyAttacks>);
static_assert(has_unknown0<EnemyAttacks>);
static_assert(has_attack_type<EnemyAttacks>);
static_assert(has_attack_power<EnemyAttacks>);
static_assert(has_attack_flags<EnemyAttacks>);
static_assert(has_unknown1<EnemyAttacks>);
static_assert(has_element<EnemyAttacks>);
static_assert(has_unknown2<EnemyAttacks>);
static_assert(has_status_attack_enabler<EnemyAttacks>);
static_assert(has_attack_parameter<EnemyAttacks>);
static_assert(has_persistent_statuses<EnemyAttacks>);
static_assert(has_battle_only_statuses<EnemyAttacks>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_ENEMYATTACKS_HPP
