//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_ENEMYATTACKS_H
#define VIIIARCHIVE_ENEMYATTACKS_H

#include "EncodedStringOffset.h"
#include "PersistentStatusesT.h"
#include "BattleOnlyStatusesT.h"
#include "AttackFlagsT.h"
#include "AttackTypeT.h"
#include "ElementT.h"
#include "CameraChangeT.h"
namespace OpenVIII::Kernel {
struct EnemyAttacks
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
  EncodedStringOffset nameOffset_{};
  uint16_t magicID_;
  CameraChangeT cameraChange_;
  uint8_t unknown0_;
  AttackTypeT attackType_;
  uint8_t attackPower_;
  AttackFlagsT attackFlags_;
  uint8_t unknown1_;
  ElementT element_;
  uint8_t unknown2_;
  uint8_t statusAttackEnabler_;
  uint8_t attackParameter_;
  PersistentStatusesT persistentStatuses_;// statuses 0-7
  BattleOnlyStatusesT battleOnlyStatuses_;// statuses 8-31

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
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
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name << ", ";
    }
    os << cameraChange_;
    return os;
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_ENEMYATTACKS_H
