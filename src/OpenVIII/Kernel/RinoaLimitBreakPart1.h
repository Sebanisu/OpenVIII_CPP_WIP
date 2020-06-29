//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_RINOALIMITBREAKPART1_H
#define VIIIARCHIVE_RINOALIMITBREAKPART1_H

#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "TargetT.h"
namespace OpenVIII::Kernel {
struct RinoaLimitBreakPart1
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Rinoa-limit-breaks-(part-1)
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	Unknown Flags
   * 0x0005	1 byte	Target
   * 0x0006	1 byte	Ability data ID
   * 0x0007	1 byte	Unknown / Unused
   */
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};
  std::uint8_t unknownFlags0_{};
  TargetT target_{};
  std::uint8_t abilityDataID_{};
  std::uint8_t unknown0_{};


public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  [[nodiscard]] auto UnknownFlags0() const noexcept { return unknownFlags0_; }
  [[nodiscard]] auto Target() const noexcept { return target_; }
  [[nodiscard]] auto AbilityDataID() const noexcept { return abilityDataID_; }
  [[nodiscard]] auto Unknown0() const noexcept { return unknown0_; }
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
    return os << ", " << static_cast<std::uint32_t>(UnknownFlags0()) << ", " << static_cast<std::uint32_t>(Target())
              << ", " << static_cast<std::uint32_t>(AbilityDataID()) << ", " << static_cast<std::uint32_t>(Unknown0());
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_RINOALIMITBREAKPART1_H
