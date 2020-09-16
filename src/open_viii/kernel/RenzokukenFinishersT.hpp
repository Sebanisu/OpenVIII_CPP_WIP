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

#ifndef VIIIARCHIVE_RENZOKUKENFINISHERST_HPP
#define VIIIARCHIVE_RENZOKUKENFINISHERST_HPP
#include <cstdint>
namespace open_viii::kernel {
enum class RenzokukenFinishersT : std::uint8_t {
  none = 0x00,
  rough_divide = 0x01,
  fated_circle = 0x02,
  blasting_zone = 0x04,
  lion_heart = 0x08,
};
}
#endif// VIIIARCHIVE_RENZOKUKENFINISHERST_HPP
