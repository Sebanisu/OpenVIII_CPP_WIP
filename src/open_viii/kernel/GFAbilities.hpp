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
#include "CommonKernel.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include "StatT.hpp"
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
struct GFAbilities_impl
{
protected:
  EncodedStringOffset m_name_offset                       = {};
  EncodedStringOffset m_description_offset                = {};
  uint8_t             m_ability_points_required_to_unlock = {};
  uint8_t             m_enable_boost                      = {};
  StatT               m_stat_to_increase                  = {};
  uint8_t             m_increase_value                    = {};
  GFAbilities_impl()                                      = default;
  static constexpr std::size_t EXPECTED_SIZE              = 8U;

public:
  constexpr auto
    operator<=>(const GFAbilities_impl &right) const noexcept = default;
};
using GFAbilities = CommonKernel<GFAbilities_impl>;
static_assert(GFAbilities::EXPECTED_SIZE == sizeof(GFAbilities));
static_assert(has_name_offset<GFAbilities>);
static_assert(has_description_offset<GFAbilities>);
static_assert(has_ability_points_required_to_unlock<GFAbilities>);
static_assert(has_enable_boost<GFAbilities>);
static_assert(has_stat_to_increase<GFAbilities>);
static_assert(has_increase_value<GFAbilities>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_GFABILITIES_HPP
