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
namespace open_viii::kernel {
/**
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Statuses-1
 */
enum class BattleOnlyStatusesT : std::uint32_t
{
  none           = 0x0,
  sleep          = 0x1,
  haste          = 0x2,
  slow           = 0x4,
  stop           = 0x8,
  regen          = 0x10,
  protect        = 0x20,
  shell          = 0x40,
  reflect        = 0x80,
  aura           = 0x100,
  curse          = 0x200,
  doom           = 0x400,
  invincible     = 0x800,
  petrifying     = 0x1000,
  floating       = 0x2000,
  confuse        = 0x4000,
  drain          = 0x8000,
  eject          = 0x10000,
  double_cast    = 0x20000,
  triple_cast    = 0x40000,
  defend         = 0x80000,
  unk0_x100000   = 0x100000,
  unk0_x200000   = 0x200000,
  charged        = 0x400000,
  back_attack    = 0x800000,
  vit0           = 0x1000000,
  angel_wing     = 0x2000000,
  unk0_x4000000  = 0x4000000,
  unk0_x8000000  = 0x8000000,
  unk0_x10000000 = 0x10000000,
  unk0_x20000000 = 0x20000000,
  has_magic      = 0x40000000,
  summon_gf      = 0x80000000,
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_BATTLEONLYSTATUSEST_HPP
