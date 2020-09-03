//
// Created by pcvii on 9/2/2020.
//

#ifndef VIIIARCHIVE_BLENDMODET_H
#define VIIIARCHIVE_BLENDMODET_H
#include <cstdint>
namespace open_viii::graphics::background {
enum struct BlendModeT : std::uint8_t {
  half_add [[maybe_unused]],
  add [[maybe_unused]],
  subtract [[maybe_unused]],
  quarter_add [[maybe_unused]],
  none [[maybe_unused]],
};
}
#endif// VIIIARCHIVE_BLENDMODET_H
