// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#ifndef VIIIARCHIVE_impl_Tile2_HPP
#define VIIIARCHIVE_impl_Tile2_HPP
#include "BlendModeT.hpp"
#include "LayerID.hpp"
#include "open_viii/graphics/Point.hpp"
#include "open_viii/graphics/Rectangle.hpp"
#include "PaletteID.hpp"
#include "TexIdBuffer.hpp"
#include "TileCommon.hpp"
namespace open_viii::graphics::background {
/**
 * @see
 * https://github.com/myst6re/deling/blob/develop/files/BackgroundFile.h#L49
 */
struct impl_Tile2
{
protected:
  Point<std::int16_t>   m_xy              = {};   // 4
  Point<std::uint16_t>  m_source_xy       = {};   // 8
  std::uint16_t         m_z               = {};   // 10
  TexIdBuffer           m_tex_id_buffer   = {};   // 12
  PaletteID             m_palette_id      = {};   // 14
  std::uint8_t          m_animation_id    = 0xFFU;// 15
  std::uint8_t          m_animation_state = {};   // 16
  constexpr static auto EXPECTED_SIZE     = 16U;
  static constexpr auto m_blend_mode      = BlendModeT::none;
  static constexpr auto m_layer_id        = LayerID{};
  impl_Tile2()                            = default;
  static constexpr auto FORCE_TYPE_VALUES
    = std::array{ std::string_view("test10"),
                  std::string_view("test11"),
                  std::string_view("test12") };

public:
  constexpr auto
    operator<=>(const impl_Tile2 &) const
    = default;
};
using Tile2 = TileCommon<impl_Tile2>;
static_assert(sizeof(Tile2) == Tile2::EXPECTED_SIZE);
static_assert(has_with_animation_state<Tile2>);
static_assert(has_with_animation_id<Tile2>);
static_assert(not has_with_blend_mode<Tile2>);
static_assert(not has_with_layer_id<Tile2>);
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_impl_Tile2_HPP
