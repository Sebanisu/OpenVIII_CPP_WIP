//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_SLOTSARRAY_H
#define VIIIARCHIVE_SLOTSARRAY_H
#include "OpenVIII/Strings/EncodedStringOffset.h"
#include <cstdint>
namespace OpenVIII::Kernel {
struct SlotsArray
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Slot-array
   * Offset	Length	Description
   * 0x0000	1 byte	Slot Set ID
   */
private:
  std::uint8_t slotSetID_{};

public:
  [[nodiscard]] auto SlotSetID() const noexcept { return slotSetID_; }
  std::ostream &Out(std::ostream &os, [[maybe_unused]] const std::string_view &buffer) const
  {
    return os << static_cast<uint16_t>(SlotSetID());
  }
  [[nodiscard]] explicit operator std::uint8_t() const { return slotSetID_; }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_SLOTSARRAY_H
