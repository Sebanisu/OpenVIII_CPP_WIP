//
// Created by pcvii on 6/17/2020.
//

#ifndef VIIIARCHIVE_CHARACTERST_H
#define VIIIARCHIVE_CHARACTERST_H
#include <cstdint>
namespace OpenVIII::Kernel {
enum class CharactersT : std::uint8_t {
  // I noticed some values were in order of these characters so I made those values into arrays
  // and put the character names into an enum.
  Squall_Leonhart = 0x0,
  Zell_Dincht = 0x1,
  Irvine_Kinneas = 0x2,
  Quistis_Trepe = 0x3,
  Rinoa_Heartilly = 0x4,
  Selphie_Tilmitt = 0x5,
  Seifer_Almasy = 0x6,
  Edea_Kramer = 0x7,
  Laguna_Loire = 0x8,// = Squall_Leonhart, //Laguna always replaces squall
  Kiros_Seagill = 0x9,// unsure who kiros and ward replace. I think it's whom ever is with squall.
  Ward_Zabac = 0xA,
  Blank = 0xFF,
};
}
#endif// VIIIARCHIVE_CHARACTERST_H
