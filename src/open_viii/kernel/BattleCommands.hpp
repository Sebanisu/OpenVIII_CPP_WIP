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
#ifndef VIIIARCHIVE_BATTLECOMMANDS_HPP
#define VIIIARCHIVE_BATTLECOMMANDS_HPP
#include "CommonKernel.hpp"
#include "TargetT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <compare>
namespace open_viii::kernel {
struct BattleCommands_impl
{
  /*
   * https://github.com/alexfilth/doomtrain/wiki/Battle-commands
   * Offset	Length	Description
   * 	2 bytes	Offset to ability name
   * 	2 bytes	Offset to ability description
   * 	1 byte	Ability data ID
   * 	1 byte	Unknown Flags
   * 	1 byte	Target
   * 	1 byte	Unknown / Unused
   */
protected:
  EncodedStringOffset          m_name_offset        = {};
  EncodedStringOffset          m_description_offset = {};
  std::uint8_t                 m_ability_data_id    = {};
  std::uint8_t                 m_unknown_flags      = {};
  TargetT                      m_target             = {};
  std::uint8_t                 m_unknown            = {};
  static constexpr std::size_t EXPECTED_SIZE        = 8U;

public:
  constexpr auto
    operator<=>(const BattleCommands_impl &right) const noexcept = default;
};
using BattleCommands = CommonKernel<BattleCommands_impl>;
static_assert(sizeof(BattleCommands) == BattleCommands::EXPECTED_SIZE);
static_assert(has_name_offset<BattleCommands>);
static_assert(has_description_offset<BattleCommands>);
static_assert(has_ability_data_id<BattleCommands>);
static_assert(has_unknown_flags<BattleCommands>);
static_assert(has_target<BattleCommands>);
static_assert(has_unknown<BattleCommands>);
static_assert(BattleCommands().target() == TargetT::none);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_BATTLECOMMANDS_HPP
