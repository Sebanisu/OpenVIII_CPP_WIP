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

#ifndef VIIIARCHIVE_CHARACTERST_HPP
#define VIIIARCHIVE_CHARACTERST_HPP
#include <cstdint>
namespace open_viii::kernel {
enum class CharactersT : std::uint8_t {
  // I noticed some values were in order of these characters so I made those
  // values into arrays
  // and put the character names into an enum.
  squall_leonhart = 0x0,
  zell_dincht = 0x1,
  irvine_kinneas = 0x2,
  quistis_trepe = 0x3,
  rinoa_heartilly = 0x4,
  selphie_tilmitt = 0x5,
  seifer_almasy = 0x6,
  edea_kramer = 0x7,
  laguna_loire = 0x8,// = Squall_Leonhart, //Laguna always replaces squall
  kiros_seagill = 0x9,// unsure who kiros and ward replace. I think it's whom
                      // ever is with squall.
  ward_zabac = 0xA,
  blank = 0xFF,
};
}
#endif// VIIIARCHIVE_CHARACTERST_HPP
