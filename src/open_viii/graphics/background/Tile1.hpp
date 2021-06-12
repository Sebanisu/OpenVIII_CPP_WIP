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
#ifndef VIIIARCHIVE_impl_Tile1_HPP
#define VIIIARCHIVE_impl_Tile1_HPP
#include "BlendModeT.hpp"
#include "LayerID.hpp"
#include "PaletteID.hpp"
#include "TexIdBuffer.hpp"
#include "TileCommon.hpp"
#include "open_viii/graphics/Point.hpp"
#include "open_viii/graphics/Rectangle.hpp"
#include <array>
#include <cstdint>
#include <string_view>
namespace open_viii::graphics::background {
/**
 * @see
 * https://github.com/myst6re/deling/blob/develop/files/BackgroundFile.h#L49
 */
struct impl_Tile1
{
protected:
  Point<std::int16_t>   m_xy              = {};              // 4
  std::uint16_t         m_z               = {};              // 6
  TexIdBuffer           m_tex_id_buffer   = {};              // 8
  PaletteID             m_palette_id      = {};              // 10
  Point<std::uint8_t>   m_source_xy       = {};              // 12
  LayerID               m_layer_id        = {};              // 13
  BlendModeT            m_blend_mode      = BlendModeT::none;// 14
  std::uint8_t          m_animation_id    = 0xFFU;           // 15
  std::uint8_t          m_animation_state = {};              // 16
  constexpr static auto EXPECTED_SIZE     = 16U;
  impl_Tile1()                            = default;
  constexpr static auto FORCE_TYPE_VALUES = std::array<std::string_view, 0U>{};

public:
  constexpr auto
    operator<=>(const impl_Tile1 &) const = default;
};
using Tile1 = TileCommon<impl_Tile1>;
static_assert(has_with_animation_state<Tile1>);
static_assert(has_with_animation_id<Tile1>);
static_assert(has_with_blend_mode<Tile1>);
static_assert(has_with_layer_id<Tile1>);
static_assert(sizeof(Tile1) == Tile1::EXPECTED_SIZE);
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_impl_Tile1_HPP
