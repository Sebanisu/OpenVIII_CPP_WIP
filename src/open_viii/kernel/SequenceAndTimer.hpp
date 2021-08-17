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
#ifndef VIIIARCHIVE_SEQUENCEANDTIMER_HPP
#define VIIIARCHIVE_SEQUENCEANDTIMER_HPP
#include "CommonKernel.hpp"
#include <compare>
#include <cstdint>
#include <iostream>
namespace open_viii::kernel {
/**
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Misc-section
 */
struct SequenceAndTimer_impl
{
protected:
  std::uint8_t m_sequence                    = {};
  std::uint8_t m_timer                       = {};
  constexpr SequenceAndTimer_impl()          = default;
  static constexpr std::size_t EXPECTED_SIZE = 2U;

public:
  constexpr auto
    operator<=>(const SequenceAndTimer_impl &right) const noexcept = default;
};
using SequenceAndTimer = CommonKernel<SequenceAndTimer_impl>;
static_assert(SequenceAndTimer::EXPECTED_SIZE == sizeof(SequenceAndTimer));
static_assert(has_sequence<SequenceAndTimer>);
static_assert(has_timer<SequenceAndTimer>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_SEQUENCEANDTIMER_HPP
