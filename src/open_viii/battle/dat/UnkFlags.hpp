//
// Created by pcvii on 4/3/2023.
//

#ifndef OPENVIII_CPP_WIP_UNKFLAGS_HPP
#define OPENVIII_CPP_WIP_UNKFLAGS_HPP
#include <cstdint>
namespace open_viii::battle {
/**
 * @brief Enumeration of unknown flags.
 */
enum class UnkFlags : std::uint8_t
{
  None = 0,   ///< No flag.
  Unk0 = 0x1, ///< Unknown flag 0.
  Unk1 = 0x2, ///< Unknown flag 1.
  Unk2 = 0x4, ///< Unknown flag 2.
  Unk3 = 0x8, ///< Unknown flag 3.
  Unk4 = 0x10,///< Unknown flag 4.
  Unk5 = 0x20,///< Unknown flag 5.
  Unk6 = 0x40,///< Unknown flag 6.
  Unk7 = 0x80,///< Unknown flag 7.
};
}// namespace open_viii::battle
#endif// OPENVIII_CPP_WIP_UNKFLAGS_HPP
