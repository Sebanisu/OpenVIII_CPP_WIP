//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_SP1ENTRY_H
#define VIIIARCHIVE_SP1ENTRY_H

#include <cstdint>
namespace OpenVIII::Graphics {
struct sp1Entry
{
public:
  uint8_t x_{};
  uint8_t y_{};
  uint8_t unknown1_{};
  uint8_t unknown2_{};
  uint8_t width_{};
  uint8_t offset_x_{};
  uint8_t height_{};
  uint8_t offset_y_{};
  friend std::ostream &operator<<(std::ostream &os, const sp1Entry &e)
  {
    return os << "{ (X, Y) = (" << static_cast<std::size_t>(e.x_) << ", " << static_cast<std::size_t>(e.y_)
              << "), (W, H) = (" << static_cast<std::size_t>(e.width_) << ", " << static_cast<std::size_t>(e.height_)
              << "), (Offset X, Offset Y) = (" << static_cast<std::size_t>(e.offset_x_) << ", "
              << static_cast<std::size_t>(e.offset_y_) << ") Unk(" << static_cast<std::size_t>(e.unknown1_) << ", "
              << static_cast<std::size_t>(e.unknown2_) << ") }";
  }
};

}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_SP1ENTRY_H
