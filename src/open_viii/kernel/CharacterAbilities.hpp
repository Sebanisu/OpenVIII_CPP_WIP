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
#ifndef VIIIARCHIVE_CHARACTERABILITIES_HPP
#define VIIIARCHIVE_CHARACTERABILITIES_HPP
#include "CharacterAbilityFlagsT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <compare>
#include <cstring>
namespace open_viii::kernel {
struct CharacterAbilities
{
  /**
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	AP Required to learn ability
   * 0x0005	3 byte	Flags
   * @see https://github.com/DarkShinryu/doomtrain/wiki/Character-abilities
   */
private:
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  std::uint8_t        m_ability_points_required_to_unlock{};
  // uint32_t characterAbilityFlags_ : 3;// cpp20 allows default member
  // initializers for bitfields add {} in cpp20.
  // std::array<std::uint8_t, 3> m_character_ability_flags{};
  std::uint8_t m_character_ability_flags0 {};
  std::uint8_t m_character_ability_flags1 {};
  std::uint8_t m_character_ability_flags2 {};
public:
  constexpr static auto EXPECTED_SIZE = 8U;
  constexpr auto
    operator<=>(const CharacterAbilities &right) const noexcept = default;
  [[maybe_unused]] [[nodiscard]] constexpr auto name_offset() const noexcept
  {
    return m_name_offset;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
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
  [[nodiscard]] constexpr CharacterAbilityFlagsT character_ability_flags() const
  {
    //    // I think this is okay.
    //    // The size of the enum is 4 bytes but the field is 3 bytes.
    //    // return static_cast<CharacterAbilityFlagsT>(characterAbilityFlags_);
    //    CharacterAbilityFlagsT out{};
    //    std::memcpy(
    //      &out, m_character_ability_flags.data(),
    //      m_character_ability_flags.size());
    //    // out =
    //    static_cast<CharacterAbilityFlagsT>(static_cast<uint32_t>(out) <<
    //    // 1U);
    //    return out;
    return static_cast<CharacterAbilityFlagsT>(m_character_ability_flags0<<16U | m_character_ability_flags1<<8U | m_character_ability_flags2);
  }
  std::ostream &out(std::ostream &                                os,
                    [[maybe_unused]] const std::span<const char> &buffer) const
  {
    auto test = character_ability_flags();
    return os << ", "
              << static_cast<std::uint32_t>(m_ability_points_required_to_unlock)
              << ", " << static_cast<uint32_t>(test);
  }
};
static_assert(sizeof(CharacterAbilities) == CharacterAbilities::EXPECTED_SIZE);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_CHARACTERABILITIES_HPP
