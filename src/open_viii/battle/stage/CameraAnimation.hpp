//
// Created by pcvii on 11/18/2020.
//

#ifndef VIIIARCHIVE_CAMERAANIMATION_HPP
#define VIIIARCHIVE_CAMERAANIMATION_HPP
#include <array>
#include <cstdint>
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
};
#endif// VIIIARCHIVE_CAMERAANIMATION_HPP
