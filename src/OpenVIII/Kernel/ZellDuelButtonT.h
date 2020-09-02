//
// Created by pcvii on 8/11/2020.
//

#ifndef VIIIARCHIVE_ZELLDUELBUTTONT_H
#define VIIIARCHIVE_ZELLDUELBUTTONT_H

namespace open_viii::kernel {
enum class ZellDuelButtonT : std::uint16_t {
  End = 0x0001,
  Up = 0x0010,
  Right = 0x0020,
  Down = 0x0040,
  Left = 0x0080,
  L2 = 0x0100,
  R2 = 0x0200,
  L1 = 0x0400,
  R1 = 0x0800,
  Triangle = 0x1000,
  Circle = 0x2000,
  Cross = 0x4000,
  Square = 0x8000,
  None = 0xFFFF,
};
}
#endif// VIIIARCHIVE_ZELLDUELBUTTONT_H
