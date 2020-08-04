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

#ifndef VIIIARCHIVE_BATTLECOMMANDS_H
#define VIIIARCHIVE_BATTLECOMMANDS_H
#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "TargetT.h"

namespace OpenVIII::Kernel {
template <LangT langVal>
struct BattleCommands
{
  /*
   * https://github.com/alexfilth/doomtrain/wiki/Battle-commands
   * Offset	Length	Description
   * 	2 bytes	Offset to ability name
   * 	2 bytes	Offset to ability description
   * 	1 byte	Ability data ID
   * 	1 byte	Unknown Flags
   * 	1 byte	Target
   * 	1 byte	Unknown / Unused
   */
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};
  std::uint8_t abilityDataID_{};
  std::uint8_t unknownFlags_{};
  TargetT target_{};
  std::uint8_t unknown_{};


public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  [[nodiscard]] auto AbilityDataID() const noexcept { return abilityDataID_; }
  [[nodiscard]] auto UnknownFlags() const noexcept { return unknownFlags_; }
  [[nodiscard]] auto GetTarget() const noexcept { return target_; }
  [[nodiscard]] auto Unknown() const noexcept { return unknown_; }

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
    return os

           << ", " << static_cast<std::uint32_t>(AbilityDataID()) << ", " << static_cast<std::uint32_t>(UnknownFlags())
           << ", " << static_cast<std::uint32_t>(GetTarget()) << ", " << static_cast<std::uint32_t>(Unknown())

      ;
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_BATTLECOMMANDS_H
