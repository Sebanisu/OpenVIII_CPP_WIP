//
// Created by pcvii on 7/13/2021.
//

#ifndef OPENVIII_CPP_WIP_COMMONKERNEL_HPP
#define OPENVIII_CPP_WIP_COMMONKERNEL_HPP
#include <compare>
#include "concepts.hpp"
namespace open_viii::kernel {
template<typename T>
struct CommonKernel : T
{
public:
  using this_type = CommonKernel<T>;
  constexpr CommonKernel() = default;
  using T::EXPECTED_SIZE;
  constexpr auto
    operator<=>(const this_type &right) const noexcept = default;
  [[maybe_unused]] [[nodiscard]] constexpr auto
    name_offset() const noexcept
    requires(requires(this_type t) { t.m_name_offset; })
  {
    return T::m_name_offset;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    description_offset() const noexcept
    requires(requires(this_type t) { t.m_description_offset; })
  {
    return T::m_description_offset;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    ability_data_id() const noexcept
    requires(requires(this_type t) { t.m_ability_data_id; })
  {
    return T::m_ability_data_id;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    unknown_flags() const noexcept
    requires(requires(this_type t) { t.m_unknown_flags; })
  {
    return T::m_unknown_flags;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    target() const noexcept requires(requires(this_type t) { t.m_target; })
  {
    return T::m_target;
  }
  [[nodiscard]] constexpr auto
    unknown() const noexcept requires(requires(this_type t) { t.m_unknown; })
  {
    return T::m_unknown;
  }
  [[nodiscard]] constexpr auto
    magic_id() const noexcept requires(requires(this_type t) { t.m_magic_id; })
  {
    return T::m_magic_id;
  }
  [[nodiscard]] constexpr auto
    attack_type() const noexcept
    requires(requires(this_type t) { t.m_attack_type; })
  {
    return T::m_attack_type;
  }
  [[nodiscard]] constexpr auto
    attack_power() const noexcept
    requires(requires(this_type t) { t.m_attack_power; })
  {
    return T::m_attack_power;
  }
  [[nodiscard]] constexpr auto
    unknown0() const noexcept requires(requires(this_type t) { t.m_unknown0; })
  {
    return T::m_unknown0;
  }
  [[nodiscard]] constexpr auto
    unknown1() const noexcept requires(requires(this_type t) { t.m_unknown1; })
  {
    return T::m_unknown1;
  }
  [[nodiscard]] constexpr auto
    attack_flags() const noexcept
    requires(requires(this_type t) { t.m_attack_flags; })
  {
    return T::m_attack_flags;
  }
  [[nodiscard]] constexpr auto
    unknown2() const noexcept requires(requires(this_type t) { t.m_unknown2; })
  {
    return T::m_unknown2;
  }
  [[nodiscard]] constexpr auto
    status_attack_enabler() const noexcept
    requires(requires(this_type t) { t.m_status_attack_enabler; })
  {
    return T::m_status_attack_enabler;
  }
  [[nodiscard]] constexpr auto
    persistent_statuses() const noexcept
    requires(requires(this_type t) { t.m_persistent_statuses; })
  {
    return T::m_persistent_statuses;
  }// statuses 0-7
  [[nodiscard]] constexpr auto
    battle_only_statuses() const noexcept
    requires(requires(this_type t) { t.m_battle_only_statuses; })
  {
    return T::m_battle_only_statuses;
  }// statuses 8-39
  [[nodiscard]] constexpr auto
    attack_param() const noexcept
    requires(requires(this_type t) { t.m_attack_param; })
  {
    return T::m_attack_param;
  }
  [[nodiscard]] constexpr auto
    unknown3() const noexcept requires(requires(this_type t) { t.m_unknown3; })
  {
    return T::m_unknown3;
  }
  [[nodiscard]] constexpr auto
    hit_count() const noexcept
    requires(requires(this_type t) { t.m_hit_count; })
  {
    return T::m_hit_count;
  }
  [[nodiscard]] constexpr auto
    element() const noexcept requires(requires(this_type t) { t.m_element; })
  {
    return T::m_element;
  }
};
}// namespace open_viii::kernel
#endif// OPENVIII_CPP_WIP_COMMONKERNEL_HPP
