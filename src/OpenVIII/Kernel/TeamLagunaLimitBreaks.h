//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_TEAMLAGUNALIMITBREAKS_H
#define VIIIARCHIVE_TEAMLAGUNALIMITBREAKS_H

#include "../Strings/EncodedStringOffset.h"
#include "AttackFlagsT.h"
#include "AttackTypeT.h"
#include "TargetT.h"
#include "ElementT.h"
#include "BattleOnlyStatusesT.h"
#include "PersistentStatusesT.h"

namespace OpenVIII::Kernel {
struct TeamLagunaLimitBreaks
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Temporary-character-limit-breaks
   * Offset	Length	Description
   * 0x0000	2 bytes	Offset to limit name
   * 0x0002	2 bytes	Offset to limit description
   * 0x0004	2 bytes	Magic ID
   * 0x0006	1 byte	Attack Type
   * 0x0007	1 byte	Attack Power
   * 0x0008	2 bytes	Unknown
   * 0x000A	1 byte	Target Info
   * 0x000B	1 byte	Attack Flags
   * 0x000C	1 byte	Hit Count
   * 0x000D	1 byte	Element Attack
   * 0x000E	1 byte	Element Attack %
   * 0x000F	1 byte	Status Attack Enabler
   * 0x0010	2 bytes	status_0; //statuses 0-7
   * 0x0012	2 bytes	Unknown
   * 0x0014	4 bytes	status_1; //statuses 8-39
   */
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};
  std::uint16_t magicID_{};
  AttackTypeT attackType_{};
  std::uint8_t attackPower_{};
  std::uint8_t unknown0_{};
  std::uint8_t unknown1_{};
  TargetT target_{};
  AttackFlagsT attackFlags_{};
  std::uint8_t hitCount_{};
  ElementT element_{};
  std::uint8_t elementAttackPercent_{};
  std::uint8_t statusAttackEnabler_{};
  PersistentStatusesT persistentStatuses_{};// statuses 0-7
  std::uint8_t unknown2_{};
  std::uint8_t unknown3_{};
  BattleOnlyStatusesT battleOnlyStatuses_{};// statuses 8-39

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  [[nodiscard]] auto MagicID() const noexcept { return magicID_; }
  [[nodiscard]] auto AttackType() const noexcept { return attackType_; }
  [[nodiscard]] auto AttackPower() const noexcept { return attackPower_; }
  [[nodiscard]] auto Unknown0() const noexcept { return unknown0_; }
  [[nodiscard]] auto Unknown1() const noexcept { return unknown1_; }
  [[nodiscard]] auto Target() const noexcept { return target_; }
  [[nodiscard]] auto AttackFlags() const noexcept { return attackFlags_; }
  [[nodiscard]] auto HitCount() const noexcept { return hitCount_; }
  [[nodiscard]] auto Element() const noexcept { return element_; }
  [[nodiscard]] auto ElementAttackPercent() const noexcept { return elementAttackPercent_; }
  [[nodiscard]] auto StatusAttackEnabler() const noexcept { return statusAttackEnabler_; }
  [[nodiscard]] auto PersistentStatuses() const noexcept { return persistentStatuses_; }// statuses 0-7
  [[nodiscard]] auto Unknown2() const noexcept { return unknown2_; }
  [[nodiscard]] auto Unknown3() const noexcept { return unknown3_; }
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
    return os << ", " << static_cast<std::uint32_t>(MagicID()) << ", " << static_cast<std::uint32_t>(AttackType())
              << ", " << static_cast<std::uint32_t>(AttackPower()) << ", " << static_cast<std::uint32_t>(Unknown0())
              << ", " << static_cast<std::uint32_t>(Unknown1()) << ", " << static_cast<std::uint32_t>(Target()) << ", "
              << static_cast<std::uint32_t>(AttackFlags()) << ", " << static_cast<std::uint32_t>(HitCount()) << ", "
              << static_cast<std::uint32_t>(Element()) << ", " << static_cast<std::uint32_t>(ElementAttackPercent())
              << ", " << static_cast<std::uint32_t>(StatusAttackEnabler()) << ", "
              << static_cast<std::uint32_t>(PersistentStatuses())// statuses 0-7
              << ", " << static_cast<std::uint32_t>(Unknown2()) << ", " << static_cast<std::uint32_t>(Unknown3())
              << ", " << static_cast<std::uint32_t>(BattleOnlyStatuses())// statuses 8-39
      ;
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_TEAMLAGUNALIMITBREAKS_H
