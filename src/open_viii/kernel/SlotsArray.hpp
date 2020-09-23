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

#ifndef VIIIARCHIVE_SLOTSARRAY_HPP
#define VIIIARCHIVE_SLOTSARRAY_HPP
#include "open_viii/Strings/EncodedStringOffset.hpp"
#include <cstdint>
namespace open_viii::kernel {
struct SlotsArray
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Slot-array
   * Offset	Length	Description
   * 0x0000	1 byte	Slot Set ID
   */
private:
  std::uint8_t m_slot_set_id{};

public:
  [[nodiscard]] auto slot_set_id() const noexcept
  {
    return m_slot_set_id;
  }
  std::ostream &out(std::ostream &os, [[maybe_unused]] const std::span<const char> &buffer) const
  {
    return os << static_cast<uint16_t>(slot_set_id());
  }
  [[nodiscard]] explicit operator std::uint8_t() const
  {
    return m_slot_set_id;
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_SLOTSARRAY_HPP
