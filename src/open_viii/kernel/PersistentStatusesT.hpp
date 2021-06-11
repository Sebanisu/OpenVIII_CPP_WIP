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
#ifndef VIIIARCHIVE_PERSISTENTSTATUSEST_HPP
#define VIIIARCHIVE_PERSISTENTSTATUSEST_HPP
#include <cstdint>
namespace open_viii::kernel {
/**
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Statuses-0
 */
enum class PersistentStatusesT : std::uint16_t
{
  none       = 0x00,
  death      = 0x01,
  poison     = 0x02,
  petrify    = 0x04,
  darkness   = 0x08,
  silence    = 0x10,
  berserk    = 0x20,
  zombie     = 0x40,
  unk0_x0080 = 0x80,
  unk0_x0100 = 0x0100,
  unk0_x0200 = 0x0200,
  unk0_x0400 = 0x0400,
  unk0_x0800 = 0x0800,
  unk0_x1000 = 0x1000,
  unk0_x2000 = 0x2000,
  unk0_x4000 = 0x4000,
  unk0_x8000 = 0x8000,
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_PERSISTENTSTATUSEST_HPP
