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

#ifndef VIIIARCHIVE_SECTIONTYPEST_H
#define VIIIARCHIVE_SECTIONTYPEST_H
#include <cstdint>
namespace open_viii::kernel {
enum class SectionTypesT : std::int8_t {
  // https://github.com/alexfilth/doomtrain/wiki/Header
  BattleCommands,
  Magic,
  JunctionableGFs,
  EnemyAttacks,
  Weapons,
  RenzokukenFinishers,
  Characters,
  BattleItems,
  NonBattleItems,
  NonJunctionableGFs,
  CommandAbilitiesData,
  JunctionAbilities,
  CommandAbilities,
  StatPercentIncreaseAbilities,
  CharacterAbilities,
  PartyAbilities,
  GFAbilities,
  MenuAbilities,
  TeamLagunaLimitBreaks,
  QuistisBlueMagicLimitBreak,
  QuistisBlueMagicLimitBreakParams,
  IrvineShotLimitBreak,
  ZellDuelLimitBreak,
  ZellDuelLimitBreakParams,
  RinoaLimitBreakPart1,
  RinoaLimitBreakPart2,
  SlotsArray,
  SlotsSets,
  Devour,
  Misc,
  MiscTextPointers,
  BattleCommandsText,
  MagicText,
  JunctionableGFsText,
  EnemyAttacksText,
  WeaponsText,
  RenzokukenFinishersText,
  CharactersText,
  BattleItemsText,
  NonBattleItemsText,
  NonJunctionableGFsText,
  JunctionAbilitiesText,
  CommandAbilitiesText,
  StatPercentIncreaseAbilitiesText,
  CharacterAbilitiesText,
  PartyAbilitiesText,
  GFAbilitiesText,
  MenuAbilitiesText,
  TeamLagunaLimitBreaksText,
  QuistisBlueMagicLimitBreakText,
  IrvineShotLimitBreakText,
  ZellDuelLimitBreakText,
  RinoaLimitBreakPart1Text,
  RinoaLimitBreakPart2Text,
  DevourText,
  MiscText,
  Count,
  First = BattleCommands
};
}
#endif// VIIIARCHIVE_SECTIONTYPEST_H
