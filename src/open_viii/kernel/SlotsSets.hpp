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
#ifndef VIIIARCHIVE_SLOTSSETS_HPP
#define VIIIARCHIVE_SLOTSSETS_HPP
#include "open_viii/strings/EncodedStringOffset.hpp"
#include "Slot.hpp"
#include <array>
#include <cstdint>
namespace open_viii::kernel {
/**
 * array of 16 bytes 8 total 2 bytes per Magic Slot { Magic ID and Count }
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Selphie-limit-break-sets
 */
using SlotsSets = std::array<Slot, 8U>;
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_SLOTSSETS_HPP