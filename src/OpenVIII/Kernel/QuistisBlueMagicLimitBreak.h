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

#ifndef VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAK_H
#define VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAK_H
#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "ElementT.h"
#include "AttackTypeT.h"
#include "AttackFlagsT.h"
namespace OpenVIII::Kernel {
template<LangT langVal> struct QuistisBlueMagicLimitBreak
{
  /* https://github.com/DarkShinryu/doomtrain/wiki/Blue-magic-(Quistis-limit-break)
   * 0x0000	2 bytes	Offset to limit name
   * 0x0002	2 bytes	Offset to limit description
   * 0x0004	2 bytes	Magic ID
   * 0x0006	1 byte	Unknown
   * 0x0007	1 byte	Attack Type
   * 0x0008	2 bytes	Unknown
   * 0x000A	1 byte	Attack Flags
   * 0x000B	1 byte	Unknown
   * 0x000C	1 byte	Element
   * 0x000D	1 byte	Status Attack
   * 0x000E	1 byte	Critical Bonus
   * 0x000F	1 byte	Unknown
   */
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

  std::uint16_t magicID_{};
  std::uint8_t unknown0_{};
  AttackTypeT attackType_{};
  std::uint8_t unknown1_{};
  std::uint8_t unknown2_{};
  AttackFlagsT attackFlags_{};
  std::uint8_t unknown3_{};
  ElementT element_{};
  std::uint8_t statusAttack_{};
  std::uint8_t criticalBonus_{};
  std::uint8_t unknown4_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  [[nodiscard]] auto MagicID() const noexcept { return magicID_; }
  [[nodiscard]] auto unknown0() const noexcept { return unknown0_; }
  [[nodiscard]] auto AttackType() const noexcept { return attackType_; }
  [[nodiscard]] auto unknown1() const noexcept { return unknown1_; }
  [[nodiscard]] auto unknown2() const noexcept { return unknown2_; }
  [[nodiscard]] auto AttackFlags() const noexcept { return attackFlags_; }
  [[nodiscard]] auto unknown3() const noexcept { return unknown3_; }
  [[nodiscard]] auto Element() const noexcept { return element_; }
  [[nodiscard]] auto StatusAttack() const noexcept { return statusAttack_; }
  [[nodiscard]] auto CriticalBonus() const noexcept { return criticalBonus_; }
  [[nodiscard]] auto unknown4() const noexcept { return unknown4_; }

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
    return os << ", " << static_cast<std::uint32_t>(MagicID()) << ", " << static_cast<std::uint32_t>(unknown0()) << ", "
              << static_cast<std::uint32_t>(AttackType()) << ", " << static_cast<std::uint32_t>(unknown1()) << ", "
              << static_cast<std::uint32_t>(unknown2()) << ", " << static_cast<std::uint32_t>(AttackFlags()) << ", "
              << static_cast<std::uint32_t>(unknown3()) << ", " << static_cast<std::uint32_t>(Element()) << ", "
              << static_cast<std::uint32_t>(StatusAttack()) << ", " << static_cast<std::uint32_t>(CriticalBonus())
              << ", " << static_cast<std::uint32_t>(unknown4());
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAK_H
