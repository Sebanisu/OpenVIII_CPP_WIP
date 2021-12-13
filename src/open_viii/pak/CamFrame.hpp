//
// Created by pcvii on 12/31/2020.
//
#ifndef VIIIARCHIVE_CAMFRAME_HPP
#define VIIIARCHIVE_CAMFRAME_HPP
#include "open_viii/graphics/Point.hpp"
#include "open_viii/graphics/Vertice.hpp"
#include <algorithm>
#include <array>
namespace open_viii::pak {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_PAK#CAM_files
 */
struct CamFrame
{
private:
  /**
   * Camera Vector X axis
   */
  graphics::Vertice<std::int16_t> m_x{};
  /**
   * Camera Vector Y axis
   */
  graphics::Vertice<std::int16_t> m_y{};
  /**
   * Camera Vector Z axis
   */
  graphics::Vertice<std::int16_t> m_z{};
  /**
   * Camera Vector Z.z (copy)
   */
  std::int16_t                    m_z_z{};
  /**
   * Camera Space Vector
   */
  graphics::Vertice<std::int32_t> m_space{};
  /**
   * Camera Pan
   */
  graphics::Point<std::int16_t>   m_pan{};
  /**
   * Zoom
   */
  std::int16_t                    m_zoom{};
  /**
   * Zoom (Repeated)
   */
  std::int16_t                    m_zoom2{};
  /**
   * Render Mode ('0x08','0x10','0x11' or '0x50')
   */
  std::uint8_t                    m_render_mode{};
  /**
   * "END" marker 3 bytes
   */
  std::array<char, 3U>            m_end{ 'E', 'N', 'D' };

public:
  /**
   * Expected "END"
   */
  static constexpr std::string_view EXPECTED_END{ "END" };
  constexpr CamFrame() = default;
  /**
   * Expected Size of struct in bytes
   */
  static constexpr auto EXPECTED_SIZE{ 44U };
  /**
   * Camera Vector X axis
   */
  [[nodiscard]] constexpr auto
    x() const noexcept
  {
    return m_x;
  }
  /**
   * Camera Vector Y axis
   */
  [[nodiscard]] constexpr auto
    y() const noexcept
  {
    return m_y;
  }
  /**
   * Camera Vector Z axis
   */
  [[nodiscard]] constexpr auto
    z() const noexcept
  {
    return m_z;
  }
  /**
   * Camera Vector Z.z (copy)
   */
  [[nodiscard]] constexpr auto
    z_z() const noexcept
  {
    return m_z_z;
  }
  /**
   * Camera Space Vector
   */
  [[nodiscard]] constexpr auto
    space() const noexcept
  {
    return m_space;
  }
  /**
   * Camera Pan
   */
  [[nodiscard]] constexpr auto
    pan() const noexcept
  {
    return m_pan;
  }
  /**
   * Zoom
   */
  [[nodiscard]] constexpr auto
    zoom() const noexcept
  {
    return m_zoom;
  }
  /**
   * Zoom (Repeated)
   */
  [[nodiscard]] constexpr auto
    zoom2() const noexcept
  {
    return m_zoom2;
  }
  /**
   * Render Mode ('0x08','0x10','0x11' or '0x50')
   */
  [[nodiscard]] constexpr auto
    render_mode() const noexcept
  {
    return m_render_mode;
  }
  /**
   * "END" marker 3 bytes
   */
  [[nodiscard]] constexpr auto
    end() const noexcept
  {
    return m_end;
  }
  /**
   * Camera Vector X axis
   */
  constexpr auto
    with_x(graphics::Vertice<std::int16_t> in_x) const noexcept
  {
    auto ret = *this;
    ret.m_x  = in_x;
    return ret;
  }
  /**
   * Camera Vector Y axis
   */
  constexpr auto
    with_y(graphics::Vertice<std::int16_t> in_y) const noexcept
  {
    auto ret = *this;
    ret.m_y  = in_y;
    return ret;
  }
  /**
   * Camera Vector Z axis
   */
  constexpr auto
    with_z(graphics::Vertice<std::int16_t> in_z) const noexcept
  {
    auto ret = *this;
    ret.m_z  = in_z;
    return ret;
  }
  /**
   * Camera Vector Z.z (copy)
   */
  constexpr auto
    with_z_z(std::int16_t in_z_z) const noexcept
  {
    auto ret  = *this;
    ret.m_z_z = in_z_z;
    return ret;
  }
  /**
   * Camera Space Vector
   */
  constexpr auto
    with_space(graphics::Vertice<std::int32_t> in_space) const noexcept
  {
    auto ret    = *this;
    ret.m_space = in_space;
    return ret;
  }
  /**
   * Camera Pan
   */
  constexpr auto
    with_pan(graphics::Point<std::int16_t> in_pan) const noexcept
  {
    auto ret  = *this;
    ret.m_pan = in_pan;
    return ret;
  }
  /**
   * Zoom
   */
  constexpr auto
    with_zoom(std::int16_t in_zoom) const noexcept
  {
    auto ret   = *this;
    ret.m_zoom = in_zoom;
    return ret;
  }
  /**
   * Zoom (Repeated)
   */
  constexpr auto
    with_zoom2(std::int16_t in_zoom2) const noexcept
  {
    auto ret    = *this;
    ret.m_zoom2 = in_zoom2;
    return ret;
  }
  /**
   * Render Mode ('0x08','0x10','0x11' or '0x50')
   */
  constexpr auto
    with_render_mode(std::uint8_t in_render_mode) const noexcept
  {
    auto ret          = *this;
    ret.m_render_mode = in_render_mode;
    return ret;
  }
  /**
   * "END" marker 3 bytes
   */
  constexpr auto
    end(std::array<char, 3U> in_end) const noexcept
  {
    auto ret  = *this;
    ret.m_end = in_end;
    return ret;
  }
  /**
   * verify end() == "END"
   */
  [[nodiscard]] constexpr bool
    valid_end() const noexcept
  {
    return std::ranges::equal(m_end, CamFrame::EXPECTED_END);
  }
  friend std::ostream &
    operator<<(std::ostream &os, const CamFrame &cam_frame)
  {
    return os
        << '{' << cam_frame.m_x << ',' << cam_frame.m_y << ',' << cam_frame.m_z
        << ',' << cam_frame.m_z_z << ',' << cam_frame.m_space << ','
        << cam_frame.m_pan << ',' << cam_frame.m_zoom << ','
        << cam_frame.m_zoom2 << ',' << std::hex << std::uppercase << "0x"
        << (static_cast<uint16_t>(cam_frame.m_render_mode) & 0xFFU) << std::dec
        << std::nouppercase << ','
        << std::string_view(cam_frame.m_end.begin(), cam_frame.m_end.end())
        << '}';
  }
  constexpr auto
    midpoint(const CamFrame other) const noexcept
  {
    return with_x(m_x.midpoint(other.x()))
      .with_y(m_y.midpoint(other.y()))
      .with_z(m_z.midpoint(other.z()))
      .with_z_z(std::midpoint(m_z_z, other.z_z()))
      .with_space(m_space.midpoint(other.space()))
      .with_pan(m_pan.midpoint(other.pan()))
      .with_zoom(std::midpoint(m_zoom, other.zoom()))
      .with_zoom2(std::midpoint(m_zoom2, other.zoom2()));
    // m_render_mode has set values. We may want to choose based on min or max.
  }
};
static_assert(sizeof(CamFrame) == CamFrame::EXPECTED_SIZE);
static_assert(CamFrame().valid_end());
}// namespace open_viii::pak
#endif// VIIIARCHIVE_CAMFRAME_HPP
