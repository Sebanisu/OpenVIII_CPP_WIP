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
#ifndef VIIIARCHIVE_TARGETT_HPP
#define VIIIARCHIVE_TARGETT_HPP
#include <cstdint>
namespace open_viii::kernel {
enum class TargetT : std::uint8_t {
  none          = 0x00,
  dead          = 0x01,
  unk02         = 0x02,
  ally          = 0x04,
  single_side   = 0x08,
  single_target = 0x10,
  unk20         = 0x20,
  enemy         = 0x40,
  unk80         = 0x80,
};
}
#endif// VIIIARCHIVE_TARGETT_HPP
