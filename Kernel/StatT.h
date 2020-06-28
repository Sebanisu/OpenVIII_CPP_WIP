//
// Created by pcvii on 6/26/2020.
//

#ifndef VIIIARCHIVE_STATT_H
#define VIIIARCHIVE_STATT_H
#include <cstdint>
namespace OpenVIII::Kernel {
enum class StatT : std::uint8_t {
  HP,
  STR,
  VIT,
  MAG,
  SPR,
  SPD,
  EVA,
  HIT,
  Luck,
  ElAtk,
  StAtk,
  ElDef1,
  ElDef2,
  ElDef3,
  ElDef4,
  StDef1,
  StDef2,
  StDef3,
  StDef4,
  None = 0xFF,
};
}
#endif// VIIIARCHIVE_STATT_H
