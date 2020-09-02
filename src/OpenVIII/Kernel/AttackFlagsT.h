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

#ifndef VIIIARCHIVE_ATTACKFLAGST_H
#define VIIIARCHIVE_ATTACKFLAGST_H

#include <cstdint>
namespace open_viii::kernel {
enum class AttackFlagsT : std::uint8_t {
  None = 0x0,
  Shelled = 0x1,
  Unk0X2 = 0x2,
  Unk0X4 = 0x4,
  BreakDamageLimit = 0x8,
  Reflected = 0x10,
  Unk0X20 = 0x20,
  Unk0X40 = 0x40,
  Revive = 0x80
};
}
#endif// VIIIARCHIVE_ATTACKFLAGST_H
