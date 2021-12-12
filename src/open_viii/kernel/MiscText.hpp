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
#include "CommonKernel.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
namespace open_viii::kernel {
/**
 * Section Structure
 * Offset	Length	Description
 * 0x0000	2 bytes	Offset to text
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Misc-text-pointers
 */
struct MiscText_impl
{
protected:
  EncodedStringOffset          m_name_offset = {};
  static constexpr std::size_t EXPECTED_SIZE = sizeof(EncodedStringOffset);
  constexpr MiscText_impl()                  = default;

public:
  constexpr auto
    operator<=>(const MiscText_impl &right) const noexcept = default;
  constexpr explicit operator EncodedStringOffset() const noexcept
  {// todo keep?
    return m_name_offset;
  }
};
using MiscText = CommonKernel<MiscText_impl>;

static_assert(MiscText::EXPECTED_SIZE == sizeof(MiscText));
static_assert(has_name_offset<MiscText>);
static_assert(EncodedStringOffset(MiscText{}).offset() == 0U);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_MISCTEXT_HPP
