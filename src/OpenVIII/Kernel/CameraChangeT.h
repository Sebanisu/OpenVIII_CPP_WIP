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

#ifndef VIIIARCHIVE_CAMERACHANGET_H
#define VIIIARCHIVE_CAMERACHANGET_H
#include <cstdint>
#include <iostream>
namespace open_viii::Kernel {
struct CameraChangeT
{
private:
  static constexpr std::uint8_t highBitMask_ = 0x80U;
  static constexpr std::uint8_t otherBitsMask_ = 0x7FU;
  std::uint8_t cameraChange_{};

public:
  [[nodiscard]] bool Checked() const noexcept { return (cameraChange_ & highBitMask_) != 0; }
  [[nodiscard]] std::uint8_t Value() const noexcept
  {
    return static_cast<std::uint8_t>(cameraChange_ & otherBitsMask_);
  }

  friend std::ostream &operator<<(std::ostream &os, const CameraChangeT &cameraChange)
  {
    os << cameraChange.Checked() << '/' << static_cast<uint16_t>(cameraChange.Value());
    return os;
  }
};
}// namespace open_viii::Kernel
#endif// VIIIARCHIVE_CAMERACHANGET_H
