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
#ifndef VIIIARCHIVE_MENUABILITIES_HPP
#define VIIIARCHIVE_MENUABILITIES_HPP
#include "CommonKernel.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
namespace open_viii::kernel {
/**
 * 0x0000	2 bytes	Offset to ability name
 * 0x0002	2 bytes	Offset to ability description
 * 0x0004	1 byte	AP Required to learn ability
 * 0x0005	1 byte	Index to m00X files in menu.fs
 * (first 3 sections are treated as special cases)
 * 0x0006	1 byte	Start offset
 * 0x0007	1 byte	End offset
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Menu-abilities
 */
struct MenuAbilities_impl
{
protected:
  EncodedStringOffset          m_name_offset                       = {};
  EncodedStringOffset          m_description_offset                = {};
  std::uint8_t                 m_ability_points_required_to_unlock = {};
  std::uint8_t                 m_menu_file_index                   = {};
  std::uint8_t                 m_start_offset                      = {};
  std::uint8_t                 m_end_offset                        = {};
  static constexpr std::size_t EXPECTED_SIZE                       = 8U;
  constexpr MenuAbilities_impl()                                   = default;

public:
  constexpr auto
    operator<=>(const MenuAbilities_impl &right) const noexcept = default;
};
using MenuAbilities = CommonKernel<MenuAbilities_impl>;
static_assert(MenuAbilities::EXPECTED_SIZE == sizeof(MenuAbilities));
static_assert(has_name_offset<MenuAbilities>);
static_assert(has_description_offset<MenuAbilities>);
static_assert(has_ability_points_required_to_unlock<MenuAbilities>);
static_assert(has_menu_file_index<MenuAbilities>);
static_assert(has_start_offset<MenuAbilities>);
static_assert(has_end_offset<MenuAbilities>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_MENUABILITIES_HPP
