//
// Created by pcvii on 9/2/2020.
//

#ifndef VIIIARCHIVE_TILE4_H
#define VIIIARCHIVE_TILE4_H
#include <cstdint>
#include "TexIdBuffer.h"
#include "PaletteID.h"
#include "LayerID.h"
#include "BlendModeT.h"
#include "Tile2.h"
namespace open_viii::graphics::background {
/**
 * @see https://github.com/myst6re/deling/blob/develop/files/BackgroundFile.h#L49
 */
struct Tile4
{
private:
  Tile2 m_t2{};

public:
  Tile4() = default;
  friend auto operator==(const Tile4 &left, const Tile4 &right) { return left.m_t2 == right.m_t2; }
  static constexpr auto HEIGHT{ 16 };
  static constexpr auto WIDTH{ 16 };
  static constexpr auto TEXTURE_PAGE_WIDTH{ 128U };
  static constexpr auto AREA = HEIGHT * WIDTH;
  [[nodiscard]] const std::int16_t &x() const noexcept { return m_t2.x(); }
  [[nodiscard]] const std::int16_t &y() const noexcept { return m_t2.y(); }
  void x(std::int16_t in_x) const noexcept { m_t2.x(in_x); }
  void y(std::int16_t in_y) const noexcept { m_t2.y(in_y); }
  [[nodiscard]] const std::uint16_t &z() const noexcept { return m_t2.z(); }
  [[nodiscard]] std::uint8_t texture_id() const noexcept { return m_t2.texture_id(); }
  [[nodiscard]] std::uint8_t blend() const noexcept { return m_t2.blend(); }
  [[nodiscard]] BPPT depth() const noexcept { return m_t2.depth(); }
  [[nodiscard]] bool draw() const noexcept { return m_t2.draw(); }
  [[nodiscard]] std::uint8_t palette_id() const noexcept { return m_t2.palette_id(); }
  [[nodiscard]] const std::uint16_t &source_x() const noexcept { return m_t2.source_x(); }
  [[nodiscard]] const std::uint16_t &source_y() const noexcept { return m_t2.source_y(); }
  [[nodiscard]] static std::uint8_t layer_id() noexcept { return 0U; }
  [[nodiscard]] static BlendModeT blend_mode() noexcept { return BlendModeT::none; }
  [[nodiscard]] const std::uint8_t &animation_id() const noexcept { return m_t2.animation_id(); }
  [[nodiscard]] const std::uint8_t &animation_state() const noexcept { return m_t2.animation_state(); }
  [[nodiscard]] Rectangle<std::uint16_t> rectangle() const noexcept { return m_t2.rectangle(); }
  [[nodiscard]] Rectangle<std::uint16_t> out_rectangle() const noexcept { return m_t2.out_rectangle(); }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_TILE4_H
