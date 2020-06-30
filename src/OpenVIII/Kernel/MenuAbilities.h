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

#ifndef VIIIARCHIVE_MENUABILITIES_H
#define VIIIARCHIVE_MENUABILITIES_H

#include "OpenVIII/Strings/EncodedStringOffset.h"
namespace OpenVIII::Kernel {
struct MenuAbilities
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Menu-abilities
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	AP Required to learn ability
   * 0x0005	1 byte	Index to m00X files in menu.fs
   * (first 3 sections are treated as special cases)
   * 0x0006	1 byte	Start offset
   * 0x0007	1 byte	End offset
   */
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};
  std::uint8_t APRequired_{};
  std::uint8_t MenuFileIndex_{};
  std::uint8_t StartOffset_{};
  std::uint8_t EndOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  [[nodiscard]] auto APRequired() const noexcept { return APRequired_; }
  [[nodiscard]] auto MenuFileIndex() const noexcept { return MenuFileIndex_; }
  [[nodiscard]] auto StartOffset() const noexcept { return StartOffset_; }
  [[nodiscard]] auto EndOffset() const noexcept { return EndOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os << ", " << static_cast<std::uint32_t>(APRequired()) << ", " << static_cast<std::uint32_t>(MenuFileIndex())
              << ", " << static_cast<std::uint32_t>(StartOffset()) << ", " << static_cast<std::uint32_t>(EndOffset());
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_MENUABILITIES_H
