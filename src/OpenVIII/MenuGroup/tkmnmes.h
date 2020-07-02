//
// Created by pcvii on 7/1/2020.
//

#ifndef VIIIARCHIVE_TKMNMES_H
#define VIIIARCHIVE_TKMNMES_H
#include <cstdint>
#include <array>
namespace OpenVIII::MenuGroup {
static constexpr std::uint16_t defaultPaddingCount_{16U};
std::uint16_t paddingCount_{defaultPaddingCount_};
std::array<std::uint16_t,defaultPaddingCount_> padding_{};

}
#endif// VIIIARCHIVE_TKMNMES_H
