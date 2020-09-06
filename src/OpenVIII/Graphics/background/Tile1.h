//
// Created by pcvii on 9/2/2020.
//

#ifndef VIIIARCHIVE_TILE1_H
#define VIIIARCHIVE_TILE1_H
#include <cstdint>
#include "TexIdBuffer.h"
#include "BlendModeT.h"
#include "LayerID.h"
#include "PaletteID.h"
namespace open_viii::graphics::background {
/**
 * @see https://github.com/myst6re/deling/blob/develop/files/BackgroundFile.h#L49
 */
struct Tile1
{
private:
  mutable std::int16_t m_x{};
  mutable std::int16_t m_y{};
  std::uint16_t m_z{};
  TexIdBuffer m_tex_id_buffer{};
  PaletteID m_palette_id{};
  std::uint8_t m_source_x{};
  std::uint8_t m_source_y{};
  LayerID m_layer_id{};
  BlendModeT m_blend_mode{};
  std::uint8_t m_animation_id{};
  std::uint8_t m_animation_state{};

public:
  Tile1() = default;
  friend auto operator==(const Tile1 &left, const Tile1 &right)
  {
    return left.m_x == right.m_x && left.m_y == right.m_y && left.m_z == right.m_z
           && left.m_tex_id_buffer == right.m_tex_id_buffer && left.m_palette_id == right.m_palette_id
           && left.m_source_x == right.m_source_x && left.m_source_y == right.m_source_y
           && left.m_layer_id == right.m_layer_id && left.m_blend_mode == right.m_blend_mode
           && left.m_animation_id == right.m_animation_id && left.m_animation_state == right.m_animation_state;
  }
  static constexpr auto HEIGHT{ 16U };
  static constexpr auto WIDTH{ 16U };
  static constexpr auto TEXTURE_PAGE_WIDTH{ 128U };
  static constexpr auto AREA = HEIGHT * WIDTH;
  [[nodiscard]] const std::int16_t &x() const noexcept { return m_x; }
  [[nodiscard]] const std::int16_t &y() const noexcept { return m_y; }
  void x(std::int16_t in_x) const noexcept { m_x = in_x; }
  void y(std::int16_t in_y) const noexcept { m_y = in_y; }
  [[nodiscard]] const std::uint16_t &z() const noexcept { return m_z; }
  [[nodiscard]] std::uint8_t texture_id() const noexcept { return m_tex_id_buffer.id(); }
  [[nodiscard]] std::uint8_t blend() const noexcept { return m_tex_id_buffer.blend(); }
  [[nodiscard]] BPPT depth() const noexcept { return m_tex_id_buffer.depth(); }
  [[nodiscard]] bool draw() const noexcept { return m_tex_id_buffer.draw(); }
  [[nodiscard]] std::uint8_t palette_id() const noexcept { return m_palette_id.id(); }
  [[nodiscard]] const std::uint8_t &source_x() const noexcept { return m_source_x; }
  [[nodiscard]] const std::uint8_t &source_y() const noexcept { return m_source_y; }
  [[nodiscard]] std::uint8_t layer_id() const noexcept { return m_layer_id.id(); }
  [[nodiscard]] const BlendModeT &blend_mode() const noexcept { return m_blend_mode; }
  [[nodiscard]] const std::uint8_t &animation_id() const noexcept { return m_animation_id; }
  [[nodiscard]] const std::uint8_t &animation_state() const noexcept { return m_animation_state; }
  [[nodiscard]] Rectangle<std::uint16_t> rectangle() const noexcept
  {
    return { source_x(), source_y(), WIDTH, HEIGHT };
  }
  [[nodiscard]] Rectangle<std::uint16_t> out_rectangle() const noexcept
  {
    return { static_cast<std::uint16_t>(m_x), std::uint16_t(m_y), WIDTH, HEIGHT };
  }
  [[maybe_unused]] constexpr static auto EXPLICIT_SIZE{ 16U };
};
static_assert(sizeof(Tile1) == Tile1::EXPLICIT_SIZE);
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_TILE1_H
