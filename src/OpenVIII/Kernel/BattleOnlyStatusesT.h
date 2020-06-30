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

#ifndef VIIIARCHIVE_BATTLEONLYSTATUSEST_H
#define VIIIARCHIVE_BATTLEONLYSTATUSEST_H
#include <cstdint>
namespace OpenVIII::Kernel {
enum class BattleOnlyStatusesT : std::uint32_t {
  //https://github.com/DarkShinryu/doomtrain/wiki/Statuses-1
  None = 0x0,
  Sleep = 0x1,
  Haste = 0x2,
  Slow = 0x4,
  Stop = 0x8,
  Regen = 0x10,
  Protect = 0x20,
  Shell = 0x40,
  Reflect = 0x80,
  Aura = 0x100,
  Curse = 0x200,
  Doom = 0x400,
  Invincible = 0x800,
  Petrifying = 0x1000,
  Float = 0x2000,
  Confuse = 0x4000,
  Drain = 0x8000,
  Eject = 0x10000,
  Double = 0x20000,
  Triple = 0x40000,
  Defend = 0x80000,
  Unk0X100000 = 0x100000,
  Unk0X200000 = 0x200000,
  Charged = 0x400000,
  BackAttack = 0x800000,
  Vit0 = 0x1000000,
  AngelWing = 0x2000000,
  Unk0X4000000 = 0x4000000,
  Unk0X8000000 = 0x8000000,
  Unk0X10000000 = 0x10000000,
  Unk0X20000000 = 0x20000000,
  HasMagic = 0x40000000,
  SummonGF = 0x80000000,
};
}
#endif// VIIIARCHIVE_BATTLEONLYSTATUSEST_H
