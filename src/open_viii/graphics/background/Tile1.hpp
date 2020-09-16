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

#ifndef VIIIARCHIVE_TILE1_HPP
#define VIIIARCHIVE_TILE1_HPP
#include <cstdint>
#include "open_viii/graphics/Point.hpp"
#include "TexIdBuffer.hpp"
#include "BlendModeT.hpp"
#include "LayerID.hpp"
#include "PaletteID.hpp"
namespace open_viii::graphics::background {
/**
 * @see https://github.com/myst6re/deling/blob/develop/files/BackgroundFile.h#L49
 */
struct Tile1
{
private:
  mutable Point<std::int16_t> m_xy{};// 4
  std::uint16_t m_z{};// 6
  TexIdBuffer m_tex_id_buffer{};// 8
  PaletteID m_palette_id{};// 10
  Point<std::uint8_t> m_source_xy{};// 12
  LayerID m_layer_id{};// 13
  BlendModeT m_blend_mode{};// 14
  std::uint8_t m_animation_id{};// 15
  std::uint8_t m_animation_state{};// 16

public:
  Tile1() = default;
  friend auto operator==(const Tile1 &left, const Tile1 &right)
  {
    return left.m_xy == right.m_xy && left.m_z == right.m_z && left.m_tex_id_buffer == right.m_tex_id_buffer
           && left.m_palette_id == right.m_palette_id && left.m_source_xy == right.m_source_xy
           && left.m_layer_id == right.m_layer_id && left.m_blend_mode == right.m_blend_mode
           && left.m_animation_id == right.m_animation_id && left.m_animation_state == right.m_animation_state;
  }
  static constexpr auto HEIGHT{ 16U };
  static constexpr auto WIDTH{ 16U };
  static constexpr auto TEXTURE_PAGE_WIDTH{ 128U };
  static constexpr auto AREA = HEIGHT * WIDTH;
  [[nodiscard]] const std::int16_t &x() const noexcept { return m_xy.x(); }
  [[nodiscard]] const std::int16_t &y() const noexcept { return m_xy.y(); }
  const std::int16_t &x(std::int16_t in_x) const noexcept { return m_xy.x(in_x); }
  const std::int16_t &y(std::int16_t in_y) const noexcept { return m_xy.y(in_y); }
  [[nodiscard]] const std::uint16_t &z() const noexcept { return m_z; }
  [[nodiscard]] std::uint8_t texture_id() const noexcept { return m_tex_id_buffer.id(); }
  [[nodiscard]] std::uint8_t blend() const noexcept { return m_tex_id_buffer.blend(); }
  [[nodiscard]] BPPT depth() const noexcept { return m_tex_id_buffer.depth(); }
  [[nodiscard]] bool draw() const noexcept { return m_tex_id_buffer.draw(); }
  [[nodiscard]] std::uint8_t palette_id() const noexcept { return m_palette_id.id(); }
  [[nodiscard]] const std::uint8_t &source_x() const noexcept { return m_source_xy.x(); }
  [[nodiscard]] const std::uint8_t &source_y() const noexcept { return m_source_xy.y(); }
  [[nodiscard]] std::uint8_t layer_id() const noexcept { return m_layer_id.id(); }
  [[nodiscard]] const BlendModeT &blend_mode() const noexcept { return m_blend_mode; }
  [[nodiscard]] const std::uint8_t &animation_id() const noexcept { return m_animation_id; }
  [[nodiscard]] const std::uint8_t &animation_state() const noexcept { return m_animation_state; }
  [[nodiscard]] Rectangle<std::uint16_t> rectangle() const noexcept
  {
    return { static_cast<std::uint16_t>(m_source_xy.x()), std::uint16_t(m_source_xy.y()), WIDTH, HEIGHT };
  }
  [[maybe_unused]] [[nodiscard]] Rectangle<std::uint16_t> out_rectangle() const noexcept
  {
    return { static_cast<std::uint16_t>(m_xy.x()), std::uint16_t(m_xy.y()), WIDTH, HEIGHT };
  }
  [[maybe_unused]] constexpr static auto EXPLICIT_SIZE{ 16U };
};
static_assert(sizeof(Tile1) == Tile1::EXPLICIT_SIZE);
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_TILE1_HPP
