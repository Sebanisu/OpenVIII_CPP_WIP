//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_SP1ENTRY_H
#define VIIIARCHIVE_SP1ENTRY_H

#include <cstdint>
namespace OpenVIII::Graphics {
struct sp1Entry
{
public:
  uint8_t x_{};
  uint8_t y_{};
  uint8_t unknown1_{};
  uint8_t unknown2_{};
  uint8_t width_{};
  uint8_t offset_x_{};
  uint8_t height_{};
  uint8_t offset_y_{};
};

}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_SP1ENTRY_H
