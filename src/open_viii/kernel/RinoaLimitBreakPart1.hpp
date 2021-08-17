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
#ifndef VIIIARCHIVE_RINOALIMITBREAKPART1_HPP
#define VIIIARCHIVE_RINOALIMITBREAKPART1_HPP
#include "CommonKernel.hpp"
#include "TargetT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
namespace open_viii::kernel {
struct RinoaLimitBreakPart1_impl
{
  /**
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	Unknown Flags
   * 0x0005	1 byte	Target
   * 0x0006	1 byte	Ability data ID
   * 0x0007	1 byte	Unknown / Unused
   * @see
   * https://github.com/DarkShinryu/doomtrain/wiki/Rinoa-limit-breaks-(part-1)
   */
protected:
  EncodedStringOffset m_name_offset          = {};
  EncodedStringOffset m_description_offset   = {};
  std::uint8_t        m_unknown_flags        = {};
  TargetT             m_target               = {};
  std::uint8_t        m_ability_data_id      = {};
  std::uint8_t        m_unknown              = {};
  constexpr RinoaLimitBreakPart1_impl()      = default;
  static constexpr std::size_t EXPECTED_SIZE = 8U;

public:
  constexpr auto
    operator<=>(
      const RinoaLimitBreakPart1_impl &right) const noexcept = default;
};
using RinoaLimitBreakPart1 = CommonKernel<RinoaLimitBreakPart1_impl>;
static_assert(RinoaLimitBreakPart1::EXPECTED_SIZE
              == sizeof(RinoaLimitBreakPart1));
static_assert(has_name_offset<RinoaLimitBreakPart1>);
static_assert(has_unknown_flags<RinoaLimitBreakPart1>);
static_assert(has_target<RinoaLimitBreakPart1>);
static_assert(has_ability_data_id<RinoaLimitBreakPart1>);
static_assert(has_unknown<RinoaLimitBreakPart1>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_RINOALIMITBREAKPART1_HPP
