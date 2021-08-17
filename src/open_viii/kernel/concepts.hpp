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
HAS(spell_power);
HAS(draw_resist);
HAS(status_attack);
HAS(junction_stats);
HAS(j_elem_attack_flag);
HAS(j_elem_attack_value);
HAS(j_elem_defense_flag);
HAS(j_elem_defense_value);
HAS(j_status_attack_value);
HAS(j_status_defense_value);
HAS(j_statuses_attack_flag);
HAS(j_statuses_defend_flag);
HAS(menu_file_index);
HAS(start_offset);
HAS(end_offset);
HAS(sleep_timer);
HAS(haste_timer);
HAS(slow_timer);
HAS(stop_timer);
HAS(regen_timer);
HAS(protect_timer);
HAS(shell_timer);
HAS(reflect_timer);
HAS(aura_timer);
HAS(curse_timer);
HAS(doom_timer);
HAS(invincible_timer);
HAS(petrifying_timer);
HAS(float_timer);
HAS(atb_speed_multiplier);
HAS(dead_timer);
HAS(death_limit_effect);
HAS(poison_limit_effect);
HAS(petrify_limit_effect);
HAS(darkness_limit_effect);
HAS(silence_limit_effect);
HAS(berserk_limit_effect);
HAS(zombie_limit_effect);
HAS(unknown_status_limit_effect);
HAS(sleep_limit_effect);
HAS(haste_limit_effect);
HAS(slow_limit_effect);
HAS(stop_limit_effect);
HAS(regen_limit_effect);
HAS(protect_limit_effect);
HAS(shell_limit_effect);
HAS(reflect_limit_effect);
HAS(aura_limit_effect);
HAS(curse_limit_effect);
HAS(doom_limit_effect);
HAS(invincible_limit_effect);
HAS(petrifying_limit_effect);
HAS(float_limit_effect);
HAS(confusion_limit_effect);
HAS(drain_limit_effect);
HAS(eject_limit_effect);
HAS(double_limit_effect);
HAS(triple_limit_effect);
HAS(defend_limit_effect);
HAS(unknown_status_limit_effect0);
HAS(unknown_status_limit_effect1);
HAS(charged_limit_effect);
HAS(back_attack_limit_effect);
HAS(duel_timers);
HAS(shot_timers);
HAS(status_flags);
HAS(flag);
HAS(critical_bonus);
HAS(blue_magic_data);
HAS(sequence);
HAS(timer);
HAS(count);
HAS(increased_value);
HAS(unlocker);
HAS(ability);
HAS(renzokuken_finishers);
HAS(character_id);
HAS(str_bonus);
HAS(weapon_tier);
HAS(melee_weapon);
HAS(button_sequence);
#undef HAS
}// namespace open_viii::kernel
#endif// OPENVIII_CPP_WIP_KERNEL_CONCEPTS_HPP
