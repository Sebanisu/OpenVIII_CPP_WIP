//
// Created by pcvii on 11/18/2020.
//
#ifndef VIIIARCHIVE_CAMERASETTINGS_HPP
#define VIIIARCHIVE_CAMERASETTINGS_HPP
#include <array>
#include <cstdint>
#include <ostream>
namespace open_viii::battle::stage {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Camera_Setting
 */
struct CameraSettings
{
  static constexpr auto      UNK_SIZE = 24U;
  std::array<char, UNK_SIZE> m_unk{};
};
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
