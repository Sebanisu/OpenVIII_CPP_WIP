//
// Created by pcvii on 7/13/2021.
//

#ifndef OPENVIII_CPP_WIP_KERNEL_CONCEPTS_HPP
#define OPENVIII_CPP_WIP_KERNEL_CONCEPTS_HPP

namespace open_viii::kernel {
#define HAS(function_name)                                                     \
  template<typename T>                                                         \
  concept has_##function_name = requires(T t)                                  \
  {                                                                            \
    t.function_name();                                                         \
  };                                                                           \
  template<typename T>                                                         \
  concept has_with_##function_name = has_##function_name<T> &&requires(T t)    \
  {                                                                            \
    t.with_##function_name(t.function_name());                                 \
  }
HAS(name_offset);
HAS(description_offset);
HAS(ability_data_id);
HAS(unknown_flags);
HAS(target);
HAS(magic_id);
HAS(attack_type);
HAS(attack_power);
HAS(attack_flags);
HAS(status_attack_enabler);
HAS(persistent_statuses);
HAS(battle_only_statuses);
HAS(attack_param);
HAS(hit_count);
HAS(element);
/**
 * Ability points required to unlock
 * @see
 * https://www.gamerguides.com/final-fantasy-viii/guide/guardian-forces/overview/ap-and-learning-abilities#learning-and-forgetting-abilities
 */
HAS(ability_points_required_to_unlock);
HAS(character_ability_flags);
HAS(crisis_level_hp_multiplier);
HAS(gender);
HAS(limit_break_id);
HAS(limit_break_param);
HAS(exp);
HAS(stats);
HAS(index_to_battle_command);
HAS(damage_or_heal);
HAS(percent_quantity);
HAS(devour_stat_flag);
HAS(raised_stat_hp_quantity);
HAS(camera_change);
HAS(attack_parameter);
HAS(enable_boost);
HAS(stat_to_increase);
HAS(increase_value);
HAS(element_attack_percent);
HAS(used_item_index);
HAS(critical_increase);
HAS(junction_flags);
HAS(gf_power);
HAS(gf_hp_modifier);
HAS(unknown);
HAS(unknown0);
HAS(unknown1);
HAS(unknown2);
HAS(unknown3);
HAS(unknown4);
HAS(unknown5);
HAS(unknown6);
HAS(unknown7);
HAS(unknown8);
HAS(unknown9);
HAS(unknown10);
HAS(unlockable_abilities);
HAS(compatibility);
HAS(power_mod);
HAS(level_mod);
#undef HAS
}// namespace open_viii::kernel
#endif// OPENVIII_CPP_WIP_KERNEL_CONCEPTS_HPP
