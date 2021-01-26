//
// Created by pcvii on 1/25/2021.
//
#ifndef VIIIARCHIVE_PERCENTQUANTITYT_HPP
#define VIIIARCHIVE_PERCENTQUANTITYT_HPP
#include <cstdint>
namespace open_viii::kernel {
enum class PercentQuantityT : std::uint8_t {
  none [[maybe_unused]] = 0x00,// 0% = 0x00
  sixteenth             = 0x01,// 6.25% = 0x01,
  eighth                = 0x02,// 12.50% = 0x02
  quarter               = 0x04,// 25% = 0x04
  half                  = 0x08,// 50% = 0x08
  full                  = 0x10,// 100% = 0x10
};
}
#endif// VIIIARCHIVE_PERCENTQUANTITYT_HPP
