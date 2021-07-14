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
}// namespace open_viii::kernel
#endif// OPENVIII_CPP_WIP_KERNEL_CONCEPTS_HPP
