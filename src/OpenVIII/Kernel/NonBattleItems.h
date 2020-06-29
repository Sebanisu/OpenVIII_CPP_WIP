//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_NONBATTLEITEMS_H
#define VIIIARCHIVE_NONBATTLEITEMS_H
#include "OpenVIII/Strings/EncodedStringOffset.h"

namespace OpenVIII::Kernel {
struct NonBattleItems
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Non-battle-item-name-and-description-offsets
   * 0x0000	2 bytes	Offset to item name
   * 0x0002	2 bytes	Offset to item description
   */
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
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
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_NONBATTLEITEMS_H
