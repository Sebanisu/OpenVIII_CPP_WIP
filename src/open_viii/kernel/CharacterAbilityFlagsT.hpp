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

#ifndef VIIIARCHIVE_CHARACTERABILITYFLAGST_HPP
#define VIIIARCHIVE_CHARACTERABILITYFLAGST_HPP
#include <cstdint>
namespace open_viii::kernel {
enum class CharacterAbilityFlagsT : std::uint32_t {
  none = 0x0,
  mug = 0x1,
  med_data [[maybe_unused]] = 0x2,
  counter [[maybe_unused]] = 0x4,
  return_damage [[maybe_unused]] = 0x8,
  cover = 0x10,
  initiative [[maybe_unused]] = 0x10000,
  move_hp_up = 0x20000,
  hp_bonus [[maybe_unused]] = 0x80,
  str_bonus [[maybe_unused]] = 0x100,
  vit_bonus [[maybe_unused]] = 0x200,
  mag_bonus [[maybe_unused]] = 0x400,
  spr_bonus [[maybe_unused]] = 0x800,
  auto_protect = 0x4000,
  auto_shell [[maybe_unused]] = 0x2000,
  auto_reflect = 0x1000,
  auto_haste = 0x8000,
  auto_potion [[maybe_unused]] = 0x40000,
  expend2 [[maybe_unused]] = 0x20,
  expend3 [[maybe_unused]] = 0x40,
  ribbon = 0x80000,
};
}
#endif// VIIIARCHIVE_CHARACTERABILITYFLAGST_HPP
