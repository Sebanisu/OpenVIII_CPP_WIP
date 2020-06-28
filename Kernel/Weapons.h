//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_WEAPONS_H
#define VIIIARCHIVE_WEAPONS_H

#include "../Strings/EncodedStringOffset.h"
#include "AttackTypeT.h"
#include "CharactersT.h"
#include "RenzokukenFinishersT.h"
#include <cstdint>

namespace OpenVIII::Kernel {
struct Weapons
{
  /* https://github.com/DarkShinryu/doomtrain/wiki/Weapons
   * Section Structure
   * Offset	Length	Description
   * 0x0000	2 bytes	Offset to weapon name
   * 0x0002	1 byte	Renzokuken finishers
   *    0x01 = Rough Divide
   *    0x02 = Fated Circle
   *    0x04 = Blasting Zone
   *    0x08 = Lion Heart
   * 0x0003	1 byte	Unknown
   * 0x0004	1 byte	Character ID
   *    0x00 - Squall
   *    0x01 - Zell
   *    0x02 - Irvine
   *    0x03 - Quistis
   *    0x04 - Rinoa
   *    0x05 - Selphie
   *    0x06 - Seifer
   *    0x07 - Edea
   *    0x08 - Laguna
   *    0x09 - Kiros
   *    0x0A - Ward
   * 0x0005	1 bytes	Attack Type
   * 0x0006	1 byte	Attack Power
   * 0x0007	1 byte	Attack Parameter
   * 0x0008	1 byte	STR Bonus
   * 0x0009	1 byte	Weapon Tier
   * 0x000A	1 byte	Crit Bonus
   * 0x000B	1 byte	Melee Weapon?
   */
private:
  EncodedStringOffset nameOffset_{};
  RenzokukenFinishersT renzokukenFinishers_{};
  std::uint8_t unknown_{};
  CharactersT characterID_{};
  AttackTypeT attackType_{};
  std::uint8_t attackPower_{};
  std::uint8_t attackParameter_{};
  std::uint8_t strBonus_{};
  std::uint8_t weaponTier_{};
  std::uint8_t criticalBonus_{};
  std::uint8_t meleeWeapon_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto RenzokukenFinishers() const noexcept { return renzokukenFinishers_; }
  [[nodiscard]] auto unknown() const noexcept { return unknown_; }
  [[nodiscard]] auto CharacterID() const noexcept { return characterID_; }
  [[nodiscard]] auto AttackType() const noexcept { return attackType_; }
  [[nodiscard]] auto AttackPower() const noexcept { return attackPower_; }
  [[nodiscard]] auto AttackParameter() const noexcept { return attackParameter_; }
  [[nodiscard]] auto STRBonus() const noexcept { return strBonus_; }
  [[nodiscard]] auto WeaponTier() const noexcept { return weaponTier_; }
  [[nodiscard]] auto CriticalBonus() const noexcept { return criticalBonus_; }
  [[nodiscard]] auto MeleeWeapon() const noexcept { return meleeWeapon_ != 0; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = nameOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    os << ", " << MeleeWeapon();
    return os;
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_WEAPONS_H
