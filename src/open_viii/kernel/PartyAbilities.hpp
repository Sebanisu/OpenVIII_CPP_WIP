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
#ifndef VIIIARCHIVE_PARTYABILITIES_HPP
#define VIIIARCHIVE_PARTYABILITIES_HPP
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <compare>
namespace open_viii::kernel {
/**
 * Offset	Length	Description
 * 0x0000	2 bytes	Offset to ability name
 * 0x0002	2 bytes	Offset to ability description
 * 0x0004	1 byte	AP Required to learn ability
 * 0x0005	1 byte	Flag
 * 0x0006	2 byte	Unknown/Unused
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Party-abilities
 */
struct PartyAbilities
{
private:
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  std::uint8_t        m_ability_points_required_to_unlock{};
  std::uint8_t        m_flag{};
  std::uint8_t        m_unknown0{};
  std::uint8_t        m_unknown1{};

public:
  constexpr auto
    operator<=>(const PartyAbilities &right) const noexcept = default;
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
    flag() const noexcept
  {
    return m_flag;
  }
  [[nodiscard]] constexpr auto
    unknown0() const noexcept
  {
    return m_unknown0;
  }
  [[nodiscard]] constexpr auto
    unknown1() const noexcept
  {
    return m_unknown1;
  }
  std::ostream &
    out(std::ostream &                                os,
        [[maybe_unused]] const std::span<const char> &buffer) const
  {
    return os << ", "
              << static_cast<std::uint32_t>(m_ability_points_required_to_unlock)
              << ", " << static_cast<std::uint32_t>(m_flag) << ", "
              << static_cast<std::uint32_t>(m_unknown0) << ", "
              << static_cast<std::uint32_t>(m_unknown1);
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_PARTYABILITIES_HPP
