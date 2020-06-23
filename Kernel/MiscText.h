//
// Created by pcvii on 6/22/2020.
//

#ifndef VIIIARCHIVE_MISCTEXT_H
#define VIIIARCHIVE_MISCTEXT_H

#include "EncodedStringOffset.h"

namespace OpenVIII::Kernel {
struct MiscText
{
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
}
#endif// VIIIARCHIVE_MISCTEXT_H
