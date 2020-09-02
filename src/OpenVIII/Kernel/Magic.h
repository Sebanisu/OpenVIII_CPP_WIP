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

#ifndef VIIIARCHIVE_MAGIC_H
#define VIIIARCHIVE_MAGIC_H
#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "AttackTypeT.h"
#include "TargetT.h"
#include "AttackFlagsT.h"
#include "ElementT.h"
#include "BattleOnlyStatusesT.h"
#include "PersistentStatusesT.h"
#include "StatGroup.h"
#include "JunctionStatusesT.h"
#include "GFGroup.h"
namespace open_viii::kernel {
template<LangT langVal> struct Magic
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Magic-data
   * Offset	Length	Description
   * 	2 bytes	Offset to spell name
   * 	2 bytes	Offset to spell description
   * 	2 bytes	[[Magic ID
   * 	1 byte	Unknown
   * 	1 byte	Attack type
   * 	1 byte	Spell power (used in damage formula)
   * 	1 byte	Unknown
   * 	1 byte	Default target
   * 	1 byte	Attack Flags
   * 	1 byte	Draw resist (how hard is the magic to draw)
   * 	1 byte	Hit count (works with meteor animation, not sure about others)
   * 	1 byte	[[Element
   * 	1 byte	Unknown
   * 	4 bytes	[[Statuses 1
   * 	2 bytes	[[Statuses 0
   * 	1 byte	Status attack enabler
   * 	1 byte	Characters HP junction value
   * 	1 byte	Characters STR junction value
   * 	1 byte	Characters VIT junction value
   * 	1 byte	Characters MAG junction value
   * 	1 byte	Characters SPR junction value
   * 	1 byte	Characters SPD junction value
   * 	1 byte	Characters EVA junction value
   * 	1 byte	Characters HIT junction value
   * 	1 byte	Characters LUCK junction value
   * 	1 byte	[[Characters J-Elem attack
   * 	1 byte	Characters J-Elem attack value
   * 	1 byte	[[Characters J-Elem defense
   * 	1 byte	Characters J-Elem defense value
   * 	1 byte	Characters J-Status attack value
   * 	1 byte	Characters J-Status defense value
   * 	2 bytes	[[Characters J-Statuses Attack
   * 	2 bytes	[[Characters J-Statuses Defend
   * 	1 byte	Quezacolt compatibility
   * 	1 byte	Shiva compatibility
   * 	1 byte	Ifrit compatibility
   * 	1 byte	Siren compatibility
   * 	1 byte	Brothers compatibility
   * 	1 byte	Diablos compatibility
   * 	1 byte	Carbuncle compatibility
   * 	1 byte	Leviathan compatibility
   * 	1 byte	Pandemona compatibility
   * 	1 byte	Cerberus compatibility
   * 	1 byte	Alexander compatibility
   * 	1 byte	Doomtrain compatibility
   * 	1 byte	Bahamut compatibility
   * 	1 byte	Cactuar compatibility
   * 	1 byte	Tonberry compatibility
   * 	1 byte	Eden compatibility
   * 	2 bytes	Unknown
   */
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};
  uint16_t magicID_{};
  uint8_t unknown0_{};
  AttackTypeT attackType_{};
  uint8_t spellPower_{};
  uint8_t unknown1_{};
  TargetT target_{};
  AttackFlagsT attackFlags_{};
  uint8_t drawResist_{};
  uint8_t hitCount_{};
  ElementT element_{};
  uint8_t unknown2_{};
  BattleOnlyStatusesT battleOnlyStatuses_{};
  PersistentStatusesT persistentStatuses_{};
  uint8_t statusAttack_{};
  StatGroup<uint8_t> junctionStats_{};
  ElementT jElemAttackFlag_{};
  uint8_t jElemAttackValue_{};
  ElementT jElemDefenseFlag_{};
  uint8_t jElemDefenseValue_{};
  uint8_t jStatusAttackValue_{};
  uint8_t jStatusDefenseValue_{};
  JunctionStatusesT jStatusesAttackFlag_{};
  JunctionStatusesT jStatusesDefendFlag_{};
  GFGroup<uint8_t> compatibility_{};
  uint16_t unknown3_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  [[nodiscard]] auto MagicID() const noexcept { return magicID_; }
  [[nodiscard]] auto unknown0() const noexcept { return unknown0_; }
  [[nodiscard]] auto AttackType() const noexcept { return attackType_; }
  [[nodiscard]] auto SpellPower() const noexcept { return spellPower_; }
  [[nodiscard]] auto unknown1() const noexcept { return unknown1_; }
  [[nodiscard]] auto Target() const noexcept { return target_; }
  [[nodiscard]] auto AttackFlags() const noexcept { return attackFlags_; }
  [[nodiscard]] auto DrawResist() const noexcept { return drawResist_; }
  [[nodiscard]] auto HitCount() const noexcept { return hitCount_; }
  [[nodiscard]] auto Element() const noexcept { return element_; }
  [[nodiscard]] auto unknown2() const noexcept { return unknown2_; }
  [[nodiscard]] auto BattleOnlyStatuses() const noexcept { return battleOnlyStatuses_; }
  [[nodiscard]] auto PersistentStatuses() const noexcept { return persistentStatuses_; }
  [[nodiscard]] auto StatusAttack() const noexcept { return statusAttack_; }
  [[nodiscard]] auto JunctionStats() const noexcept { return junctionStats_; }
  [[nodiscard]] auto JElemAttackFlag() const noexcept { return jElemAttackFlag_; }
  [[nodiscard]] auto JElemAttackValue() const noexcept { return jElemAttackValue_; }
  [[nodiscard]] auto JElemDefenseFlag() const noexcept { return jElemDefenseFlag_; }
  [[nodiscard]] auto JElemDefenseValue() const noexcept { return jElemDefenseValue_; }
  [[nodiscard]] auto JStatusAttackValue() const noexcept { return jStatusAttackValue_; }
  [[nodiscard]] auto JStatusDefenseValue() const noexcept { return jStatusDefenseValue_; }
  [[nodiscard]] auto JStatusesAttackFlag() const noexcept { return jStatusesAttackFlag_; }
  [[nodiscard]] auto JStatusesDefendFlag() const noexcept { return jStatusesDefendFlag_; }
  [[nodiscard]] auto Compatibility() const noexcept { return compatibility_; }
  [[nodiscard]] auto unknown3() const noexcept { return unknown3_; }

  std::ostream &out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = nameOffset_.decoded_string<langVal>(buffer);
    auto description = descriptionOffset_.decoded_string<langVal>(buffer);
    if (!std::empty(name)) {
      os << Tools::u8tosv(name);
    }
    if (!std::empty(description)) {
      os << ", " << Tools::u8tosv(description);
    }
    return os

           << ", " << static_cast<std::uint32_t>(MagicID()) << ", " << static_cast<std::uint32_t>(unknown0()) << ", "
           << static_cast<std::uint32_t>(AttackType()) << ", " << static_cast<std::uint32_t>(SpellPower()) << ", "
           << static_cast<std::uint32_t>(unknown1()) << ", " << static_cast<std::uint32_t>(Target()) << ", "
           << static_cast<std::uint32_t>(AttackFlags()) << ", " << static_cast<std::uint32_t>(DrawResist()) << ", "
           << static_cast<std::uint32_t>(HitCount()) << ", " << static_cast<std::uint32_t>(Element()) << ", "
           << static_cast<std::uint32_t>(unknown2()) << ", " << static_cast<std::uint32_t>(BattleOnlyStatuses()) << ", "
           << static_cast<std::uint32_t>(PersistentStatuses()) << ", " << static_cast<std::uint32_t>(StatusAttack())
           << ", " << JunctionStats() << ", " << static_cast<std::uint32_t>(JElemAttackFlag()) << ", "
           << static_cast<std::uint32_t>(JElemAttackValue()) << ", " << static_cast<std::uint32_t>(JElemDefenseFlag())
           << ", " << static_cast<std::uint32_t>(JElemDefenseValue()) << ", "
           << static_cast<std::uint32_t>(JStatusAttackValue()) << ", "
           << static_cast<std::uint32_t>(JStatusDefenseValue()) << ", "
           << static_cast<std::uint32_t>(JStatusesAttackFlag()) << ", "
           << static_cast<std::uint32_t>(JStatusesDefendFlag()) << ", " << Compatibility() << ", "
           << static_cast<std::uint32_t>(unknown3());
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_MAGIC_H
