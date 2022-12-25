//
// Created by pcvii on 12/25/2022.
//

#ifndef OPENVIII_CPP_WIP_ARRAY_WRAPPER_HPP
#define OPENVIII_CPP_WIP_ARRAY_WRAPPER_HPP
#include <array>
#include <compare>
#include <cstdlib>
#include <ranges>
namespace open_viii::tools {
template<typename T, size_t size_value>
struct array_wrapper
{
  std::array<T, size_value> value;
  constexpr decltype(auto)
    begin()
  {
    return std::ranges::begin(value);
  }
  constexpr decltype(auto)
    begin() const
  {
    return std::ranges::begin(value);
  }
  constexpr decltype(auto)
    end()
  {
    return std::ranges::end(value);
  }
  constexpr decltype(auto)
    end() const
  {
    return std::ranges::end(value);
  }
  static constexpr decltype(auto)
    size()
  {
    return size_value;
  }
  constexpr std::strong_ordering
    operator<=>(const array_wrapper<T, size_value> &other) const
  {
    auto j = value.begin();
    auto o = other.begin();
    for (; j != value.end(); (void)++j, ++o) {
      if (*j <=> *o != std::strong_ordering::equal) {
        return *j <=> *o;
      }
    }
    return std::strong_ordering::equal;
  }
  constexpr bool
    operator==(const array_wrapper<T, size_value> &other) const
  {
    auto j = value.begin();
    auto o = other.begin();
    for (; j != value.end(); (void)++j, ++o) {
      if (*j != *o) {
        return false;
      }
    }
    return true;
  }
};
}// namespace open_viii::tools
#endif// OPENVIII_CPP_WIP_ARRAY_WRAPPER_HPP
