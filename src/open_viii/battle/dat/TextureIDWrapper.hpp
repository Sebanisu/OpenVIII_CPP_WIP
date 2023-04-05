//
// Created by pcvii on 4/5/2023.
//

#ifndef OPENVIII_CPP_WIP_TEXTUREIDWRAPPER_HPP
#define OPENVIII_CPP_WIP_TEXTUREIDWRAPPER_HPP
#include <bit>
#include <cstdint>
namespace open_viii::battle::dat {
struct TextureIDWrapper
{
  constexpr static std::uint16_t MASK  = 0b111U;
  constexpr static std::uint8_t  shift = 6;
  constexpr static std::uint16_t InverseMASK
    = std::rotl(static_cast<uint16_t>(~MASK), 6);
  operator std::uint8_t() const
  {
    return std::rotr(raw_value, shift) & MASK;
  }
  TextureIDWrapper &
    operator=(std::uint8_t input)
  {
    raw_value = static_cast<std::uint16_t>(raw_value & InverseMASK)
              | std::rotl(static_cast<std::uint16_t>(input & MASK), shift);
    return *this;
  }

private:
  std::uint16_t raw_value = {};
};
}// namespace open_viii::battle::dat
#endif// OPENVIII_CPP_WIP_TEXTUREIDWRAPPER_HPP
