//
// Created by pcvii on 4/5/2023.
//

#ifndef OPENVIII_CPP_WIP_FACEWRAPPER_HPP
#define OPENVIII_CPP_WIP_FACEWRAPPER_HPP
#include <cstdint>
namespace open_viii::battle::dat {
struct FaceWrapper
{
  constexpr static std::uint16_t MASK        = 0xFFF;
  constexpr static std::uint16_t InverseMASK = 0xF000;
                                 operator std::uint16_t() const
  {
    return raw_value & MASK;
  }
  FaceWrapper &
    operator=(std::uint16_t input)
  {
    raw_value = static_cast<std::uint16_t>(raw_value & InverseMASK)
              | static_cast<std::uint16_t>(input & MASK);
    return *this;
  }

private:
  std::uint16_t raw_value = {};
};
}
#endif// OPENVIII_CPP_WIP_FACEWRAPPER_HPP
