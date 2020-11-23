//
// Created by pcvii on 11/18/2020.
//

#ifndef VIIIARCHIVE_CAMERAANIMATION_HPP
#define VIIIARCHIVE_CAMERAANIMATION_HPP
#include "Vertice.hpp"
#include <array>
#include <cstdint>
namespace open_viii::battle::stage {
/**
 * http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Camera_Animation_.28WIP.29
 */
struct CameraAnimation
{
private:
  static constexpr auto MAX_FRAMES = 32U;
  static constexpr auto UNK_SIZE_0 = 20U;
  static constexpr auto UNK_SIZE_2 = 128U;
  static constexpr auto UNK_SIZE_3 = 34U;
  std::uint8_t m_animation_id{};
  std::uint8_t m_key_frame_count{};
  std::uint16_t m_main_controller{};//, if 0xFFFF then return
  std::uint16_t m_starting_fov_usually{};//~280
  std::uint16_t m_ending_fov{};// ~006
  std::uint16_t m_starting_camera_roll{};// usually 0
  std::uint16_t m_starting_time{};// usually 0
  std::uint16_t m_current_time{};// I'm questioning if this is part of the struct.
  std::array<std::uint8_t, UNK_SIZE_0> m_unknown_bytes_0{};
  std::array<std::uint16_t, MAX_FRAMES> m_start_frame_offsets{};
  std::array<std::int16_t, MAX_FRAMES> m_camera_world_z{};
  std::array<std::int16_t, MAX_FRAMES> m_camera_world_x{};
  std::array<std::int16_t, MAX_FRAMES> m_camera_world_y{};
  std::array<std::uint8_t, MAX_FRAMES> m_is_frame_durations_shot{};// ~ bool?
  std::array<std::int16_t, MAX_FRAMES> m_camera_look_at_z{};
  std::array<std::int16_t, MAX_FRAMES> m_camera_look_at_x{};
  std::array<std::int16_t, MAX_FRAMES> m_camera_look_at_y{};
  std::array<std::uint8_t, MAX_FRAMES> m_is_frame_ending_shots{};// ~ bool?
  std::array<std::uint8_t, UNK_SIZE_2> m_unknown_bytes_1{};
  std::array<std::uint8_t, UNK_SIZE_2> m_unknown_bytes_2{};
  std::array<std::uint8_t, UNK_SIZE_2> m_unknown_bytes_3{};
  std::array<std::uint8_t, UNK_SIZE_2> m_unknown_bytes_4{};
  std::array<std::uint8_t, UNK_SIZE_3> m_unknown_bytes_5{};

public:
  template<size_t I> requires(I < MAX_FRAMES) [[nodiscard]] Vertice<std::int16_t> get_camera_world() const noexcept
  {
    return { static_cast<std::int16_t>(m_camera_world_x[I]),
      static_cast<std::int16_t>(-m_camera_world_y[I]),
      static_cast<std::int16_t>(-m_camera_world_z[I]) };
  }
  template<size_t I> requires(I < MAX_FRAMES) [[nodiscard]] Vertice<std::int16_t> get_camera_look_at() const noexcept
  {
    return { static_cast<std::int16_t>(m_camera_look_at_x[I]),
      static_cast<std::int16_t>(-m_camera_look_at_y[I]),
      static_cast<std::int16_t>(-m_camera_look_at_z[I]) };
  }
  template<size_t I> requires(I < MAX_FRAMES) [[nodiscard]] bool is_frame_durations_shot() const noexcept
  {
    return m_is_frame_durations_shot[I] != 0;
  }
  template<size_t I> requires(I < MAX_FRAMES) [[nodiscard]] bool is_frame_ending_shots() const noexcept
  {
    return m_is_frame_ending_shots[I] != 0;
  }
  template<size_t I> requires(I < MAX_FRAMES) [[nodiscard]] std::uint16_t start_frame_offsets() const noexcept
  {
    return m_start_frame_offsets[I];
  }
  [[nodiscard]] const std::uint8_t &animation_id() const noexcept
  {
    return m_animation_id;
  }
  [[nodiscard]] const std::uint8_t &key_frame_count() const noexcept
  {
    return m_key_frame_count;
  }
  [[nodiscard]] const std::uint16_t &main_controller() const noexcept
  {
    return m_main_controller;//, if 0xFFFF then return
  }
  [[nodiscard]] const std::uint16_t &starting_fov_usually() const noexcept
  {
    return m_starting_fov_usually;//~280
  }
  [[nodiscard]] const std::uint16_t &ending_fov() const noexcept
  {
    return m_ending_fov;// ~006
  }
  [[nodiscard]] const std::uint16_t &starting_camera_roll() const noexcept
  {
    return m_starting_camera_roll;// usually 0
  }
  [[nodiscard]] const std::uint16_t &starting_time() const noexcept
  {
    return m_starting_time;// usually 0
  }
  [[nodiscard]] const std::uint16_t &current_time() const noexcept
  {
    return m_current_time;// I'm questioning if this is part of the struct.
  };
  static constexpr auto EXPECTED_SIZE = 1092U;
};
static_assert(sizeof(CameraAnimation) == CameraAnimation::EXPECTED_SIZE);
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_CAMERAANIMATION_HPP
