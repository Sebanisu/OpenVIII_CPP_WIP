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

#ifndef VIIIARCHIVE_COMMANDABILITIES_HPP
#define VIIIARCHIVE_COMMANDABILITIES_HPP

#include "open_viii/strings/EncodedStringOffset.hpp"

namespace open_viii::kernel {
/**
 * https://github.com/DarkShinryu/doomtrain/wiki/Command-abilities
 * 0x0000	2 bytes	Offset to ability name
 * 0x0002	2 bytes	Offset to ability description
 * 0x0004	1 byte	AP Required to learn ability
 * 0x0005	1 byte	Index to Battle commands
 * 0x0006	2 bytes	Unknown/Unused
 */
template<LangT langVal> struct CommandAbilities
{
private:
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  std::uint8_t m_ability_points_required_to_unlock{};
  std::uint8_t m_index_to_battle_command{};
  std::uint8_t m_unknown0{};
  std::uint8_t m_unknown1{};

public:
  /**
   * Offset to encoded name
   * @return EncodedStringOffset
   */
  [[nodiscard]] auto &name_offset() const noexcept
  {
    return m_name_offset;
  }
  /**
   * Offset to encoded description
   */
  [[nodiscard]] auto &description_offset() const noexcept
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
  /**
   * Index to battle command. The related commands have different offsets as the
   * have a different quantity.
   */
  [[maybe_unused]] [[nodiscard]] constexpr auto
    index_to_battle_command() const noexcept
  {
    return m_index_to_battle_command;
  }
  [[nodiscard]] constexpr auto unknown0() const noexcept
  {
    return m_unknown0;
  }
  [[nodiscard]] constexpr auto unknown1() const noexcept
  {
    return m_unknown1;
  }
  /**
   * Dumps values to stream
   * @param os output stream
   * @param buffer contains encoded names and descriptions
   */
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
    os << ", "
       << static_cast<std::uint32_t>(m_ability_points_required_to_unlock)
       << ", " << static_cast<std::uint32_t>(m_index_to_battle_command) << ", "
       << static_cast<std::uint32_t>(m_unknown0) << ", "
       << static_cast<std::uint32_t>(m_unknown1);
    return os;
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_COMMANDABILITIES_HPP
