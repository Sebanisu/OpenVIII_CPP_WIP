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

#ifndef VIIIARCHIVE_JUNCTIONFLAGST_H
#define VIIIARCHIVE_JUNCTIONFLAGST_H
#include <cstdint>

namespace open_viii::kernel {
enum class JunctionFlagsT : std::uint32_t// cannot contain all abilities because
{
  none = 0x0,
  hp = 0x1,
  str = 0x2,
  vit = 0x4,
  mag = 0x8,
  spr = 0x10,
  spd = 0x20,
  eva = 0x40,
  hit = 0x80,
  luck = 0x100,
  elem_atk = 0x200,
  st_atk = 0x400,
  elem_def = 0x800,
  st_def = 0x1000,
  elem_def2 = 0x2000,
  elem_def4 = 0x4000,
  st_def2 = 0x8000,
  st_def4 = 0x10000,
  ability3 = 0x20000,
  ability4 = 0x40000,
};
}
#endif// VIIIARCHIVE_JUNCTIONFLAGST_H
