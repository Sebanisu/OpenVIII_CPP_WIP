//
// Created by pcvii on 10/5/2020.
//

#ifndef VIIIARCHIVE_MIMFROMPATH_HPP
#define VIIIARCHIVE_MIMFROMPATH_HPP
#include "Mim.hpp"
#include <array>
#include "open_viii/graphics/Ppm.hpp"
namespace open_viii::graphics::background {
/**
 * This surfaces the same interface as Mim but the backend will handle multiple images located in path.
 */
struct MimFromPath
{
private:
  static constexpr auto MAX_PALETTES_PLUS_1 = 17U;
  static constexpr auto MAX_TEXTURE_PAGES = 13U;
  MimType m_mim_type{};
  std::array<std::array<Ppm,MAX_PALETTES_PLUS_1>,MAX_TEXTURE_PAGES> m_textures{};

public:
  MimFromPath() = default;
  explicit MimFromPath(MimType mim_type) : m_mim_type(mim_type) {}
  std::size_t get_raw_width(const BPPT &depth) const
  {
    return Mim::get_raw_width(depth,m_mim_type.width());
  }
  Color auto get_color([[maybe_unused]]const std::size_t &x,
                       [[maybe_unused]]const std::size_t&y,
                                  [[maybe_unused]]const BPPT &depth,
                       [[maybe_unused]]const std::uint8_t & palette,
                       [[maybe_unused]]const std::uint8_t & texture_id) const
  {

    return Color24{};
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_MIMFROMPATH_HPP
