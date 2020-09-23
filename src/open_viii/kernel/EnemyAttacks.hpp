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

#ifndef VIIIARCHIVE_ENEMYATTACKS_HPP
#define VIIIARCHIVE_ENEMYATTACKS_HPP

#include "AttackFlagsT.hpp"
#include "AttackTypeT.hpp"
#include "BattleOnlyStatusesT.hpp"
#include "CameraChange.hpp"
#include "ElementT.hpp"
#include "PersistentStatusesT.hpp"
#include "open_viii/Strings/EncodedStringOffset.hpp"
namespace open_viii::kernel {
template<LangT langVal> struct EnemyAttacks
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
  EncodedStringOffset m_name_offset{};
  uint16_t m_magic_id{};
  CameraChange m_camera_change{};
  uint8_t m_unknown0{};
  AttackTypeT m_attack_type{};
  uint8_t m_attack_power{};
  AttackFlagsT m_attack_flags{};
  uint8_t m_unknown1{};
  ElementT m_element{};
  uint8_t m_unknown2{};
  uint8_t m_status_attack_enabler{};
  uint8_t m_attack_parameter{};
  PersistentStatusesT m_persistent_statuses{};// statuses 0-7
  BattleOnlyStatusesT m_battle_only_statuses{};// statuses 8-31

public:
  [[nodiscard]] const auto &name_offset() const noexcept
  {
    return m_name_offset;
  }
  [[nodiscard]] const auto &magic_id() const noexcept
  {
    return m_magic_id;
  }
  [[maybe_unused]] [[nodiscard]] const auto &camera_change() const noexcept
  {
    return m_camera_change;
  }
  [[nodiscard]] const auto &unknown0() const noexcept
  {
    return m_unknown0;
  }
  [[nodiscard]] const auto &attack_type() const noexcept
  {
    return m_attack_type;
  }
  [[maybe_unused]] [[nodiscard]] const auto &attack_power() const noexcept
  {
    return m_attack_power;
  }
  [[nodiscard]] const auto &attack_flags() const noexcept
  {
    return m_attack_flags;
  }
  [[nodiscard]] const auto &unknown1() const noexcept
  {
    return m_unknown1;
  }
  [[nodiscard]] const auto &element() const noexcept
  {
    return m_element;
  }
  [[nodiscard]] const auto &unknown2() const noexcept
  {
    return m_unknown2;
  }
  [[nodiscard]] const auto &status_attack_enabler() const noexcept
  {
    return m_status_attack_enabler;
  }
  [[maybe_unused]] [[nodiscard]] const auto &attack_parameter() const noexcept
  {
    return m_attack_parameter;
  }
  [[nodiscard]] const auto &persistent_statuses() const noexcept
  {
    return m_persistent_statuses;
  }// statuses 0-7
  [[nodiscard]] const auto &battle_only_statuses() const noexcept
  {
    return m_battle_only_statuses;
  }// statuses 8-31
  std::ostream &out(std::ostream &os, const std::span<const char> &buffer) const
  {
    auto name = m_name_offset.decoded_string<langVal>(buffer);
    if (!std::empty(name)) {
      os << Tools::u8_to_sv(name) << ", ";
    }
    return os << static_cast<std::uint32_t>(m_magic_id) << ", " << m_camera_change << ", "
              << static_cast<std::uint32_t>(m_unknown0) << ", " << static_cast<std::uint32_t>(m_attack_type) << ", "
              << static_cast<std::uint32_t>(m_attack_power) << ", " << static_cast<std::uint32_t>(m_attack_flags)
              << ", " << static_cast<std::uint32_t>(m_unknown1) << ", " << static_cast<std::uint32_t>(m_element) << ", "
              << static_cast<std::uint32_t>(m_unknown2) << ", " << static_cast<std::uint32_t>(m_status_attack_enabler)
              << ", " << static_cast<std::uint32_t>(m_attack_parameter) << ", "
              << static_cast<std::uint32_t>(m_persistent_statuses)// statuses 0-7
              << ", " << static_cast<std::uint32_t>(m_battle_only_statuses)// statuses 8-31
      ;
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_ENEMYATTACKS_HPP
