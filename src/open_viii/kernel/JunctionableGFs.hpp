#ifndef VIIIARCHIVE_JUNCTIONABLEGFS_HPP
#define VIIIARCHIVE_JUNCTIONABLEGFS_HPP
#include "open_viii/Strings/EncodedStringOffset.hpp"
#include "AttackTypeT.hpp"
#include "AttackFlagsT.hpp"
#include "BattleOnlyStatusesT.hpp"
#include "PersistentStatusesT.hpp"
#include "ElementT.hpp"
#include "GFGroup.hpp"
#include "UnlockableAbility.hpp"
namespace open_viii::kernel {
template<LangT langVal> struct JunctionableGFs
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
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  std::uint16_t m_magic_id{};
  AttackTypeT m_attack_type{};
  std::uint8_t m_gf_power{};
  std::uint16_t m_unknown0{};
  AttackFlagsT m_attack_flags{};
  std::uint8_t m_unknown1{};
  std::uint8_t m_unknown2{};
  ElementT m_element{};
  PersistentStatusesT m_persistent_statuses{};
  BattleOnlyStatusesT m_battle_only_statuses{};
  std::uint8_t m_gf_hp_modifier{};
  std::uint8_t m_unknown3{};
  std::uint8_t m_unknown4{};
  std::uint8_t m_unknown5{};
  std::uint8_t m_unknown6{};
  std::uint8_t m_unknown7{};
  std::uint8_t m_unknown8{};
  std::uint8_t m_status_attack_enabler{};
  static constexpr auto MAX_ABILITIES = 21U;
  std::array<UnlockableAbility, MAX_ABILITIES> m_unlockable_abilities{};
  GFGroup<std::uint8_t> m_compatibility{};
  std::uint8_t m_unknown9{};
  std::uint8_t m_unknown10{};
  std::uint8_t m_power_mod{};
  std::uint8_t m_level_mod{};

public:
  [[nodiscard]] const auto &name_offset() const noexcept { return m_name_offset; }
  [[nodiscard]] const auto &description_offset() const noexcept { return m_description_offset; }
  [[nodiscard]] const auto &magic_id() const noexcept { return m_magic_id; }
  [[nodiscard]] const auto &attack_type() const noexcept { return m_attack_type; }
  [[maybe_unused]] [[nodiscard]] const auto &gf_power() const noexcept { return m_gf_power; }
  [[nodiscard]] const auto &unknown0() const noexcept { return m_unknown0; }
  [[nodiscard]] const auto &attack_flags() const noexcept { return m_attack_flags; }
  [[nodiscard]] const auto &unknown1() const noexcept { return m_unknown1; }
  [[nodiscard]] const auto &unknown2() const noexcept { return m_unknown2; }
  [[nodiscard]] const auto &element() const noexcept { return m_element; }
  [[nodiscard]] const auto &persistent_statuses() const noexcept { return m_persistent_statuses; }
  [[nodiscard]] const auto &battle_only_statuses() const noexcept { return m_battle_only_statuses; }
  [[maybe_unused]] [[nodiscard]] const auto &gfhp_modifier() const noexcept { return m_gf_hp_modifier; }
  [[maybe_unused]] [[nodiscard]] const auto &unknown3() const noexcept { return m_unknown3; }
  [[nodiscard]] const auto &unknown4() const noexcept { return m_unknown4; }
  [[maybe_unused]] [[nodiscard]] const auto &unknown5() const noexcept { return m_unknown5; }
  [[maybe_unused]] [[nodiscard]] const auto &unknown6() const noexcept { return m_unknown6; }
  [[maybe_unused]] [[nodiscard]] const auto &unknown7() const noexcept { return m_unknown7; }
  [[maybe_unused]] [[nodiscard]] const auto &unknown8() const noexcept { return m_unknown8; }
  [[nodiscard]] const auto &status_attack_enabler() const noexcept { return m_status_attack_enabler; }
  template<size_t i> [[nodiscard]] const auto &unlockable_abilities() const noexcept
  {
    static_assert(i < MAX_ABILITIES);
    return m_unlockable_abilities.at(i);
  }
  [[nodiscard]] const auto &unlockable_abilities() const noexcept { return m_unlockable_abilities; }
  [[maybe_unused]] [[nodiscard]] const auto &compatibility() const noexcept { return m_compatibility; }
  [[maybe_unused]] [[nodiscard]] const auto &unknown9() const noexcept { return m_unknown9; }
  [[maybe_unused]] [[nodiscard]] const auto &unknown10() const noexcept { return m_unknown10; }
  [[nodiscard]] const auto &power_mod() const noexcept { return m_power_mod; }
  [[maybe_unused]] [[nodiscard]] const auto &level_mod() const noexcept { return m_level_mod; }
  std::ostream &out(std::ostream &os, const std::span<const char> &buffer) const
  {
    auto name = m_name_offset.decoded_string<langVal>(buffer);
    auto description = m_description_offset.decoded_string<langVal>(buffer);
    if (!std::empty(name)) {
      os << Tools::u8_to_sv(name);
    }
    if (!std::empty(description)) {
      os << ", " << Tools::u8_to_sv(description);
    }
    os << ", " << static_cast<std::uint32_t>(m_magic_id) << ", " << static_cast<std::uint32_t>(m_attack_type) << ", "
       << static_cast<std::uint32_t>(m_gf_power) << ", " << static_cast<std::uint32_t>(m_unknown0) << ", "
       << static_cast<std::uint32_t>(m_attack_flags) << ", " << static_cast<std::uint32_t>(m_unknown1) << ", "
       << static_cast<std::uint32_t>(m_unknown2) << ", " << static_cast<std::uint32_t>(m_element) << ", "
       << static_cast<std::uint32_t>(m_persistent_statuses) << ", "
       << static_cast<std::uint32_t>(m_battle_only_statuses) << ", " << static_cast<std::uint32_t>(m_gf_hp_modifier)
       << ", " << static_cast<std::uint32_t>(m_unknown3) << ", " << static_cast<std::uint32_t>(m_unknown4) << ", "
       << static_cast<std::uint32_t>(m_unknown5) << ", " << static_cast<std::uint32_t>(m_unknown6) << ", "
       << static_cast<std::uint32_t>(m_unknown7) << ", " << static_cast<std::uint32_t>(m_unknown8) << ", "
       << static_cast<std::uint32_t>(m_status_attack_enabler) << ", ";
    bool first = true;
    for (const auto &item : m_unlockable_abilities) {
      if (!first) {
        os << ", ";
      } else {
        first = false;
      }
      os << item;
    }
    return os << ", " << m_compatibility << ", " << static_cast<std::uint32_t>(m_unknown9) << ", "
              << static_cast<std::uint32_t>(m_unknown10) << ", " << static_cast<std::uint32_t>(m_power_mod) << ", "
              << static_cast<std::uint32_t>(m_level_mod);
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_JUNCTIONABLEGFS_HPP
