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
#ifndef VIIIARCHIVE_GFABILITIES_HPP
#define VIIIARCHIVE_GFABILITIES_HPP
#include "StatT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <compare>
namespace open_viii::kernel {
/**
 * Offset	Length	Description
 * 0x0000	2 bytes	Offset to ability name
 * 0x0002	2 bytes	Offset to ability description
 * 0x0004	1 byte	AP Required to learn ability
 * 0x0005	1 byte	Enable Boost
 * 0x0006	1 byte	Stat to increase
 * 0x0007	1 byte	Increase value
 * @see https://github.com/DarkShinryu/doomtrain/wiki/GF-abilities
 */
struct GFAbilities
{
private:
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  uint8_t             m_ability_points_required_to_unlock{};
  uint8_t             m_enable_boost{};
  StatT               m_stat_to_increase{};
  uint8_t             m_increase_value{};

public:
  constexpr auto operator<=>(const GFAbilities &right) const noexcept = default;
  [[nodiscard]] constexpr auto name_offset() const noexcept
  {
    return m_name_offset;
  }
  [[nodiscard]] constexpr auto description_offset() const noexcept
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
  [[maybe_unused]] [[nodiscard]] constexpr auto enable_boost() const noexcept
  {
    return m_enable_boost != 0;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    stat_to_increase() const noexcept
  {
    return m_stat_to_increase;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto increase_value() const noexcept
  {
    return m_increase_value;
  }
  std::ostream &out(std::ostream &                                os,
                    [[maybe_unused]] const std::span<const char> &buffer) const
  {
    os << ", " << static_cast<int>(m_ability_points_required_to_unlock);
    os << ", " << static_cast<int>(m_enable_boost);
    os << ", " << static_cast<int>(m_stat_to_increase);
    os << ", " << static_cast<int>(m_increase_value);
    return os;
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_GFABILITIES_HPP
