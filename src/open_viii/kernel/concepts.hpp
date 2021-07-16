//
// Created by pcvii on 7/13/2021.
//

#ifndef OPENVIII_CPP_WIP_KERNEL_CONCEPTS_HPP
#define OPENVIII_CPP_WIP_KERNEL_CONCEPTS_HPP

namespace open_viii::kernel {
template<typename T>
concept has_name_offset = requires(T t)
{
  t.name_offset();
};
template<typename T>
concept has_description_offset = requires(T t)
{
  t.description_offset();
};
template<typename T>
concept has_ability_data_id = requires(T t)
{
  t.ability_data_id();
};
template<typename T>
concept has_unknown_flags = requires(T t)
{
  t.unknown_flags();
};
template<typename T>
concept has_target = requires(T t)
{
  t.target();
};
template<typename T>
concept has_unknown = requires(T t)
{
  t.unknown();
};
template<typename T>
concept has_magic_id = requires(T t)
{
  t.magic_id();
};
template<typename T>
concept has_attack_type = requires(T t)
{
  t.attack_type();
};
template<typename T>
concept has_attack_power = requires(T t)
{
  t.attack_power();
};
template<typename T>
concept has_unknown0 = requires(T t)
{
  t.unknown0();
};
template<typename T>
concept has_unknown1 = requires(T t)
{
  t.unknown1();
};
template<typename T>
concept has_attack_flags = requires(T t)
{
  t.attack_flags();
};
template<typename T>
concept has_unknown2 = requires(T t)
{
  t.unknown2();
};
template<typename T>
concept has_status_attack_enabler = requires(T t)
{
  t.status_attack_enabler();
};
template<typename T>
concept has_persistent_statuses = requires(T t)
{
  t.persistent_statuses();
};
template<typename T>
concept has_battle_only_statuses = requires(T t)
{
  t.battle_only_statuses();
};
template<typename T>
concept has_attack_param = requires(T t)
{
  t.attack_param();
};
template<typename T>
concept has_unknown3 = requires(T t)
{
  t.unknown3();
};
template<typename T>
concept has_hit_count = requires(T t)
{
  t.hit_count();
};
template<typename T>
concept has_element = requires(T t)
{
  t.element();
};
template<typename T>
/**
 * Ability points required to unlock
 * @see
 * https://www.gamerguides.com/final-fantasy-viii/guide/guardian-forces/overview/ap-and-learning-abilities#learning-and-forgetting-abilities
 */
concept has_ability_points_required_to_unlock = requires(T t)
{
  t.ability_points_required_to_unlock();
};
template<typename T>
concept has_character_ability_flags = requires(T t)
{
  t.character_ability_flags();
};
template<typename T>
concept has_crisis_level_hp_multiplier = requires(T t)
{
  t.crisis_level_hp_multiplier();
};
template<typename T>
concept has_gender = requires(T t)
{
  t.gender();
};
template<typename T>
concept has_limit_break_id = requires(T t)
{
  t.limit_break_id();
};
template<typename T>
concept has_limit_break_param = requires(T t)
{
  t.limit_break_param();
};
template<typename T>
concept has_exp = requires(T t)
{
  t.exp();
};
template<typename T>
concept has_stats = requires(T t)
{
  t.stats();
};
template<typename T>
concept has_index_to_battle_command = requires(T t)
{
  t.index_to_battle_command();
};
template<typename T>
concept has_damage_or_heal = requires(T t)
{
  t.damage_or_heal();
};
template<typename T>
concept has_percent_quantity = requires(T t)
{
  t.percent_quantity();
};
template<typename T>
concept has_devour_stat_flag = requires(T t)
{
  t.devour_stat_flag();
};
template<typename T>
concept has_raised_stat_hp_quantity = requires(T t)
{
  t.raised_stat_hp_quantity();
};
template<typename T>
concept has_camera_change = requires(T t)
{
  t.camera_change();
};
template<typename T>
concept has_attack_parameter = requires(T t)
{
  t.attack_parameter();
};
template<typename T>
concept has_enable_boost = requires(T t)
{
  t.enable_boost();
};
template<typename T>
concept has_stat_to_increase = requires(T t)
{
  t.stat_to_increase();
};
template<typename T>
concept has_increase_value = requires(T t)
{
  t.increase_value();
};
template<typename T>
concept has_element_attack_percent = requires(T t)
{
  t.element_attack_percent();
};
template<typename T>
concept has_used_item_index = requires(T t)
{
  t.used_item_index();
};
template<typename T>
concept has_critical_increase = requires(T t)
{
  t.critical_increase();
};
template<typename T>
concept has_junction_flags = requires(T t)
{
  t.junction_flags();
};
template<typename T>
concept has_gf_power = requires(T t)
{
  t.gf_power();
};
template<typename T>
concept has_gf_hp_modifier = requires(T t)
{
  t.gf_hp_modifier();
};
template<typename T>
concept has_unknown4 = requires(T t)
{
  t.unknown4();
};
template<typename T>
concept has_unknown5 = requires(T t)
{
  t.unknown5();
};
template<typename T>
concept has_unknown6 = requires(T t)
{
  t.unknown6();
};
template<typename T>
concept has_unknown7 = requires(T t)
{
  t.unknown7();
};
template<typename T>
concept has_unknown8 = requires(T t)
{
  t.unknown8();
};
template<typename T>
concept has_unknown9 = requires(T t)
{
  t.unknown9();
};
template<typename T>
concept has_unknown10 = requires(T t)
{
  t.unknown10();
};
template<typename T>
concept has_unlockable_abilities = requires(T t)
{
  t.unlockable_abilities();
};
template<typename T>
concept has_compatibility = requires(T t)
{
  t.compatibility();
};
template<typename T>
concept has_power_mod = requires(T t)
{
  t.power_mod();
};
template<typename T>
concept has_level_mod = requires(T t)
{
  t.level_mod();
};
}// namespace open_viii::kernel
#endif// OPENVIII_CPP_WIP_KERNEL_CONCEPTS_HPP
