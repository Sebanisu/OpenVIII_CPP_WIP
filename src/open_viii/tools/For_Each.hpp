//
// Created by pcvii on 2/15/2021.
//
#ifndef VIIIARCHIVE_FOR_EACH_HPP
#define VIIIARCHIVE_FOR_EACH_HPP
#include <concepts>
#include <iostream>
#include <ranges>
namespace open_viii::tools {
template<std::unsigned_integral intT, std::invocable<intT, intT> lambdaT>
static constexpr void
  for_each_xy(const intT max_x, const intT max_y, const lambdaT &lambda)
{
  constexpr intT start{};
  for (const auto y : std::views::iota(start, max_y)) {
    for (const auto x : std::views::iota(start, max_x)) {
      if constexpr (std::is_same_v<std::invoke_result_t<lambdaT, intT, intT>,
                                   bool>) {
        if (lambda(x, y)) {
          break;
        }
      } else {
        lambda(x, y);
      }
    }
  }
}
static consteval auto test_for_each_xy(std::size_t max_x, std::size_t max_y)
{
  std::size_t total{};
  for_each_xy(max_x, max_y, [&total](const auto x, const auto y) {
    total += x + y;
  });
  return total;
}
static_assert(test_for_each_xy(5U, 1U) == 10U);
static_assert(test_for_each_xy(1U, 5U) == 10U);
template<std::unsigned_integral intT, std::invocable<intT, intT> lambdaT>
static constexpr void for_each_xy(const intT &max_xy, const lambdaT &lambda)
{
  for_each_xy(max_xy, max_xy, lambda);
}
static consteval auto test_for_each_xy(std::size_t max_xy)
{
  std::size_t total{};
  for_each_xy(max_xy, [&total](const auto x, const auto y) {
    total += x + y;
  });
  return total;
}
static_assert(test_for_each_xy(5U) == 100U);
}// namespace open_viii::tools
#endif// VIIIARCHIVE_FOR_EACH_HPP
