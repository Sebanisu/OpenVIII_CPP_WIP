//
// Created by pcvii on 6/17/2020.
//

#ifndef VIIIARCHIVE_CAMERACHANGET_H
#define VIIIARCHIVE_CAMERACHANGET_H
#include <cstdint>
#include <iostream>
namespace OpenVIII::Kernel {
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
}
#endif// VIIIARCHIVE_CAMERACHANGET_H
