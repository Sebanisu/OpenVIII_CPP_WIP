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

#ifndef VIIIARCHIVE_ENEMYATTACKS_H
#define VIIIARCHIVE_ENEMYATTACKS_H

#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "PersistentStatusesT.h"
#include "BattleOnlyStatusesT.h"
#include "AttackFlagsT.h"
#include "AttackTypeT.h"
#include "ElementT.h"
#include "CameraChange.h"
namespace open_viii::kernel {
template<LangT langVal> struct EnemyAttacks
{
  /* https://github.com/DarkShinryu/doomtrain/wiki/Enemy-attacks
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
   */
private:
  EncodedStringOffset m_name_offset{};
  uint16_t magicID_{};
  CameraChange cameraChange_{};
  uint8_t unknown0_{};
  AttackTypeT attackType_{};
  uint8_t attackPower_{};
  AttackFlagsT attackFlags_{};
  uint8_t unknown1_{};
  ElementT element_{};
  uint8_t unknown2_{};
  uint8_t statusAttackEnabler_{};
  uint8_t attackParameter_{};
  PersistentStatusesT persistentStatuses_{};// statuses 0-7
  BattleOnlyStatusesT battleOnlyStatuses_{};// statuses 8-31

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return m_name_offset; }
  [[nodiscard]] auto MagicID() const noexcept { return magicID_; }
  [[nodiscard]] auto CameraChange() const noexcept { return cameraChange_; }
  [[nodiscard]] auto unknown0() const noexcept { return unknown0_; }
  [[nodiscard]] auto AttackType() const noexcept { return attackType_; }
  [[nodiscard]] auto attackPower() const noexcept { return attackPower_; }
  [[nodiscard]] auto AttackFlags() const noexcept { return attackFlags_; }
  [[nodiscard]] auto unknown1() const noexcept { return unknown1_; }
  [[nodiscard]] auto Element() const noexcept { return element_; }
  [[nodiscard]] auto unknown2() const noexcept { return unknown2_; }
  [[nodiscard]] auto StatusAttackEnabler() const noexcept { return statusAttackEnabler_; }
  [[nodiscard]] auto AttackParameter() const noexcept { return attackParameter_; }
  [[nodiscard]] auto PersistentStatuses() const noexcept { return persistentStatuses_; }// statuses 0-7
  [[nodiscard]] auto BattleOnlyStatuses() const noexcept { return battleOnlyStatuses_; }// statuses 8-31
  std::ostream &out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = m_name_offset.decoded_string<langVal>(buffer);
    if (!std::empty(name)) {
      os << Tools::u8tosv(name) << ", ";
    }
    return os << static_cast<std::uint32_t>(MagicID()) << ", " << CameraChange() << ", "
              << static_cast<std::uint32_t>(unknown0()) << ", " << static_cast<std::uint32_t>(AttackType()) << ", "
              << static_cast<std::uint32_t>(attackPower()) << ", " << static_cast<std::uint32_t>(AttackFlags()) << ", "
              << static_cast<std::uint32_t>(unknown1()) << ", " << static_cast<std::uint32_t>(Element()) << ", "
              << static_cast<std::uint32_t>(unknown2()) << ", " << static_cast<std::uint32_t>(StatusAttackEnabler())
              << ", " << static_cast<std::uint32_t>(AttackParameter()) << ", "
              << static_cast<std::uint32_t>(PersistentStatuses())// statuses 0-7
              << ", " << static_cast<std::uint32_t>(BattleOnlyStatuses())// statuses 8-31
      ;
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_ENEMYATTACKS_H
