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
  std::int16_t m_x{};
  std::int16_t m_y{};
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
  static constexpr auto END_X{ 0x7FFFU };
  static constexpr auto HEIGHT{ 16 };
  static constexpr auto WIDTH{ 16 };
  static constexpr auto AREA = HEIGHT * WIDTH;
  [[nodiscard]] const std::int16_t &x() const noexcept { return m_x; }
  [[nodiscard]] const std::int16_t &y() const noexcept { return m_y; }
  [[nodiscard]] const std::uint16_t &z() const noexcept { return m_z; }
  [[nodiscard]] std::uint8_t texture_id() const noexcept { return m_tex_id_buffer.id(); }
  [[nodiscard]] std::uint8_t blend() const noexcept { return m_tex_id_buffer.blend(); }
  [[nodiscard]] std::uint8_t depth() const noexcept { return m_tex_id_buffer.depth(); }
  [[nodiscard]] bool draw() const noexcept { return m_tex_id_buffer.draw(); }
  [[nodiscard]] std::uint8_t palette_id() const noexcept { return m_palette_id.id(); }
  [[nodiscard]] const std::uint8_t &source_x() const noexcept { return m_source_x; }
  [[nodiscard]] const std::uint8_t &source_y() const noexcept { return m_source_y; }
  [[nodiscard]] std::uint8_t layer_id() const noexcept { return m_layer_id.id(); }
  [[nodiscard]] const BlendModeT &blend_mode() const noexcept { return m_blend_mode; }
  [[nodiscard]] const std::uint8_t &animation_id() const noexcept { return m_animation_id; }
  [[nodiscard]] const std::uint8_t &animation_state() const noexcept { return m_animation_state; }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_TILE1_H
