//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_SP2ENTRY_H
#define VIIIARCHIVE_SP2ENTRY_H
#include <cstdint>
namespace open_viii::graphics {
struct sp2Entry
{
private:
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

public:
  [[nodiscard]] auto Count() const noexcept { return count_; }
  [[nodiscard]] auto X() const noexcept { return x_; }
  [[nodiscard]] auto Y() const noexcept { return y_; }
  [[nodiscard]] auto unknown1() const noexcept { return unknown1_; }
  [[nodiscard]] auto unknown2() const noexcept { return unknown2_; }
  [[nodiscard]] auto Width() const noexcept { return width_; }
  [[nodiscard]] auto Height() const noexcept { return height_; }
  [[nodiscard]] auto unknown3() const noexcept { return unknown3_; }
  [[nodiscard]] auto unknown4() const noexcept { return unknown4_; }
  [[nodiscard]] auto unknown5() const noexcept { return unknown5_; }
  [[nodiscard]] auto unknown6() const noexcept { return unknown6_; }
  sp2Entry() = default;
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
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_SP2ENTRY_H
