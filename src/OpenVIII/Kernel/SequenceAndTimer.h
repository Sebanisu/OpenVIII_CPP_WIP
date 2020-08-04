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

#ifndef VIIIARCHIVE_SEQUENCEANDTIMER_H
#define VIIIARCHIVE_SEQUENCEANDTIMER_H
#include <cstdint>
#include <iostream>
namespace OpenVIII::Kernel { struct SequenceAndTimer
{
private:
  std::uint8_t Sequence_{};
  std::uint8_t Timer_{};

public:
  [[nodiscard]] auto Sequence() const noexcept { return Sequence_; }
  [[nodiscard]] auto Timer() const noexcept { return Timer_; }
  friend std::ostream &operator<<(std::ostream &os, const SequenceAndTimer &input)
  {
    return os << '{' << static_cast<std::uint32_t>(input.Sequence()) << ", "
              << static_cast<std::uint32_t>(input.Timer()) << '}';
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_SEQUENCEANDTIMER_H
