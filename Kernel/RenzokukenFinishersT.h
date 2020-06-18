//
// Created by pcvii on 6/17/2020.
//

#ifndef VIIIARCHIVE_RENZOKUKENFINISHERST_H
#define VIIIARCHIVE_RENZOKUKENFINISHERST_H
#include <cstdint>
namespace OpenVIII::Kernel {
enum class RenzokukenFinishersT : std::uint8_t {
  None = 0x00,
  RoughDivide = 0x01,
  FatedCircle = 0x02,
  BlastingZone = 0x04,
  LionHeart = 0x08,
};
}
#endif// VIIIARCHIVE_RENZOKUKENFINISHERST_H
