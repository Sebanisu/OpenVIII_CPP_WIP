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
#ifndef VIIIARCHIVE_STATPERCENTINCREASEABILITIES_HPP
#define VIIIARCHIVE_STATPERCENTINCREASEABILITIES_HPP
#include "open_viii/strings/EncodedStringOffset.hpp"
namespace open_viii::kernel {
/**
 * 0x0000	2 bytes	Offset to ability name
 * 0x0002	2 bytes	Offset to ability description
 * 0x0004	1 byte	AP needed to learn the ability
 * 0x0005	1 byte	Stat to increase
 * 0x0006	1 byte	Increase value
 * 0x0007	1 byte	Unknown/Unused
 * @see
 * https://github.com/DarkShinryu/doomtrain/wiki/Stat-percentage-increasing-abilities
 */
struct StatPercentIncreaseAbilities
{
private:
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  std::uint8_t        m_ability_points_required_to_unlock{};
  std::uint8_t        m_stat_to_increase{};
  std::uint8_t        m_increased_value{};
  std::uint8_t        m_unknown0{};

public:
  constexpr auto operator<=>(
    const StatPercentIncreaseAbilities &right) const noexcept = default;
  [[nodiscard]] constexpr auto
    name_offset() const noexcept
  {
    return m_name_offset;
  }
  [[nodiscard]] constexpr auto
    description_offset() const noexcept
  {
    return m_description_offset;
  }
  /**
   * Ability points required to unlock
   * @see
   * https://www.gamerguides.com/final-fantasy-viii/guide/guardian-forces/overview/ap-and-learning-abilities#learning-and-forgetting-abilities
   */
  [[maybe_unused]] [[nodiscard]] constexpr auto
    ability_points_required_to_unlock() const noexcept
  {
    return m_ability_points_required_to_unlock;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    stat_to_increase() const noexcept
  {
    return m_stat_to_increase;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    increased_value() const noexcept
  {
    return m_increased_value;
  }
  [[nodiscard]] constexpr auto
    unknown0() const noexcept
  {
    return m_unknown0;
  }
  std::ostream &
    out(std::ostream &                                os,
        [[maybe_unused]] const std::span<const char> &buffer) const
  {
    return os << ", "
              << static_cast<std::uint32_t>(m_ability_points_required_to_unlock)
              << ", " << static_cast<std::uint32_t>(m_stat_to_increase) << ", "
              << static_cast<std::uint32_t>(m_increased_value) << ", "
              << static_cast<std::uint32_t>(m_unknown0);
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_STATPERCENTINCREASEABILITIES_HPP
