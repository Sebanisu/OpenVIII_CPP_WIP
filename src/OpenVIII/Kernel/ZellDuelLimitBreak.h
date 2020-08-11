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

#ifndef VIIIARCHIVE_ZELLDUELLIMITBREAK_H
#define VIIIARCHIVE_ZELLDUELLIMITBREAK_H
#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "ElementT.h"
#include "BattleOnlyStatusesT.h"
#include "PersistentStatusesT.h"
#include "AttackFlagsT.h"
#include "AttackTypeT.h"
#include "TargetT.h"
#include "ZellDuelButtonT.h"
#include <array>

namespace OpenVIII::Kernel {
template<LangT langVal> struct ZellDuelLimitBreak
{ /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Duel-(Zell-limit-break)
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
   */
private:
  static constexpr uint8_t MaxNumberOfButtons{ 5U };
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

  std::uint16_t magicID_{};
  AttackTypeT attackType_{};
  std::uint8_t attackPower_{};
  AttackFlagsT attackFlags_{};
  std::uint8_t unknown0_{};
  TargetT target_{};
  std::uint8_t unknown1_{};
  std::uint8_t hitCount_{};
  ElementT element_{};
  uint8_t elementAttackPercent_{};
  std::uint8_t statusAttackEnabler_{};
  std::array<ZellDuelButtonT, MaxNumberOfButtons> buttonSequence_{};
  PersistentStatusesT persistentStatuses_{};// statuses 0-7
  BattleOnlyStatusesT battleOnlyStatuses_{};// statuses 8-39

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }

  [[nodiscard]] auto MagicID() const noexcept { return magicID_; }
  [[nodiscard]] auto AttackType() const noexcept { return attackType_; }
  [[nodiscard]] auto AttackPower() const noexcept { return attackPower_; }
  [[nodiscard]] auto AttackFlags() const noexcept { return attackFlags_; }
  [[nodiscard]] auto unknown0() const noexcept { return unknown0_; }
  [[nodiscard]] auto Target() const noexcept { return target_; }
  [[nodiscard]] auto unknown1() const noexcept { return unknown1_; }
  [[nodiscard]] auto HitCount() const noexcept { return hitCount_; }
  [[nodiscard]] auto Element() const noexcept { return element_; }
  [[nodiscard]] auto ElementAttackPercent() const noexcept { return elementAttackPercent_; }
  [[nodiscard]] auto StatusAttackEnabler() const noexcept { return statusAttackEnabler_; }
  [[nodiscard]] auto ButtonSequence() const noexcept { return buttonSequence_; }
  [[nodiscard]] auto PersistentStatuses() const noexcept { return persistentStatuses_; }// statuses 0-7
  [[nodiscard]] auto BattleOnlyStatuses() const noexcept { return battleOnlyStatuses_; }// statuses 8-39
  std::ostream &Out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = nameOffset_.DecodedString<langVal>(buffer);
    auto description = descriptionOffset_.DecodedString<langVal>(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    os << ", " << static_cast<std::uint32_t>(MagicID()) << ", " << static_cast<std::uint32_t>(AttackType()) << ", "
       << static_cast<std::uint32_t>(AttackPower()) << ", " << static_cast<std::uint32_t>(AttackFlags()) << ", "
       << static_cast<std::uint32_t>(unknown0()) << ", " << static_cast<std::uint32_t>(Target()) << ", "
       << static_cast<std::uint32_t>(unknown1()) << ", " << static_cast<std::uint32_t>(HitCount()) << ", "
       << static_cast<std::uint32_t>(Element()) << ", " << static_cast<std::uint32_t>(ElementAttackPercent()) << ", "
       << static_cast<std::uint32_t>(StatusAttackEnabler()) << ", {";
    auto first = true;
    for (const auto &button : ButtonSequence()) {
      if (!first)

      {
        os << ", ";
      } else {
        first = false;
      }
      os << static_cast<std::uint32_t>(button);
    }
    return os << "}, " << static_cast<std::uint32_t>(PersistentStatuses())// statuses 0-7
              << ", " << static_cast<std::uint32_t>(BattleOnlyStatuses())// statuses 8-39
      ;
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_ZELLDUELLIMITBREAK_H
