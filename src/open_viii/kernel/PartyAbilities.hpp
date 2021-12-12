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
#include "CommonKernel.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
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
struct PartyAbilities_impl
{
protected:
  EncodedStringOffset          m_name_offset                       = {};
  EncodedStringOffset          m_description_offset                = {};
  std::uint8_t                 m_ability_points_required_to_unlock = {};
  std::uint8_t                 m_flag                              = {};
  std::uint8_t                 m_unknown0                          = {};
  std::uint8_t                 m_unknown1                          = {};
  static constexpr std::size_t EXPECTED_SIZE                       = 8U;
  constexpr PartyAbilities_impl()                                  = default;

public:
  constexpr auto
    operator<=>(const PartyAbilities_impl &right) const noexcept = default;
};
using PartyAbilities = CommonKernel<PartyAbilities_impl>;
static_assert(sizeof(PartyAbilities) == PartyAbilities::EXPECTED_SIZE);
static_assert(has_name_offset<PartyAbilities>);
static_assert(has_description_offset<PartyAbilities>);
static_assert(has_ability_points_required_to_unlock<PartyAbilities>);
static_assert(has_flag<PartyAbilities>);
static_assert(has_unknown0<PartyAbilities>);
static_assert(has_unknown1<PartyAbilities>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_PARTYABILITIES_HPP
