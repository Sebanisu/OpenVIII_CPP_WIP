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
#include "TestKernel.hpp"
#include "open_viii/archive/Archives.hpp"
#include "open_viii/kernel/Header.hpp"
#include "open_viii/paths/Paths.hpp"
[[nodiscard]] auto
  write(bool &first, std::ostream &os, const auto &value)
{
  using valueT = std::decay_t<decltype(value)>;
  if (!first) {
    os << ", ";
  }
  first = false;
  if constexpr (std::is_integral_v<valueT>) {
    os << +value;
    return std::nullptr_t{};
  }
  else if constexpr (std::is_enum_v<valueT>) {
    using underT = std::underlying_type_t<valueT>;
    os << +static_cast<underT>(value);
    return std::nullptr_t{};
  }
  else if constexpr (
    std::ranges::range<
      valueT> && !std::is_convertible_v<valueT, std::string_view>) {
    return value;
  }
  else {
    os << value;
    return std::nullptr_t{};
  }
}
template<typename KernelElementT>
void
  writeif(bool &first, std::ostream &os, const KernelElementT &kernel_element)
{
  if constexpr (std::ranges::range<KernelElementT>) {
    bool nested_first = true;
    os << "{";
    for (const auto &nested : kernel_element) {
      using nestedT = std::decay_t<decltype(nested)>;
      if constexpr (open_viii::kernel::is_kernel_object(nestedT())) {
        writeif(nested_first, os, nested);
      }
      else {
        auto ret_value = write(nested_first, os, nested);
        using retT     = std::decay_t<decltype(ret_value)>;
        if constexpr (!std::is_null_pointer_v<retT>) {
          writeif(first, os, ret_value);
        }
      }
    }
    os << "}";
    return;
  }
  else if constexpr (!open_viii::kernel::is_kernel_object(KernelElementT())) {
    auto ret_value = write(first, os, kernel_element);
    using retT     = std::decay_t<decltype(ret_value)>;
    if constexpr (!std::is_null_pointer_v<retT>) {
      writeif(first, os, ret_value);
    }
    return;
  }
  else {
#define IF_EXIST_WRITE(function_name)                                          \
  if constexpr (open_viii::kernel::has_##function_name<KernelElementT>) {      \
    auto ret_value = write(first, os, kernel_element.function_name());         \
    using retT     = std::decay_t<decltype(ret_value)>;                        \
    if constexpr (!std::is_null_pointer_v<retT>) {                             \
      writeif(first, os, ret_value);                                           \
    }                                                                          \
  }
    IF_EXIST_WRITE(ability_data_id)
    IF_EXIST_WRITE(unknown_flags)
    IF_EXIST_WRITE(target)
    IF_EXIST_WRITE(unknown)
    IF_EXIST_WRITE(magic_id)
    IF_EXIST_WRITE(attack_type)
    IF_EXIST_WRITE(attack_power)
    IF_EXIST_WRITE(unknown0)
    IF_EXIST_WRITE(unknown1)
    IF_EXIST_WRITE(attack_flags)
    IF_EXIST_WRITE(unknown2)
    IF_EXIST_WRITE(status_attack_enabler)
    // statuses 0-7
    IF_EXIST_WRITE(persistent_statuses)
    // statuses 8-39
    IF_EXIST_WRITE(battle_only_statuses)
    IF_EXIST_WRITE(attack_param)
    IF_EXIST_WRITE(unknown3)
    IF_EXIST_WRITE(hit_count)
    IF_EXIST_WRITE(element)
    /**
     * Ability points required to unlock
     * @see
     * https://www.gamerguides.com/final-fantasy-viii/guide/guardian-forces/overview/ap-and-learning-abilities#learning-and-forgetting-abilities
     */
    IF_EXIST_WRITE(ability_points_required_to_unlock)
    IF_EXIST_WRITE(crisis_level_hp_multiplier)
    IF_EXIST_WRITE(gender)
    IF_EXIST_WRITE(limit_break_id)
    IF_EXIST_WRITE(limit_break_param)
    IF_EXIST_WRITE(exp)
    IF_EXIST_WRITE(stats)
    IF_EXIST_WRITE(index_to_battle_command)
    IF_EXIST_WRITE(devour_stat_flag)
    IF_EXIST_WRITE(raised_stat_hp_quantity)
    IF_EXIST_WRITE(camera_change)
    IF_EXIST_WRITE(attack_parameter)
    IF_EXIST_WRITE(enable_boost)
    IF_EXIST_WRITE(stat_to_increase)
    IF_EXIST_WRITE(increase_value)
    IF_EXIST_WRITE(element_attack_percent)
    IF_EXIST_WRITE(used_item_index)
    IF_EXIST_WRITE(critical_increase)
    IF_EXIST_WRITE(gf_power)
    IF_EXIST_WRITE(gf_hp_modifier)
    IF_EXIST_WRITE(unlockable_abilities)
    IF_EXIST_WRITE(power_mod)
    IF_EXIST_WRITE(compatibility)
    IF_EXIST_WRITE(level_mod)
    IF_EXIST_WRITE(unknown4)
    IF_EXIST_WRITE(unknown5)
    IF_EXIST_WRITE(unknown6)
    IF_EXIST_WRITE(unknown7)
    IF_EXIST_WRITE(unknown8)
    IF_EXIST_WRITE(unknown9)
    IF_EXIST_WRITE(unknown10)
    IF_EXIST_WRITE(spell_power)
    IF_EXIST_WRITE(draw_resist)
    IF_EXIST_WRITE(status_attack)
    IF_EXIST_WRITE(junction_stats)
    IF_EXIST_WRITE(j_elem_attack_flag)
    IF_EXIST_WRITE(j_elem_attack_value)
    IF_EXIST_WRITE(j_elem_defense_flag)
    IF_EXIST_WRITE(j_elem_defense_value)
    IF_EXIST_WRITE(j_status_attack_value)
    IF_EXIST_WRITE(j_status_defense_value)
    IF_EXIST_WRITE(j_statuses_attack_flag)
    IF_EXIST_WRITE(j_statuses_defend_flag)
    IF_EXIST_WRITE(menu_file_index)
    IF_EXIST_WRITE(start_offset)
    IF_EXIST_WRITE(end_offset)
    IF_EXIST_WRITE(sleep_timer)
    IF_EXIST_WRITE(haste_timer)
    IF_EXIST_WRITE(slow_timer)
    IF_EXIST_WRITE(stop_timer)
    IF_EXIST_WRITE(regen_timer)
    IF_EXIST_WRITE(protect_timer)
    IF_EXIST_WRITE(shell_timer)
    IF_EXIST_WRITE(reflect_timer)
    IF_EXIST_WRITE(aura_timer)
    IF_EXIST_WRITE(curse_timer)
    IF_EXIST_WRITE(doom_timer)
    IF_EXIST_WRITE(invincible_timer)
    IF_EXIST_WRITE(petrifying_timer)
    IF_EXIST_WRITE(float_timer)
    IF_EXIST_WRITE(atb_speed_multiplier)
    IF_EXIST_WRITE(dead_timer)
    IF_EXIST_WRITE(death_limit_effect)
    IF_EXIST_WRITE(poison_limit_effect)
    IF_EXIST_WRITE(petrify_limit_effect)
    IF_EXIST_WRITE(darkness_limit_effect)
    IF_EXIST_WRITE(silence_limit_effect)
    IF_EXIST_WRITE(berserk_limit_effect)
    IF_EXIST_WRITE(zombie_limit_effect)
    IF_EXIST_WRITE(unknown_status_limit_effect)
    IF_EXIST_WRITE(sleep_limit_effect)
    IF_EXIST_WRITE(haste_limit_effect)
    IF_EXIST_WRITE(slow_limit_effect)
    IF_EXIST_WRITE(stop_limit_effect)
    IF_EXIST_WRITE(regen_limit_effect)
    IF_EXIST_WRITE(protect_limit_effect)
    IF_EXIST_WRITE(shell_limit_effect)
    IF_EXIST_WRITE(reflect_limit_effect)
    IF_EXIST_WRITE(aura_limit_effect)
    IF_EXIST_WRITE(curse_limit_effect)
    IF_EXIST_WRITE(doom_limit_effect)
    IF_EXIST_WRITE(invincible_limit_effect)
    IF_EXIST_WRITE(petrifying_limit_effect)
    IF_EXIST_WRITE(float_limit_effect)
    IF_EXIST_WRITE(confusion_limit_effect)
    IF_EXIST_WRITE(drain_limit_effect)
    IF_EXIST_WRITE(eject_limit_effect)
    IF_EXIST_WRITE(double_limit_effect)
    IF_EXIST_WRITE(triple_limit_effect)
    IF_EXIST_WRITE(defend_limit_effect)
    IF_EXIST_WRITE(unknown_status_limit_effect0)
    IF_EXIST_WRITE(unknown_status_limit_effect1)
    IF_EXIST_WRITE(charged_limit_effect)
    IF_EXIST_WRITE(back_attack_limit_effect)
    IF_EXIST_WRITE(duel_timers)
    IF_EXIST_WRITE(shot_timers)
    IF_EXIST_WRITE(status_flags)
    IF_EXIST_WRITE(flag)
    IF_EXIST_WRITE(critical_bonus)
    IF_EXIST_WRITE(blue_magic_data)
    IF_EXIST_WRITE(sequence)
    IF_EXIST_WRITE(timer)
    IF_EXIST_WRITE(count)
    IF_EXIST_WRITE(increased_value)
    IF_EXIST_WRITE(unlocker)
    IF_EXIST_WRITE(ability)
    IF_EXIST_WRITE(renzokuken_finishers)
    IF_EXIST_WRITE(character_id)
    IF_EXIST_WRITE(str_bonus)
    IF_EXIST_WRITE(weapon_tier)
    IF_EXIST_WRITE(melee_weapon)
    IF_EXIST_WRITE(button_sequence)
    IF_EXIST_WRITE(character_ability_flags)
    IF_EXIST_WRITE(junction_flags)
    IF_EXIST_WRITE(percent_quantity)
    IF_EXIST_WRITE(damage_or_heal)
#undef IF_EXIST_WRITE
  }
}
/**
 * Dumps values to stream
 * @param os output stream
 * @param buffer contains encoded names and descriptions
 */
