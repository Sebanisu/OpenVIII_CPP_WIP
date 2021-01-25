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
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <array>
#include <cstdint>
namespace open_viii::kernel {
struct Slot
{
private:
  std::uint8_t m_magic_id{};
  std::uint8_t m_count{};

public:
  [[nodiscard]] auto magic_id() const noexcept
  {
    return m_magic_id;
  }
  [[nodiscard]] auto count() const noexcept
  {
    return m_count;
  }
  friend std::ostream &operator<<(std::ostream &os, const Slot &slot)
  {
    return os << '{' << static_cast<uint16_t>(slot.magic_id()) << ", "
              << static_cast<uint16_t>(slot.count()) << '}';
  }
};
struct SlotsSets
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Selphie-limit-break-sets
   * array of 16 bytes 8 total 2 bytes per Magic Slot { Magic ID and Count }
   */
private:
  static constexpr auto TOTAL = 8U;
  std::array<Slot, TOTAL> m_slots{};

public:
  [[nodiscard]] const auto &slots() const noexcept
  {
    return m_slots;
  }

  friend std::ostream &operator<<(std::ostream &os, const SlotsSets &set)
  {
    return set.out(os, {});
  }
  std::ostream &out(std::ostream &os,
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
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_SLOTSSETS_HPP
