//
// Created by pcvii on 7/8/2021.
//

#ifndef OPENVIII_CPP_WIP_COLOR16_INDEX_VALUE_HPP
#define OPENVIII_CPP_WIP_COLOR16_INDEX_VALUE_HPP
namespace open_viii::graphics {
struct Color16_index_value
{
private:
  std::uint16_t CONVERT_SHIFT      = {};
  std::uint16_t GET_HIGH_BIT_SHIFT = {};
  std::uint16_t mask               = {};
  std::uint16_t shift              = {};
  std::uint16_t inverse_mask       = {};

public:
  consteval Color16_index_value(std::uint16_t maskT, std::uint16_t shiftT)
    : CONVERT_SHIFT{ static_cast<std::uint16_t>(8 - std::popcount(maskT)) },
      GET_HIGH_BIT_SHIFT{ static_cast<std::uint16_t>(std::popcount(maskT)
                                                     - CONVERT_SHIFT) },
      mask{ maskT }, shift{ shiftT }, inverse_mask{ static_cast<std::uint16_t>(
                                        ~maskT) }
  {
    assert(std::popcount(maskT) < 8 && shiftT < 16U);
  }
  [[nodiscard]] constexpr std::uint8_t
    convert(const std::uint16_t value) const noexcept
  {
    const std::uint16_t temp = (value & mask) >> shift;
    return static_cast<std::uint8_t>((temp << CONVERT_SHIFT)
                                     | (temp >> GET_HIGH_BIT_SHIFT));
  }
  [[nodiscard]] constexpr std::uint16_t
    with(const std::uint16_t value, const std::uint8_t change) const noexcept
  {
    return (value & inverse_mask)
         | static_cast<std::uint16_t>(((change >> CONVERT_SHIFT) << shift)
                                      & mask);
  }
};
}
#endif// OPENVIII_CPP_WIP_COLOR16_INDEX_VALUE_HPP