template<open_viii::LangT langVal, typename KernelElementT>
static void
  out(KernelElementT               kernel_element,
      std::ostream                &os,
      const std::span<const char> &buffer)
{
  bool first = true;
#define IF_EXIST_WRITE_DECODE_STRING(function_name)                            \
  if constexpr (open_viii::kernel::has_##function_name<KernelElementT>) {      \
    if (!std::ranges::empty(buffer)) {                                         \
      const auto decoded_string                                                \
        = kernel_element.function_name().template decoded_string<langVal>(     \
          buffer);                                                             \
      if (!std::empty(decoded_string)) {                                       \
        (void)write(first, os, open_viii::tools::u8_to_sv(decoded_string));    \
      }                                                                        \
    }                                                                          \
  }
  IF_EXIST_WRITE_DECODE_STRING(name_offset)
  IF_EXIST_WRITE_DECODE_STRING(description_offset)
  writeif(first, os, kernel_element);
#undef IF_EXIST_WRITE_DECODE_STRING
}

int
  main()
{
  std::vector<std::pair<std::filesystem::path, open_viii::kernel::Header>>
                        kernels{};
  static constexpr auto coo   = open_viii::LangT::en;
  const auto execution_lambda = [&kernels](const std::filesystem::path &path) {
    std::cout << path << std::endl;
    const auto archives
      = open_viii::archive::Archives(path,
                                     open_viii::LangCommon::to_string<coo>());
    if (!static_cast<bool>(archives)) {
      std::cerr << "Failed to load path: " << path.string() << '\n';
      return;
    }
    [[maybe_unused]] const auto &main
      = archives.get<open_viii::archive::ArchiveTypeT::main>();

    std::cout << main << std::endl;
    auto &kernel = kernels.emplace_back(path, main).second;
    [[maybe_unused]] const auto &buffer = kernel.buffer();
    std::cout << "kernel.bin " << buffer.size() << " bytes; "
              << kernel.section_count() << " section count\n";
    std::cout << static_cast<int>(open_viii::kernel::SectionTypesT::count)
              << std::endl;
    kernel.static_for([](const std::string_view      &string,
                         const std::span<const char> &span,
                         const auto                  &data) {
      std::cout << string << " ( " << std::ranges::size(span) << "bytes) has "
                << std::ranges::size(data) << " entries\n";
      for (size_t i = 0; i < data.size(); i++) {
        auto entry = data.at(i);
        if (entry != decltype(entry){}) {
          std::cout << i << ": ";
          //todo fix cout<coo> broken with clang
          out<coo>(entry, std::cout, data.text_span());
          std::cout << '\n';
        }
      }
    });
  };
  open_viii::Paths::for_each_path(execution_lambda);
  int i = 0;
  for (const auto &[path, kernel] : kernels) {
    for (const auto &[other_path, other_kernel] :
         kernels | std::views::drop(++i)) {
      std::cout << "Comparing " << open_viii::kernel::Header::FILE_NAME << " {"
                << path.string() << ", " << other_path.string() << "}:\n";
      std::cout << (std::ranges::equal(kernel.buffer(), other_kernel.buffer())
                      ? "equal"
                      : "not equal")
                << '\n';
    }
  }
  return 0;
}