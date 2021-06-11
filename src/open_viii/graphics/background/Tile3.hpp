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
#ifndef VIIIARCHIVE_TILE3_HPP
#define VIIIARCHIVE_TILE3_HPP
#include "BlendModeT.hpp"
#include "LayerID.hpp"
#include "PaletteID.hpp"
#include "TexIdBuffer.hpp"
#include "open_viii/graphics/Point.hpp"
#include <array>
#include <cstdint>
#include <string_view>
namespace open_viii::graphics::background {
/**
 * @see
 * https://github.com/myst6re/deling/blob/develop/files/BackgroundFile.h#L49
 * from pupu Logo.map is 14 bytes per entry.
 */
struct impl_Tile3
{
protected:
  Point<std::int16_t>   m_xy              = {};// 4
  Point<std::uint16_t>  m_source_xy       = {};// 8
  std::uint16_t         m_z               = {};// 10
  TexIdBuffer           m_tex_id_buffer   = {};// 12
  PaletteID             m_palette_id      = {};// 14
  static constexpr auto EXPECTED_SIZE     = 14U;
  static constexpr auto m_animation_id    = std::uint8_t{ 0xFFU };
  static constexpr auto m_animation_state = std::uint8_t{};
  static constexpr auto m_blend_mode      = BlendModeT::none;
  static constexpr auto m_layer_id        = LayerID{};
  impl_Tile3()                            = default;
  static constexpr auto FORCE_TYPE_VALUES
    = std::array{ std::string_view("logo") };

public:
  constexpr auto
    operator<=>(const impl_Tile3 &) const = default;
};
using Tile3 = TileCommon<impl_Tile3>;
static_assert(sizeof(Tile3) == Tile3::EXPECTED_SIZE);
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_TILE3_HPP
