//
// Created by pcvii on 10/19/2021.
//

#ifndef OPEN_VIII_GRAPHICS_BACKGROUND_PUPUID_HPP
#define OPEN_VIII_GRAPHICS_BACKGROUND_PUPUID_HPP
#include "Map.hpp"
#include <compare>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
namespace open_viii::graphics::background {
struct PupuID
{

  // 1 bit for x not aligned
  static constexpr std::uint32_t x_not_aligned_offset   = 31U;
  static constexpr std::uint32_t x_not_aligned_mask     = 0x1U;
  static constexpr std::uint32_t x_not_aligned_to_grid  = x_not_aligned_mask
                                                       << x_not_aligned_offset;
  // layer is 7 bits
  static constexpr std::uint32_t layer_offset           = 24U;
  static constexpr std::uint32_t layer_mask             = 0x7FU;
  // 1 bit for y not aligned
  static constexpr std::uint32_t y_not_aligned_offset   = 23U;
  static constexpr std::uint32_t y_not_aligned_mask     = 0x1U;
  static constexpr std::uint32_t y_not_aligned_to_grid  = y_not_aligned_mask
                                                       << y_not_aligned_offset;
  // blend is only 0,1,2,3,4. So 3 bits
  static constexpr std::uint32_t blend_offset           = 20U;
  static constexpr std::uint32_t blend_mask             = 0x07U;
  // animation id 8 bits
  static constexpr std::uint32_t animation_offset       = 12U;
  static constexpr std::uint32_t animation_mask         = 0xFFU;
  // animation state 8 bits
  static constexpr std::uint32_t animation_state_offset = 4U;
  static constexpr std::uint32_t animation_state_mask   = 0xFFU;
  // the offset is 4 bits
  static constexpr std::uint32_t offset_mask            = 0xFU;
  // We just used this to check if the x and y are not aligned.
  static constexpr std::int8_t   tile_grid_size         = 16;

