//
// Created by pcvii on 11/18/2020.
//

#ifndef VIIIARCHIVE_CAMERAANIMATIONCOLLECTION_HPP
#define VIIIARCHIVE_CAMERAANIMATIONCOLLECTION_HPP
#include <cstdint>
#include <vector>
namespace open_viii::battle::stage {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Camera_Animations_Collection
 */
struct CameraAnimationCollection
{
private:
  std::uint16_t m_number_of_sets{};
  std::vector<std::uint16_t> m_offsets_to_sets{};
  std::uint16_t m_camera_end{};
};
}
#endif// VIIIARCHIVE_CAMERAANIMATIONCOLLECTION_HPP
