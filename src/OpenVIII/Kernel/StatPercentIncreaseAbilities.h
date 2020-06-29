//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_STATPERCENTINCREASEABILITIES_H
#define VIIIARCHIVE_STATPERCENTINCREASEABILITIES_H
#include "OpenVIII/Strings/EncodedStringOffset.h"
namespace OpenVIII::Kernel {
struct StatPercentIncreaseAbilities
{
private:
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Stat-percentage-increasing-abilities
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	AP needed to learn the ability
   * 0x0005	1 byte	Stat to increase
   * 0x0006	1 byte	Increase value
   * 0x0007	1 byte	Unknown/Unused
   */
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};
  std::uint8_t apRequired_{};
  std::uint8_t statToIncrease_{};
  std::uint8_t increasedValue_{};
  std::uint8_t unknown0_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }

  [[nodiscard]] auto APRequired() const noexcept { return apRequired_; }
  [[nodiscard]] auto StatToIncrease() const noexcept { return statToIncrease_; }
  [[nodiscard]] auto IncreasedValue() const noexcept { return increasedValue_; }
  [[nodiscard]] auto unknown0() const noexcept { return unknown0_; }
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
    return os << ", " << static_cast<std::uint32_t>(APRequired()) << ", "
              << static_cast<std::uint32_t>(StatToIncrease()) << ", " << static_cast<std::uint32_t>(IncreasedValue())
              << ", " << static_cast<std::uint32_t>(unknown0());
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_STATPERCENTINCREASEABILITIES_H