  static_assert(x_not_aligned_to_grid == 0x8000'0000U);
  static_assert((layer_mask << layer_offset) == 0x7F00'0000U);
  static_assert(y_not_aligned_to_grid == 0x0080'0000U);
  static_assert((blend_mask << blend_offset) == 0x0070'0000U);
  static_assert((animation_mask << animation_offset) == 0x000F'F000U);
  static_assert(
    (animation_state_mask << animation_state_offset) == 0x0000'0FF0);

  constexpr PupuID() noexcept = default;
  constexpr explicit PupuID(std::uint32_t raw) noexcept : m_raw(raw) {}
  constexpr explicit PupuID(
    const is_tile auto &tile,
    std::uint8_t        offset = 0U) noexcept
    : PupuID(
        std::uint32_t{
          ((static_cast<std::uint32_t>(tile.layer_id()) & layer_mask)
           << layer_offset)
          | ((static_cast<std::uint32_t>(tile.blend_mode()) & blend_mask) << blend_offset)
          | static_cast<std::uint32_t>(tile.animation_id() << animation_offset)
          | static_cast<std::uint32_t>(
            tile.animation_state() << animation_state_offset)
          | (offset & offset_mask) })
  {
    if (tile.x() % tile_grid_size != 0) {
      m_raw |= x_not_aligned_to_grid;
    }
    if (tile.y() % tile_grid_size != 0) {
      m_raw |= y_not_aligned_to_grid;
    }
    assert(tile.layer_id() == layer_id());
    assert(tile.blend_mode() == blend_mode());
    assert(tile.animation_id() == animation_id());
    assert(tile.animation_state() == animation_state());
  }
  [[nodiscard]] constexpr std::uint8_t
    layer_id() const noexcept
  {
    return static_cast<std::uint8_t>((m_raw >> layer_offset) & layer_mask);
  }
  [[nodiscard]] constexpr BlendModeT
    blend_mode() const noexcept
  {
    return static_cast<BlendModeT>((m_raw >> blend_offset) & blend_mask);
  }
  [[nodiscard]] constexpr std::uint8_t
    animation_id() const noexcept
  {
    return static_cast<std::uint8_t>(
      (m_raw >> animation_offset) & std::numeric_limits<std::uint8_t>::max());
  }
  [[nodiscard]] constexpr std::uint8_t
    animation_state() const noexcept
  {
    return static_cast<std::uint8_t>(
      (m_raw >> animation_state_offset)
      & std::numeric_limits<std::uint8_t>::max());
  }
  [[nodiscard]] constexpr std::uint8_t
    offset() const noexcept
  {
    return static_cast<std::uint8_t>(m_raw & offset_mask);
  }
  [[nodiscard]] constexpr bool
    is_x_not_aligned_to_grid() const noexcept
  {
    return (m_raw & x_not_aligned_to_grid) != 0;
  }
  [[nodiscard]] constexpr bool
    is_y_not_aligned_to_grid() const noexcept
  {
    return (m_raw & y_not_aligned_to_grid) != 0;
  }
  constexpr PupuID &
    operator+=(std::uint32_t right) noexcept
  {
    m_raw += right;
    return *this;
  }
  [[nodiscard]] constexpr PupuID
    operator+(std::uint32_t right) const noexcept
  {
    auto cpy = *this;
    return cpy += right;
  }
  constexpr PupuID &
    operator|=(std::uint32_t right) noexcept
  {
    m_raw |= right;
    return *this;
  }
  [[nodiscard]] constexpr PupuID
    operator|(std::uint32_t right) const noexcept
  {
    auto cpy = *this;
    return cpy |= right;
  }
  constexpr PupuID &
    operator+=(PupuID right) noexcept
  {
    m_raw += right.raw();
    return *this;
  }
  [[nodiscard]] constexpr PupuID
    operator+(PupuID right) const noexcept
  {
    auto cpy = *this;
    return cpy += right.raw();
  }
  [[nodiscard]] constexpr explicit
    operator std::uint32_t() const noexcept
  {
    return m_raw;
  }
  [[nodiscard]] constexpr std::uint32_t
    raw() const noexcept
  {
    return m_raw;
  }
  [[nodiscard]] bool constexpr same_base(PupuID right) const noexcept
  {
    constexpr std::uint32_t mask = ~offset_mask;
    static_assert(mask == 0xFFFFFFF0u, "same_base mask mismatch");
    return (m_raw & mask) == (right.raw() & mask);
  }

  [[nodiscard]] bool constexpr same_layer_base(PupuID right) const noexcept
  {
    constexpr std::uint32_t mask
      = ~((layer_mask << layer_offset) | offset_mask);
    static_assert(mask == 0x80FFFFF0u, "same_layer_base mask mismatch");
    return (m_raw & mask) == (right.raw() & mask);
  }

  [[nodiscard]] bool constexpr same_animation_base(PupuID right) const noexcept
  {
    constexpr std::uint32_t mask = ~(
      (animation_mask << animation_offset)
      | (animation_state_mask << animation_state_offset) | offset_mask);
    static_assert(mask == 0xFFF00000u, "same_animation_base mask mismatch");
    return (m_raw & mask) == (right.raw() & mask);
  }

  [[nodiscard]] bool constexpr same_animation_id_base(
    PupuID right) const noexcept
  {
    constexpr std::uint32_t mask
      = ~((animation_state_mask << animation_state_offset) | offset_mask);
    static_assert(mask == 0xFFFFF000u, "same_animation_id_base mask mismatch");
    return (m_raw & mask) == (right.raw() & mask);
  }

  [[nodiscard]] bool constexpr same_animation_state_base(
    PupuID right) const noexcept
  {
    constexpr std::uint32_t mask
      = ~((animation_mask << animation_offset) | offset_mask);
    static_assert(
      mask == 0xFFF00FF0u,
      "same_animation_state_base mask mismatch");
    return (m_raw & mask) == (right.raw() & mask);
  }

  constexpr auto
    operator<=>(const PupuID &) const noexcept = default;
  [[nodiscard]] std::string
    create_summary() const;

private:
  std::uint32_t m_raw{};
};
}// namespace open_viii::graphics::background
template<>
struct fmt::formatter<open_viii::graphics::background::PupuID> : fmt::formatter<std::string>
{
  // Formats value using the parsed format specification stored in this
  // formatter and writes the output to ctx.out().
  auto
    format(const open_viii::graphics::background::PupuID &value, format_context &ctx) const
    -> format_context::iterator
  {
    return fmt::format_to(ctx.out(), "{:08X}", value.raw());
  }
};

#endif// OPEN_VIII_GRAPHICS_BACKGROUND_PUPUID_HPP
