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

#ifndef VIIIARCHIVE_BATTLEONLYSTATUSEST_HPP
#define VIIIARCHIVE_BATTLEONLYSTATUSEST_HPP
#include <cstdint>
namespace open_viii::kernel {
enum class BattleOnlyStatusesT : std::uint32_t {
  // https://github.com/DarkShinryu/doomtrain/wiki/Statuses-1
  none = 0x0,
  sleep [[maybe_unused]] = 0x1,
  haste [[maybe_unused]] = 0x2,
  slow [[maybe_unused]] = 0x4,
  stop [[maybe_unused]] = 0x8,
  regen [[maybe_unused]] = 0x10,
  protect [[maybe_unused]] = 0x20,
  shell [[maybe_unused]] = 0x40,
  reflect [[maybe_unused]] = 0x80,
  aura [[maybe_unused]] = 0x100,
  curse = 0x200,
  doom [[maybe_unused]] = 0x400,
  invincible [[maybe_unused]] = 0x800,
  petrifying [[maybe_unused]] = 0x1000,
  floating [[maybe_unused]] = 0x2000,
  confuse [[maybe_unused]] = 0x4000,
  drain [[maybe_unused]] = 0x8000,
  eject = 0x10000,
  double_cast [[maybe_unused]] = 0x20000,
  triple_cast [[maybe_unused]] = 0x40000,
  defend [[maybe_unused]] = 0x80000,
  unk0_x100000 [[maybe_unused]] = 0x100000,
  unk0_x200000 [[maybe_unused]] = 0x200000,
  charged = 0x400000,
  back_attack [[maybe_unused]] = 0x800000,
  vit0 = 0x1000000,
  angel_wing [[maybe_unused]] = 0x2000000,
  unk0_x4000000 [[maybe_unused]] = 0x4000000,
  unk0_x8000000 [[maybe_unused]] = 0x8000000,
  unk0_x10000000 [[maybe_unused]] = 0x10000000,
  unk0_x20000000 [[maybe_unused]] = 0x20000000,
  has_magic [[maybe_unused]] = 0x40000000,
  summon_gf [[maybe_unused]] = 0x80000000,
};
}
#endif// VIIIARCHIVE_BATTLEONLYSTATUSEST_HPP
