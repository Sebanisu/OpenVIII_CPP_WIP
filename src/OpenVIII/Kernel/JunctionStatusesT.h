// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef VIIIARCHIVE_JUNCTIONSTATUSEST_H
#define VIIIARCHIVE_JUNCTIONSTATUSEST_H

#include <cstdint>
namespace open_viii::Kernel {
enum class JunctionStatusesT : std::uint16_t {
  // https://github.com/DarkShinryu/doomtrain/wiki/Characters-J-Statuses
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
