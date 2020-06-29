//
// Created by pcvii on 6/22/2020.
//

#ifndef VIIIARCHIVE_MISCTEXT_H
#define VIIIARCHIVE_MISCTEXT_H
#include <iostream>
#include "OpenVIII/Strings/EncodedStringOffset.h"

namespace OpenVIII::Kernel {
struct MiscText
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Misc-text-pointers
   * Section Structure
   * Offset	Length	Description
   * 0x0000	2 bytes	Offset to text
   */
private:
  EncodedStringOffset nameOffset_{};
  //EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  //[[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = nameOffset_.DecodedString(buffer);
    //auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
//    if (!std::empty(description)) {
//      os << ", " << description;
//    }
    return os;
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_MISCTEXT_H
