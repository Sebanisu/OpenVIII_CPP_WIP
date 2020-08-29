//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_SP2ENTRY_H
#define VIIIARCHIVE_SP2ENTRY_H
#include <cstdint>
namespace OpenVIII::Graphics {
struct sp2Entry
{
public:
  std::uint32_t count_{};
  std::uint8_t x_{};
  std::uint8_t y_{};
  std::uint8_t unknown1_{};
  std::uint8_t unknown2_{};
  std::uint16_t width_{};
  std::uint16_t height_{};
  std::uint8_t unknown3_{};
  std::uint8_t unknown4_{};
  std::uint8_t unknown5_{};
  std::uint8_t unknown6_{};
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_SP2ENTRY_H
