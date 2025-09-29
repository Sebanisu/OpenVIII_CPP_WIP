//
// Created by pcvii on 4/3/2023.
//

#ifndef OPENVIII_CPP_WIP_STATUSFLAGS2_HPP
#define OPENVIII_CPP_WIP_STATUSFLAGS2_HPP
#include <cstdint>
namespace open_viii::battle::dat {
/**
 * @brief Enumeration of additional status flags for a character.
 */
enum class StatusFlags2 : std::uint8_t
{
  None          = 0,   ///< No status flag.
  Zz1           = 0x1, ///< Unused status flag 1.
  Zz2           = 0x2, ///< Unused status flag 2.
  Unused1       = 0x4, ///< Unused status flag 3.
  Unused2       = 0x8, ///< Unused status flag 4.
  Unused3       = 0x10,///< Unused status flag 5.
  Unused4       = 0x20,///< Unused status flag 6.
  DiablosMissed = 0x40,///< Diablos missed status flag.
  AlwaysCard    = 0x80,///< Always card status flag.
};
}// namespace open_viii::battle::dat
#endif// OPENVIII_CPP_WIP_STATUSFLAGS2_HPP
