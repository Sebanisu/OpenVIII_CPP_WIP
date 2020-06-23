//
// Created by pcvii on 6/22/2020.
//

#ifndef VIIIARCHIVE_JUNCTIONFLAGST_H
#define VIIIARCHIVE_JUNCTIONFLAGST_H
#include <cstdint>
enum class JunctionFlagsT : std::uint32_t // cannot contain all abilities because 
{
  None = 0x0,
  HP = 0x1,
  Str = 0x2,
  Vit = 0x4,
  Mag = 0x8,
  Spr = 0x10,
  Spd = 0x20,
  Eva = 0x40,
  Hit = 0x80,
  Luck = 0x100,
  ElemAtk = 0x200,
  StAtk = 0x400,
  ElemDef = 0x800,
  StDef = 0x1000,
  ElemDef2 = 0x2000,
  ElemDef4 = 0x4000,
  StDef2 = 0x8000,
  StDef4 = 0x10000,
  Ability3 = 0x20000,
  Ability4 = 0x40000,
};
#endif// VIIIARCHIVE_JUNCTIONFLAGST_H
