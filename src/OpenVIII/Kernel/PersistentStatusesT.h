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

#ifndef VIIIARCHIVE_PERSISTENTSTATUSEST_H
#define VIIIARCHIVE_PERSISTENTSTATUSEST_H
#include <cstdint>
namespace open_viii::kernel {
enum class PersistentStatusesT : std::uint16_t {
  // https://github.com/DarkShinryu/doomtrain/wiki/Statuses-0
  None = 0x00,
  Death = 0x01,
  Poison = 0x02,
  Petrify = 0x04,
  Darkness = 0x08,
  Silence = 0x10,
  Berserk = 0x20,
  Zombie = 0x40,
  Unk0X0080 = 0x80,
  Unk0X0100 = 0x0100,
  Unk0X0200 = 0x0200,
  Unk0X0400 = 0x0400,
  Unk0X0800 = 0x0800,
  Unk0X1000 = 0x1000,
  Unk0X2000 = 0x2000,
  Unk0X4000 = 0x4000,
  Unk0X8000 = 0x8000,
};
}
#endif// VIIIARCHIVE_PERSISTENTSTATUSEST_H
