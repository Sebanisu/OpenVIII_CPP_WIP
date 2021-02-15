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
#include <compare>
#include <cstdint>
#include <iostream>
namespace open_viii::kernel {
struct SequenceAndTimer
{
private:
  std::uint8_t m_sequence{};
  std::uint8_t m_timer{};

public:
  constexpr auto
    operator<=>(const SequenceAndTimer &right) const noexcept = default;
  [[nodiscard]] constexpr auto sequence() const noexcept
  {
    return m_sequence;
  }
  [[nodiscard]] constexpr auto timer() const noexcept
  {
    return m_timer;
  }
  friend std::ostream &operator<<(std::ostream &          os,
                                  const SequenceAndTimer &input)
  {
    return os << '{' << static_cast<std::uint32_t>(input.sequence()) << ", "
              << static_cast<std::uint32_t>(input.timer()) << '}';
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_SEQUENCEANDTIMER_HPP
