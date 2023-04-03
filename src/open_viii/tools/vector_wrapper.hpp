//
// Created by pcvii on 12/25/2022.
//

#ifndef OPENVIII_CPP_WIP_VECTOR_WRAPPER_HPP
#define OPENVIII_CPP_WIP_VECTOR_WRAPPER_HPP
#include <compare>
#include <cstdlib>
#include <ranges>
#include <vector>
namespace open_viii::tools {
template<typename T>
struct vector_wrapper
{
  std::vector<T> value;
  constexpr vector_wrapper() = default;
  constexpr vector_wrapper(std::vector<T> in_value) : value(std::move(in_value))
  {}
  constexpr decltype(auto)
    data()
  {
    return std::ranges::data(value);
  }
  constexpr decltype(auto)
    data() const
  {
    return std::ranges::data(value);
  }
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
  constexpr decltype(auto)
    size()
  {
    return std::ranges::size(value);
  }
  constexpr decltype(auto)
    size() const
  {
    return std::ranges::size(value);
  }

  constexpr void
    reserve(std::size_t res_size)
  {
    return value.reserve(res_size);
  }
  constexpr decltype(auto)
    at(std::size_t pos)
  {
    return value.at(pos);
  }
  constexpr decltype(auto)
    at(std::size_t pos) const
  {
    return value.at(pos);
  }
  template<typename... valuesT>
  constexpr decltype(auto)
    emplace_back(valuesT &&...values)
  {
    return value.emplace_back(std::forward<valuesT>(values)...);
  }
  constexpr std::strong_ordering
    operator<=>(const vector_wrapper<T> &other) const
  {
    auto       j  = value.begin();
    const auto je = value.end();
    auto       o  = other.begin();
    const auto oe = other.begin();
    for (; j != je && o != oe; (void)++j, ++o) {
      if (*j <=> *o != std::strong_ordering::equal) {
        return *j <=> *o;
      }
    }
    return std::strong_ordering::equal;
  }
  constexpr bool
    operator==(const vector_wrapper<T> &other) const
  {
    if (size() != other.size())
      return false;
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
#endif// OPENVIII_CPP_WIP_VECTOR_WRAPPER_HPP
