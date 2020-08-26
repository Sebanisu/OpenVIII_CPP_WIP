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

#ifndef VIIIARCHIVE_MISCTEXT_H
#define VIIIARCHIVE_MISCTEXT_H
#include <iostream>
#include "OpenVIII/Strings/EncodedStringOffset.h"

namespace OpenVIII::Kernel {
template<LangT langVal> struct MiscText
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Misc-text-pointers
   * Section Structure
   * Offset	Length	Description
   * 0x0000	2 bytes	Offset to text
   */
private:
  EncodedStringOffset nameOffset_{};
  // EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  //[[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = nameOffset_.DecodedString<langVal>(buffer);
    // auto description = descriptionOffset_.DecodedString<langVal>(buffer);
    if (!std::empty(name)) {
      os << Tools::u8tosv(name);
    }
    //    if (!std::empty(description)) {
    //      os << ", " << description;
    //    }
    return os;
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_MISCTEXT_H
