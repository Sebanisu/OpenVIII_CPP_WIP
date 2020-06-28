//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_NONJUNCTIONABLEGFS_H
#define VIIIARCHIVE_NONJUNCTIONABLEGFS_H
#include "../Strings/EncodedStringOffset.h"
#include "ElementT.h"
#include "PersistentStatusesT.h"
#include "BattleOnlyStatusesT.h"
#include "AttackTypeT.h"
namespace OpenVIII::Kernel {
struct NonJunctionableGFs
{
  /* https://github.com/DarkShinryu/doomtrain/wiki/Non-junctionable-GF-attacks
   * Offset	Length	Description
   * 0x0000	2 bytes	Offset to GF attack name
   * 0x0002	2 bytes	Magic ID (decides what animation to play)
   * 0x0004	1 byte	Attack type
   * 0x0005	1 byte	GF power (used in damage formula)
   * 0x0006	1 byte	Status attack enabler
   * 0x0007	1 byte	Unknown
   * 0x0008	1 byte	Status flags
   * 0x0009	2 bytes	Unknown
   * 0x000B	1 byte	Element
   *  0x00 - Non-Elemental
   *  0x01 - Fire
   *  0x02 - Ice
   *  0x04 - Thunder
   *  0x08 - Earth
   *  0x10 - Poison
   *  0x20 - Wind
   *  0x40 - Water
   *  0x80 - Holy
   * 0x000C	1 byte	Status 1
   *  0x00 - None
   *  0x01 - Sleep
   *  0x02 - Haste
   *  0x04 - Slow
   *  0x08 - Stop
   *  0x10 - Regen
   *  0x20 - Protect
   *  0x40 - Shell
   *  0x80 - Reflect
   * 0x000D	1 byte	Status 2
   *  0x00 - None
   *  0x01 - Aura
   *  0x02 - Curse
   *  0x04 - Doom
   *  0x08 - Invincible
   *  0x10 - Petrifying
   *  0x20 - Float
   *  0x40 - Confusion
   *  0x80 - Drain
   * 0x000E	1 byte	Status 3
   *  0x00 - None
   *  0x01 - Eject
   *  0x02 - Double
   *  0x04 - Triple
   *  0x08 - Defend
   *  0x10 - ???
   *  0x20 - ???
   *  0x40 - ???
   *  0x80 - ???
   * 0x000F	1 byte	Status 4
   *  0x00 - None
   *  0x01 - Vit0
   *  0x02 - ???
   *  0x04 - ???
   *  0x08 - ???
   *  0x10 - ???
   *  0x20 - ???
   *  0x40 - ???
   *  0x80 - ???
   * 0x0010	1 byte	Status 5
   *  0x00 - None
   *  0x01 - Death
   *  0x02 - Poison
   *  0x04 - Petrify
   *  0x08 - Darkness
   *  0x10 - Silence
   *  0x20 - Berserk
   *  0x40 - Zombie
   *  0x80 - ???
   * 0x0011	1 byte	Unknown
   * 0x0012	1 byte	Power Mod (used in damage formula)
   * 0x0013	1 byte	Level Mod (used in damage formula)
   */
private:
  EncodedStringOffset nameOffset_{};
  std::uint16_t magicID_{};
  AttackTypeT attackType_{};
  std::uint8_t gfPower_{};// (used in damage formula)
  std::uint8_t statusAttackEnabler_{};
  std::uint8_t unknown0_{};
  std::uint8_t statusFlags_{};
  std::uint8_t unknown1_{};
  std::uint8_t unknown2_{};
  ElementT element_{};
  BattleOnlyStatusesT battleOnlyStatuses_{};
  PersistentStatusesT persistentStatuses_{};
  // std::uint8_t Unknown3_{}; // assuming this is part of persistent statuses.
  std::uint8_t powerMod_{};// (used in damage formula)
  std::uint8_t levelMod_{};// (used in damage formula)
public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto MagicID() const noexcept { return magicID_; }
  [[nodiscard]] auto AttackType() const noexcept { return attackType_; }
  [[nodiscard]] auto GFPower() const noexcept { return gfPower_; }// (used in damage formula)
  [[nodiscard]] auto StatusAttackEnabler() const noexcept { return statusAttackEnabler_; }
  [[nodiscard]] auto unknown0() const noexcept { return unknown0_; }
  [[nodiscard]] auto StatusFlags() const noexcept { return statusFlags_; }
  [[nodiscard]] auto unknown1() const noexcept { return unknown1_; }
  [[nodiscard]] auto unknown2() const noexcept { return unknown2_; }
  [[nodiscard]] auto Element() const noexcept { return element_; }
  [[nodiscard]] auto BattleOnlyStatuses() const noexcept { return battleOnlyStatuses_; }
  [[nodiscard]] auto PersistentStatuses() const noexcept { return persistentStatuses_; }
  // [[nodiscard]] auto Unknown3() const noexcept { return Unknown3_; } // assuming this is part of persistent statuses.
  [[nodiscard]] auto PowerMod() const noexcept { return powerMod_; }// (used in damage formula)
  [[nodiscard]] auto LevelMod() const noexcept { return levelMod_; }// (used in damage formula)
  std::ostream &Out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = nameOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    return os << ", " << static_cast<std::uint32_t>(MagicID())
              << ", " << static_cast<std::uint32_t>(AttackType()) << ", "
              << static_cast<std::uint32_t>(GFPower())// (used in damage formula)
              << ", " << static_cast<std::uint32_t>(StatusAttackEnabler()) << ", "
              << static_cast<std::uint32_t>(unknown0()) << ", " << static_cast<std::uint32_t>(StatusFlags()) << ", "
              << static_cast<std::uint32_t>(unknown1()) << ", " << static_cast<std::uint32_t>(unknown2()) << ", "
              << static_cast<std::uint32_t>(Element()) << ", " << static_cast<std::uint32_t>(BattleOnlyStatuses())
              << ", " << static_cast<std::uint32_t>(PersistentStatuses())
              << ", " << static_cast<std::uint32_t>(PowerMod())// (used in damage formula)
              << ", " << static_cast<std::uint32_t>(LevelMod())// (used in damage formula)
      ;
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_NONJUNCTIONABLEGFS_H
