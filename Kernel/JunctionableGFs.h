#ifndef VIIIARCHIVE_JUNCTIONABLEGFS_H
#define VIIIARCHIVE_JUNCTIONABLEGFS_H
#include "EncodedStringOffset.h"
#include "AttackTypeT.h"
#include "AttackFlagsT.h"
#include "BattleOnlyStatusesT.h"
#include "PersistentStatusesT.h"
#include "ElementT.h"
#include "GFGroup.h"
#include "UnlockableAbility.h"
namespace OpenVIII::Kernel {
struct JunctionableGFs
{
    /*
     * https://github.com/DarkShinryu/doomtrain/wiki/Junctionable-GFs
     * Offset	Length	Description
     * 0x0000	2 bytes	Offset to GF attack name
     * 0x0002	2 bytes	Offset to GF attack description
     * 0x0004	2 bytes	[[Magic ID
     * 0x0006	1 byte	Attack type
     * 0x0007	1 byte	GF power (used in damage formula)
     * 0x0008	2 bytes	Unknown
     * 0x000A	1 byte	Attack Flags
     * 0x000B	2 bytes	Unknown
     * 0x000D	1 byte	[[Element
     * 0x000E	2 bytes	[[Statuses 0
     * 0x0010	4 bytes	[[Statuses 1
     * 0x0014	1 byte	GF HP Modifier (used in GF HP formula)
     * 0x0015	6 bytes	Unknown
     * 0x001B	1 byte	Status attack enabler
     * 0x001C	1 byte	[[Ability 1 Unlocker
     * 0x001D	1 byte	Unknown
     * 0x001E	1 byte	[[Ability 1
     * 0x001F	1 byte	Unknown
     * 0x0020	1 byte	[[Ability 2 Unlocker
     * 0x0021	1 byte	Unknown
     * 0x0022	1 byte	[[Ability 2
     * 0x0023	1 byte	Unknown
     * 0x0024	1 byte	[[Ability 3 Unlocker
     * 0x0025	1 byte	Unknown
     * 0x0026	1 byte	[[Ability 3
     * 0x0027	1 byte	Unknown
     * 0x0028	1 byte	[[Ability 4 Unlocker
     * 0x0029	1 byte	Unknown
     * 0x002A	1 byte	[[Ability 4
     * 0x002B	1 byte	Unknown
     * 0x002C	1 byte	[[Ability 5 Unlocker
     * 0x002D	1 byte	Unknown
     * 0x002E	1 byte	[[Ability 5
     * 0x002F	1 byte	Unknown
     * 0x0030	1 byte	[[Ability 6 Unlocker
     * 0x0031	1 byte	Unknown
     * 0x0032	1 byte	[[Ability 6
     * 0x0033	1 byte	Unknown
     * 0x0034	1 byte	[[Ability 7 Unlocker
     * 0x0035	1 byte	Unknown
     * 0x0036	1 byte	[[Ability 7
     * 0x0037	1 byte	Unknown
     * 0x0038	1 byte	[[Ability 8 Unlocker
     * 0x0039	1 byte	Unknown
     * 0x003A	1 byte	[[Ability 8
     * 0x003B	1 byte	Unknown
     * 0x003C	1 byte	[[Ability 9 Unlocker
     * 0x003D	1 byte	Unknown
     * 0x003E	1 byte	[[Ability 9
     * 0x003F	1 byte	Unknown
     * 0x0040	1 byte	[[Ability 10 Unlocker
     * 0x0041	1 byte	Unknown
     * 0x0042	1 byte	[[Ability 10
     * 0x0043	1 byte	Unknown
     * 0x0044	1 byte	[[Ability 11 Unlocker
     * 0x0045	1 byte	Unknown
     * 0x0046	1 byte	[[Ability 11
     * 0x0047	1 byte	Unknown
     * 0x0048	1 byte	[[Ability 12 Unlocker
     * 0x0049	1 byte	Unknown
     * 0x004A	1 byte	[[Ability 12
     * 0x004B	1 byte	Unknown
     * 0x004C	1 byte	[[Ability 13 Unlocker
     * 0x004D	1 byte	Unknown
     * 0x004E	1 byte	[[Ability 13
     * 0x004F	1 byte	Unknown
     * 0x0050	1 byte	[[Ability 14 Unlocker
     * 0x0051	1 byte	Unknown
     * 0x0052	1 byte	[[Ability 14
     * 0x0053	1 byte	Unknown
     * 0x0054	1 byte	[[Ability 15 Unlocker
     * 0x0055	1 byte	Unknown
     * 0x0056	1 byte	[[Ability 15
     * 0x0057	1 byte	Unknown
     * 0x0058	1 byte	[[Ability 16 Unlocker
     * 0x0059	1 byte	Unknown
     * 0x005A	1 byte	[[Ability 16
     * 0x005B	1 byte	Unknown
     * 0x005C	1 byte	[[Ability 17 Unlocker
     * 0x005D	1 byte	Unknown
     * 0x005E	1 byte	[[Ability 17
     * 0x005F	1 byte	Unknown
     * 0x0060	1 byte	[[Ability 18 Unlocker
     * 0x0061	1 byte	Unknown
     * 0x0062	1 byte	[[Ability 18
     * 0x0063	1 byte	Unknown
     * 0x0064	1 byte	[[Ability 19 Unlocker
     * 0x0065	1 byte	Unknown
     * 0x0066	1 byte	[[Ability 19
     * 0x0067	1 byte	Unknown
     * 0x0068	1 byte	[[Ability 20 Unlocker
     * 0x0069	1 byte	Unknown
     * 0x006A	1 byte	[[Ability 20
     * 0x006B	1 byte	Unknown
     * 0x006C	1 byte	[[Ability 21 Unlocker
     * 0x006D	1 byte	Unknown
     * 0x006E	1 byte	[[Ability 21
     * 0x006F	1 byte	Unknown
     * 0x0070	1 byte	Quezacolt compatibility
     * 0x0071	1 byte	Shiva compatibility
     * 0x0072	1 byte	Ifrit compatibility
     * 0x0073	1 byte	Siren compatibility
     * 0x0074	1 byte	Brothers compatibility
     * 0x0075	1 byte	Diablos compatibility
     * 0x0076	1 byte	Carbuncle compatibility
     * 0x0077	1 byte	Leviathan compatibility
     * 0x0078	1 byte	Pandemona compatibility
     * 0x0079	1 byte	Cerberus compatibility
     * 0x007A	1 byte	Alexander compatibility
     * 0x007B	1 byte	Doomtrain compatibility
     * 0x007C	1 byte	Bahamut compatibility
     * 0x007D	1 byte	Cactuar compatibility
     * 0x007E	1 byte	Tonberry compatibility
     * 0x007F	1 byte	Eden compatibility
     * 0x0080	2 bytes	Unknown
     * 0x0082	1 byte	Power Mod (used in damage formula)
     * 0x0083	1 byte	Level Mod (used in damage formula)
     */
private:
EncodedStringOffset nameOffset_{};
EncodedStringOffset descriptionOffset_{};
std::uint16_t magicID_{};
AttackTypeT attackType_{};
std::uint8_t gfPower_{};
std::uint16_t unknown0_{};
AttackFlagsT attackFlags_{};
std::uint8_t unknown1_{};
std::uint8_t unknown2_{};
ElementT element_{};
PersistentStatusesT PersistentStatuses_{};
BattleOnlyStatusesT BattleOnlyStatuses_{};
std::uint8_t gfHPModifier_{};
std::uint8_t unknown3_{};
std::uint8_t unknown4_{};
std::uint8_t unknown5_{};
std::uint8_t unknown6_{};
std::uint8_t unknown7_{};
std::uint8_t unknown8_{};
std::uint8_t statusAttackEnabler_{};
UnlockableAbility unlockableAbility1_{};
UnlockableAbility unlockableAbility2_{};
UnlockableAbility unlockableAbility3_{};
UnlockableAbility unlockableAbility4_{};
UnlockableAbility unlockableAbility5_{};
UnlockableAbility unlockableAbility6_{};
UnlockableAbility unlockableAbility7_{};
UnlockableAbility unlockableAbility8_{};
UnlockableAbility unlockableAbility9_{};
UnlockableAbility unlockableAbility10_{};
UnlockableAbility unlockableAbility11_{};
UnlockableAbility unlockableAbility12_{};
UnlockableAbility unlockableAbility13_{};
UnlockableAbility unlockableAbility14_{};
UnlockableAbility unlockableAbility15_{};
UnlockableAbility unlockableAbility16_{};
UnlockableAbility unlockableAbility17_{};
UnlockableAbility unlockableAbility18_{};
UnlockableAbility unlockableAbility19_{};
UnlockableAbility unlockableAbility20_{};
UnlockableAbility unlockableAbility21_{};
GFGroup<std::uint8_t> compatibility_{};
std::uint16_t unknown9_{};
std::uint8_t powerMod_{};
std::uint8_t levelMod_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  [[nodiscard]] auto MagicID() const noexcept { return magicID_; }
  [[nodiscard]] auto AttackType() const noexcept { return attackType_; }
  [[nodiscard]] auto GFPower() const noexcept { return gfPower_; }
  [[nodiscard]] auto unknown0() const noexcept { return unknown0_; }
  [[nodiscard]] auto AttackFlags() const noexcept { return attackFlags_; }
  [[nodiscard]] auto unknown1() const noexcept { return unknown1_; }
  [[nodiscard]] auto unknown2() const noexcept { return unknown2_; }
  [[nodiscard]] auto Element() const noexcept { return element_; }
  [[nodiscard]] auto PersistentStatuses() const noexcept { return PersistentStatuses_; }
  [[nodiscard]] auto BattleOnlyStatuses() const noexcept { return BattleOnlyStatuses_; }
  [[nodiscard]] auto GFHPModifier() const noexcept { return gfHPModifier_; }
  [[nodiscard]] auto unknown3() const noexcept { return unknown3_; }
  [[nodiscard]] auto unknown4() const noexcept { return unknown4_; }
  [[nodiscard]] auto unknown5() const noexcept { return unknown5_; }
  [[nodiscard]] auto unknown6() const noexcept { return unknown6_; }
  [[nodiscard]] auto unknown7() const noexcept { return unknown7_; }
  [[nodiscard]] auto unknown8() const noexcept { return unknown8_; }
  [[nodiscard]] auto StatusAttackEnabler() const noexcept { return statusAttackEnabler_; }
  [[nodiscard]] auto UnlockableAbility1() const noexcept { return unlockableAbility1_; }
  [[nodiscard]] auto UnlockableAbility2() const noexcept { return unlockableAbility2_; }
  [[nodiscard]] auto UnlockableAbility3() const noexcept { return unlockableAbility3_; }
  [[nodiscard]] auto UnlockableAbility4() const noexcept { return unlockableAbility4_; }
  [[nodiscard]] auto UnlockableAbility5() const noexcept { return unlockableAbility5_; }
  [[nodiscard]] auto UnlockableAbility6() const noexcept { return unlockableAbility6_; }
  [[nodiscard]] auto UnlockableAbility7() const noexcept { return unlockableAbility7_; }
  [[nodiscard]] auto UnlockableAbility8() const noexcept { return unlockableAbility8_; }
  [[nodiscard]] auto UnlockableAbility9() const noexcept { return unlockableAbility9_; }
  [[nodiscard]] auto UnlockableAbility10() const noexcept { return unlockableAbility10_; }
  [[nodiscard]] auto UnlockableAbility11() const noexcept { return unlockableAbility11_; }
  [[nodiscard]] auto UnlockableAbility12() const noexcept { return unlockableAbility12_; }
  [[nodiscard]] auto UnlockableAbility13() const noexcept { return unlockableAbility13_; }
  [[nodiscard]] auto UnlockableAbility14() const noexcept { return unlockableAbility14_; }
  [[nodiscard]] auto UnlockableAbility15() const noexcept { return unlockableAbility15_; }
  [[nodiscard]] auto UnlockableAbility16() const noexcept { return unlockableAbility16_; }
  [[nodiscard]] auto UnlockableAbility17() const noexcept { return unlockableAbility17_; }
  [[nodiscard]] auto UnlockableAbility18() const noexcept { return unlockableAbility18_; }
  [[nodiscard]] auto UnlockableAbility19() const noexcept { return unlockableAbility19_; }
  [[nodiscard]] auto UnlockableAbility20() const noexcept { return unlockableAbility20_; }
  [[nodiscard]] auto UnlockableAbility21() const noexcept { return unlockableAbility21_; }
  [[nodiscard]] auto Compatibility() const noexcept { return compatibility_; }
  [[nodiscard]] auto unknown9() const noexcept { return unknown9_; }
  [[nodiscard]] auto PowerMod() const noexcept { return powerMod_; }
  [[nodiscard]] auto LevelMod() const noexcept { return levelMod_; }
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
}
#endif// VIIIARCHIVE_JUNCTIONABLEGFS_H
