//
// Created by pcvii on 8/11/2020.
//

#ifndef VIIIARCHIVE_ZELLDUELBUTTONT_H
#define VIIIARCHIVE_ZELLDUELBUTTONT_H

namespace open_viii::kernel {
enum class ZellDuelButtonT : std::uint16_t {
  end = 0x0001,
  up = 0x0010,
  right = 0x0020,
  down = 0x0040,
  left = 0x0080,
  l2 = 0x0100,
  r2 = 0x0200,
  l1 = 0x0400,
  r1 = 0x0800,
  triangle = 0x1000,
  circle = 0x2000,
  cross = 0x4000,
  square = 0x8000,
  none = 0xFFFF,
};
}
#endif// VIIIARCHIVE_ZELLDUELBUTTONT_H
