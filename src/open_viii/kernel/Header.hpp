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
#include <utility>
#include <vector>
// will be moving kernel into it"s own project once it"svs more fully baked.
namespace open_viii::kernel {
struct Header
{
  // https://github.com/alexfilth/doomtrain/wiki/Header
private:
  std::vector<char>          m_buffer{};
  std::vector<std::uint32_t> m_section_offsets{};
  static constexpr auto      FILE_NAME = std::string_view{ "kernel.bin" };
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
    if (std::ranges::empty(m_buffer)) {
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
      return BulkSectionData<BattleCommands>{
        get_span<sectionType>(), get_span<SectionTypesT::battle_commands_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::magic) {
      return BulkSectionData<Magic>{ get_span<sectionType>(),
                                     get_span<SectionTypesT::magic_text>() };
    } else if constexpr (sectionType == SectionTypesT::junctionable_g_fs) {
      return BulkSectionData<JunctionableGFs>{
        get_span<sectionType>(),
        get_span<SectionTypesT::junctionable_g_fs_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::enemy_attacks) {
      return BulkSectionData<EnemyAttacks>{
        get_span<sectionType>(), get_span<SectionTypesT::enemy_attacks_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::weapons) {
      return BulkSectionData<Weapons>{
        get_span<sectionType>(), get_span<SectionTypesT::weapons_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::renzokuken_finishers) {
      return BulkSectionData<RenzokukenFinishers>{
        get_span<sectionType>(),
        get_span<SectionTypesT::renzokuken_finishers_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::characters) {
      return BulkSectionData<Characters>{
        get_span<sectionType>(), get_span<SectionTypesT::characters_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::battle_items) {
      return BulkSectionData<BattleItems>{
        get_span<sectionType>(), get_span<SectionTypesT::battle_items_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::non_battle_items) {
      return BulkSectionData<NonBattleItems>{
        get_span<sectionType>(),
        get_span<SectionTypesT::non_battle_items_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::non_junctionable_g_fs) {
      return BulkSectionData<NonJunctionableGFs>{
        get_span<sectionType>(),
        get_span<SectionTypesT::non_junctionable_g_fs_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::command_abilities_data) {
      return BulkSectionData<CommandAbilitiesData>{ get_span<sectionType>() };
    } else if constexpr (sectionType == SectionTypesT::junction_abilities) {
      return BulkSectionData<JunctionAbilities>{
        get_span<sectionType>(),
        get_span<SectionTypesT::junction_abilities_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::command_abilities) {
      return BulkSectionData<CommandAbilities>{
        get_span<sectionType>(),
        get_span<SectionTypesT::character_abilities_text>()
      };
    } else if constexpr (sectionType
                         == SectionTypesT::stat_percent_increase_abilities) {
      return BulkSectionData<StatPercentIncreaseAbilities>{
        get_span<sectionType>(),
        get_span<SectionTypesT::stat_percent_increase_abilities_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::character_abilities) {
      return BulkSectionData<CharacterAbilities>{
        get_span<sectionType>(),
        get_span<SectionTypesT::character_abilities_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::party_abilities) {
      return BulkSectionData<PartyAbilities>{
        get_span<sectionType>(), get_span<SectionTypesT::party_abilities_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::gf_abilities) {
      return BulkSectionData<GFAbilities>{
        get_span<sectionType>(), get_span<SectionTypesT::gf_abilities_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::menu_abilities) {
      return BulkSectionData<MenuAbilities>{
        get_span<sectionType>(), get_span<SectionTypesT::menu_abilities_text>()
      };
    } else if constexpr (sectionType
                         == SectionTypesT::team_laguna_limit_breaks) {
      return BulkSectionData<TeamLagunaLimitBreaks>{
        get_span<sectionType>(),
        get_span<SectionTypesT::team_laguna_limit_breaks_text>()
      };
    } else if constexpr (sectionType
                         == SectionTypesT::quistis_blue_magic_limit_break) {
      return BulkSectionData<QuistisBlueMagicLimitBreak>{
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
      return BulkSectionData<IrvineShotLimitBreak>{
        get_span<sectionType>(),
        get_span<SectionTypesT::irvine_shot_limit_break_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::zell_duel_limit_break) {
      return BulkSectionData<ZellDuelLimitBreak>{
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
      return BulkSectionData<RinoaLimitBreakPart1>{
        get_span<sectionType>(),
        get_span<SectionTypesT::rinoa_limit_break_part1_text>()
      };
    } else if constexpr (sectionType
                         == SectionTypesT::rinoa_limit_break_part2) {
      return BulkSectionData<RinoaLimitBreakPart2>{
        get_span<sectionType>(),
        get_span<SectionTypesT::rinoa_limit_break_part2_text>()
      };
    } else if constexpr (sectionType == SectionTypesT::slots_array) {
      return BulkSectionData<SlotsArray>{ get_span<sectionType>() };
    } else if constexpr (sectionType == SectionTypesT::slots_sets) {
      return BulkSectionData<SlotsSets>{ get_span<sectionType>() };
    } else if constexpr (sectionType == SectionTypesT::devour) {
      return BulkSectionData<Devour>{ get_span<sectionType>(),
                                      get_span<SectionTypesT::devour_text>() };
    } else if constexpr (sectionType == SectionTypesT::misc) {
      return BulkSectionData<Misc>{ get_span<sectionType>() };
    } else if constexpr (sectionType == SectionTypesT::misc_text_pointers) {
      return BulkSectionData<MiscText>{ get_span<sectionType>(),
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
    return {};
  }
  template<FIFLFS_Has_get_entry_data mainT> explicit Header(const mainT &main)
  {
    m_buffer         = main.get_entry_data(FILE_NAME);
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
  /**
   * static_for loop through each valid SectionTypesT
   * @tparam Begin
   * @tparam End
   * @tparam Lambda type of f
   * @param f
   * @see
   * https://stackoverflow.com/questions/13816850/is-it-possible-to-develop-static-for-loop-in-c
   */
  template<int Begin = static_cast<int>(SectionTypesT::begin),
           int End   = static_cast<int>(SectionTypesT::end),
           typename Lambda>
  requires(section_type_test<static_cast<SectionTypesT>(Begin)>() && Begin < End
           && (section_type_test<static_cast<SectionTypesT>(End)>()
               || End
                    == static_cast<int>(
                      SectionTypesT::
                        end))) void static_for([[maybe_unused]] const Lambda &f)
  {
    if (std::ranges::empty(m_buffer)) {
      return;
    }
    constexpr auto sectionType =
      std::integral_constant<SectionTypesT,
                             static_cast<SectionTypesT>(Begin)>{};
    const auto data = get_section_data<sectionType>();
    if constexpr (!std::is_null_pointer_v<decltype(data)>) {
      f(get_section_name<sectionType>(), get_span<sectionType>(), data);
    }
    if constexpr (Begin + 1 < End) {
      static_for<Begin + 1, End>(f);
    }
    // TODO see if i can pass the integral constant instead of passing values to
    // the lambda. As template arguments.
  }
};
using namespace std::string_view_literals;
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::battle_commands>() const
{
  return "Battle Commands"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::magic>() const
{
  return "Magic"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::junctionable_g_fs>() const
{
  return "Junctionable GFs"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::enemy_attacks>() const
{
  return "Enemy Attacks"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::weapons>() const
{
  return "Weapons"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::renzokuken_finishers>() const
{
  return "Renzokuken Finishers"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::characters>() const
{
  return "Characters"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::battle_items>() const
{
  return "Battle Items"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::non_battle_items>() const
{
  return "Non Battle Items"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::non_junctionable_g_fs>() const
{
  return "Non Junctionable GFs"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::command_abilities_data>() const
{
  return "Command AbilitiesData"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::junction_abilities>() const
{
  return "Junction Abilities"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::command_abilities>() const
{
  return "Command Abilities"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::stat_percent_increase_abilities>()
    const
{
  return "Stat Percent Increase Abilities"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::character_abilities>() const
{
  return "Character Abilities"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::party_abilities>() const
{
  return "Party Abilities"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::gf_abilities>() const
{
  return "GF Abilities"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::menu_abilities>() const
{
  return "Menu Abilities"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::team_laguna_limit_breaks>() const
{
  return "Team Laguna Limit Breaks"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::quistis_blue_magic_limit_break>()
    const
{
  return "Quistis Blue Magic Limit Break"sv;
}
template<>
[[nodiscard]] constexpr std::string_view Header::get_section_name<
  SectionTypesT::quistis_blue_magic_limit_break_params>() const
{
  return "Quistis Blue Magic Limit Break Params"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::irvine_shot_limit_break>() const
{
  return "Irvine Shot Limit Break"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::zell_duel_limit_break>() const
{
  return "Zell Duel Limit Break"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::zell_duel_limit_break_params>() const
{
  return "Zell Duel Limit Break Params"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::rinoa_limit_break_part1>() const
{
  return "Rinoa Limit Break Part 1"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::rinoa_limit_break_part2>() const
{
  return "Rinoa Limit Break Part 2"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::slots_array>() const
{
  return "Slots Array"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::slots_sets>() const
{
  return "Slots Sets"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::devour>() const
{
  return "Devour"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::misc>() const
{
  return "Misc"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::misc_text_pointers>() const
{
  return "Misc Text Pointers"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::battle_commands_text>() const
{
  return "Battle Commands Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::magic_text>() const
{
  return "Magic Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::junctionable_g_fs_text>() const
{
  return "Junctionable GFs Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::enemy_attacks_text>() const
{
  return "Enemy Attacks Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::weapons_text>() const
{
  return "Weapons Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::renzokuken_finishers_text>() const
{
  return "Renzokuken Finishers Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::characters_text>() const
{
  return "Characters Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::battle_items_text>() const
{
  return "Battle Items Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::non_battle_items_text>() const
{
  return "Non Battle Items Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::non_junctionable_g_fs_text>() const
{
  return "Non Junctionable GFs Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::junction_abilities_text>() const
{
  return "Junction Abilities Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::command_abilities_text>() const
{
  return "Command Abilities Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view Header::get_section_name<
  SectionTypesT::stat_percent_increase_abilities_text>() const
{
  return "Stat Percent Increase Abilities Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::character_abilities_text>() const
{
  return "Character Abilities Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::party_abilities_text>() const
{
  return "Party Abilities Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::gf_abilities_text>() const
{
  return "GF Abilities Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::menu_abilities_text>() const
{
  return "Menu Abilities Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::team_laguna_limit_breaks_text>() const
{
  return "Team Laguna Limit Breaks Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::quistis_blue_magic_limit_break_text>()
    const
{
  return "Quistis Blue Magic Limit Break Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::irvine_shot_limit_break_text>() const
{
  return "Irvine Shot Limit Break Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::zell_duel_limit_break_text>() const
{
  return "Zell Duel Limit Break Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::rinoa_limit_break_part1_text>() const
{
  return "Rinoa Limit Break Part 1 Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::rinoa_limit_break_part2_text>() const
{
  return "Rinoa Limit Break Part 2 Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::devour_text>() const
{
  return "Devour Text"sv;
}
template<>
[[nodiscard]] constexpr std::string_view
  Header::get_section_name<SectionTypesT::misc_text>() const
{
  return "Misc Text"sv;
}
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_KERNEL_HEADER_H
