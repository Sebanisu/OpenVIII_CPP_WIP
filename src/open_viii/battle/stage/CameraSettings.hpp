//
// Created by pcvii on 11/18/2020.
//
#ifndef VIIIARCHIVE_CAMERASETTINGS_HPP
#define VIIIARCHIVE_CAMERASETTINGS_HPP
namespace open_viii::battle::stage {
/**
 * @brief Represents the battle camera settings used for a stage, including
 * geometry.
 * @details The camera settings are a series of unknown flags and variables
 * (about 24 bytes).
 * @see
 * https://github.com/MaKiPL/OpenVIII-monogame/blob/master/Core/Battle/Camera/BattleCameraSettings.cs
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Camera_Setting
 */
struct CameraSettings
{

  /**
   * @brief The size of the unknown data in the camera settings.
   */
  static constexpr auto      UNK_SIZE = 24U;

  /**
   * @brief An array containing the unknown data of the camera settings.
   */
  std::array<char, UNK_SIZE> m_unk{};
};

/**
 * @brief Output stream operator for CameraSettings.
 * @param os The output stream to write the CameraSettings to.
 * @param in The CameraSettings to write to the output stream.
 * @return A reference to the output stream.
 */
inline std::ostream &
  operator<<(std::ostream &os, const CameraSettings &in)
{
  os << "{0x" << std::hex << std::uppercase;
  std::ranges::for_each(in.m_unk, [&os](const char &c) {
    os << static_cast<std::uint16_t>(c);
  });
  return os << std::dec << std::nouppercase << "}";
}
static_assert(sizeof(CameraSettings) == CameraSettings::UNK_SIZE);
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_CAMERASETTINGS_HPP
