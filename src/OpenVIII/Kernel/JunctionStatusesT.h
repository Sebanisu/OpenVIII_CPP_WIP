//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_JUNCTIONSTATUSEST_H
#define VIIIARCHIVE_JUNCTIONSTATUSEST_H

#include <cstdint>
namespace OpenVIII::Kernel {
enum class JunctionStatusesT : std::uint16_t {
  //https://github.com/DarkShinryu/doomtrain/wiki/Characters-J-Statuses
  None = 0x0,
  Death = 0x1,
  Poison = 0x2,
  Petrify = 0x4,
  Darkness = 0x8,
  Silence = 0x10,
  Berserk = 0x20,
  Zombie = 0x40,
  Sleep = 0x80,
  Slow = 0x100,
  Stop = 0x200,
  // Curse; unused for attack
  Curse = 0x400,
  Confusion = 0x800,
  Drain = 0x1000,
};
}
#endif// VIIIARCHIVE_JUNCTIONSTATUSEST_H
