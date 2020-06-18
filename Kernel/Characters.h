//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_CHARACTERS_H
#define VIIIARCHIVE_CHARACTERS_H

#include "EncodedStringOffset.h"
#include "GenderT.h"
#include "StatGroupNoEVANoHIT.h"
namespace OpenVIII::Kernel {
struct Characters
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
  std::uint8_t crisisLevelhpMultiplier;
  GenderT Gender;
  std::uint8_t LimitBreakID;
  std::uint8_t LimitBreakParam;
//  std::uint8_t EXP0;
//  std::uint8_t EXP1;
//  std::uint8_t  EXP[2];
  std::array<std::uint8_t,2> EXP;
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
StatGroupNoEVANoHIT<std::array<std::uint8_t,4>> stats_;

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    return os;
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_CHARACTERS_H
