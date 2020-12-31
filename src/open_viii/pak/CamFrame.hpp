//
// Created by pcvii on 12/31/2020.
//

#ifndef VIIIARCHIVE_CAMFRAME_HPP
#define VIIIARCHIVE_CAMFRAME_HPP
#include "open_viii/battle/stage/Vertice.hpp"
#include "open_viii/graphics/Point.hpp"
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
  battle::stage::Vertice<std::int16_t> m_x{};
  /**
   * Camera Vector Y axis
   */
  battle::stage::Vertice<std::int16_t> m_y{};
  /**
   * Camera Vector Z axis
   */
  battle::stage::Vertice<std::int16_t> m_z{};
  /**
   * Camera Vector Z.z (copy)
   */
  std::int16_t m_z_z{};
  /**
   * Camera Space Vector
   */
  battle::stage::Vertice<std::int32_t> m_z{};
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
  static constexpr std::string_view EXPECTED_END{ "END" };
  constexpr CamFrame() = default;
  /**
   * Expected Size of struct in bytes
   */
  static constexpr auto EXPECTED_SIZE{ 44U };
  [[nodiscard]] constexpr auto end() const noexcept
  {
    return m_end;
  }
};
static_assert(sizeof(CamFrame) == CamFrame::EXPECTED_SIZE);
static_assert(std::ranges::equal(CamFrame().end(), CamFrame::EXPECTED_END));
}// namespace open_viii

#endif// VIIIARCHIVE_CAMFRAME_HPP
