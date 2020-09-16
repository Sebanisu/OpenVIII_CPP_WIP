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

#ifndef VIIIARCHIVE_TILE3_H
#define VIIIARCHIVE_TILE3_H
#include <cstdint>
#include <array>
#include <string_view>
#include "OpenVIII/Graphics/Point.h"
#include "TexIdBuffer.h"
#include "PaletteID.h"
#include "LayerID.h"
#include "BlendModeT.h"
namespace open_viii::graphics::background {
/**
 * @see https://github.com/myst6re/deling/blob/develop/files/BackgroundFile.h#L49
 * from pupu Logo.map is 14 bytes per entry.
 */
struct Tile3
{
private:
  mutable Point<std::int16_t> m_xy{};// 4
  Point<std::uint16_t> m_source_xy{};// 8
  std::uint16_t m_z{};// 10
  TexIdBuffer m_tex_id_buffer{};// 12
  PaletteID m_palette_id{};// 14

public:
  Tile3() = default;
  friend auto operator==(const Tile3 &left, const Tile3 &right)
  {
    return left.m_xy == right.m_xy && left.m_z == right.m_z && left.m_tex_id_buffer == right.m_tex_id_buffer
           && left.m_palette_id == right.m_palette_id && left.m_source_xy == right.m_source_xy;
  }
  static constexpr auto HEIGHT{ 16 };
  static constexpr auto WIDTH{ 16 };
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
  [[nodiscard]] const std::uint16_t &source_x() const noexcept { return m_source_xy.x(); }
  [[nodiscard]] const std::uint16_t &source_y() const noexcept { return m_source_xy.y(); }
  [[nodiscard]] static std::uint8_t layer_id() noexcept { return 0U; }
  [[nodiscard]] static BlendModeT blend_mode() noexcept { return BlendModeT::none; }
  [[nodiscard]] static std::uint8_t animation_id() noexcept
  {
    static constexpr std::uint8_t default_value = 0xFFU;
    return default_value;
  }
  [[nodiscard]] static std::uint8_t animation_state() noexcept { return 0U; }
  [[nodiscard]] Rectangle<std::int32_t> rectangle() const noexcept { return { source_x(), source_y(), WIDTH, HEIGHT }; }
  [[maybe_unused]] [[nodiscard]] Rectangle<std::uint16_t> out_rectangle() const noexcept
  {
    return { static_cast<std::uint16_t>(m_xy.x()), std::uint16_t(m_xy.y()), WIDTH, HEIGHT };
  }
  constexpr static auto EXPLICIT_SIZE{ 14U };
  static constexpr std::array FORCE_TYPE_VALUES = { std::string_view("logo") };
};
static_assert(sizeof(Tile3) == Tile3::EXPLICIT_SIZE);
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_TILE3_H
