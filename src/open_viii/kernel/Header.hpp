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
#include "BattleCommands.hpp"
#include "BattleItems.hpp"
#include "CharacterAbilities.hpp"
#include "Characters.hpp"
#include "CommandAbilities.hpp"
#include "CommandAbilitiesData.h"
#include "Devour.hpp"
#include "EnemyAttacks.hpp"
#include "GFAbilities.hpp"
#include "IrvineShotLimitBreak.hpp"
#include "JunctionAbilities.hpp"
#include "JunctionableGFs.hpp"
#include "Magic.hpp"
#include "MenuAbilities.hpp"
#include "Misc.hpp"
#include "MiscText.hpp"
#include "NonBattleItems.hpp"
#include "NonJunctionableGFs.hpp"
#include "PartyAbilities.hpp"
#include "QuistisBlueMagicLimitBreak.hpp"
#include "QuistisBlueMagicLimitBreakParams.hpp"
#include "RenzokukenFinishers.hpp"
#include "RinoaLimitBreakPart1.hpp"
#include "RinoaLimitBreakPart2.hpp"
#include "SectionTypesT.hpp"
#include "SlotsArray.hpp"
#include "SlotsSets.hpp"
#include "StatPercentIncreaseAbilities.hpp"
#include "TeamLagunaLimitBreaks.hpp"
#include "Weapons.hpp"
#include "ZellDuelLimitBreak.hpp"
#include "ZellDuelLimitBreakParams.hpp"
#include "open_viii/BulkSectionData.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
// will be moving kernel into it"s own project once it"svs more fully baked.
namespace open_viii::kernel {

template<LangT langVal> struct Header
{
  // https://github.com/alexfilth/doomtrain/wiki/Header
private:
  std::vector<char> m_buffer{};
  std::vector<std::uint32_t> m_section_offsets{};
  static constexpr auto FILE_NAME = std::string_view{ "kernel.bin" };

public:
  template<SectionTypesT sectionType> static consteval bool section_type_test()
  {
    return static_cast<int>(sectionType)
             < static_cast<int>(SectionTypesT::count)
           && static_cast<int>(sectionType) >= 0;
  }

  template<SectionTypesT sectionType>
  requires(section_type_test<sectionType>())
    [[nodiscard]] constexpr std::span<const char> get_span() const
  {
    if(std::ranges::empty(m_buffer)) {
      return std::span<const char>{};
    }
    auto length = [this]() {
      if constexpr (static_cast<int>(sectionType)
                    >= (static_cast<int>(SectionTypesT::count) - 1)) {
        return std::ranges::size(m_buffer)
               - m_section_offsets.at(static_cast<size_t>(sectionType));
      } else {
        return static_cast<size_t>(
          m_section_offsets.at(static_cast<size_t>(sectionType) + 1)
          - m_section_offsets.at(static_cast<size_t>(sectionType)));
      }
    }();
    return std::span<const char>(m_buffer).subspan(
      m_section_offsets.at(static_cast<size_t>(sectionType)), length);
  }
  template<SectionTypesT sectionType>
  requires(section_type_test<sectionType>())
    [[nodiscard]] auto get_section_data() const
  {
    using namespace std::string_literals;
    if constexpr (sectionType == SectionTypesT::battle_commands) {
      return BulkSectionData<BattleCommands<langVal>>{ get_span<sectionType>(),
        get_span<SectionTypesT::battle_commands_text>() };
    } else if constexpr (sectionType == SectionTypesT::magic) {
      return BulkSectionData<Magic<langVal>>{ get_span<sectionType>(),
        get_span<SectionTypesT::magic_text>() };
    } else if constexpr (sectionType == SectionTypesT::junctionable_g_fs) {
      return BulkSectionData<JunctionableGFs<langVal>>{ get_span<sectionType>(),
        get_span<SectionTypesT::junctionable_g_fs_text>() };
    } else if constexpr (sectionType == SectionTypesT::enemy_attacks) {
      return BulkSectionData<EnemyAttacks<langVal>>{ get_span<sectionType>(),
        get_span<SectionTypesT::enemy_attacks_text>() };
    } else if constexpr (sectionType == SectionTypesT::weapons) {
      return BulkSectionData<Weapons<langVal>>{ get_span<sectionType>(),
        get_span<SectionTypesT::weapons_text>() };
    } else if constexpr (sectionType == SectionTypesT::renzokuken_finishers) {
      return BulkSectionData<RenzokukenFinishers<langVal>>{
        get_span<sectionType>(),
        get_span<SectionTypesT::renzokuken_finishers_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::characters) {
      return BulkSectionData<Characters<langVal>>{ get_span<sectionType>(),
        get_span<SectionTypesT::characters_text>() };
    } else if constexpr (sectionType == SectionTypesT::battle_items) {
      return BulkSectionData<BattleItems<langVal>>{ get_span<sectionType>(),
        get_span<SectionTypesT::battle_items_text>() };
    } else if constexpr (sectionType == SectionTypesT::non_battle_items) {
      return BulkSectionData<NonBattleItems<langVal>>{ get_span<sectionType>(),
        get_span<SectionTypesT::non_battle_items_text>() };
    } else if constexpr (sectionType == SectionTypesT::non_junctionable_g_fs) {
      return BulkSectionData<NonJunctionableGFs<langVal>>{
        get_span<sectionType>(),
        get_span<SectionTypesT::non_junctionable_g_fs_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::command_abilities_data) {
      return BulkSectionData<CommandAbilitiesData<langVal>>{
        get_span<sectionType>()
      };
    } else if constexpr (sectionType == SectionTypesT::junction_abilities) {
      return BulkSectionData<JunctionAbilities<langVal>>{
        get_span<sectionType>(),
        get_span<SectionTypesT::junction_abilities_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::command_abilities) {
      return BulkSectionData<CommandAbilities<langVal>>{
        get_span<sectionType>(),
        get_span<SectionTypesT::character_abilities_text>()
      };
    } else if constexpr (sectionType
                         == SectionTypesT::stat_percent_increase_abilities) {
      return BulkSectionData<StatPercentIncreaseAbilities<langVal>>{
        get_span<sectionType>(),
        get_span<SectionTypesT::stat_percent_increase_abilities_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::character_abilities) {
      return BulkSectionData<CharacterAbilities<langVal>>{
        get_span<sectionType>(),
        get_span<SectionTypesT::character_abilities_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::party_abilities) {
      return BulkSectionData<PartyAbilities<langVal>>{ get_span<sectionType>(),
        get_span<SectionTypesT::party_abilities_text>() };
    } else if constexpr (sectionType == SectionTypesT::gf_abilities) {
      return BulkSectionData<GFAbilities<langVal>>{ get_span<sectionType>(),
        get_span<SectionTypesT::gf_abilities_text>() };
    } else if constexpr (sectionType == SectionTypesT::menu_abilities) {
      return BulkSectionData<MenuAbilities<langVal>>{ get_span<sectionType>(),
        get_span<SectionTypesT::menu_abilities_text>() };
    } else if constexpr (sectionType
                         == SectionTypesT::team_laguna_limit_breaks) {
      return BulkSectionData<TeamLagunaLimitBreaks<langVal>>{
        get_span<sectionType>(),
        get_span<SectionTypesT::team_laguna_limit_breaks_text>()
      };
    } else if constexpr (sectionType
                         == SectionTypesT::quistis_blue_magic_limit_break) {
      return BulkSectionData<QuistisBlueMagicLimitBreak<langVal>>{
        get_span<sectionType>(),
        get_span<SectionTypesT::quistis_blue_magic_limit_break_text>()
      };
    } else if constexpr (sectionType
                         == SectionTypesT::
                           quistis_blue_magic_limit_break_params) {
      return BulkSectionData<QuistisBlueMagicLimitBreakParams>{
        get_span<sectionType>()
      };
    } else if constexpr (sectionType
                         == SectionTypesT::irvine_shot_limit_break) {
      return BulkSectionData<IrvineShotLimitBreak<langVal>>{
        get_span<sectionType>(),
        get_span<SectionTypesT::irvine_shot_limit_break_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::zell_duel_limit_break) {
      return BulkSectionData<ZellDuelLimitBreak<langVal>>{
        get_span<sectionType>(),
        get_span<SectionTypesT::zell_duel_limit_break_text>()
      };
    } else if constexpr (sectionType
                         == SectionTypesT::zell_duel_limit_break_params) {
      return BulkSectionData<ZellDuelLimitBreakParams>{
        get_span<sectionType>()
      };
    } else if constexpr (sectionType
                         == SectionTypesT::rinoa_limit_break_part1) {
      return BulkSectionData<RinoaLimitBreakPart1<langVal>>{
        get_span<sectionType>(),
        get_span<SectionTypesT::rinoa_limit_break_part1_text>()
      };
    } else if constexpr (sectionType
                         == SectionTypesT::rinoa_limit_break_part2) {
      return BulkSectionData<RinoaLimitBreakPart2<langVal>>{
        get_span<sectionType>(),
        get_span<SectionTypesT::rinoa_limit_break_part2_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::slots_array) {
      return BulkSectionData<SlotsArray>{ get_span<sectionType>() };
    } else if constexpr (sectionType == SectionTypesT::slots_sets) {
      return BulkSectionData<SlotsSets>{ get_span<sectionType>() };
    } else if constexpr (sectionType == SectionTypesT::devour) {
      return BulkSectionData<Devour<langVal>>{ get_span<sectionType>(),
        get_span<SectionTypesT::devour_text>() };
    } else if constexpr (sectionType == SectionTypesT::misc) {
      return BulkSectionData<Misc<langVal>>{ get_span<sectionType>() };
    } else if constexpr (sectionType == SectionTypesT::misc_text_pointers) {
      return BulkSectionData<MiscText<langVal>>{ get_span<sectionType>(),
        get_span<SectionTypesT::misc_text>() };
    } else {
      // anything that isn't a main section like Text is returning null here.
      return nullptr;
    }
  }

  template<SectionTypesT sectionType>
  requires(section_type_test<sectionType>())
    [[nodiscard]] constexpr std::string_view get_section_name() const
  {
    using namespace std::string_view_literals;
    if constexpr (sectionType == SectionTypesT::battle_commands) {
      return "Battle Commands"sv;
    } else if constexpr (sectionType == SectionTypesT::magic) {
      return "Magic"sv;
    } else if constexpr (sectionType == SectionTypesT::junctionable_g_fs) {
      return "Junctionable GFs"sv;
    } else if constexpr (sectionType == SectionTypesT::enemy_attacks) {
      return "Enemy Attacks"sv;
    } else if constexpr (sectionType == SectionTypesT::weapons) {
      return "Weapons"sv;
    } else if constexpr (sectionType == SectionTypesT::renzokuken_finishers) {
      return "Renzokuken Finishers"sv;
    } else if constexpr (sectionType == SectionTypesT::characters) {
      return "Characters"sv;
    } else if constexpr (sectionType == SectionTypesT::battle_items) {
      return "Battle Items"sv;
    } else if constexpr (sectionType == SectionTypesT::non_battle_items) {
      return "Non Battle Items"sv;
    } else if constexpr (sectionType == SectionTypesT::non_junctionable_g_fs) {
      return "Non Junctionable GFs"sv;
    } else if constexpr (sectionType == SectionTypesT::command_abilities_data) {
      return "Command AbilitiesData"sv;
    } else if constexpr (sectionType == SectionTypesT::junction_abilities) {
      return "Junction Abilities"sv;
    } else if constexpr (sectionType == SectionTypesT::command_abilities) {
      return "Command Abilities"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::stat_percent_increase_abilities) {
      return "Stat Percent Increase Abilities"sv;
    } else if constexpr (sectionType == SectionTypesT::character_abilities) {
      return "Character Abilities"sv;
    } else if constexpr (sectionType == SectionTypesT::party_abilities) {
      return "Party Abilities"sv;
    } else if constexpr (sectionType == SectionTypesT::gf_abilities) {
      return "GF Abilities"sv;
    } else if constexpr (sectionType == SectionTypesT::menu_abilities) {
      return "Menu Abilities"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::team_laguna_limit_breaks) {
      return "Team Laguna Limit Breaks"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::quistis_blue_magic_limit_break) {
      return "Quistis Blue Magic Limit Break"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::
                           quistis_blue_magic_limit_break_params) {
      return "Quistis Blue Magic Limit Break Params"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::irvine_shot_limit_break) {
      return "Irvine Shot Limit Break"sv;
    } else if constexpr (sectionType == SectionTypesT::zell_duel_limit_break) {
      return "Zell Duel Limit Break"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::zell_duel_limit_break_params) {
      return "Zell Duel Limit Break Params"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::rinoa_limit_break_part1) {
      return "Rinoa Limit Break Part 1"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::rinoa_limit_break_part2) {
      return "Rinoa Limit Break Part 2"sv;
    } else if constexpr (sectionType == SectionTypesT::slots_array) {
      return "Slots Array"sv;
    } else if constexpr (sectionType == SectionTypesT::slots_sets) {
      return "Slots Sets"sv;
    } else if constexpr (sectionType == SectionTypesT::devour) {
      return "Devour"sv;
    } else if constexpr (sectionType == SectionTypesT::misc) {
      return "Misc"sv;
    } else if constexpr (sectionType == SectionTypesT::misc_text_pointers) {
      return "Misc Text Pointers"sv;
    } else if constexpr (sectionType == SectionTypesT::battle_commands_text) {
      return "Battle Commands Text"sv;
    } else if constexpr (sectionType == SectionTypesT::magic_text) {
      return "Magic Text"sv;
    } else if constexpr (sectionType == SectionTypesT::junctionable_g_fs_text) {
      return "Junctionable GFs Text"sv;
    } else if constexpr (sectionType == SectionTypesT::enemy_attacks_text) {
      return "Enemy Attacks Text"sv;
    } else if constexpr (sectionType == SectionTypesT::weapons_text) {
      return "Weapons Text"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::renzokuken_finishers_text) {
      return "Renzokuken Finishers Text"sv;
    } else if constexpr (sectionType == SectionTypesT::characters_text) {
      return "Characters Text"sv;
    } else if constexpr (sectionType == SectionTypesT::battle_items_text) {
      return "Battle Items Text"sv;
    } else if constexpr (sectionType == SectionTypesT::non_battle_items_text) {
      return "Non Battle Items Text"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::non_junctionable_g_fs_text) {
      return "Non Junctionable GFs Text"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::junction_abilities_text) {
      return "Junction Abilities Text"sv;
    } else if constexpr (sectionType == SectionTypesT::command_abilities_text) {
      return "Command Abilities Text"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::
                           stat_percent_increase_abilities_text) {
      return "Stat Percent Increase Abilities Text"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::character_abilities_text) {
      return "Character Abilities Text"sv;
    } else if constexpr (sectionType == SectionTypesT::party_abilities_text) {
      return "Party Abilities Text"sv;
    } else if constexpr (sectionType == SectionTypesT::gf_abilities_text) {
      return "GF Abilities Text"sv;
    } else if constexpr (sectionType == SectionTypesT::menu_abilities_text) {
      return "Menu Abilities Text"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::team_laguna_limit_breaks_text) {
      return "Team Laguna Limit Breaks Text"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::
                           quistis_blue_magic_limit_break_text) {
      return "Quistis Blue Magic Limit Break Text"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::irvine_shot_limit_break_text) {
      return "Irvine Shot Limit Break Text"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::zell_duel_limit_break_text) {
      return "Zell Duel Limit Break Text"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::rinoa_limit_break_part1_text) {
      return "Rinoa Limit Break Part 1 Text"sv;
    } else if constexpr (sectionType
                         == SectionTypesT::rinoa_limit_break_part2_text) {
      return "Rinoa Limit Break Part 2 Text"sv;
    } else if constexpr (sectionType == SectionTypesT::devour_text) {
      return "Devour Text"sv;
    } else if constexpr (sectionType == SectionTypesT::misc_text) {
      return "Misc Text"sv;
    }
  }

  template<FIFLFS_Has_get_entry_data mainT> explicit Header(const mainT &main)
  {
    m_buffer = main.get_entry_data(FILE_NAME);
    auto buffer_span = std::span<const char>(m_buffer);
    if (std::ranges::size(buffer_span) < sizeof(std::uint32_t)) {
      return;
    }
    std::uint32_t section_count{};
    std::memcpy(
      &section_count, std::ranges::data(buffer_span), sizeof(section_count));
    if (std::ranges::size(buffer_span)
        < sizeof(std::uint32_t) * (section_count + 1)) {
      return;
    }
    m_section_offsets.reserve(section_count);
    while (section_count-- > 0) {
      buffer_span = buffer_span.subspan(sizeof(section_count));
      std::memcpy(&m_section_offsets.emplace_back(),
        std::ranges::data(buffer_span),
        sizeof(section_count));
    }
  }
  [[nodiscard]] const auto &buffer() const noexcept
  {
    return m_buffer;
  }
  [[nodiscard]] auto section_count() const noexcept
  {
    return m_section_offsets.size();
  }
  [[nodiscard]] const auto &section_offsets() const noexcept
  {
    return m_section_offsets;
  }


  template<int First = static_cast<int>(SectionTypesT::first),
    int Count = static_cast<int>(SectionTypesT::count),
    typename Lambda>
  requires(
    section_type_test<static_cast<SectionTypesT>(First)>()
    && (section_type_test<static_cast<SectionTypesT>(Count)>()
        || Count
             == static_cast<int>(SectionTypesT::
                 count))) void static_for([[maybe_unused]] const Lambda &f)
  {// https://stackoverflow.com/questions/13816850/is-it-possible-to-develop-static-for-loop-in-c
    if constexpr (First < Count) {
      constexpr auto sectionType = std::integral_constant<SectionTypesT,
        static_cast<SectionTypesT>(First)>{};
      const auto data = get_section_data<sectionType>();
      if constexpr (!std::is_null_pointer_v<decltype(data)>) {
        f(get_section_name<sectionType>(), get_span<sectionType>(), data);
      }
      if constexpr (First + 1 < Count) {
        static_for<First + 1, Count>(f);
      }
    }
  }
};// namespace open_viii::kernel
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_KERNEL_HEADER_H
