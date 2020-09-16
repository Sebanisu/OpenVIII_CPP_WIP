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

#ifndef VIIIARCHIVE_ATTACKTYPET_HPP
#define VIIIARCHIVE_ATTACKTYPET_HPP
#include <cstdint>
namespace open_viii::kernel {
// https://github.com/alexfilth/doomtrain/blob/master/Doomtrain/Resources/Attack_Type_List.txt"/>
enum class AttackTypeT : std::uint8_t {
  none,
  physical_attack [[maybe_unused]],
  magic_attack [[maybe_unused]],
  curative_magic [[maybe_unused]],
  curative_item [[maybe_unused]],
  revive,
  revive_at_full_hp [[maybe_unused]],
  physical_damage [[maybe_unused]],
  magic_damage [[maybe_unused]],
  renzokuken_finisher [[maybe_unused]],
  squall_gunblade_attack [[maybe_unused]],
  gf [[maybe_unused]],
  scan,
  lv_down [[maybe_unused]],
  summon_item [[maybe_unused]],
  gf_ignore_target_spr [[maybe_unused]],
  lv_up [[maybe_unused]],
  card,
  kamikaze [[maybe_unused]],
  devour [[maybe_unused]],
  gf_damage [[maybe_unused]],
  unknown1,
  magic_attack_ignore_target_spr [[maybe_unused]],
  angelo_search [[maybe_unused]],
  moogle_dance [[maybe_unused]],
  white_wind_quistis [[maybe_unused]],
  lv_attack [[maybe_unused]],
  fixed_damage [[maybe_unused]],
  target_current_h_p1 [[maybe_unused]],
  fixed_magic_damage_based_on_gf_level [[maybe_unused]],
  unknown2,
  unknown3 [[maybe_unused]],
  give_percentage_hp [[maybe_unused]],
  unknown4,
  everyone_grudge [[maybe_unused]],
  one_hp_damage [[maybe_unused]],
  physical_attack_ignore_target_vit [[maybe_unused]]
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_ATTACKTYPET_HPP
