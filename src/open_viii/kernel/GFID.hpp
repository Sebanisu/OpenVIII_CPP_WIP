//
// Created by pcvii on 3/30/2023.
//

#ifndef OPENVIII_CPP_WIP_GFID_HPP
#define OPENVIII_CPP_WIP_GFID_HPP
#include <cstdint>
namespace open_viii::kernel {
enum class GFID : std::uint8_t
{
  Quezacotl,
  Shiva,
  Ifrit,
  Siren,
  Brothers,
  Diablos,
  Carbuncle,
  Leviathan,
  Pandemona,
  Cerberus,
  Alexander,
  Doomtrain,
  Bahamut,
  Cactuar,
  Tonberry,
  Eden,
  Count,
  Blank = 0xFE,
  All   = 0xFF,
};
}// namespace open_viii
#endif// OPENVIII_CPP_WIP_GFID_HPP
