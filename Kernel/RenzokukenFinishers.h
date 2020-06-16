//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_RENZOKUKENFINISHERS_H
#define VIIIARCHIVE_RENZOKUKENFINISHERS_H

#include "EncodedStringOffset.h"
struct RenzokukenFinishers
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
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
#endif// VIIIARCHIVE_RENZOKUKENFINISHERS_H
