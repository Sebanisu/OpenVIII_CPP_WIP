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
#include "CommonKernel.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <compare>
namespace open_viii::kernel {
struct CharacterAbilities_impl
{
  /**
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	AP Required to learn ability
   * 0x0005	3 byte	Flags
   * @see https://github.com/DarkShinryu/doomtrain/wiki/Character-abilities
   */
protected:
  EncodedStringOffset m_name_offset                       = {};
  EncodedStringOffset m_description_offset                = {};
  std::uint8_t        m_ability_points_required_to_unlock = {};

private:
  std::uint8_t m_character_ability_flags0 = {};
  std::uint8_t m_character_ability_flags1 = {};
  std::uint8_t m_character_ability_flags2 = {};

protected:
  constexpr static auto EXPECTED_SIZE = 8U;
  CharacterAbilities_impl()           = default;
  [[nodiscard]] constexpr CharacterAbilityFlagsT
    character_ability_flags_impl() const
  {
    return static_cast<CharacterAbilityFlagsT>(
      m_character_ability_flags0 << 16U | m_character_ability_flags1 << 8U
      | m_character_ability_flags2);
  }

public:
  constexpr auto
    operator<=>(const CharacterAbilities_impl &right) const noexcept = default;
};
using CharacterAbilities = CommonKernel<CharacterAbilities_impl>;
static_assert(sizeof(CharacterAbilities) == CharacterAbilities::EXPECTED_SIZE);
static_assert(has_name_offset<CharacterAbilities>);
static_assert(has_description_offset<CharacterAbilities>);
static_assert(has_ability_points_required_to_unlock<CharacterAbilities>);
static_assert(has_character_ability_flags<CharacterAbilities>);
static_assert(CharacterAbilities().name_offset().offset() == 0U);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_CHARACTERABILITIES_HPP
