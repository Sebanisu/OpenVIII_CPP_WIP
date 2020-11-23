//
// Created by pcvii on 11/18/2020.
//

#ifndef VIIIARCHIVE_CAMERAHEADER_HPP
#define VIIIARCHIVE_CAMERAHEADER_HPP
#include <cstdint>
namespace open_viii::battle::stage {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Camera_data
 */
struct CameraHeader
{
private:
  std::uint16_t m_offsets_count{};// should be 0x0200
  std::uint16_t m_offset_camera_settings{};
  std::uint16_t m_offset_camera_animation_collection{};
  std::uint16_t m_camera_data_size{};
};
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_CAMERAHEADER_HPP
