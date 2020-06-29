//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_ATTACKTYPET_H
#define VIIIARCHIVE_ATTACKTYPET_H
#include <cstdint>
namespace OpenVIII::Kernel {
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
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_ATTACKTYPET_H
