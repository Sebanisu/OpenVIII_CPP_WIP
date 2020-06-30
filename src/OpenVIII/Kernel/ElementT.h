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
namespace OpenVIII::Kernel {
enum class ElementT : std::uint8_t {
  NonElemental = 0x00,
  Fire = 0x01,
  Ice = 0x02,
  Thunder = 0x04,
  Earth = 0x08,
  Poison = 0x10,
  Wind = 0x20,
  Water = 0x40,
  Holy = 0x80,
};
}
#endif// VIIIARCHIVE_ELEMENTT_H
