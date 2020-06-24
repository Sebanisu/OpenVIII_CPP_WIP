//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_GFABILITIES_H
#define VIIIARCHIVE_GFABILITIES_H

#include "EncodedStringOffset.h"
namespace OpenVIII::Kernel {
enum class StatT : std::uint8_t {
  HP,
  STR,
  VIT,
  MAG,
  SPR,
  SPD,
  EVA,
  HIT,
  Luck,
  ElAtk,
  StAtk,
  ElDef1,
  ElDef2,
  ElDef3,
  ElDef4,
  StDef1,
  StDef2,
  StDef3,
  StDef4,
  None = 0xFF,
};
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
