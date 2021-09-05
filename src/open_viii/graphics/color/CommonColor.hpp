//
// Created by pcvii on 7/3/2021.
//

#ifndef OPENVIII_CPP_WIP_COMMONCOLOR_HPP
#define OPENVIII_CPP_WIP_COMMONCOLOR_HPP
#include "ColorLayoutT.hpp"
#include "open_viii/Concepts.hpp"
namespace open_viii::graphics {
template<typename T>
struct CommonColor : T
{
private:
  template<std::size_t index, Color cT>
  constexpr std::uint8_t
    swap_index(const cT &color) const noexcept
  {
    if constexpr (index == red_index) {
      return color.r();
    }
    else if constexpr (index == green_index) {
      return color.g();
    }
    else if constexpr (index == blue_index) {
      return color.b();
    }
    else// if constexpr(index == alpha_index)
    {
      return color.a();
    }
  }
  using T::operator[];
  using T::alpha_size;
  using T::value;
  using T::with;

public:
  using T::current_layout;

private:
  static constexpr auto red_index   = get_index<current_layout, 2U, 0U, 1U>();
  static constexpr auto green_index = get_index<current_layout, 1U, 1U, 1U>();
  static constexpr auto blue_index  = get_index<current_layout, 0U, 2U, 1U>();
  static constexpr auto alpha_index = get_index<current_layout>(0U, 3U);

public:
  using T::EXPECTED_SIZE;
  using T::has_alpha;
  using this_type         = CommonColor<T>;
  constexpr CommonColor() = default;
  template<typename... Ts>
  requires(std::integral<std::decay_t<Ts>>
             &&...) constexpr explicit CommonColor(Ts &&...ts)
    : T{ std::forward<Ts>(ts)... }
  {}
  template<Color cT>
  requires(
    has_alpha
    && !std::is_same_v<std::decay_t<cT>,
                       this_type>) constexpr explicit CommonColor(cT &&color)
    : CommonColor{ swap_index<0>(color),
                   swap_index<1>(color),
                   swap_index<2>(color),
                   swap_index<3>(color) }
  {}
  template<Color cT>
  requires(
    !has_alpha
    && !std::is_same_v<std::decay_t<cT>,
                       this_type>) constexpr explicit CommonColor(cT &&color)
    : CommonColor{ swap_index<0>(color),
                   swap_index<1>(color),
                   swap_index<2>(color) }
  {}
  constexpr std::uint8_t
    r() const noexcept
  {
    return operator[](red_index);
  }

  constexpr std::uint8_t
    b() const noexcept
  {
    return operator[](blue_index);
  }

  constexpr std::uint8_t
    g() const noexcept
  {
    return operator[](green_index);
  }

  constexpr std::uint8_t
    a() const noexcept
  {
    if constexpr (has_alpha) {
      if constexpr (alpha_size == 1U) {
        if (operator[](red_index) == 0 && operator[](green_index) == 0
            &&                            operator[](blue_index) == 0)
          return 0U;
        else
          return (std::numeric_limits<std::uint8_t>::max)();
      }
      else
        return operator[](alpha_index);
    }
    else {
      return (std::numeric_limits<std::uint8_t>::max)();
    }
  }
  friend std::ostream &
    operator<<(std::ostream &os, const this_type &color)
  {
    return os << std::uppercase << std::hex << '{' << +color.r() << ", "
              << +color.g() << ", " << +color.b() << ", " << +color.a() << '}'
              << std::dec << std::nouppercase;
  }
  //   friend auto
  //   operator<=>(const this_type &left,
  //               const this_type &right) noexcept = default;
  //   auto
  //   operator<=>(const this_type &right) const noexcept = default;
  constexpr bool
    is_black() const noexcept
  {
    return r() == 0 && g() == 0 && b() == 0;
  }
  constexpr this_type
    with_r(const std::uint8_t change) const noexcept
  {
    this_type tmp{};
    tmp.value = with(red_index, change);
    return tmp;
  }
  constexpr this_type
    with_g(const std::uint8_t change) const noexcept
  {
    this_type tmp{};
    tmp.value = with(green_index, change);
    return tmp;
  }
  constexpr this_type
    with_b(const std::uint8_t change) const noexcept
  {
    this_type tmp{};
    tmp.value = with(blue_index, change);
    return tmp;
  }
  constexpr this_type
    with_a(const std::uint8_t change) const noexcept requires(has_alpha)
  {
    this_type tmp{};
    tmp.value = with(alpha_index, change);
    return tmp;
  }
  template<Color cT>
  constexpr auto
    operator<=>(const cT &rhs) const noexcept
  {
    // if constexpr (std::is_same_v<std::decay_t<cT>, this_type>) {
    //   if (auto cmp = std::compare_three_way{}(value, rhs.value); cmp == 0)
    //     return cmp;
    // }
    if (auto cmp = std::compare_three_way{}(r(), rhs.r()); cmp != 0)
      return cmp;
    if (auto cmp = std::compare_three_way{}(g(), rhs.g()); cmp != 0)
      return cmp;
    if (auto cmp = std::compare_three_way{}(b(), rhs.b()); cmp != 0)
      return cmp;
    return std::compare_three_way{}(a(), rhs.a());
  }

  constexpr auto
    operator==(const this_type &rhs) const noexcept
  {
    return value == rhs.value;
  }
  template<Color cT>
  constexpr auto
    operator==(const cT &rhs) const noexcept
  {
    return operator<=>(rhs) == 0;
  }

  constexpr auto
    operator!=(const this_type &rhs) const noexcept
  {
    return value != rhs.value;
  }
  template<Color cT>
  constexpr auto
    operator!=(const cT &rhs) const noexcept
  {
    return operator<=>(rhs) != 0;
  }
};
}// namespace open_viii::graphics
#endif// OPENVIII_CPP_WIP_COMMONCOLOR_HPP