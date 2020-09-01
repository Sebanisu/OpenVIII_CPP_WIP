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

#ifndef VIIIARCHIVE_RENZOKUKENFINISHERS_H
#define VIIIARCHIVE_RENZOKUKENFINISHERS_H
#include "AttackTypeT.h"
#include "AttackFlagsT.h"
#include "ElementT.h"
#include "TargetT.h"
#include "PersistentStatusesT.h"
#include "BattleOnlyStatusesT.h"
#include "OpenVIII/Strings/EncodedStringOffset.h"

namespace open_viii::Kernel {
template<LangT langVal> struct RenzokukenFinishers
{
  /* https://github.com/DarkShinryu/doomtrain/wiki/Renzokuken-finishers
   * 0x0000	2 bytes	Offset to limit name
   * 0x0002	2 bytes	Offset to limit description
   * 0x0004	2 bytes	Magic ID
   * 0x0006	1 byte	Attack Type
   * 0x0007	1 byte	Unknown
   * 0x0008	1 byte	Attack power
   * 0x0009	1 byte	Unknown
   * 0x000A	1 byte	Target info
   * 0x000B	1 byte	Attack Flags
   * 0x000C	1 byte	Hit count
   * 0x000D	1 byte	Element Attack
   * 0x000E	1 byte	Element Attack %
   * 0x000F	1 byte	Status Attack Enabler
   * 0x0010	2 bytes	Unknown
   * 0x0012	2 bytes	status_0; //statuses 0-7
   * 0x0014	4 bytes	status_1; //statuses 8-39
   */
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};
  std::uint16_t magicID_{};
  AttackTypeT attackType_{};
  std::uint8_t unknown0_{};
  std::uint8_t attackPower_{};
  std::uint8_t unknown1_{};
  std::uint8_t target_{};
  AttackFlagsT attackFlags_{};
  std::uint8_t hitCount_{};
  ElementT element_{};
  std::uint8_t elementAttackPercent_{};
  std::uint8_t statusAttackEnabler_{};
  std::uint16_t unknown2_{};
  PersistentStatusesT persistentStatuses_{};// statuses 0-7
  BattleOnlyStatusesT battleOnlyStatuses_{};// statuses 8-39

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  [[nodiscard]] auto MagicID() const noexcept { return magicID_; }
  [[nodiscard]] auto AttackType() const noexcept { return attackType_; }
  [[nodiscard]] auto unknown0() const noexcept { return unknown0_; }
  [[nodiscard]] auto AttackPower() const noexcept { return attackPower_; }
  [[nodiscard]] auto unknown1() const noexcept { return unknown1_; }
  [[nodiscard]] auto Target() const noexcept { return target_; }
  [[nodiscard]] auto AttackFlags() const noexcept { return attackFlags_; }
  [[nodiscard]] auto HitCount() const noexcept { return hitCount_; }
  [[nodiscard]] auto Element() const noexcept { return element_; }
  [[nodiscard]] auto ElementAttackPercent() const noexcept { return elementAttackPercent_; }
  [[nodiscard]] auto AtatusAttackEnabler() const noexcept { return statusAttackEnabler_; }
  [[nodiscard]] auto unknown2() const noexcept { return unknown2_; }
  [[nodiscard]] auto PersistentStatuses() const noexcept { return persistentStatuses_; }// statuses 0-7
  [[nodiscard]] auto BattleOnlyStatuses() const noexcept { return battleOnlyStatuses_; }// statuses 8-39

  std::ostream &Out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = nameOffset_.DecodedString<langVal>(buffer);
    auto description = descriptionOffset_.DecodedString<langVal>(buffer);
    if (!std::empty(name)) {
      os << Tools::u8tosv(name);
    }
    if (!std::empty(description)) {
      os << ", " << Tools::u8tosv(description);
    }
    return os;
  }
};
}// namespace open_viii::Kernel
#endif// VIIIARCHIVE_RENZOKUKENFINISHERS_H
