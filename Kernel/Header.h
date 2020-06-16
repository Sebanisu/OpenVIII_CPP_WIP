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
#include "SectionData.h"
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
#include "Devour.h"
#include "Misc.h"
// will be moving kernel into it"s own project once it"svs more fully baked.
namespace OpenVIII::Kernel {


struct Header
{
  // https://github.com/alexfilth/doomtrain/wiki/Header
private:
  std::vector<char> buffer_{};
  std::vector<std::uint32_t> sectionOffsets_{};

public:
  template<SectionTypesT sectionType>[[nodiscard]] constexpr auto GetSpan() const
  {
    if constexpr (static_cast<int>(sectionType) >= static_cast<int>(SectionTypesT::Count)
                  || static_cast<int>(sectionType) < 0) {
      return nullptr;// failure
    }
    auto length = [this]() {
      if constexpr (static_cast<int>(sectionType) >= static_cast<int>(SectionTypesT::Count) - 1) {
        return std::size(buffer_) - sectionOffsets_.at(static_cast<size_t>(sectionType));
      }
      return static_cast<size_t>(sectionOffsets_.at(static_cast<size_t>(sectionType) + 1)
                                 - sectionOffsets_.at(static_cast<size_t>(sectionType)));
    }();
    return std::string_view(buffer_.data() + sectionOffsets_.at(static_cast<size_t>(sectionType)), length);
  }
  template<SectionTypesT sectionType>[[nodiscard]] auto GetSectionData() const
  {
    using namespace std::string_literals;
    if constexpr (sectionType == SectionTypesT::BattleCommands) {
      return SectionData<BattleCommands>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::BattleCommandsText>() };
    } else if constexpr (sectionType == SectionTypesT::Magic) {
      return SectionData<Magic>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::MagicText>() };
    } else if constexpr (sectionType == SectionTypesT::JunctionableGFs) {
      return SectionData<JunctionableGFs>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::JunctionableGFsText>() };
    } else if constexpr (sectionType == SectionTypesT::EnemyAttacks) {
      return SectionData<EnemyAttacks>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::EnemyAttacksText>() };
    } else if constexpr (sectionType == SectionTypesT::Weapons) {
      return SectionData<Weapons>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::WeaponsText>() };
    } else if constexpr (sectionType == SectionTypesT::RenzokukenFinishers) {
      return SectionData<RenzokukenFinishers>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::RenzokukenFinishersText>() };
    } else if constexpr (sectionType == SectionTypesT::Characters) {
      return SectionData<Characters>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::CharactersText>() };
    } else if constexpr (sectionType == SectionTypesT::BattleItems) {
      return SectionData<BattleItems>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::BattleItemsText>() };
    } else if constexpr (sectionType == SectionTypesT::NonBattleItems) {
      return SectionData<NonBattleItems>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::NonBattleItemsText>() };
    } else if constexpr (sectionType == SectionTypesT::NonJunctionableGFs) {
      return SectionData<NonJunctionableGFs>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::NonJunctionableGFsText>() };
    } else if constexpr (sectionType == SectionTypesT::CommandAbilitiesData) {
      return SectionData<CommandAbilitiesData>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::CommandAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypesT::JunctionAbilities) {
      return SectionData<JunctionAbilities>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::JunctionAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypesT::CommandAbilities) {
      return SectionData<CommandAbilities>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::CommandAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypesT::StatPercentIncreaseAbilities) {
      return SectionData<StatPercentIncreaseAbilities>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::StatPercentIncreaseAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypesT::CharacterAbilities) {
      return SectionData<CharacterAbilities>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::CommandAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypesT::PartyAbilities) {
      return SectionData<PartyAbilities>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::PartyAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypesT::GFAbilities) {
      return SectionData<GFAbilities>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::GFAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypesT::MenuAbilities) {
      return SectionData<MenuAbilities>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::MenuAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypesT::TeamLagunaLimitBreaks) {
      return SectionData<TeamLagunaLimitBreaks>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::TeamLagunaLimitBreaksText>() };
    } else if constexpr (sectionType == SectionTypesT::QuistisBlueMagicLimitBreak) {
      return SectionData<QuistisBlueMagicLimitBreak>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::QuistisBlueMagicLimitBreakText>(),
        GetSpan<SectionTypesT::QuistisBlueMagicLimitBreakParams>() };
    } else if constexpr (sectionType == SectionTypesT::IrvineShotLimitBreak) {
      return SectionData<IrvineShotLimitBreak>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::IrvineShotLimitBreakText>() };
    } else if constexpr (sectionType == SectionTypesT::ZellDuelLimitBreak) {
      return SectionData<ZellDuelLimitBreak>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::ZellDuelLimitBreakText>(),
        GetSpan<SectionTypesT::ZellDuelLimitBreakParams>() };
    } else if constexpr (sectionType == SectionTypesT::RinoaLimitBreakPart1) {
      return SectionData<RinoaLimitBreakPart1>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::RinoaLimitBreakPart1Text>() };
    } else if constexpr (sectionType == SectionTypesT::RinoaLimitBreakPart2) {
      return SectionData<RinoaLimitBreakPart2>{ GetSpan<sectionType>(),
        GetSpan<SectionTypesT::RinoaLimitBreakPart2Text>() };
    } else if constexpr (sectionType == SectionTypesT::SlotsArray) {
      return SectionData<SlotsArray>{ GetSpan<sectionType>(), {}, GetSpan<SectionTypesT::SlotsSets>() };
    }

    else if constexpr (sectionType == SectionTypesT::Devour) {
      return SectionData<Devour>{ GetSpan<sectionType>(), GetSpan<SectionTypesT::DevourText>() };
    } else if constexpr (sectionType == SectionTypesT::Misc) {
      return SectionData<Misc>{
        GetSpan<sectionType>(), GetSpan<SectionTypesT::MiscText>(), GetSpan<SectionTypesT::MiscText2>()
      };
    } else {
      return nullptr;
    }
  }

  template<SectionTypesT sectionType>[[nodiscard]] constexpr std::string_view GetSectionName() const
  {
    using namespace std::string_literals;
    if constexpr (sectionType == SectionTypesT::BattleCommands) {
      return "BattleCommands"sv;
    }
    if constexpr (sectionType == SectionTypesT::Magic) {
      return "Magic"sv;
    }
    if constexpr (sectionType == SectionTypesT::JunctionableGFs) {
      return "JunctionableGFs"sv;
    }
    if constexpr (sectionType == SectionTypesT::EnemyAttacks) {
      return "EnemyAttacks"sv;
    }
    if constexpr (sectionType == SectionTypesT::Weapons) {
      return "Weapons"sv;
    }
    if constexpr (sectionType == SectionTypesT::RenzokukenFinishers) {
      return "RenzokukenFinishers"sv;
    }
    if constexpr (sectionType == SectionTypesT::Characters) {
      return "Characters"sv;
    }
    if constexpr (sectionType == SectionTypesT::BattleItems) {
      return "BattleItems"sv;
    }
    if constexpr (sectionType == SectionTypesT::NonBattleItems) {
      return "NonBattleItems"sv;
    }
    if constexpr (sectionType == SectionTypesT::NonJunctionableGFs) {
      return "NonJunctionableGFs"sv;
    }
    if constexpr (sectionType == SectionTypesT::CommandAbilitiesData) {
      return "CommandAbilitiesData"sv;
    }
    if constexpr (sectionType == SectionTypesT::JunctionAbilities) {
      return "JunctionAbilities"sv;
    }
    if constexpr (sectionType == SectionTypesT::CommandAbilities) {
      return "CommandAbilities"sv;
    }
    if constexpr (sectionType == SectionTypesT::StatPercentIncreaseAbilities) {
      return "StatPercentIncreaseAbilities"sv;
    }
    if constexpr (sectionType == SectionTypesT::CharacterAbilities) {
      return "CharacterAbilities"sv;
    }
    if constexpr (sectionType == SectionTypesT::PartyAbilities) {
      return "PartyAbilities"sv;
    }
    if constexpr (sectionType == SectionTypesT::GFAbilities) {
      return "GFAbilities"sv;
    }
    if constexpr (sectionType == SectionTypesT::MenuAbilities) {
      return "MenuAbilities"sv;
    }
    if constexpr (sectionType == SectionTypesT::TeamLagunaLimitBreaks) {
      return "TeamLagunaLimitBreaks"sv;
    }
    if constexpr (sectionType == SectionTypesT::QuistisBlueMagicLimitBreak) {
      return "QuistisBlueMagicLimitBreak"sv;
    }
    if constexpr (sectionType == SectionTypesT::QuistisBlueMagicLimitBreakParams) {
      return "QuistisBlueMagicLimitBreakParams"sv;
    }
    if constexpr (sectionType == SectionTypesT::IrvineShotLimitBreak) {
      return "IrvineShotLimitBreak"sv;
    }
    if constexpr (sectionType == SectionTypesT::ZellDuelLimitBreak) {
      return "ZellDuelLimitBreak"sv;
    }
    if constexpr (sectionType == SectionTypesT::ZellDuelLimitBreakParams) {
      return "ZellDuelLimitBreakParams"sv;
    }
    if constexpr (sectionType == SectionTypesT::RinoaLimitBreakPart1) {
      return "RinoaLimitBreakPart1"sv;
    }
    if constexpr (sectionType == SectionTypesT::RinoaLimitBreakPart2) {
      return "RinoaLimitBreakPart2"sv;
    }
    if constexpr (sectionType == SectionTypesT::SlotsArray) {
      return "SlotsArray"sv;
    }
    if constexpr (sectionType == SectionTypesT::SlotsSets) {
      return "SlotsSets"sv;
    }
    if constexpr (sectionType == SectionTypesT::Devour) {
      return "Devour"sv;
    }
    if constexpr (sectionType == SectionTypesT::Misc) {
      return "Misc"sv;
    }
    if constexpr (sectionType == SectionTypesT::MiscText) {
      return "MiscText"sv;
    }
    if constexpr (sectionType == SectionTypesT::BattleCommandsText) {
      return "BattleCommandsText"sv;
    }
    if constexpr (sectionType == SectionTypesT::MagicText) {
      return "MagicText"sv;
    }
    if constexpr (sectionType == SectionTypesT::JunctionableGFsText) {
      return "JunctionableGFsText"sv;
    }
    if constexpr (sectionType == SectionTypesT::EnemyAttacksText) {
      return "EnemyAttacksText"sv;
    }
    if constexpr (sectionType == SectionTypesT::WeaponsText) {
      return "WeaponsText"sv;
    }
    if constexpr (sectionType == SectionTypesT::RenzokukenFinishersText) {
      return "RenzokukenFinishersText"sv;
    }
    if constexpr (sectionType == SectionTypesT::CharactersText) {
      return "CharactersText"sv;
    }
    if constexpr (sectionType == SectionTypesT::BattleItemsText) {
      return "BattleItemsText"sv;
    }
    if constexpr (sectionType == SectionTypesT::NonBattleItemsText) {
      return "NonBattleItemsText"sv;
    }
    if constexpr (sectionType == SectionTypesT::NonJunctionableGFsText) {
      return "NonJunctionableGFsText"sv;
    }
    if constexpr (sectionType == SectionTypesT::JunctionAbilitiesText) {
      return "JunctionAbilitiesText"sv;
    }
    if constexpr (sectionType == SectionTypesT::CommandAbilitiesText) {
      return "CommandAbilitiesText"sv;
    }
    if constexpr (sectionType == SectionTypesT::StatPercentIncreaseAbilitiesText) {
      return "StatPercentIncreaseAbilitiesText"sv;
    }
    if constexpr (sectionType == SectionTypesT::CharacterAbilitiesText) {
      return "CharacterAbilitiesText"sv;
    }
    if constexpr (sectionType == SectionTypesT::PartyAbilitiesText) {
      return "PartyAbilitiesText"sv;
    }
    if constexpr (sectionType == SectionTypesT::GFAbilitiesText) {
      return "GFAbilitiesText"sv;
    }
    if constexpr (sectionType == SectionTypesT::MenuAbilitiesText) {
      return "MenuAbilitiesText"sv;
    }
    if constexpr (sectionType == SectionTypesT::TeamLagunaLimitBreaksText) {
      return "TeamLagunaLimitBreaksText"sv;
    }
    if constexpr (sectionType == SectionTypesT::QuistisBlueMagicLimitBreakText) {
      return "QuistisBlueMagicLimitBreakText"sv;
    }
    if constexpr (sectionType == SectionTypesT::IrvineShotLimitBreakText) {
      return "IrvineShotLimitBreakText"sv;
    }
    if constexpr (sectionType == SectionTypesT::ZellDuelLimitBreakText) {
      return "ZellDuelLimitBreakText"sv;
    }
    if constexpr (sectionType == SectionTypesT::RinoaLimitBreakPart1Text) {
      return "RinoaLimitBreakPart1Text"sv;
    }
    if constexpr (sectionType == SectionTypesT::RinoaLimitBreakPart2Text) {
      return "RinoaLimitBreakPart2Text"sv;
    }
    if constexpr (sectionType == SectionTypesT::DevourText) {
      return "DevourText"sv;
    }
    if constexpr (sectionType == SectionTypesT::MiscText2) {
      return "MiscText2"sv;
    }
    return ""sv;
  }

  template<typename mainT> explicit Header(const mainT &main)
  {
    buffer_ = main.GetEntryData(std::string_view("kernel.bin"));
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


  template<int First, int Count, typename Lambda> void static_for(Lambda const &f)
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
