//
// Created by pcvii on 9/6/2022.
//

#ifndef OPEN_VIII_GRAPHICS_BACKGROUND__UNIQUIFYPUPU_HPP
#define OPEN_VIII_GRAPHICS_BACKGROUND__UNIQUIFYPUPU_HPP
#include "PupuID.hpp"
#include <map>
namespace open_viii::graphics::background {
class UniquifyPupu
{
public:
  struct PupuKey
  {
    PupuID       pupu_id = {};
    std::int16_t x       = {};
    std::int16_t y       = {};
    auto
      operator<=>(const PupuKey &) const = default;
  };

  PupuID
    operator()(const is_tile auto &tile_const);

private:
  std::map<PupuKey, std::uint8_t> m_pupu_map = {};
};
}// namespace open_viii::graphics::background
#endif// OPEN_VIII_GRAPHICS_BACKGROUND__UNIQUIFYPUPU_HPP
