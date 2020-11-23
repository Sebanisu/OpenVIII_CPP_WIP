//
// Created by pcvii on 11/18/2020.
//

#ifndef VIIIARCHIVE_CAMERASETTINGS_HPP
#define VIIIARCHIVE_CAMERASETTINGS_HPP
#include <array>
#include <cstdint>
namespace open_viii::battle::stage {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Camera_Setting
 */
struct CameraSettings
{
  static constexpr auto UNK_SIZE = 24U;
  const std::array<char, UNK_SIZE> m_unk{};
};
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_CAMERASETTINGS_HPP
