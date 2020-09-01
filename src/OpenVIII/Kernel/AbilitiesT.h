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

#ifndef VIIIARCHIVE_ABILITIEST_H
#define VIIIARCHIVE_ABILITIEST_H
#include <cstdint>
namespace open_viii::Kernel {
enum class AbilitiesT : std::uint16_t {
  // https://github.com/alexfilth/doomtrain/wiki/Junction-abilities
  None,
  // Enables HP junction
  HPJ,

  // Enables Strength junction

  StrJ,

  // Enables Vitality junction

  VitJ,

  // Enables Magic junction

  MagJ,

  // Enables Spirit junction

  SprJ,

  // Enables Speed junction

  SpdJ,

  // Enables Evasion junction

  EvaJ,

  // Enables Hit junction

  HitJ,

  // Enables Luck junction

  LuckJ,

  // Increased elemental attack slot Count to 1

  ElAtkJ,

  // Increased status attack slot Count to 1

  StAtkJ,

  // Increased elemental defense slot Count to 1

  ElDefJ,

  // Increased status defense slot Count to 1

  StDefJ,

  // Increased elemental defense slot Count to 2

  ElDefJ2,

  // Increased elemental defense slot Count to 4

  ElDefJ4,

  // Increased status defense slot Count to 2

  StDefJ2,

  // Increased status defense slot Count to 4

  StDefJ4,

  // Increases ability slot Count to 3

  Ability3,

  // Increases ability slot Count to 4

  Ability4,

  // Equippable commands start here
  // https://github.com/alexfilth/doomtrain/wiki/Command-abilities
  Magic,
  GF,
  Draw,
  Item,
  Empty,
  Card,
  Doom,
  MadRush,
  Treatment,
  Defend,
  Darkside,
  Recover,
  Absorb,
  Revive,
  LvDown,
  LvUp,
  Kamikaze,
  Devour,
  MiniMog,
  // Equippable commands end here

  // Equippable abilities start here
  // https://github.com/alexfilth/doomtrain/wiki/Stat-percentage-increasing-abilities
  HP20,
  HP40,
  HP80,
  Str20,
  Str40,
  Str60,
  Vit20,
  Vit40,
  Vit60,
  Mag20,
  Mag40,
  Mag60,
  Spr20,
  Spr40,
  Spr60,
  Spd20,
  Spd40,
  Eva30,
  Luck50,

  // https://github.com/alexfilth/doomtrain/wiki/Character-abilities
  Mug,
  MedData,
  Counter,
  ReturnDamage,
  Cover,
  Initiative,
  MoveHPUp,
  HPBonus,
  StrBonus,
  VitBonus,
  MagBonus,
  SprBonus,
  AutoProtect,
  AutoShell,
  AutoReflect,
  AutoHaste,
  AutoPotion,
  Expend2,
  Expend3,
  Ribbon,

  // https://github.com/alexfilth/doomtrain/wiki/Party-abilities
  Alert,
  MoveFind,
  EncHalf,
  EncNone,
  RareItem,

  // https://github.com/alexfilth/doomtrain/wiki/GF-abilities
  SumMag10,
  SumMag20,
  SumMag30,
  SumMag40,
  GFHP10,
  GFHP20,
  GFHP30,
  GFHP40,
  Boost,
  // Equipable abilities end here

  // https://github.com/alexfilth/doomtrain/wiki/Menu-abilities
  Haggle,
  SellHigh,
  Familiar,
  CallShop,
  JunkShop,
  ThunderMagRF,
  IceMagRF,
  FireMagRF,
  LifeMagRF,
  TimeMagRF,
  StatusMagRF,
  SuptMagRF,
  ForbidMagRF,
  RecoveryMedRF,
  StatusMedRF,
  AmmoRF,
  ToolRF,
  ForbidMedRF,
  GFRecoveryMedRF,
  GFAblMedRF,
  MidMagRF,
  HighMagRF,
  MedLvUp,
  CardMod,
};
}
#endif// VIIIARCHIVE_ABILITIEST_H
