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

#ifndef VIIIARCHIVE_IRVINESHOTLIMITBREAK_H
#define VIIIARCHIVE_IRVINESHOTLIMITBREAK_H
#include "PersistentStatusesT.h"
#include "BattleOnlyStatusesT.h"
#include "ElementT.h"
#include "AttackTypeT.h"
#include "AttackFlagsT.h"
#include "TargetT.h"
#include "OpenVIII/Strings/EncodedStringOffset.h"
namespace open_viii::kernel {
template<LangT langVal> struct IrvineShotLimitBreak
{
  /* https://github.com/DarkShinryu/doomtrain/wiki/Shot-(Irvine-limit-break)
   * Offset	Length	Description
   std::uint16_t Offset to limit name
   std::uint16_t Offset to limit description
   std::uint16_t Magic ID
   std::uint8_t Attack Type
   std::uint8_t Attack Power
   std::uint16_t Unknown
   std::uint8_t Target Info
   std::uint8_t Attack Flags
   std::uint8_t Hit Count
   std::uint8_t Element Attack
   std::uint8_t Element Attack %
   std::uint8_t Status Attack Enabler
   std::uint16_t status_0; //statuses 0-7
   std::uint8_t Used item index
   std::uint8_t Critical increase
   4 bytes	status_1; //statuses 8-39
   */
private:
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  std::uint16_t m_magic_id{};
  AttackTypeT m_attack_type{};
  std::uint8_t m_attack_power{};
  std::uint8_t m_unknown0{};
  std::uint8_t m_unknown1{};
  TargetT m_target{};
  AttackFlagsT m_attack_flags{};
  std::uint8_t m_hit_count{};
  ElementT m_element{};
  std::uint8_t m_element_attack_percent{};
  std::uint8_t m_status_attack_enabler{};
  PersistentStatusesT m_persistent_statuses{};// statuses 0-7
  std::uint8_t m_used_item_index{};
  std::uint8_t m_critical_increase{};
  BattleOnlyStatusesT m_battle_only_statuses{};// statuses 8-39
public:
  [[nodiscard]] auto &name_offset() const noexcept { return m_name_offset; }
  [[nodiscard]] auto &description_offset() const noexcept { return m_description_offset; }
  [[nodiscard]] auto magic_id() const noexcept { return m_magic_id; }
  [[nodiscard]] auto attack_type() const noexcept { return m_attack_type; }
  [[nodiscard]] auto attack_power() const noexcept { return m_attack_power; }
  [[nodiscard]] auto unknown0() const noexcept { return m_unknown0; }
  [[nodiscard]] auto unknown1() const noexcept { return m_unknown1; }
  [[nodiscard]] auto target() const noexcept { return m_target; }
  [[nodiscard]] auto attack_flags() const noexcept { return m_attack_flags; }
  [[nodiscard]] auto hit_count() const noexcept { return m_hit_count; }
  [[nodiscard]] auto element() const noexcept { return m_element; }
  [[nodiscard]] auto element_attack_percent() const noexcept { return m_element_attack_percent; }
  [[nodiscard]] auto status_attack_enabler() const noexcept { return m_status_attack_enabler; }
  [[nodiscard]] auto persistent_statuses() const noexcept { return m_persistent_statuses; }// statuses 0-7
  [[nodiscard]] auto used_item_index() const noexcept { return m_used_item_index; }
  [[nodiscard]] auto critical_increase() const noexcept { return m_critical_increase; }
  [[nodiscard]] auto battle_only_statuses() const noexcept { return m_battle_only_statuses; }// statuses 8-39
  std::ostream &out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = m_name_offset.decoded_string<langVal>(buffer);
    auto description = m_description_offset.decoded_string<langVal>(buffer);
    if (!std::empty(name)) {
      os << Tools::u8_to_sv(name);
    }
    if (!std::empty(description)) {
      os << ", " << Tools::u8_to_sv(description);
    }
    return os << ", " << static_cast<std::uint32_t>(m_magic_id) << ", " << static_cast<std::uint32_t>(m_attack_type)
              << ", " << static_cast<std::uint32_t>(m_attack_power) << ", " << static_cast<std::uint32_t>(m_unknown0)
              << ", " << static_cast<std::uint32_t>(m_unknown1) << ", " << static_cast<std::uint32_t>(m_target) << ", "
              << static_cast<std::uint32_t>(m_attack_flags) << ", " << static_cast<std::uint32_t>(m_hit_count) << ", "
              << static_cast<std::uint32_t>(m_element) << ", " << static_cast<std::uint32_t>(m_element_attack_percent)
              << ", " << static_cast<std::uint32_t>(m_status_attack_enabler) << ", "
              << static_cast<std::uint32_t>(m_persistent_statuses) << ", "
              << static_cast<std::uint32_t>(m_used_item_index) << ", "
              << static_cast<std::uint32_t>(m_critical_increase) << ", "
              << static_cast<std::uint32_t>(m_battle_only_statuses)// statuses 8-39

      ;
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_IRVINESHOTLIMITBREAK_H
