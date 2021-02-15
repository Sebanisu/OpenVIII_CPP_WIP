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
#ifndef VIIIARCHIVE_SLOTSSETS_HPP
#define VIIIARCHIVE_SLOTSSETS_HPP
#include "Slot.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <array>
#include <cstdint>
namespace open_viii::kernel {
/**
 * array of 16 bytes 8 total 2 bytes per Magic Slot { Magic ID and Count }
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Selphie-limit-break-sets
 */
struct SlotsSets
{
public:
  static constexpr auto TOTAL                                       = 8U;
  constexpr auto operator<=>(const SlotsSets &right) const noexcept = default;
  [[nodiscard]] const auto &slots() const noexcept
  {
    return m_slots;
  }
  friend std::ostream &operator<<(std::ostream &os, const SlotsSets &set)
  {
    return set.out(os, {});
  }
  std::ostream &out(std::ostream &                                os,
                    [[maybe_unused]] const std::span<const char> &buffer) const
  {
    os << '{';
    bool first = true;
    for (const auto &item : slots()) {
      if (!first) {
        os << ", ";
      } else {
        first = false;
      }
      os << item;
    }
    return os << '}';
  }

private:
  std::array<Slot, TOTAL> m_slots{};
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_SLOTSSETS_HPP
