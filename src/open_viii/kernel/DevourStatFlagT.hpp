//
// Created by pcvii on 1/25/2021.
//
#ifndef VIIIARCHIVE_DEVOURSTATFLAGT_HPP
#define VIIIARCHIVE_DEVOURSTATFLAGT_HPP
namespace open_viii::kernel {
enum class DevourStatFlagT : std::uint8_t
{
  none                 = 0x00,
  str                  = 0x01,
  vit [[maybe_unused]] = 0x02,
  mag                  = 0x04,
  spr                  = 0x08,
  spd                  = 0x10,
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_DEVOURSTATFLAGT_HPP
