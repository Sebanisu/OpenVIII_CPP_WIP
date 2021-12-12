//
// Created by pcvii on 12/22/2020.
//
#ifndef VIIIARCHIVE_CAMERAANIMATIONFRAME_HPP
#define VIIIARCHIVE_CAMERAANIMATIONFRAME_HPP
#include "open_viii/graphics/Vertice.hpp"
namespace open_viii::battle::stage {
struct CameraAnimationFrame
{
private:
  std::uint16_t                    m_is_frame_durations_shot{};
  graphics::Vertice<std::uint16_t> m_world{};
  std::uint16_t                    m_is_frame_ending_shot{};
  graphics::Vertice<std::uint16_t> m_look_at{};

public:
  constexpr CameraAnimationFrame() = default;
  constexpr CameraAnimationFrame(
    std::uint16_t                    is_frame_durations_shot,
    graphics::Vertice<std::uint16_t> world,
    std::uint16_t                    is_frame_ending_shots,
    graphics::Vertice<std::uint16_t> look_at)
    : m_is_frame_durations_shot(is_frame_durations_shot), m_world(world),
      m_is_frame_ending_shot(is_frame_ending_shots), m_look_at(look_at)
  {}
  [[nodiscard]] constexpr auto
    is_frame_durations_shot() const noexcept
  {
    return m_is_frame_durations_shot;
  }
  [[nodiscard]] constexpr auto
    world() const noexcept
  {
    return m_world;
  }
  [[nodiscard]] constexpr auto
    is_frame_ending_shots() const noexcept
  {
    return m_is_frame_ending_shot;
  }
  [[nodiscard]] constexpr auto
    look_at() const noexcept
  {
    return m_look_at;
  }
  constexpr static auto EXPECTED_SIZE = 16U;
};
static_assert(
  sizeof(CameraAnimationFrame) == CameraAnimationFrame::EXPECTED_SIZE);
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_CAMERAANIMATIONFRAME_HPP
