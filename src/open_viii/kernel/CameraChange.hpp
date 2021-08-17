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
#ifndef VIIIARCHIVE_CAMERACHANGE_HPP
#define VIIIARCHIVE_CAMERACHANGE_HPP
#include <compare>
#include <cstdint>
#include <iostream>
namespace open_viii::kernel {
struct CameraChange
{
private:
  static constexpr std::uint8_t HIGH_BIT_MASK   = 0x80U;
  static constexpr std::uint8_t OTHER_BITS_MASK = 0x7FU;
  std::uint8_t                  m_camera_change{};

public:
  constexpr auto
    operator<=>(const CameraChange &right) const noexcept = default;
  [[nodiscard]] constexpr bool
    checked() const noexcept
  {
    return (m_camera_change & HIGH_BIT_MASK) != 0;
  }
  [[nodiscard]] constexpr std::uint8_t
    value() const noexcept
  {
    return static_cast<std::uint8_t>(m_camera_change & OTHER_BITS_MASK);
  }
  friend std::ostream &
    operator<<(std::ostream &os, const CameraChange &camera_change)
  {
    os << +camera_change.checked() << '/'
       << +camera_change.value();
    return os;
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_CAMERACHANGE_HPP
