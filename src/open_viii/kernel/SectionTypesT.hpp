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

#ifndef VIIIARCHIVE_SECTIONTYPEST_HPP
#define VIIIARCHIVE_SECTIONTYPEST_HPP
#include <cstdint>
namespace open_viii::kernel {
enum class SectionTypesT : std::int8_t {
  // https://github.com/alexfilth/doomtrain/wiki/Header
  battle_commands,
  magic,
  junctionable_g_fs,
  enemy_attacks,
  weapons,
  renzokuken_finishers,
  characters,
  battle_items,
  non_battle_items,
  non_junctionable_g_fs,
  command_abilities_data,
  junction_abilities,
  command_abilities,
  stat_percent_increase_abilities,
  character_abilities,
  party_abilities,
  gf_abilities,
  menu_abilities,
  team_laguna_limit_breaks,
  quistis_blue_magic_limit_break,
  quistis_blue_magic_limit_break_params,
  irvine_shot_limit_break,
  zell_duel_limit_break,
  zell_duel_limit_break_params,
  rinoa_limit_break_part1,
  rinoa_limit_break_part2,
  slots_array,
  slots_sets,
  devour,
  misc,
  misc_text_pointers,
  battle_commands_text,
  magic_text,
  junctionable_g_fs_text,
  enemy_attacks_text,
  weapons_text,
  renzokuken_finishers_text,
  characters_text,
  battle_items_text,
  non_battle_items_text,
  non_junctionable_g_fs_text,
  junction_abilities_text,
  command_abilities_text,
  stat_percent_increase_abilities_text,
  character_abilities_text,
  party_abilities_text,
  gf_abilities_text,
  menu_abilities_text,
  team_laguna_limit_breaks_text,
  quistis_blue_magic_limit_break_text,
  irvine_shot_limit_break_text,
  zell_duel_limit_break_text,
  rinoa_limit_break_part1_text,
  rinoa_limit_break_part2_text,
  devour_text,
  misc_text,
  count,
  first = battle_commands,
};
}
#endif// VIIIARCHIVE_SECTIONTYPEST_HPP
