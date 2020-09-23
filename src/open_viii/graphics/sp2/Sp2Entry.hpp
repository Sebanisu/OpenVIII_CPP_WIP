// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef VIIIARCHIVE_SP2ENTRY_HPP
#define VIIIARCHIVE_SP2ENTRY_HPP
#include <cstdint>
namespace open_viii::graphics {
struct Sp2Entry
{
private:
  std::uint32_t m_count{};
  std::uint8_t m_x{};
  std::uint8_t m_y{};
  std::uint8_t m_unknown1{};
  std::uint8_t m_unknown2{};
  std::uint16_t m_width{};
  std::uint16_t m_height{};
  std::uint8_t m_unknown3{};
  std::uint8_t m_unknown4{};
  std::uint8_t m_unknown5{};
  std::uint8_t m_unknown6{};

public:
  [[nodiscard]] auto count() const noexcept
  {
    return m_count;
  }
  [[maybe_unused]] [[nodiscard]] auto x() const noexcept
  {
    return m_x;
  }
  [[maybe_unused]] [[nodiscard]] auto y() const noexcept
  {
    return m_y;
  }
  [[nodiscard]] auto unknown1() const noexcept
  {
    return m_unknown1;
  }
  [[nodiscard]] auto unknown2() const noexcept
  {
    return m_unknown2;
  }
  [[nodiscard]] auto width() const noexcept
  {
    return m_width;
  }
  [[nodiscard]] auto height() const noexcept
  {
    return m_height;
  }
  [[maybe_unused]] [[nodiscard]] auto unknown3() const noexcept
  {
    return m_unknown3;
  }
  [[nodiscard]] auto unknown4() const noexcept
  {
    return m_unknown4;
  }
  [[maybe_unused]] [[nodiscard]] auto unknown5() const noexcept
  {
    return m_unknown5;
  }
  [[maybe_unused]] [[nodiscard]] auto unknown6() const noexcept
  {
    return m_unknown6;
  }
  Sp2Entry() = default;
  friend std::ostream &operator<<(std::ostream &os, const Sp2Entry &e)
  {
    return os << "{ " << static_cast<std::size_t>(e.m_count) << " (X, Y) = (" << static_cast<std::size_t>(e.m_x) << ", "
              << static_cast<std::size_t>(e.m_y) << "), (W, H) = (" << static_cast<std::size_t>(e.m_width) << ", "
              << static_cast<std::size_t>(e.m_height) << "), Unk(" << static_cast<std::size_t>(e.m_unknown1) << ", "
              << static_cast<std::size_t>(e.m_unknown2) << ", " << static_cast<std::size_t>(e.m_unknown3) << ", "
              << static_cast<std::size_t>(e.m_unknown4) << ", " << static_cast<std::size_t>(e.m_unknown5) << ", "
              << static_cast<std::size_t>(e.m_unknown6) << ") }";
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_SP2ENTRY_HPP
