//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_SP2ENTRY_H
#define VIIIARCHIVE_SP2ENTRY_H
#include <cstdint>
namespace OpenVIII::Graphics {
struct sp2Entry
{
public:
  std::uint32_t count_{};
  std::uint8_t x_{};
  std::uint8_t y_{};
  std::uint8_t unknown1_{};
  std::uint8_t unknown2_{};
  std::uint16_t width_{};
  std::uint16_t height_{};
  std::uint8_t unknown3_{};
  std::uint8_t unknown4_{};
  std::uint8_t unknown5_{};
  std::uint8_t unknown6_{};
  friend std::ostream &operator<<(std::ostream &os, const sp2Entry &e)
  {
    return os << "{ " << static_cast<std::size_t>(e.count_) << " (X, Y) = (" << static_cast<std::size_t>(e.x_) << ", "
              << static_cast<std::size_t>(e.y_) << "), (W, H) = (" << static_cast<std::size_t>(e.width_) << ", "
              << static_cast<std::size_t>(e.height_) << "), Unk(" << static_cast<std::size_t>(e.unknown1_) << ", "
              << static_cast<std::size_t>(e.unknown2_) << ", " << static_cast<std::size_t>(e.unknown3_) << ", "
              << static_cast<std::size_t>(e.unknown4_) << ", " << static_cast<std::size_t>(e.unknown5_) << ", "
              << static_cast<std::size_t>(e.unknown6_) << ") }";
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_SP2ENTRY_H
