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

#ifndef VIIIARCHIVE_ABILITIEST_HPP
#define VIIIARCHIVE_ABILITIEST_HPP
#include <cstdint>
namespace open_viii::kernel {
enum class AbilitiesT : std::uint16_t {
  // https://github.com/alexfilth/doomtrain/wiki/Junction-abilities
  none,
  // Enables HP junction
  hpj [[maybe_unused]],

  // Enables Strength junction

  str_j [[maybe_unused]],

  // Enables Vitality junction

  vit_j [[maybe_unused]],

  // Enables Magic junction

  mag_j [[maybe_unused]],

  // Enables Spirit junction

  spr_j [[maybe_unused]],

  // Enables Speed junction

  spd_j [[maybe_unused]],

  // Enables Evasion junction

  eva_j [[maybe_unused]],

  // Enables Hit junction

  hit_j [[maybe_unused]],

  // Enables Luck junction

  luck_j [[maybe_unused]],

  // Increased elemental attack slot Count to 1

  el_atk_j [[maybe_unused]],

  // Increased status attack slot Count to 1

  st_atk_j [[maybe_unused]],

  // Increased elemental defense slot Count to 1

  el_def_j [[maybe_unused]],

  // Increased status defense slot Count to 1

  st_def_j [[maybe_unused]],

  // Increased elemental defense slot Count to 2

  el_def_j2 [[maybe_unused]],

  // Increased elemental defense slot Count to 4

  el_def_j4 [[maybe_unused]],

  // Increased status defense slot Count to 2

  st_def_j2 [[maybe_unused]],

  // Increased status defense slot Count to 4

  st_def_j4,

  // Increases ability slot Count to 3

  ability3 [[maybe_unused]],

  // Increases ability slot Count to 4

  ability4 [[maybe_unused]],

  // Equippable commands start here
  // https://github.com/alexfilth/doomtrain/wiki/Command-abilities
  magic,
  gf [[maybe_unused]],
  draw [[maybe_unused]],
  item,
  empty,
  card,
  doom [[maybe_unused]],
  mad_rush [[maybe_unused]],
  treatment [[maybe_unused]],
  defend [[maybe_unused]],
  dark_side [[maybe_unused]],
  recover [[maybe_unused]],
  absorb [[maybe_unused]],
  revive,
  lv_down [[maybe_unused]],
  lv_up [[maybe_unused]],
  kamikaze [[maybe_unused]],
  devour [[maybe_unused]],
  mini_mog [[maybe_unused]],
  // Equippable commands end here

  // Equippable abilities start here
  // https://github.com/alexfilth/doomtrain/wiki/Stat-percentage-increasing-abilities
  hp20 [[maybe_unused]],
  hp40 [[maybe_unused]],
  hp80 [[maybe_unused]],
  str20 [[maybe_unused]],
  str40 [[maybe_unused]],
  str60 [[maybe_unused]],
  vit20 [[maybe_unused]],
  vit40 [[maybe_unused]],
  vit60 [[maybe_unused]],
  mag20 [[maybe_unused]],
  mag40 [[maybe_unused]],
  mag60 [[maybe_unused]],
  spr20 [[maybe_unused]],
  spr40 [[maybe_unused]],
  spr60 [[maybe_unused]],
  spd20 [[maybe_unused]],
  spd40 [[maybe_unused]],
  eva30 [[maybe_unused]],
  luck50 [[maybe_unused]],

  // https://github.com/alexfilth/doomtrain/wiki/Character-abilities
  mug,
  med_data [[maybe_unused]],
  counter [[maybe_unused]],
  return_damage [[maybe_unused]],
  cover,
  initiative [[maybe_unused]],
  move_hp_up,
  hp_bonus [[maybe_unused]],
  str_bonus [[maybe_unused]],
  vit_bonus [[maybe_unused]],
  mag_bonus [[maybe_unused]],
  spr_bonus [[maybe_unused]],
  auto_protect,
  auto_shell [[maybe_unused]],
  auto_reflect,
  auto_haste,
  auto_potion [[maybe_unused]],
  expend2 [[maybe_unused]],
  expend3 [[maybe_unused]],
  ribbon,

  // https://github.com/alexfilth/doomtrain/wiki/Party-abilities
  alert [[maybe_unused]],
  move_find [[maybe_unused]],
  enc_half [[maybe_unused]],
  enc_none [[maybe_unused]],
  rare_item [[maybe_unused]],

  // https://github.com/alexfilth/doomtrain/wiki/GF-abilities
  sum_mag10 [[maybe_unused]],
  sum_mag20 [[maybe_unused]],
  sum_mag30 [[maybe_unused]],
  sum_mag40 [[maybe_unused]],
  gf_hp10 [[maybe_unused]],
  gf_hp20 [[maybe_unused]],
  gf_hp30 [[maybe_unused]],
  gf_hp40 [[maybe_unused]],
  boost [[maybe_unused]],
  // Equipable abilities end here

  // https://github.com/alexfilth/doomtrain/wiki/Menu-abilities
  haggle [[maybe_unused]],
  sell_high [[maybe_unused]],
  familiar [[maybe_unused]],
  call_shop [[maybe_unused]],
  junk_shop [[maybe_unused]],
  thunder_mag_rf [[maybe_unused]],
  ice_mag_rf [[maybe_unused]],
  fire_mag_rf [[maybe_unused]],
  life_mag_rf [[maybe_unused]],
  time_mag_rf [[maybe_unused]],
  status_mag_rf [[maybe_unused]],
  supt_mag_rf [[maybe_unused]],
  forbid_mag_rf [[maybe_unused]],
  recovery_med_rf [[maybe_unused]],
  status_med_rf [[maybe_unused]],
  ammo_rf [[maybe_unused]],
  tool_rf [[maybe_unused]],
  forbid_med_rf [[maybe_unused]],
  gf_recovery_med_rf [[maybe_unused]],
  gf_abl_med_rf [[maybe_unused]],
  mid_mag_rf [[maybe_unused]],
  high_mag_rf [[maybe_unused]],
  med_lv_up [[maybe_unused]],
  card_mod [[maybe_unused]],
};
}
#endif// VIIIARCHIVE_ABILITIEST_HPP
