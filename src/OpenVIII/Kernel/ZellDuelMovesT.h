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

#ifndef VIIIARCHIVE_ZELLDUELMOVEST_H
#define VIIIARCHIVE_ZELLDUELMOVEST_H

namespace open_viii::kernel {
enum struct ZellDuelMoveT : std::uint8_t {
  // https://github.com/DarkShinryu/doomtrain/wiki/Zell's-Duel-Moves
  // Offset	Description
  //  0x00	Punch Rush
  //  0x01	Booya
  //  0x02	Heel Drop
  //  0x03	Mach Kick
  //  0x04	Dolphin Blow
  //  0x05	Meteor Strike
  //  0x06	Burning Rave
  //  0x07	Meteor Barret
  //  0x08	Different Beat
  //  0x09	My Final Heaven

  punch_rush,
  booya,
  heel_drop,
  mach_kick,
  dolphin_blow,
  meteor_strike,
  burning_rave,
  meteor_barret,
  different_beat,
  my_final_heaven
};
}
#endif// VIIIARCHIVE_ZELLDUELMOVEST_H
