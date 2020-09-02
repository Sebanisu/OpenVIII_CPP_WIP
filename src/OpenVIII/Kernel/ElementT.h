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

#ifndef VIIIARCHIVE_ELEMENTT_H
#define VIIIARCHIVE_ELEMENTT_H
#include <cstdint>
namespace open_viii::kernel {
enum class ElementT : std::uint8_t {
  non_elemental = 0x00,
  fire = 0x01,
  ice = 0x02,
  thunder = 0x04,
  earth = 0x08,
  poison = 0x10,
  wind = 0x20,
  water = 0x40,
  holy = 0x80,
};
}
#endif// VIIIARCHIVE_ELEMENTT_H
