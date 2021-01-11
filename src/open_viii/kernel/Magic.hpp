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

#ifndef VIIIARCHIVE_MAGIC_HPP
#define VIIIARCHIVE_MAGIC_HPP
#include "AttackFlagsT.hpp"
#include "AttackTypeT.hpp"
#include "BattleOnlyStatusesT.hpp"
#include "ElementT.hpp"
#include "GFGroup.hpp"
#include "JunctionStatusesT.hpp"
#include "PersistentStatusesT.hpp"
#include "StatGroup.hpp"
#include "TargetT.hpp"
#include "open_viii/Strings/EncodedStringOffset.hpp"
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
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  uint16_t m_magic_id{};
  uint8_t m_unknown0{};
  AttackTypeT m_attack_type{};
  uint8_t m_spell_power{};
  uint8_t m_unknown1{};
  TargetT m_target{};
  AttackFlagsT m_attack_flags{};
  uint8_t m_draw_resist{};
  uint8_t m_hit_count{};
  ElementT m_element{};
  uint8_t m_unknown2{};
  BattleOnlyStatusesT m_battle_only_statuses{};
  PersistentStatusesT m_persistent_statuses{};
  uint8_t m_status_attack{};
  StatGroup<uint8_t> m_junction_stats{};
  ElementT m_j_elem_attack_flag{};
  uint8_t m_j_elem_attack_value{};
  ElementT m_j_elem_defense_flag{};
  uint8_t m_j_elem_defense_value{};
  uint8_t m_j_status_attack_value{};
  uint8_t m_j_status_defense_value{};
  JunctionStatusesT m_j_statuses_attack_flag{};
  JunctionStatusesT m_j_statuses_defend_flag{};
  GFGroup<uint8_t> m_compatibility{};
  uint16_t m_unknown3{};

public:
  [[nodiscard]] auto &name_offset() const noexcept
  {
    return m_name_offset;
  }
  [[nodiscard]] auto &description_offset() const noexcept
  {
    return m_description_offset;
  }
  [[nodiscard]] auto magic_id() const noexcept
  {
    return m_magic_id;
  }
  [[nodiscard]] auto unknown0() const noexcept
  {
    return m_unknown0;
  }
  [[nodiscard]] auto attack_type() const noexcept
  {
    return m_attack_type;
  }
  [[maybe_unused]] [[nodiscard]] auto spell_power() const noexcept
  {
    return m_spell_power;
  }
  [[nodiscard]] auto unknown1() const noexcept
  {
    return m_unknown1;
  }
  [[nodiscard]] auto target() const noexcept
  {
    return m_target;
  }
  [[nodiscard]] auto attack_flags() const noexcept
  {
    return m_attack_flags;
  }
  [[maybe_unused]] [[nodiscard]] auto draw_resist() const noexcept
  {
    return m_draw_resist;
  }
  [[nodiscard]] auto hit_count() const noexcept
  {
    return m_hit_count;
  }
  [[nodiscard]] auto element() const noexcept
  {
    return m_element;
  }
  [[nodiscard]] auto unknown2() const noexcept
  {
    return m_unknown2;
  }
  [[nodiscard]] auto battle_only_statuses() const noexcept
  {
    return m_battle_only_statuses;
  }
  [[nodiscard]] auto persistent_statuses() const noexcept
  {
    return m_persistent_statuses;
  }
  [[maybe_unused]] [[nodiscard]] auto status_attack() const noexcept
  {
    return m_status_attack;
  }
  [[maybe_unused]] [[nodiscard]] auto junction_stats() const noexcept
  {
    return m_junction_stats;
  }
  [[maybe_unused]] [[nodiscard]] auto j_elem_attack_flag() const noexcept
  {
    return m_j_elem_attack_flag;
  }
  [[maybe_unused]] [[nodiscard]] auto j_elem_attack_value() const noexcept
  {
    return m_j_elem_attack_value;
  }
  [[maybe_unused]] [[nodiscard]] auto j_elem_defense_flag() const noexcept
  {
    return m_j_elem_defense_flag;
  }
  [[maybe_unused]] [[nodiscard]] auto j_elem_defense_value() const noexcept
  {
    return m_j_elem_defense_value;
  }
  [[maybe_unused]] [[nodiscard]] auto j_status_attack_value() const noexcept
  {
    return m_j_status_attack_value;
  }
  [[maybe_unused]] [[nodiscard]] auto j_status_defense_value() const noexcept
  {
    return m_j_status_defense_value;
  }
  [[maybe_unused]] [[nodiscard]] auto j_statuses_attack_flag() const noexcept
  {
    return m_j_statuses_attack_flag;
  }
  [[maybe_unused]] [[nodiscard]] auto j_statuses_defend_flag() const noexcept
  {
    return m_j_statuses_defend_flag;
  }
  [[maybe_unused]] [[nodiscard]] auto compatibility() const noexcept
  {
    return m_compatibility;
  }
  [[maybe_unused]] [[nodiscard]] auto unknown3() const noexcept
  {
    return m_unknown3;
  }

  std::ostream &out(std::ostream &os, const std::span<const char> &buffer) const
  {
    auto name = m_name_offset.decoded_string<langVal>(buffer);
    auto description = m_description_offset.decoded_string<langVal>(buffer);
    if (!std::empty(name)) {
      os << tools::u8_to_sv(name);
    }
    if (!std::empty(description)) {
      os << ", " << tools::u8_to_sv(description);
    }
    return os

           << ", " << static_cast<std::uint32_t>(m_magic_id) << ", "
           << static_cast<std::uint32_t>(m_unknown0) << ", "
           << static_cast<std::uint32_t>(m_attack_type) << ", "
           << static_cast<std::uint32_t>(m_spell_power) << ", "
           << static_cast<std::uint32_t>(m_unknown1) << ", "
           << static_cast<std::uint32_t>(m_target) << ", "
           << static_cast<std::uint32_t>(m_attack_flags) << ", "
           << static_cast<std::uint32_t>(m_draw_resist) << ", "
           << static_cast<std::uint32_t>(m_hit_count) << ", "
           << static_cast<std::uint32_t>(m_element) << ", "
           << static_cast<std::uint32_t>(m_unknown2) << ", "
           << static_cast<std::uint32_t>(m_battle_only_statuses) << ", "
           << static_cast<std::uint32_t>(m_persistent_statuses) << ", "
           << static_cast<std::uint32_t>(m_status_attack) << ", "
           << m_junction_stats << ", "
           << static_cast<std::uint32_t>(m_j_elem_attack_flag) << ", "
           << static_cast<std::uint32_t>(m_j_elem_attack_value) << ", "
           << static_cast<std::uint32_t>(m_j_elem_defense_flag) << ", "
           << static_cast<std::uint32_t>(m_j_elem_defense_value) << ", "
           << static_cast<std::uint32_t>(m_j_status_attack_value) << ", "
           << static_cast<std::uint32_t>(m_j_status_defense_value) << ", "
           << static_cast<std::uint32_t>(m_j_statuses_attack_flag) << ", "
           << static_cast<std::uint32_t>(m_j_statuses_defend_flag) << ", "
           << m_compatibility << ", " << static_cast<std::uint32_t>(m_unknown3);
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_MAGIC_HPP
