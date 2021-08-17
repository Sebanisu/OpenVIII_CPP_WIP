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
#ifndef VIIIARCHIVE_STATPERCENTINCREASEABILITIES_HPP
#define VIIIARCHIVE_STATPERCENTINCREASEABILITIES_HPP
#include "CommonKernel.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
namespace open_viii::kernel {
/**
 * 0x0000	2 bytes	Offset to ability name
 * 0x0002	2 bytes	Offset to ability description
 * 0x0004	1 byte	AP needed to learn the ability
 * 0x0005	1 byte	Stat to increase
 * 0x0006	1 byte	Increase value
 * 0x0007	1 byte	Unknown/Unused
 * @see
 * https://github.com/DarkShinryu/doomtrain/wiki/Stat-percentage-increasing-abilities
 */
struct StatPercentIncreaseAbilities_impl
{
protected:
  EncodedStringOffset m_name_offset                       = {};
  EncodedStringOffset m_description_offset                = {};
  std::uint8_t        m_ability_points_required_to_unlock = {};
  std::uint8_t        m_stat_to_increase                  = {};
  std::uint8_t        m_increased_value                   = {};
  std::uint8_t        m_unknown                           = {};
  constexpr StatPercentIncreaseAbilities_impl()           = default;
  static constexpr std::size_t EXPECTED_SIZE              = 8U;

public:
  constexpr auto
    operator<=>(
      const StatPercentIncreaseAbilities_impl &right) const noexcept = default;
};
using StatPercentIncreaseAbilities
  = CommonKernel<StatPercentIncreaseAbilities_impl>;
static_assert(StatPercentIncreaseAbilities::EXPECTED_SIZE
              == sizeof(StatPercentIncreaseAbilities));
static_assert(has_name_offset<StatPercentIncreaseAbilities>);
static_assert(has_description_offset<StatPercentIncreaseAbilities>);
static_assert(
  has_ability_points_required_to_unlock<StatPercentIncreaseAbilities>);
static_assert(has_stat_to_increase<StatPercentIncreaseAbilities>);
static_assert(has_increased_value<StatPercentIncreaseAbilities>);
static_assert(has_unknown<StatPercentIncreaseAbilities>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_STATPERCENTINCREASEABILITIES_HPP
