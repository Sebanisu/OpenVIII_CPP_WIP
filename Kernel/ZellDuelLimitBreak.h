//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_ZELLDUELLIMITBREAK_H
#define VIIIARCHIVE_ZELLDUELLIMITBREAK_H
#include "../Strings/EncodedStringOffset.h"
#include "ElementT.h"
#include "BattleOnlyStatusesT.h"
#include "PersistentStatusesT.h"
#include "AttackFlagsT.h"
#include "AttackTypeT.h"
#include "TargetT.h"
#include <array>

namespace OpenVIII::Kernel {
enum class ButtonFlagT : std::uint16_t {
  End = 0x0001,
  Up = 0x0010,
  Right = 0x0020,
  Down = 0x0040,
  Left = 0x0080,
  L2 = 0x0100,
  R2 = 0x0200,
  L1 = 0x0400,
  R1 = 0x0800,
  Triangle = 0x1000,
  Circle = 0x2000,
  Cross = 0x4000,
  Square = 0x8000,
  None = 0xFFFF,
};

struct ZellDuelLimitBreak
{ /*
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
  std::array<ButtonFlagT, MaxNumberOfButtons> buttonSequence_{};
  PersistentStatusesT persistentStatuses_{};// statuses 0-7
  BattleOnlyStatusesT battleOnlyStatuses_{};// statuses 8-39

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }

  [[nodiscard]] auto MagicID() const noexcept { return magicID_; }
  [[nodiscard]] auto AttackType() const noexcept { return attackType_; }
  [[nodiscard]] auto AttackPower() const noexcept { return attackPower_; }
  [[nodiscard]] auto AttackFlags() const noexcept { return attackFlags_; }
  [[nodiscard]] auto Unknown0() const noexcept { return unknown0_; }
  [[nodiscard]] auto Target() const noexcept { return target_; }
  [[nodiscard]] auto Unknown1() const noexcept { return unknown1_; }
  [[nodiscard]] auto HitCount() const noexcept { return hitCount_; }
  [[nodiscard]] auto Element() const noexcept { return element_; }
  [[nodiscard]] auto ElementAttackPercent() const noexcept { return elementAttackPercent_; }
  [[nodiscard]] auto StatusAttackEnabler() const noexcept { return statusAttackEnabler_; }
  [[nodiscard]] auto ButtonSequence() const noexcept { return buttonSequence_; }
  [[nodiscard]] auto PersistentStatuses() const noexcept { return persistentStatuses_; }// statuses 0-7
  [[nodiscard]] auto BattleOnlyStatuses() const noexcept { return battleOnlyStatuses_; }// statuses 8-39
  std::ostream &Out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_ZELLDUELLIMITBREAK_H
