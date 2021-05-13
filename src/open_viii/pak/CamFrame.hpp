//
// Created by pcvii on 12/31/2020.
//
#ifndef VIIIARCHIVE_CAMFRAME_HPP
#define VIIIARCHIVE_CAMFRAME_HPP
#include "open_viii/graphics/Point.hpp"
#include "open_viii/graphics/Vertice.hpp"
#include <array>
namespace open_viii {
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
  std::int16_t m_z_z{};
  /**
   * Camera Space Vector
   */
  graphics::Vertice<std::int32_t> m_space{};
  /**
   * Camera Pan
   */
  graphics::Point<std::int16_t> m_pan{};
  /**
   * Zoom
   */
  std::int16_t m_zoom{};
  /**
   * Zoom (Repeated)
   */
  std::int16_t m_zoom2{};
  /**
   * Render Mode ('0x08','0x10','0x11' or '0x50')
   */
  std::uint8_t m_render_mode{};
  /**
   * "END" marker 3 bytes
   */
  std::array<char, 3U> m_end{ 'E', 'N', 'D' };

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
  [[nodiscard]] constexpr auto x() const noexcept
  {
    return m_x;
  }
  /**
   * Camera Vector Y axis
   */
  [[nodiscard]] constexpr auto y() const noexcept
  {
    return m_y;
  }
  /**
   * Camera Vector Z axis
   */
  [[nodiscard]] constexpr auto z() const noexcept
  {
    return m_z;
  }
  /**
   * Camera Vector Z.z (copy)
   */
  [[nodiscard]] constexpr auto z_z() const noexcept
  {
    return m_z_z;
  }
  /**
   * Camera Space Vector
   */
  [[nodiscard]] constexpr auto space() const noexcept
  {
    return m_space;
  }
  /**
   * Camera Pan
   */
  [[nodiscard]] constexpr auto pan() const noexcept
  {
    return m_pan;
  }
  /**
   * Zoom
   */
  [[nodiscard]] constexpr auto zoom() const noexcept
  {
    return m_zoom;
  }
  /**
   * Zoom (Repeated)
   */
  [[nodiscard]] constexpr auto zoom2() const noexcept
  {
    return m_zoom2;
  }
  /**
   * Render Mode ('0x08','0x10','0x11' or '0x50')
   */
  [[nodiscard]] constexpr auto render_mode() const noexcept
  {
    return m_render_mode;
  }
  /**
   * "END" marker 3 bytes
   */
  [[nodiscard]] constexpr auto end() const noexcept
  {
    return m_end;
  }
  /**
   * Camera Vector X axis
   */
  void x(graphics::Vertice<std::int16_t> in_x) noexcept
  {
    m_x = in_x;
  }
  /**
   * Camera Vector Y axis
   */
  void y(graphics::Vertice<std::int16_t> in_y) noexcept
  {
    m_y = in_y;
  }
  /**
   * Camera Vector Z axis
   */
  void z(graphics::Vertice<std::int16_t> in_z) noexcept
  {
    m_z = in_z;
  }
  /**
   * Camera Vector Z.z (copy)
   */
  void z_z(std::int16_t in_z_z) noexcept
  {
    m_z_z = in_z_z;
  }
  /**
   * Camera Space Vector
   */
  void z(graphics::Vertice<std::int32_t> in_space) noexcept
  {
    m_space = in_space;
  }
  /**
   * Camera Pan
   */
  void pan(graphics::Point<std::int16_t> in_pan) noexcept
  {
    m_pan = in_pan;
  }
  /**
   * Zoom
   */
  void zoom(std::int16_t in_zoom) noexcept
  {
    m_zoom = in_zoom;
  }
  /**
   * Zoom (Repeated)
   */
  void zoom2(std::int16_t in_zoom2) noexcept
  {
    m_zoom2 = in_zoom2;
  }
  /**
   * Render Mode ('0x08','0x10','0x11' or '0x50')
   */
  void render_mode(std::uint8_t in_render_mode) noexcept
  {
    m_render_mode = in_render_mode;
  }
  /**
   * "END" marker 3 bytes
   */
  void end(std::array<char, 3U> in_end) noexcept
  {
    m_end = in_end;
  }
  /**
   * verify end() == "END"
   */
  [[nodiscard]] constexpr bool valid_end() const noexcept
  {
    return std::ranges::equal(m_end, CamFrame::EXPECTED_END);
  }
  friend std::ostream &operator<<(std::ostream &os, const CamFrame &cam_frame)
  {
    return os << '{' << cam_frame.m_x << ',' << cam_frame.m_y << ','
              << cam_frame.m_z << ',' << cam_frame.m_z_z << ','
              << cam_frame.m_space << ',' << cam_frame.m_pan << ','
              << cam_frame.m_zoom << ',' << cam_frame.m_zoom2 << ',' << std::hex
              << std::uppercase << "0x"
              << (static_cast<uint16_t>(cam_frame.m_render_mode) & 0xFFU)
              << std::dec << std::nouppercase << ','
              << std::string_view(cam_frame.m_end.begin(),
                                  cam_frame.m_end.end())
              << '}';
  }
};
static_assert(sizeof(CamFrame) == CamFrame::EXPECTED_SIZE);
static_assert(CamFrame().valid_end());
}// namespace open_viii
#endif// VIIIARCHIVE_CAMFRAME_HPP
