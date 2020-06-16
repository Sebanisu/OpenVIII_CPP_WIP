//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_SECTIONTYPEST_H
#define VIIIARCHIVE_SECTIONTYPEST_H
#include <cstdint>
namespace OpenVIII::Kernel {
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
  MiscText,
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
  MiscText2,
  Count,
  First = BattleCommands
};
}
#endif// VIIIARCHIVE_SECTIONTYPEST_H
