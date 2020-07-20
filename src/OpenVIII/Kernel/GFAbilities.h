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

#ifndef VIIIARCHIVE_GFABILITIES_H
#define VIIIARCHIVE_GFABILITIES_H

#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "StatT.h"
namespace OpenVIII::Kernel {
struct GFAbilities
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/GF-abilities
   * Offset	Length	Description
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	AP Required to learn ability
   * 0x0005	1 byte	Enable Boost
   * 0x0006	1 byte	Stat to increase
   * 0x0007	1 byte	Increase value
   */
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};
  uint8_t apRequired_{};
  uint8_t enableBoost_{};
  StatT statToIncrease_{};
  uint8_t increaseValue_{};

public:
  [[nodiscard]] const auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] const auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  [[nodiscard]] auto APRequired() const noexcept { return apRequired_; }
  [[nodiscard]] auto EnableBoost() const noexcept { return enableBoost_ != 0; }
  [[nodiscard]] auto StatToIncrease() const noexcept { return statToIncrease_; }
  [[nodiscard]] auto IncreaseValue() const noexcept { return increaseValue_; }
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
    os << ", " << static_cast<int>(APRequired());
    os << ", " << static_cast<int>(EnableBoost());
    os << ", " << static_cast<int>(StatToIncrease());
    os << ", " << static_cast<int>(IncreaseValue());
    return os;
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_GFABILITIES_H