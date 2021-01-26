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
#ifndef VIIIARCHIVE_STATT_HPP
#define VIIIARCHIVE_STATT_HPP
#include <cstdint>
namespace open_viii::kernel {
enum class StatT : std::uint8_t {
  hp,
  str,
  vit,
  mag,
  spr,
  spd,
  eva,
  hit,
  luck,
  el_atk,
  st_atk,
  el_def1,
  el_def2,
  el_def3,
  el_def4,
  st_def1,
  st_def2,
  st_def3,
  st_def4,
  none = 0xFF,
};
}
#endif// VIIIARCHIVE_STATT_HPP
