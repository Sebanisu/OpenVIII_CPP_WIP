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
#ifndef VIIIARCHIVE_IRVINESHOTLIMITBREAK_HPP
#define VIIIARCHIVE_IRVINESHOTLIMITBREAK_HPP
#include "AttackFlagsT.hpp"
#include "AttackTypeT.hpp"
#include "BattleOnlyStatusesT.hpp"
#include "ElementT.hpp"
#include "PersistentStatusesT.hpp"
#include "TargetT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
namespace open_viii::kernel {
/**
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
 @see https://github.com/DarkShinryu/doomtrain/wiki/Shot-(Irvine-limit-break)
 */
struct IrvineShotLimitBreak
{
private:
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  std::uint16_t       m_magic_id{};
  AttackTypeT         m_attack_type{};
  std::uint8_t        m_attack_power{};
  std::uint8_t        m_unknown0{};
  std::uint8_t        m_unknown1{};
  TargetT             m_target{};
  AttackFlagsT        m_attack_flags{};
  std::uint8_t        m_hit_count{};
  ElementT            m_element{};
  std::uint8_t        m_element_attack_percent{};
  std::uint8_t        m_status_attack_enabler{};
  PersistentStatusesT m_persistent_statuses{};// statuses 0-7
  std::uint8_t        m_used_item_index{};
  std::uint8_t        m_critical_increase{};
  BattleOnlyStatusesT m_battle_only_statuses{};// statuses 8-39
public:
  constexpr auto
    operator<=>(const IrvineShotLimitBreak &right) const noexcept = default;
  [[nodiscard]] constexpr auto name_offset() const noexcept
  {
    return m_name_offset;
  }
  [[nodiscard]] constexpr auto description_offset() const noexcept
  {
    return m_description_offset;
  }
  [[nodiscard]] constexpr auto magic_id() const noexcept
  {
    return m_magic_id;
  }
  [[nodiscard]] constexpr auto attack_type() const noexcept
  {
    return m_attack_type;
  }
  [[nodiscard]] constexpr auto attack_power() const noexcept
  {
    return m_attack_power;
  }
  [[nodiscard]] constexpr auto unknown0() const noexcept
  {
    return m_unknown0;
  }
  [[nodiscard]] constexpr auto unknown1() const noexcept
  {
    return m_unknown1;
  }
  [[nodiscard]] constexpr auto target() const noexcept
  {
    return m_target;
  }
  [[nodiscard]] constexpr auto attack_flags() const noexcept
  {
    return m_attack_flags;
  }
  [[nodiscard]] constexpr auto hit_count() const noexcept
  {
    return m_hit_count;
  }
  [[nodiscard]] constexpr auto element() const noexcept
  {
    return m_element;
  }
  [[nodiscard]] constexpr auto element_attack_percent() const noexcept
  {
    return m_element_attack_percent;
  }
  [[nodiscard]] constexpr auto status_attack_enabler() const noexcept
  {
    return m_status_attack_enabler;
  }
  [[nodiscard]] constexpr auto persistent_statuses() const noexcept
  {
    return m_persistent_statuses;
  }// statuses 0-7
  [[nodiscard]] constexpr auto used_item_index() const noexcept
  {
    return m_used_item_index;
  }
  [[nodiscard]] constexpr auto critical_increase() const noexcept
  {
    return m_critical_increase;
  }
  [[nodiscard]] constexpr auto battle_only_statuses() const noexcept
  {
    return m_battle_only_statuses;
  }// statuses 8-39
  std::ostream &out(std::ostream &                                os,
                    [[maybe_unused]] const std::span<const char> &buffer) const
  {
    return os << ", " << static_cast<std::uint32_t>(m_magic_id) << ", "
              << static_cast<std::uint32_t>(m_attack_type) << ", "
              << static_cast<std::uint32_t>(m_attack_power) << ", "
              << static_cast<std::uint32_t>(m_unknown0) << ", "
              << static_cast<std::uint32_t>(m_unknown1) << ", "
              << static_cast<std::uint32_t>(m_target) << ", "
              << static_cast<std::uint32_t>(m_attack_flags) << ", "
              << static_cast<std::uint32_t>(m_hit_count) << ", "
              << static_cast<std::uint32_t>(m_element) << ", "
              << static_cast<std::uint32_t>(m_element_attack_percent) << ", "
              << static_cast<std::uint32_t>(m_status_attack_enabler) << ", "
              << static_cast<std::uint32_t>(m_persistent_statuses) << ", "
              << static_cast<std::uint32_t>(m_used_item_index) << ", "
              << static_cast<std::uint32_t>(m_critical_increase) << ", "
              << static_cast<std::uint32_t>(
                   m_battle_only_statuses)// statuses 8-39
      ;
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_IRVINESHOTLIMITBREAK_HPP
