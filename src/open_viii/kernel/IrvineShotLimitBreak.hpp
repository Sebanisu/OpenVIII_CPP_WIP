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
#ifndef VIIIARCHIVE_IRVINESHOTLIMITBREAK_HPP
#define VIIIARCHIVE_IRVINESHOTLIMITBREAK_HPP
#include "AttackFlagsT.hpp"
#include "AttackTypeT.hpp"
#include "BattleOnlyStatusesT.hpp"
#include "ElementT.hpp"
#include "PersistentStatusesT.hpp"
#include "TargetT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
namespace open_viii::kernel {
/**
 * Offset	Length	Description
 std::uint16_t Offset to limit name
 std::uint16_t Offset to limit description
 std::uint16_t Magic ID
 std::uint8_t Attack Type
 std::uint8_t Attack Power
 std::uint16_t Unknown
 std::uint8_t Target Info
 std::uint8_t Attack Flags
 std::uint8_t Hit Count
 std::uint8_t Element Attack
 std::uint8_t Element Attack %
 std::uint8_t Status Attack Enabler
 std::uint16_t status_0; //statuses 0-7
 std::uint8_t Used item index
 std::uint8_t Critical increase
 4 bytes	status_1; //statuses 8-39
 @see https://github.com/DarkShinryu/doomtrain/wiki/Shot-(Irvine-limit-break)
 */
struct IrvineShotLimitBreak_impl
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
  std::uint8_t        m_used_item_index        = {};
  std::uint8_t        m_critical_increase      = {};
  BattleOnlyStatusesT m_battle_only_statuses   = {};// statuses 8-39
  constexpr IrvineShotLimitBreak_impl()        = default;
  static constexpr std::size_t EXPECTED_SIZE   = 24U;

public:
  constexpr auto
    operator<=>(
      const IrvineShotLimitBreak_impl &right) const noexcept = default;
};
using IrvineShotLimitBreak = CommonKernel<IrvineShotLimitBreak_impl>;
static_assert(IrvineShotLimitBreak::EXPECTED_SIZE
              == sizeof(IrvineShotLimitBreak));
static_assert(has_name_offset<IrvineShotLimitBreak>);
static_assert(has_description_offset<IrvineShotLimitBreak>);
static_assert(has_magic_id<IrvineShotLimitBreak>);
static_assert(has_attack_type<IrvineShotLimitBreak>);
static_assert(has_attack_power<IrvineShotLimitBreak>);
static_assert(has_unknown0<IrvineShotLimitBreak>);
static_assert(has_unknown1<IrvineShotLimitBreak>);
static_assert(has_target<IrvineShotLimitBreak>);
static_assert(has_attack_flags<IrvineShotLimitBreak>);
static_assert(has_hit_count<IrvineShotLimitBreak>);
static_assert(has_element<IrvineShotLimitBreak>);
static_assert(has_element_attack_percent<IrvineShotLimitBreak>);
static_assert(has_status_attack_enabler<IrvineShotLimitBreak>);
static_assert(has_persistent_statuses<IrvineShotLimitBreak>);// statuses 0-7
static_assert(has_used_item_index<IrvineShotLimitBreak>);
static_assert(has_critical_increase<IrvineShotLimitBreak>);
static_assert(has_battle_only_statuses<IrvineShotLimitBreak>);// statuses 8-39
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_IRVINESHOTLIMITBREAK_HPP
