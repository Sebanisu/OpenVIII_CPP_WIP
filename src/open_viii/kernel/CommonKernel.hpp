//
// Created by pcvii on 7/13/2021.
//

#ifndef OPENVIII_CPP_WIP_COMMONKERNEL_HPP
#define OPENVIII_CPP_WIP_COMMONKERNEL_HPP
#include <compare>
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
struct CommonKernel : T
{
public:
  using this_type = CommonKernel<T>;
  CommonKernel()  = default;
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
};
}// namespace open_viii::kernel
#endif// OPENVIII_CPP_WIP_COMMONKERNEL_HPP
