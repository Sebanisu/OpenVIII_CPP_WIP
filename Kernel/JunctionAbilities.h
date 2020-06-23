//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_JUNCTIONABILITIES_H
#define VIIIARCHIVE_JUNCTIONABILITIES_H

#include "EncodedStringOffset.h"
#include <cstring>
#include "JunctionFlagsT.h"
namespace OpenVIII::Kernel {
struct JunctionAbilities
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Junction-abilities
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	AP Required to learn ability
   * 0x0005	3 byte	J-Flag
   */
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};
  std::uint8_t apRequired_{};
  std::array<std::uint8_t, 3> junctionFlags_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  [[nodiscard]] auto APRequired() const noexcept { return apRequired_; }
  [[nodiscard]] auto JunctionFlags() const
  {
    JunctionFlagsT out{};
    std::memcpy(&out, junctionFlags_.data(), junctionFlags_.size());
    return out;
  }
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
    return os << ", " << static_cast<uint32_t>(APRequired()) << ", " << static_cast<uint32_t>(JunctionFlags());
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_JUNCTIONABILITIES_H
