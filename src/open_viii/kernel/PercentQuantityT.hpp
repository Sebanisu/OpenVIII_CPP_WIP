//
// Created by pcvii on 1/25/2021.
//
#ifndef VIIIARCHIVE_PERCENTQUANTITYT_HPP
#define VIIIARCHIVE_PERCENTQUANTITYT_HPP
#include <cstdint>
namespace open_viii::kernel {
enum class PercentQuantityT : std::uint8_t
{
  none [[maybe_unused]] = 0b0'0000,// 0% = 0x00
  sixteenth             = 0b0'0001,// 6.25% = 0x01,
  eighth                = 0b0'0010,// 12.50% = 0x02
  quarter               = 0b0'0100,// 25% = 0x04
  half                  = 0b0'1000,// 50% = 0x08
  full                  = 0b1'0000,// 100% = 0x10
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_PERCENTQUANTITYT_HPP
