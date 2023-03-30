//
// Created by pcvii on 3/30/2023.
//

#ifndef OPENVIII_CPP_WIP_CARDID_HPP
#define OPENVIII_CPP_WIP_CARDID_HPP
#include <cstdint>
namespace open_viii {
enum class CardID : std::uint8_t
{
  // Level 1
  Geezard,
  Funguar,
  Bite_Bug,
  Red_Bat,
  Blobra,
  Gayla,
  Gesper,
  Fastitocalon_F,
  Blood_Soul,
  Caterchipillar,
  Cockatrice,

  // Level 2
  Grat,
  Buel,
  Mesmerize,
  Glacial_Eye,
  Belhelmel,
  Thrustaevis,
  Anacondaur,
  Creeps,
  Grendel,
  Jelleye,
  Grand_Mantis,

  // Level 3
  Forbidden,
  Armadodo,
  Tri_Face,
  Fastitocalon,
  Snow_Lion,
  Ochu,
  SAM08G,
  Death_Claw,
  Cactuar,
  Tonberry,
  Abyss_Worm,

  // Level 4
  Turtapod,
  Vysage,
  T_Rexaur,
  Bomb,
  Blitz,
  Wendigo,
  Torama,
  Imp,
  Blue_Dragon,
  Adamantoise,
  Hexadragon,

  // Level 5
  Iron_Giant,
  Behemoth,
  Chimera,
  PuPu,
  Elastoid,
  GIM47N,
  Malboro,
  Ruby_Dragon,
  Elnoyle,
  Tonberry_King,
  Biggs_Wedge,

  // Boss Cards
  // Level 6
  Fujin_Raijin,
  Elvoret,
  X_ATM092,
  Granaldo,
  Gerogero,
  Iguion,
  Abadon,
  Trauma,
  Oilboyle,
  Shumi_Tribe,
  Krysta,

  // Level 7
  Propagator,
  Jumbo_Cactuar,
  Tri_Point,
  Gargantua,
  Mobile_Type_8,
  Sphinxara,
  Tiamat,
  BGH251F2,
  Red_Giant,
  Catoblepas,
  Ultima_Weapon,

  // Guardian Force cards
  // Level 8
  Chubby_Chocobo,
  Angelo,
  Gilgamesh,
  MiniMog,
  Chicobo,
  Quezacotl,
  Shiva,
  Ifrit,
  Siren,
  Sacred,
  Minotaur,

  // Level 9
  Carbuncle,
  Diablos,
  Leviathan,
  Odin,
  Pandemona,
  Cerberus,
  Alexander,
  Phoenix,
  Bahamut,
  Doomtrain,
  Eden,

  // Player cards
  // Level 10
  Ward,
  Kiros,
  Laguna,
  Selphie,
  Quistis,
  Irvine,
  Zell,
  Rinoa,
  Edea,
  Seifer,
  Squall,

  Card_Back,
  /// <summary>
  /// for using in battle if a enemy is immune the value will be 0xff
  /// </summary>
  Immune = 0xff,
  Fail   = 0xfe
};
}// namespace open_viii
#endif// OPENVIII_CPP_WIP_CARDID_HPP
