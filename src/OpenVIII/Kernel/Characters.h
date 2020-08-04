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

#ifndef VIIIARCHIVE_CHARACTERS_H
#define VIIIARCHIVE_CHARACTERS_H

#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "GenderT.h"
#include "StatGroupNoEVANoHIT.h"
namespace OpenVIII::Kernel { template <LangT langVal> struct Characters
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Characters
   * Offset	Length	Description
   * 0x0000	2 bytes	Offset to character name
   * Squall and Rinoa have name offsets of 0xFFFF because their name is in the save game data rather than kernel.bin.
   * 0x0002	1 byte	Crisis level hp multiplier
   * 0x0003	1 byte	Gender
   *  0x00 - Male
   *  0x01 - Female
   * 0x0004	1 byte	Limit Break ID
   * 0x0005	1 byte	Limit Break Param
   * used for the power of each renzokuken hit before finisher
   * 0x0006	2 bytes	EXP modifier
   * 0x0008	4 bytes	HP
   * 0x000C	4 bytes	STR
   * 0x0010	4 bytes	VIT
   * 0x0014	4 bytes	MAG
   * 0x0018	4 bytes	SPR
   * 0x001C	4 bytes	SPD
   * 0x0020	4 bytes	LUCK
   * */
private:
  EncodedStringOffset nameOffset_{};
  std::uint8_t crisisLevelHPMultiplier_{};
  GenderT gender_{};
  std::uint8_t limitBreakID_{};
  std::uint8_t limitBreakParam_{};
  //  std::uint8_t EXP0;
  //  std::uint8_t EXP1;
  //  std::uint8_t EXP[2];
  std::array<std::uint8_t, 2> exp_{};
  //  std::uint8_t HP0;
  //  std::uint8_t HP1;
  //  std::uint8_t HP2;
  //  std::uint8_t HP3;
  //  std::uint8_t STR0;
  //  std::uint8_t STR1;
  //  std::uint8_t STR2;
  //  std::uint8_t STR3;
  //  std::uint8_t VIT0;
  //  std::uint8_t VIT1;
  //  std::uint8_t VIT2;
  //  std::uint8_t VIT3;
  //  std::uint8_t MAG0;
  //  std::uint8_t MAG1;
  //  std::uint8_t MAG2;
  //  std::uint8_t MAG3;
  //  std::uint8_t SPR0;
  //  std::uint8_t SPR1;
  //  std::uint8_t SPR2;
  //  std::uint8_t SPR3;
  //  std::uint8_t SPD0;
  //  std::uint8_t SPD1;
  //  std::uint8_t SPD2;
  //  std::uint8_t SPD3;
  //  std::uint8_t LUCK0;
  //  std::uint8_t LUCK1;
  //  std::uint8_t LUCK2;
  //  std::uint8_t LUCK3;
  StatGroupNoEVANoHIT<std::array<std::uint8_t, 4>> stats_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto CrisisLevelHPMultiplier() const noexcept { return crisisLevelHPMultiplier_; }
  [[nodiscard]] auto Gender() const noexcept { return gender_; }
  [[nodiscard]] auto LimitBreakID() const noexcept { return limitBreakID_; }
  [[nodiscard]] auto LimitBreakParam() const noexcept { return limitBreakParam_; }
  [[nodiscard]] auto EXP() const noexcept { return exp_; }
  [[nodiscard]] auto Stats() const noexcept { return stats_; }
  [[nodiscard]] auto operator->() const noexcept { return &stats_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = nameOffset_.DecodedString<langVal>(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    return os << ", " << static_cast<std::uint32_t>(CrisisLevelHPMultiplier()) << ", "
              << static_cast<std::uint32_t>(Gender()) << ", " << static_cast<std::uint32_t>(LimitBreakID()) << ", "
              << static_cast<std::uint32_t>(LimitBreakParam()) << ", {" << static_cast<std::uint32_t>(EXP()[0]) << ", "
              << static_cast<std::uint32_t>(EXP()[1]) << "}, " << Stats();
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_CHARACTERS_H
