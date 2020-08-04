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

#ifndef VIIIARCHIVE_PARTYABILITIES_H
#define VIIIARCHIVE_PARTYABILITIES_H

#include "OpenVIII/Strings/EncodedStringOffset.h"

namespace OpenVIII::Kernel { template <LangT langVal> struct PartyAbilities
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Party-abilities
   * Offset	Length	Description
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	AP Required to learn ability
   * 0x0005	1 byte	Flag
   * 0x0006	2 byte	Unknown/Unused
   */
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};
  std::uint8_t apRequired_{};
  std::uint8_t flag_{};
  std::uint8_t unknown0_{};
  std::uint8_t unknown1_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }

  [[nodiscard]] auto APRequired() const noexcept { return apRequired_; }
  [[nodiscard]] auto Flag() const noexcept { return flag_; }
  [[nodiscard]] auto unknown0() const noexcept { return unknown0_; }
  [[nodiscard]] auto unknown1() const noexcept { return unknown1_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = nameOffset_.DecodedString<langVal>(buffer);
    auto description = descriptionOffset_.DecodedString<langVal>(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os << ", " << static_cast<std::uint32_t>(APRequired()) << ", " << static_cast<std::uint32_t>(Flag()) << ", "
              << static_cast<std::uint32_t>(unknown0()) << ", " << static_cast<std::uint32_t>(unknown1());
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_PARTYABILITIES_H
