//
// Created by pcvii on 4/27/2023.
//

#ifndef OPENVIII_CPP_WIP_BITREADER_HPP
#define OPENVIII_CPP_WIP_BITREADER_HPP
#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>
class BitReader
{
private:
  static constexpr inline auto PositionReadHelper = std::array{ 3, 6, 9, 16 };
  static constexpr inline auto RotationReadHelper = std::array{ 3, 6, 8, 12 };
  std::uint8_t                 _bitPosition{ 0 };
  std::vector<std::uint8_t>    data{};
  std::size_t                  position{ 0 };

public:
  explicit BitReader(std::vector<std::uint8_t> inputData)
    : data(std::move(inputData))
  {}

  std::uint16_t
    ReadBits(std::uint8_t count)
  {
    // Throw an exception if the count is greater than 16 bits
    const std::uint8_t bit16 = 16;
    const std::uint8_t bit8  = 8;
    if (count > bit16) {
      throw std::invalid_argument("Count cannot be greater than 16");
    }

    // Save the current position within the data vector
    std::size_t         current_position = position;

    // Read three consecutive bytes from the data vector
    std::uint32_t const byte1            = data[current_position++];
    std::uint32_t const byte2            = data[current_position++];
    std::uint32_t const byte3            = data[current_position++];

    // Combine the three bytes into a single 32-bit integer
    std::uint32_t       temp = byte1 | byte2 << bit8 | byte3 << bit16;

    // Create a mask to extract the desired number of bits
    std::uint32_t const mask = (1U << count) - 1;

    // Shift the combined integer right by the current bit position and mask out
    // unwanted bits
    temp                     = (temp >> _bitPosition) & mask;

    // Update the position and bit position within the data vector
    position                 = current_position + (count + _bitPosition) / bit8;
    _bitPosition             = (count + _bitPosition) % bit8;

    // Return the extracted bits as a std::uint16_t
    return static_cast<std::uint16_t>(temp);
  }

  std::uint16_t
    ReadPositionType()
  {
    std::uint8_t const countIndex = ReadBits(2);
    return ReadBits(PositionReadHelper.at(countIndex));
  }

  std::uint8_t
    ReadPositionLength()
  {
    return static_cast<std::uint8_t>(PositionReadHelper.at(ReadBits(2)));
  }

  std::uint8_t
    ReadRotationLength()
  {
    return static_cast<std::uint8_t>(RotationReadHelper.at(ReadBits(2)));
  }

  std::uint16_t
    ReadRotationType()
  {

    if (bool const readRotation = (ReadBits(1)) != 0; !readRotation) {
      return 0;
    }

    std::uint8_t const countIndex = ReadBits(2);
    return ReadBits(RotationReadHelper.at(countIndex));
  }
};
#endif// OPENVIII_CPP_WIP_BITREADER_HPP
