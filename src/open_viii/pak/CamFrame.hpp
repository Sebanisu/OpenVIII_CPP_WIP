//
// Created by pcvii on 12/31/2020.
//
#ifndef VIIIARCHIVE_CAMFRAME_HPP
#define VIIIARCHIVE_CAMFRAME_HPP
#include "open_viii/graphics/Point.hpp"
#include "open_viii/graphics/Vertice.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <fmt/format.h>
#include <ranges>
#include <string_view>

namespace open_viii::pak {

struct CamFrame
{
  // Public data (trivial access)
  graphics::Vertice<std::int16_t>   x{};
  graphics::Vertice<std::int16_t>   y{};
  graphics::Vertice<std::int16_t>   z{};
  std::int16_t                      z_z{};
  graphics::Vertice<std::int32_t>   space{};
  graphics::Point<std::int16_t>     pan{};
  std::int16_t                      zoom{};
  std::int16_t                      zoom2{};
  std::uint8_t                      render_mode{};
  std::array<char, 3U>              end{ 'E', 'N', 'D' };

  static constexpr std::string_view EXPECTED_END{ "END" };
  static constexpr auto             EXPECTED_SIZE{ 44U };

  constexpr CamFrame() = default;

  [[nodiscard]] constexpr bool
    valid_end() const noexcept
  {
    return std::ranges::equal(end, EXPECTED_END);
  }

  // Immutable-style modifiers
  [[nodiscard]] constexpr CamFrame
    with_x(graphics::Vertice<std::int16_t> v) const noexcept
  {
    auto ret = *this;
    ret.x    = v;
    return ret;
  }

  [[nodiscard]] constexpr CamFrame
    with_y(graphics::Vertice<std::int16_t> v) const noexcept
  {
    auto ret = *this;
    ret.y    = v;
    return ret;
  }

  [[nodiscard]] constexpr CamFrame
    with_z(graphics::Vertice<std::int16_t> v) const noexcept
  {
    auto ret = *this;
    ret.z    = v;
    return ret;
  }

  [[nodiscard]] constexpr CamFrame
    with_z_z(std::int16_t v) const noexcept
  {
    auto ret = *this;
    ret.z_z  = v;
    return ret;
  }

  [[nodiscard]] constexpr CamFrame
    with_space(graphics::Vertice<std::int32_t> v) const noexcept
  {
    auto ret  = *this;
    ret.space = v;
    return ret;
  }

  [[nodiscard]] constexpr CamFrame
    with_pan(graphics::Point<std::int16_t> v) const noexcept
  {
    auto ret = *this;
    ret.pan  = v;
    return ret;
  }

  [[nodiscard]] constexpr CamFrame
    with_zoom(std::int16_t v) const noexcept
  {
    auto ret = *this;
    ret.zoom = v;
    return ret;
  }

  [[nodiscard]] constexpr CamFrame
    with_zoom2(std::int16_t v) const noexcept
  {
    auto ret  = *this;
    ret.zoom2 = v;
    return ret;
  }

  [[nodiscard]] constexpr CamFrame
    with_render_mode(std::uint8_t v) const noexcept
  {
    auto ret        = *this;
    ret.render_mode = v;
    return ret;
  }

  [[nodiscard]] constexpr CamFrame
    with_end(std::array<char, 3U> v) const noexcept
  {
    auto ret = *this;
    ret.end  = v;
    return ret;
  }

  [[nodiscard]] constexpr CamFrame
    midpoint(const CamFrame other) const noexcept
  {
    auto ret  = *this;
    ret.x     = x.midpoint(other.x);
    ret.y     = y.midpoint(other.y);
    ret.z     = z.midpoint(other.z);
    ret.z_z   = std::midpoint(z_z, other.z_z);
    ret.space = space.midpoint(other.space);
    ret.pan   = pan.midpoint(other.pan);
    ret.zoom  = std::midpoint(zoom, other.zoom);
    ret.zoom2 = std::midpoint(zoom2, other.zoom2);
    return ret;
  }
};

static_assert(sizeof(CamFrame) == CamFrame::EXPECTED_SIZE);
static_assert(CamFrame().valid_end());

}// namespace open_viii::pak

template<>
struct fmt::formatter<open_viii::pak::CamFrame>
{
  // No custom format specs for now
  constexpr auto
    parse(format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(const open_viii::pak::CamFrame &v, FormatContext &ctx) const
  {
    constexpr auto mask = 0xFFU;

    return fmt::format_to(
      ctx.out(),
      "{{{},{},{},{},{},{},{},{},0x{:02X},{}}}",
      v.x,
      v.y,
      v.z,
      v.z_z,
      v.space,
      v.pan,
      v.zoom,
      v.zoom2,
      static_cast<uint16_t>(v.render_mode) & mask,
      std::string_view(v.end.data(), v.end.size()));
  }
};
#endif// VIIIARCHIVE_CAMFRAME_HPP
