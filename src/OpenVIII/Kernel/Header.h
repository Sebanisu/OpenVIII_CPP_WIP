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

#ifndef VIIIARCHIVE_KERNEL_HEADER_H
#define VIIIARCHIVE_KERNEL_HEADER_H
#include <vector>
#include <string_view>
#include <string>
#include <array>
#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "SectionTypesT.h"
#include "OpenVIII/SectionData.h"
#include "BattleCommands.h"
#include "Magic.h"
#include "JunctionableGFs.h"
#include "EnemyAttacks.h"
#include "Weapons.h"
#include "RenzokukenFinishers.h"
#include "Characters.h"
#include "BattleItems.h"
#include "NonBattleItems.h"
#include "NonJunctionableGFs.h"
#include "CommandAbilitiesData.h"
#include "JunctionAbilities.h"
#include "CommandAbilities.h"
#include "StatPercentIncreaseAbilities.h"
#include "CharacterAbilities.h"
#include "PartyAbilities.h"
#include "GFAbilities.h"
#include "MenuAbilities.h"
#include "TeamLagunaLimitBreaks.h"
#include "QuistisBlueMagicLimitBreak.h"
#include "IrvineShotLimitBreak.h"
#include "ZellDuelLimitBreak.h"
#include "RinoaLimitBreakPart1.h"
#include "RinoaLimitBreakPart2.h"
#include "SlotsArray.h"
#include "SlotsSets.h"
#include "Devour.h"
#include "Misc.h"
#include "MiscText.h"
#include "QuistisBlueMagicLimitBreakParams.h"
#include "ZellDuelLimitBreakParams.h"
// will be moving kernel into it"s own project once it"svs more fully baked.
namespace OpenVIII::Kernel {

template<LangT langVal> struct Header
{
  // https://github.com/alexfilth/doomtrain/wiki/Header
private:
  std::vector<char> buffer_{};
  std::vector<std::uint32_t> sectionOffsets_{};
  static constexpr auto fileName_ = std::string_view{ "kernel.bin" };

public:
  template<SectionTypesT sectionType> [[nodiscard]] constexpr auto GetSpan() const
  {
    if constexpr (static_cast<int>(sectionType) >= static_cast<int>(SectionTypesT::Count)
                  || static_cast<int>(sectionType) < 0) {
      return nullptr;// failure
    }
    auto length = [this]() {
      if constexpr (static_cast<int>(sectionType) >= (static_cast<int>(SectionTypesT::Count) - 1)) {
        return std::size(buffer_) - sectionOffsets_.at(static_cast<size_t>(sectionType));
      } else {
        return static_cast<size_t>(sectionOffsets_.at(static_cast<size_t>(sectionType) + 1)
                                   - sectionOffsets_.at(static_cast<size_t>(sectionType)));
      }
    }();
    return std::string_view(buffer_.data() + sectionOffsets_.at(static_cast<size_t>(sectionType)), length);
  }
  template<SectionTypesT sectionType> [[nodiscard]] auto GetSectionData() const
  {
    using namespace std::string_literals;
    if constexpr (sectionType == SectionTypesT::BattleCommands) {
      return SectionData<BattleCommands<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::BattleCommandsText>() };
    } else if constexpr (sectionType == SectionTypesT::Magic) {
      return SectionData<Magic<langVal>>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::MagicText>() };
    } else if constexpr (sectionType == SectionTypesT::JunctionableGFs) {
      return SectionData<JunctionableGFs<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::JunctionableGFsText>() };
    } else if constexpr (sectionType == SectionTypesT::EnemyAttacks) {
      return SectionData<EnemyAttacks<langVal>>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::EnemyAttacksText>() };
    } else if constexpr (sectionType == SectionTypesT::Weapons) {
      return SectionData<Weapons<langVal>>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::WeaponsText>() };
    } else if constexpr (sectionType == SectionTypesT::RenzokukenFinishers) {
      return SectionData<RenzokukenFinishers<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::RenzokukenFinishersText>() };
    } else if constexpr (sectionType == SectionTypesT::Characters) {
      return SectionData<Characters<langVal>>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::CharactersText>() };
    } else if constexpr (sectionType == SectionTypesT::BattleItems) {
      return SectionData<BattleItems<langVal>>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::BattleItemsText>() };
    } else if constexpr (sectionType == SectionTypesT::NonBattleItems) {
      return SectionData<NonBattleItems<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::NonBattleItemsText>() };
    } else if constexpr (sectionType == SectionTypesT::NonJunctionableGFs) {
      return SectionData<NonJunctionableGFs<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::NonJunctionableGFsText>() };
    } else if constexpr (sectionType == SectionTypesT::CommandAbilitiesData) {
      return SectionData<CommandAbilitiesData<langVal>>{ GetSpan<sectionType>() };
    } else if constexpr (sectionType == SectionTypesT::JunctionAbilities) {
      return SectionData<JunctionAbilities<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::JunctionAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypesT::CommandAbilities) {
      return SectionData<CommandAbilities<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::CommandAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypesT::StatPercentIncreaseAbilities) {
      return SectionData<StatPercentIncreaseAbilities<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::StatPercentIncreaseAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypesT::CharacterAbilities) {
      return SectionData<CharacterAbilities<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::CharacterAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypesT::PartyAbilities) {
      return SectionData<PartyAbilities<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::PartyAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypesT::GFAbilities) {
      return SectionData<GFAbilities<langVal>>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::GFAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypesT::MenuAbilities) {
      return SectionData<MenuAbilities<langVal>>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::MenuAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypesT::TeamLagunaLimitBreaks) {
      return SectionData<TeamLagunaLimitBreaks<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::TeamLagunaLimitBreaksText>() };
    } else if constexpr (sectionType == SectionTypesT::QuistisBlueMagicLimitBreak) {
      return SectionData<QuistisBlueMagicLimitBreak<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::QuistisBlueMagicLimitBreakText>() };
    } else if constexpr (sectionType == SectionTypesT::QuistisBlueMagicLimitBreakParams) {
      return SectionData<QuistisBlueMagicLimitBreakParams>{ GetSpan<sectionType>() };
    } else if constexpr (sectionType == SectionTypesT::IrvineShotLimitBreak) {
      return SectionData<IrvineShotLimitBreak<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::IrvineShotLimitBreakText>() };
    } else if constexpr (sectionType == SectionTypesT::ZellDuelLimitBreak) {
      return SectionData<ZellDuelLimitBreak<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::ZellDuelLimitBreakText>() };
    } else if constexpr (sectionType == SectionTypesT::ZellDuelLimitBreakParams) {
      return SectionData<ZellDuelLimitBreakParams>{ GetSpan<sectionType>() };
    } else if constexpr (sectionType == SectionTypesT::RinoaLimitBreakPart1) {
      return SectionData<RinoaLimitBreakPart1<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::RinoaLimitBreakPart1Text>() };
    } else if constexpr (sectionType == SectionTypesT::RinoaLimitBreakPart2) {
      return SectionData<RinoaLimitBreakPart2<langVal>>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::RinoaLimitBreakPart2Text>() };
    } else if constexpr (sectionType == SectionTypesT::SlotsArray) {
      return SectionData<SlotsArray>{ GetSpan<sectionType>() };
    } else if constexpr (sectionType == SectionTypesT::SlotsSets) {
      return SectionData<SlotsSets>{ GetSpan<sectionType>() };
    } else if constexpr (sectionType == SectionTypesT::Devour) {
      return SectionData<Devour<langVal>>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::DevourText>() };
    } else if constexpr (sectionType == SectionTypesT::Misc) {
      return SectionData<Misc<langVal>>{ GetSpan<sectionType>() };
    } else if constexpr (sectionType == SectionTypesT::MiscTextPointers) {
      return SectionData<MiscText<langVal>>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::MiscText>() };
    } else {
      return nullptr;
    }
  }

  template<SectionTypesT sectionType> [[nodiscard]] constexpr std::string_view GetSectionName() const
  {
    using namespace std::string_view_literals;
    if constexpr (sectionType == SectionTypesT::BattleCommands) {
      constexpr auto battleCommands = "BattleCommands"sv;
      return battleCommands;
    } else if constexpr (sectionType == SectionTypesT::Magic) {
      constexpr auto magic = "Magic"sv;
      return magic;
    } else if constexpr (sectionType == SectionTypesT::JunctionableGFs) {
      constexpr auto junctionableGFs = "JunctionableGFs"sv;
      return junctionableGFs;
    } else if constexpr (sectionType == SectionTypesT::EnemyAttacks) {
      constexpr auto enemyAttacks = "EnemyAttacks"sv;
      return enemyAttacks;
    } else if constexpr (sectionType == SectionTypesT::Weapons) {
      constexpr auto weapons = "Weapons"sv;
      return weapons;
    } else if constexpr (sectionType == SectionTypesT::RenzokukenFinishers) {
      constexpr auto renzokukenFinishers = "RenzokukenFinishers"sv;
      return renzokukenFinishers;
    } else if constexpr (sectionType == SectionTypesT::Characters) {
      constexpr auto characters = "Characters"sv;
      return characters;
    } else if constexpr (sectionType == SectionTypesT::BattleItems) {
      constexpr auto battleItems = "BattleItems"sv;
      return battleItems;
    } else if constexpr (sectionType == SectionTypesT::NonBattleItems) {
      constexpr auto nonBattleItems = "NonBattleItems"sv;
      return nonBattleItems;
    } else if constexpr (sectionType == SectionTypesT::NonJunctionableGFs) {
      constexpr auto nonJunctionableGFs = "NonJunctionableGFs"sv;
      return nonJunctionableGFs;
    } else if constexpr (sectionType == SectionTypesT::CommandAbilitiesData) {
      constexpr auto commandAbilitiesData = "CommandAbilitiesData"sv;
      return commandAbilitiesData;
    } else if constexpr (sectionType == SectionTypesT::JunctionAbilities) {
      constexpr auto junctionAbilities = "JunctionAbilities"sv;
      return junctionAbilities;
    } else if constexpr (sectionType == SectionTypesT::CommandAbilities) {
      constexpr auto commandAbilities = "CommandAbilities"sv;
      return commandAbilities;
    } else if constexpr (sectionType == SectionTypesT::StatPercentIncreaseAbilities) {
      constexpr auto statPercentIncreaseAbilities = "StatPercentIncreaseAbilities"sv;
      return statPercentIncreaseAbilities;
    } else if constexpr (sectionType == SectionTypesT::CharacterAbilities) {
      constexpr auto characterAbilities = "CharacterAbilities"sv;
      return characterAbilities;
    } else if constexpr (sectionType == SectionTypesT::PartyAbilities) {
      constexpr auto partyAbilities = "PartyAbilities"sv;
      return partyAbilities;
    } else if constexpr (sectionType == SectionTypesT::GFAbilities) {
      constexpr auto gfAbilities = "GFAbilities"sv;
      return gfAbilities;
    } else if constexpr (sectionType == SectionTypesT::MenuAbilities) {
      constexpr auto menuAbilities = "MenuAbilities"sv;
      return menuAbilities;
    } else if constexpr (sectionType == SectionTypesT::TeamLagunaLimitBreaks) {
      constexpr auto teamLagunaLimitBreaks = "TeamLagunaLimitBreaks"sv;
      return teamLagunaLimitBreaks;
    } else if constexpr (sectionType == SectionTypesT::QuistisBlueMagicLimitBreak) {
      constexpr auto quistisBlueMagicLimitBreak = "QuistisBlueMagicLimitBreak"sv;
      return quistisBlueMagicLimitBreak;
    } else if constexpr (sectionType == SectionTypesT::QuistisBlueMagicLimitBreakParams) {
      constexpr auto quistisBlueMagicLimitBreakParams = "QuistisBlueMagicLimitBreakParams"sv;
      return quistisBlueMagicLimitBreakParams;
    } else if constexpr (sectionType == SectionTypesT::IrvineShotLimitBreak) {
      constexpr auto irvineShotLimitBreak = "IrvineShotLimitBreak"sv;
      return irvineShotLimitBreak;
    } else if constexpr (sectionType == SectionTypesT::ZellDuelLimitBreak) {
      constexpr auto zellDuelLimitBreak = "ZellDuelLimitBreak"sv;
      return zellDuelLimitBreak;
    } else if constexpr (sectionType == SectionTypesT::ZellDuelLimitBreakParams) {
      constexpr auto zellDuelLimitBreakParams = "ZellDuelLimitBreakParams"sv;
      return zellDuelLimitBreakParams;
    } else if constexpr (sectionType == SectionTypesT::RinoaLimitBreakPart1) {
      constexpr auto rinoaLimitBreakPart1 = "RinoaLimitBreakPart1"sv;
      return rinoaLimitBreakPart1;
    } else if constexpr (sectionType == SectionTypesT::RinoaLimitBreakPart2) {
      constexpr auto rinoaLimitBreakPart2 = "RinoaLimitBreakPart2"sv;
      return rinoaLimitBreakPart2;
    } else if constexpr (sectionType == SectionTypesT::SlotsArray) {
      constexpr auto slotsArray = "SlotsArray"sv;
      return slotsArray;
    } else if constexpr (sectionType == SectionTypesT::SlotsSets) {
      constexpr auto slotsSets = "SlotsSets"sv;
      return slotsSets;
    } else if constexpr (sectionType == SectionTypesT::Devour) {
      constexpr auto devour = "Devour"sv;
      return devour;
    } else if constexpr (sectionType == SectionTypesT::Misc) {
      constexpr auto misc = "Misc"sv;
      return misc;
    } else if constexpr (sectionType == SectionTypesT::MiscTextPointers) {
      constexpr auto miscTextPointers = "MiscTextPointers"sv;
      return miscTextPointers;
    } else if constexpr (sectionType == SectionTypesT::BattleCommandsText) {
      constexpr auto battleCommandsText = "BattleCommandsText"sv;
      return battleCommandsText;
    } else if constexpr (sectionType == SectionTypesT::MagicText) {
      constexpr auto magicText = "MagicText"sv;
      return magicText;
    } else if constexpr (sectionType == SectionTypesT::JunctionableGFsText) {
      constexpr auto junctionableGFsText = "JunctionableGFsText"sv;
      return junctionableGFsText;
    } else if constexpr (sectionType == SectionTypesT::EnemyAttacksText) {
      constexpr auto enemyAttacksText = "EnemyAttacksText"sv;
      return enemyAttacksText;
    } else if constexpr (sectionType == SectionTypesT::WeaponsText) {
      constexpr auto weaponsText = "WeaponsText"sv;
      return weaponsText;
    } else if constexpr (sectionType == SectionTypesT::RenzokukenFinishersText) {
      constexpr auto renzokukenFinishersText = "RenzokukenFinishersText"sv;
      return renzokukenFinishersText;
    } else if constexpr (sectionType == SectionTypesT::CharactersText) {
      constexpr auto charactersText = "CharactersText"sv;
      return charactersText;
    } else if constexpr (sectionType == SectionTypesT::BattleItemsText) {
      constexpr auto battleItemsText = "BattleItemsText"sv;
      return battleItemsText;
    } else if constexpr (sectionType == SectionTypesT::NonBattleItemsText) {
      constexpr auto nonBattleItemsText = "NonBattleItemsText"sv;
      return nonBattleItemsText;
    } else if constexpr (sectionType == SectionTypesT::NonJunctionableGFsText) {
      constexpr auto nonJunctionableGFsText = "NonJunctionableGFsText"sv;
      return nonJunctionableGFsText;
    } else if constexpr (sectionType == SectionTypesT::JunctionAbilitiesText) {
      constexpr auto junctionAbilitiesText = "JunctionAbilitiesText"sv;
      return junctionAbilitiesText;
    } else if constexpr (sectionType == SectionTypesT::CommandAbilitiesText) {
      constexpr auto commandAbilitiesText = "CommandAbilitiesText"sv;
      return commandAbilitiesText;
    } else if constexpr (sectionType == SectionTypesT::StatPercentIncreaseAbilitiesText) {
      constexpr auto statPercentIncreaseAbilitiesText = "StatPercentIncreaseAbilitiesText"sv;
      return statPercentIncreaseAbilitiesText;
    } else if constexpr (sectionType == SectionTypesT::CharacterAbilitiesText) {
      constexpr auto characterAbilitiesText = "CharacterAbilitiesText"sv;
      return characterAbilitiesText;
    } else if constexpr (sectionType == SectionTypesT::PartyAbilitiesText) {
      constexpr auto partyAbilitiesText = "PartyAbilitiesText"sv;
      return partyAbilitiesText;
    } else if constexpr (sectionType == SectionTypesT::GFAbilitiesText) {
      constexpr auto gfAbilitiesText = "GFAbilitiesText"sv;
      return gfAbilitiesText;
    } else if constexpr (sectionType == SectionTypesT::MenuAbilitiesText) {
      constexpr auto menuAbilitiesText = "MenuAbilitiesText"sv;
      return menuAbilitiesText;
    } else if constexpr (sectionType == SectionTypesT::TeamLagunaLimitBreaksText) {
      constexpr auto teamLagunaLimitBreaksText = "TeamLagunaLimitBreaksText"sv;
      return teamLagunaLimitBreaksText;
    } else if constexpr (sectionType == SectionTypesT::QuistisBlueMagicLimitBreakText) {
      constexpr auto quistisBlueMagicLimitBreakText = "QuistisBlueMagicLimitBreakText"sv;
      return quistisBlueMagicLimitBreakText;
    } else if constexpr (sectionType == SectionTypesT::IrvineShotLimitBreakText) {
      constexpr auto irvineShotLimitBreakText = "IrvineShotLimitBreakText"sv;
      return irvineShotLimitBreakText;
    } else if constexpr (sectionType == SectionTypesT::ZellDuelLimitBreakText) {
      constexpr auto zellDuelLimitBreakText = "ZellDuelLimitBreakText"sv;
      return zellDuelLimitBreakText;
    } else if constexpr (sectionType == SectionTypesT::RinoaLimitBreakPart1Text) {
      constexpr auto rinoaLimitBreakPart1Text = "RinoaLimitBreakPart1Text"sv;
      return rinoaLimitBreakPart1Text;
    } else if constexpr (sectionType == SectionTypesT::RinoaLimitBreakPart2Text) {
      constexpr auto rinoaLimitBreakPart2Text = "RinoaLimitBreakPart2Text"sv;
      return rinoaLimitBreakPart2Text;
    } else if constexpr (sectionType == SectionTypesT::DevourText) {
      constexpr auto devourText = "DevourText"sv;
      return devourText;
    } else if constexpr (sectionType == SectionTypesT::MiscText) {
      constexpr auto miscText = "MiscText"sv;
      return miscText;
    } else {
      return ""sv;
    }
  }

  template<typename mainT> explicit Header(const mainT &main)
  {
    buffer_ = main.GetEntryData(fileName_);
    if (std::size(buffer_) < sizeof(std::uint32_t)) {
      return;
    }
    std::uint32_t sectionCount{};
    std::memcpy(&sectionCount, buffer_.data(), sizeof(sectionCount));
    if (std::size(buffer_) < sizeof(std::uint32_t) * (sectionCount + 1)) {
      return;
    }
    sectionOffsets_.reserve(sectionCount);
    size_t i{ 1 };
    while (sectionCount-- > 0) {
      std::memcpy(
        &sectionOffsets_.emplace_back(), buffer_.data() + (sizeof(sectionCount) * (i++)), sizeof(sectionCount));
    }
  }
  [[nodiscard]] const auto &Buffer() const noexcept { return buffer_; }
  [[nodiscard]] auto SectionCount() const noexcept { return sectionOffsets_.size(); }
  [[nodiscard]] const auto &SectionOffsets() const noexcept { return sectionOffsets_; }


  template<int First, int Count, typename Lambda> void static_for([[maybe_unused]] Lambda const &f)
  {// https://stackoverflow.com/questions/13816850/is-it-possible-to-develop-static-for-loop-in-c
    if constexpr (First < Count) {
      constexpr auto sectionType = std::integral_constant<SectionTypesT, static_cast<SectionTypesT>(First)>{};
      const auto &data = GetSectionData<sectionType>();
      f(GetSectionName<sectionType>(), GetSpan<sectionType>(), data);

      static_for<First + 1, Count>(f);
    }
  }
};// namespace OpenVIII::Kernel
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_KERNEL_HEADER_H
