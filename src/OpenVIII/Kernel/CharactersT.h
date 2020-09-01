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

#ifndef VIIIARCHIVE_CHARACTERST_H
#define VIIIARCHIVE_CHARACTERST_H
#include <cstdint>
namespace open_viii::Kernel {
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
