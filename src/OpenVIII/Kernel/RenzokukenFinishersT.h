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

#ifndef VIIIARCHIVE_RENZOKUKENFINISHERST_H
#define VIIIARCHIVE_RENZOKUKENFINISHERST_H
#include <cstdint>
namespace open_viii::kernel {
enum class RenzokukenFinishersT : std::uint8_t {
  None = 0x00,
  RoughDivide = 0x01,
  FatedCircle = 0x02,
  BlastingZone = 0x04,
  LionHeart = 0x08,
};
}
#endif// VIIIARCHIVE_RENZOKUKENFINISHERST_H
