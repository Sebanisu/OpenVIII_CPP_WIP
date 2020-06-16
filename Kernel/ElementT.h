//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_ELEMENTT_H
#define VIIIARCHIVE_ELEMENTT_H
#include <cstdint>
namespace OpenVIII::Kernel {
enum class ElementT : std::uint8_t {
  NonElemental = 0x00,
  Fire = 0x01,
  Ice = 0x02,
  Thunder = 0x04,
  Earth = 0x08,
  Poison = 0x10,
  Wind = 0x20,
  Water = 0x40,
  Holy = 0x80,
};
}
#endif// VIIIARCHIVE_ELEMENTT_H
