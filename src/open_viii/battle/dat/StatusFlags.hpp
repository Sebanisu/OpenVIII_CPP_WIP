//
// Created by pcvii on 4/3/2023.
//

#ifndef OPENVIII_CPP_WIP_STATUSFLAGS_HPP
#define OPENVIII_CPP_WIP_STATUSFLAGS_HPP
#include <cstdint>
namespace open_viii::battle {
/**
   * @brief Enumeration of status flags for a character.
 */
enum class StatusFlags : std::uint8_t
{
  None        = 0,   ///< No status flag.
  Zombie      = 0x1, ///< Zombie status flag.
  Fly         = 0x2, ///< Fly status flag.
  Zz1         = 0x4, ///< Unused status flag 1.
  Zz2         = 0x8, ///< Unused status flag 2.
  Zz3         = 0x10,///< Unused status flag 3.
  AutoReflect = 0x20,///< Auto-Reflect status flag.
  AutoShell   = 0x40,///< Auto-Shell status flag.
  AutoProtect = 0x80,///< Auto-Protect status flag.
};
}
#endif// OPENVIII_CPP_WIP_STATUSFLAGS_HPP
