//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_SP1ENTRY_H
#define VIIIARCHIVE_SP1ENTRY_H

#include <cstdint>
namespace open_viii::graphics {
struct Sp1Entry
{
private:
  uint8_t m_x{};
  uint8_t m_y{};
  uint8_t m_unknown1{};
  uint8_t m_unknown2{};
  uint8_t m_width{};
  int8_t m_offset_x{};
  uint8_t m_height{};
  int8_t m_offset_y{};

public:
  [[nodiscard]] const auto &x() const noexcept { return m_x; }
  [[nodiscard]] const auto &y() const noexcept { return m_y; }
  [[nodiscard]] const auto &unknown1() const noexcept { return m_unknown1; }
  [[nodiscard]] const auto &unknown2() const noexcept { return m_unknown2; }
  [[nodiscard]] const auto &width() const noexcept { return m_width; }
  [[nodiscard]] const auto &offset_x() const noexcept { return m_offset_x; }
  [[nodiscard]] const auto &height() const noexcept { return m_height; }
  [[nodiscard]] const auto &offset_y() const noexcept { return m_offset_y; }
  Sp1Entry() = default;
  friend std::ostream &operator<<(std::ostream &os, const Sp1Entry &e)
  {
    return os << "{ (X, Y) = (" << static_cast<std::size_t>(e.m_x) << ", " << static_cast<std::size_t>(e.m_y)
              << "), (W, H) = (" << static_cast<std::size_t>(e.m_width) << ", " << static_cast<std::size_t>(e.m_height)
              << "), (Offset X, Offset Y) = (" << static_cast<std::size_t>(e.m_offset_x) << ", "
              << static_cast<std::size_t>(e.m_offset_y) << ") Unk(" << static_cast<std::size_t>(e.m_unknown1) << ", "
              << static_cast<std::size_t>(e.m_unknown2) << ") }";
  }
};

}// namespace open_viii::graphics
#endif// VIIIARCHIVE_SP1ENTRY_H
