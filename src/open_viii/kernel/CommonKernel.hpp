//
// Created by pcvii on 7/13/2021.
//

#ifndef OPENVIII_CPP_WIP_COMMONKERNEL_HPP
#define OPENVIII_CPP_WIP_COMMONKERNEL_HPP
#include "concepts.hpp"
#include <compare>
namespace open_viii::kernel {
template<typename T>
struct CommonKernel : public T
{
public:
  using this_type          = CommonKernel<T>;
  constexpr CommonKernel() = default;
  using T::EXPECTED_SIZE;
  constexpr auto
    operator<=>(const this_type &right) const noexcept = default;
#define GET(value_name)                                                        \
  [[nodiscard]] constexpr auto value_name()                                    \
    const noexcept requires(requires(this_type t) { t.m_##value_name; })       \
  {                                                                            \
    return T::m_##value_name;                                                  \
  }
#define GET_impl(value_name)                                                   \
  [[nodiscard]] constexpr auto value_name() const noexcept requires(           \
    requires(this_type t) { t.value_name##_impl(); })                      \
  {                                                                            \
    return T::value_name##_impl();                                         \
  }
#define WITH(value_name)                                                       \
  [[nodiscard]] constexpr auto with_##value_name(                              \
    const auto &new_val) &&noexcept requires(requires(this_type t) {           \
    t.m_##value_name;                                                          \
  })                                                                           \
  {                                                                            \
    T::m_##value_name = new_val;                                               \
    return *this;                                                              \
  }                                                                            \
  [[nodiscard]] constexpr auto with_##value_name(const auto &new_val)          \
    const &noexcept requires(requires(this_type t) { t.m_##value_name; })      \
  {                                                                            \
    return this_type{*this}.with_##value_name(new_val);                        \
  }                                                                            \
  [[nodiscard]] constexpr auto with_##value_name(const auto &new_val)          \
    const &&noexcept requires(requires(this_type t) { t.m_##value_name; })     \
  {                                                                            \
    return this_type{*this}.with_##value_name(new_val);                        \
  }
#define BOTH(value_name)                                                       \
  GET(value_name)                                                              \
  WITH(value_name)
  BOTH(name_offset)
  BOTH(description_offset)
  BOTH(ability_data_id)
  BOTH(unknown_flags)
  BOTH(target)
  BOTH(unknown)
  BOTH(magic_id)
  BOTH(attack_type)
  BOTH(attack_power)
  BOTH(unknown0)
  BOTH(unknown1)
  BOTH(attack_flags)
  BOTH(unknown2)
  BOTH(status_attack_enabler)
  // statuses 0-7
  BOTH(persistent_statuses)
  // statuses 8-39
  BOTH(battle_only_statuses)
  BOTH(attack_param)
  BOTH(unknown3)
  BOTH(hit_count)
  BOTH(element)
  /**
   * Ability points required to unlock
   * @see
   * https://www.gamerguides.com/final-fantasy-viii/guide/guardian-forces/overview/ap-and-learning-abilities#learning-and-forgetting-abilities
   */
  BOTH(ability_points_required_to_unlock)
  BOTH(crisis_level_hp_multiplier)
  BOTH(gender)
  BOTH(limit_break_id)
  BOTH(limit_break_param)
  BOTH(exp)
  BOTH(stats)
  BOTH(index_to_battle_command)
  BOTH(devour_stat_flag)
  BOTH(raised_stat_hp_quantity)
  BOTH(camera_change)
  BOTH(attack_parameter)
  BOTH(enable_boost)
  BOTH(stat_to_increase)
  BOTH(increase_value)
  BOTH(element_attack_percent)
  BOTH(used_item_index)
  BOTH(critical_increase)
  BOTH(gf_power)
  BOTH(gf_hp_modifier)
  BOTH(unlockable_abilities)
  BOTH(power_mod)
  BOTH(compatibility)
  BOTH(level_mod)
  BOTH(unknown4)
  BOTH(unknown5)
  BOTH(unknown6)
  BOTH(unknown7)
  BOTH(unknown8)
  BOTH(unknown9)
  BOTH(unknown10)
  BOTH(spell_power)
  BOTH(draw_resist)
  BOTH(status_attack)
  BOTH(junction_stats)
  BOTH(j_elem_attack_flag)
  BOTH(j_elem_attack_value)
  BOTH(j_elem_defense_flag)
  BOTH(j_elem_defense_value)
  BOTH(j_status_attack_value)
  BOTH(j_status_defense_value)
  BOTH(j_statuses_attack_flag)
  BOTH(j_statuses_defend_flag)

  GET_impl(character_ability_flags) GET_impl(junction_flags)
    GET_impl(percent_quantity) GET_impl(damage_or_heal)
#undef GET_impl
#undef BOTH
#undef GET
#undef WITH
};
}// namespace open_viii::kernel
#endif// OPENVIII_CPP_WIP_COMMONKERNEL_HPP
