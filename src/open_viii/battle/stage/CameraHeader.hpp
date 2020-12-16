//
// Created by pcvii on 11/18/2020.
//

#ifndef VIIIARCHIVE_CAMERAHEADER_HPP
#define VIIIARCHIVE_CAMERAHEADER_HPP
#include <cstdint>
#include <ostream>
#include <span>
namespace open_viii::battle::stage {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Camera_data
 */
struct CameraHeader
{
private:
  std::uint16_t m_offset_count{};
  std::uint16_t m_offset_camera_settings{};
  std::uint16_t m_offset_camera_animation_collection{};
  std::uint16_t m_camera_data_size{};
  static constexpr std::uint16_t EXPECTED_OFFSET_COUNT{ 0x2U };
  static constexpr std::uint16_t EXPECTED_OFFSET_CAMERA_SETTINGS{ 0x8U };
  static constexpr std::uint16_t EXPECTED_OFFSET_CAMERA_ANIMATION_COLLECTION{ 0x8U };

public:
  [[nodiscard]] bool constexpr check() const noexcept
  {
    return m_offset_count == EXPECTED_OFFSET_COUNT && m_offset_camera_settings == EXPECTED_OFFSET_CAMERA_SETTINGS
           && m_offset_camera_animation_collection == EXPECTED_OFFSET_CAMERA_ANIMATION_COLLECTION;
  }
  [[nodiscard]] constexpr std::uint16_t offset_count() const noexcept
  {
    return m_offset_count;
  }// should be
  [[nodiscard]] constexpr std::uint16_t offset_camera_settings() const noexcept
  {
    return m_offset_camera_settings;
  }
  [[nodiscard]] constexpr std::uint16_t offset_camera_animation_collection() const noexcept
  {
    return m_offset_camera_animation_collection;
  }
  [[nodiscard]] constexpr std::uint16_t camera_data_size() const noexcept
  {
    return m_camera_data_size;
  }
  friend std::ostream &operator<<(std::ostream &os, const CameraHeader &in)
  {
    return os << "{\n\t\t\tOFFSET COUNT: " << in.m_offset_count << "\n\t\t\tSETTINGS OFFSET: 0x" << std::hex
              << std::uppercase << in.m_offset_camera_settings << std::dec << std::nouppercase
              << "\n\t\t\tCOLLECTION OFFSET: 0x" << std::hex << std::uppercase
              << in.m_offset_camera_animation_collection << std::dec << std::nouppercase
              << "\n\t\t\tDATA SIZE: " << in.m_camera_data_size << "}";
  }
};
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_CAMERAHEADER_HPP
