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

#ifndef VIIIARCHIVE_ATTACKTYPET_H
#define VIIIARCHIVE_ATTACKTYPET_H
#include <cstdint>
namespace open_viii::Kernel {
// https://github.com/alexfilth/doomtrain/blob/master/Doomtrain/Resources/Attack_Type_List.txt"/>
enum class AttackTypeT : std::uint8_t {
  None,
  PhysicalAttack,
  MagicAttack,
  CurativeMagic,
  CurativeItem,
  Revive,
  ReviveAtFullHP,
  PhysicalDamage,
  MagicDamage,
  RenzokukenFinisher,
  SquallGunbladeAttack,
  GF,
  Scan,
  LvDown,
  SummonItem,
  GFIgnoreTargetSPR,
  LvUp,
  Card,
  Kamikaze,
  Devour,
  GFDamage,
  Unknown1,
  MagicAttackIgnoreTargetSPR,
  AngeloSearch,
  MoogleDance,
  WhiteWindQuistis,
  LvAttack,
  FixedDamage,
  TargetCurrentHP1,
  FixedMagicDamageBasedOnGFLevel,
  Unknown2,
  Unknown3,
  GivePercentageHP,
  Unknown4,
  EveryoneGrudge,
  OneHPDamage,
  PhysicalAttackIgnoreTargetVIT
};
}// namespace open_viii::Kernel
#endif// VIIIARCHIVE_ATTACKTYPET_H
