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

#ifndef VIIIARCHIVE_BLENDMODET_HPP
#define VIIIARCHIVE_BLENDMODET_HPP
#include <cstdint>
namespace open_viii::graphics::background {
enum struct BlendModeT : std::uint8_t {
  half_add [[maybe_unused]],
  add [[maybe_unused]],
  subtract [[maybe_unused]],
  quarter_add [[maybe_unused]],
  none [[maybe_unused]],
};
}
#endif// VIIIARCHIVE_BLENDMODET_HPP
