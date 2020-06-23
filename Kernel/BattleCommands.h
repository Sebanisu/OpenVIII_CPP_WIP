//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_BATTLECOMMANDS_H
#define VIIIARCHIVE_BATTLECOMMANDS_H
#include "EncodedStringOffset.h"
#include "TargetT.h"

namespace OpenVIII::Kernel {
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
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
}
#endif// VIIIARCHIVE_BATTLECOMMANDS_H
