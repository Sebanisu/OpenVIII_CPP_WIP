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
#ifndef VIIIARCHIVE_COMMANDABILITIESDATA_H
#define VIIIARCHIVE_COMMANDABILITIESDATA_H
#include "AttackFlagsT.hpp"
#include "AttackTypeT.hpp"
#include "BattleOnlyStatusesT.hpp"
#include "ElementT.hpp"
#include "PersistentStatusesT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <compare>
namespace open_viii::kernel {
/**
 * 0x0000	2 bytes	Magic ID
 * 0x0002	2 bytes	Unknown
 * 0x0004	1 byte	Attack type
 * 0x0005	1 byte	Attack power
 * 0x0006	1 byte	Attack flags
 * 0x0007	1 byte	Hit Count
 * 0x0008	1 byte	Element
 * 0x0009	1 byte	Status attack enabler
 * 0x000A	6 bytes	Statuses
 *  public PersistentStatuses Statuses0 { get;  }
 *  public BattleOnlyStatuses Statuses1 { get;  }
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Command-ability-data
 */
struct CommandAbilitiesData
{
private:
  std::uint16_t       m_magic_id{};
  std::uint16_t       m_unknown{};
  AttackTypeT         m_attack_type{};
  std::uint8_t        m_attack_power{};
  AttackFlagsT        m_attack_flags{};
  std::uint8_t        m_hit_count{};
  ElementT            m_element{};
  std::uint8_t        m_status_attack_enabler{};
  PersistentStatusesT m_persistent_statuses{};
  BattleOnlyStatusesT m_battle_only_statuses{};

public:
  constexpr auto
    operator<=>(const CommandAbilitiesData &right) const noexcept = default;
  [[nodiscard]] constexpr auto magic_id() const noexcept
  {
    return m_magic_id;
  }
  [[nodiscard]] constexpr auto unknown() const noexcept
  {
    return m_unknown;
  }
  [[nodiscard]] constexpr auto attack_type() const noexcept
  {
    return m_attack_type;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto attack_power() const noexcept
  {
    return m_attack_power;
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
  [[nodiscard]] constexpr auto status_attack_enabler() const noexcept
  {
    return m_status_attack_enabler;
  }
  [[nodiscard]] constexpr auto persistent_statuses() const noexcept
  {
    return m_persistent_statuses;
  }
  [[nodiscard]] constexpr auto battle_only_statuses() const noexcept
  {
    return m_battle_only_statuses;
  }
  std::ostream &out(std::ostream &                                os,
                    [[maybe_unused]] const std::span<const char> &buffer) const
  {
    return os << m_magic_id << ", " << m_unknown << ", "
              << static_cast<std::uint32_t>(m_attack_type) << ", "
              << static_cast<std::uint32_t>(m_attack_power) << ", "
              << static_cast<std::uint32_t>(m_attack_flags) << ", "
              << static_cast<std::uint32_t>(m_hit_count) << ", "
              << static_cast<std::uint32_t>(m_element) << ", "
              << static_cast<std::uint32_t>(m_status_attack_enabler) << ", "
              << static_cast<std::uint32_t>(m_persistent_statuses) << ", "
              << static_cast<std::uint32_t>(m_battle_only_statuses);
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_COMMANDABILITIESDATA_H
