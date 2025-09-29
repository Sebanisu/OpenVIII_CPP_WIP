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
#include "CommonKernel.hpp"
#include "open_viii/Concepts.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
namespace open_viii::kernel {
/**
 * 0x0000	2 bytes	Offset to ability name
 * 0x0002	2 bytes	Offset to ability description
 * 0x0004	1 byte	AP Required to learn ability
 * 0x0005	1 byte	Index to Battle commands
 * 0x0006	2 bytes	Unknown/Unused
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Command-abilities
 */
struct CommandAbilities_impl
{
protected:
  EncodedStringOffset m_name_offset                       = {};
  EncodedStringOffset m_description_offset                = {};
  std::uint8_t        m_ability_points_required_to_unlock = {};
  std::uint8_t        m_index_to_battle_command           = {};
  std::uint8_t        m_unknown0                          = {};
  std::uint8_t        m_unknown1                          = {};
  constexpr CommandAbilities_impl()                       = default;
  static constexpr std::size_t EXPECTED_SIZE              = 8U;

public:
  constexpr auto
    operator<=>(const CommandAbilities_impl &right) const noexcept
    = default;
};
using CommandAbilities = CommonKernel<CommandAbilities_impl>;
static_assert(sizeof(CommandAbilities) == CommandAbilities::EXPECTED_SIZE);
static_assert(has_name_offset<CommandAbilities>);
static_assert(has_description_offset<CommandAbilities>);
static_assert(has_ability_points_required_to_unlock<CommandAbilities>);
static_assert(has_index_to_battle_command<CommandAbilities>);
static_assert(has_unknown0<CommandAbilities>);
static_assert(has_unknown1<CommandAbilities>);
static_assert(is_kernel_object(CommandAbilities()));
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_COMMANDABILITIES_HPP
