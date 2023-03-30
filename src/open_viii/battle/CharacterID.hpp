//
// Created by pcvii on 3/30/2023.
//

#ifndef OPENVIII_CPP_WIP_CHARACTERID_HPP
#define OPENVIII_CPP_WIP_CHARACTERID_HPP
#include <cstdint>
namespace open_viii {
enum class CharacterID : std::uint8_t
{
  SquallLeonhart = 0,
  ZellDincht     = 1,
  IrvineKinneas  = 2,
  QuistisTrepe   = 3,
  RinoaHeartilly = 4,
  SelphieTilmitt = 5,
  SeiferAlmasy   = 6,
  EdeaKramer     = 7,
  LagunaLoire    = 8,
  KirosSeagill   = 9,
  WardZabac      = 10,
  BlankCharacter = 0xFF
};
}
#endif// OPENVIII_CPP_WIP_CHARACTERID_HPP
