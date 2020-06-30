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

#ifndef VIIIARCHIVE_CHARACTERABILITYFLAGST_H
#define VIIIARCHIVE_CHARACTERABILITYFLAGST_H
#include <cstdint>
namespace OpenVIII::Kernel {
enum class CharacterAbilityFlagsT : std::uint32_t {
  None = 0x0,
  Mug = 0x1,
  MedData = 0x2,
  Counter = 0x4,
  ReturnDamage = 0x8,
  Cover = 0x10,
  Initiative = 0x10000,
  MoveHPUp = 0x20000,
  HPBonus = 0x80,
  StrBonus = 0x100,
  VitBonus = 0x200,
  MagBonus = 0x400,
  SprBonus = 0x800,
  AutoProtect = 0x4000,
  AutoShell = 0x2000,
  AutoReflect = 0x1000,
  AutoHaste = 0x8000,
  AutoPotion = 0x40000,
  Expend2 = 0x20,
  Expend3 = 0x40,
  Ribbon = 0x80000,
};
}
#endif// VIIIARCHIVE_CHARACTERABILITYFLAGST_H
