//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_TARGETT_H
#define VIIIARCHIVE_TARGETT_H
#include <cstdint>

namespace OpenVIII::Kernel {
enum class TargetT : std::uint8_t {
  None = 0x00,
  Dead = 0x01,
  Unk02 = 0x02,
  Ally = 0x04,
  SingleSide = 0x08,
  SingleTarget = 0x10,
  Unk20 = 0x20,
  Enemy = 0x40,
  Unk80 = 0x80,
};
}
#endif// VIIIARCHIVE_TARGETT_H
