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
#ifndef VIIIARCHIVE_MISCTEXT_HPP
#define VIIIARCHIVE_MISCTEXT_HPP
#include "open_viii/strings/EncodedStringOffset.hpp"
#include <compare>
#include <iostream>
namespace open_viii::kernel {
/**
 * Section Structure
 * Offset	Length	Description
 * 0x0000	2 bytes	Offset to text
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Misc-text-pointers
 */
struct MiscText
{
private:
  EncodedStringOffset m_name_offset{};

public:
  constexpr auto operator<=>(const MiscText &right) const noexcept = default;
  [[nodiscard]] constexpr auto
    name_offset() const noexcept
  {
    return m_name_offset;
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_MISCTEXT_HPP
