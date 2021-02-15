//
// Created by pcvii on 2/15/2021.
//
#ifndef VIIIARCHIVE_COMPARE_HPP
#define VIIIARCHIVE_COMPARE_HPP
#include "Convert.hpp"
#include <span>
#include <ranges>
#include <algorithm>

namespace open_viii::tools {

/**
 * Check if two characters are equal case insensitive
 * @param ch1 character 1
 * @param ch2 character 2
 * @return true if both are the same or after to uppercase are both same.
 */
constexpr static auto TOUPPER_EQUALS_PREDICATE = [](const auto &ch1,
                                                    const auto &ch2) -> bool {
  if constexpr (requires(decltype(ch1) c1, decltype(ch2) c2) {
                  upper(c1) == upper(c2);
                }) {
    return upper(ch1) == upper(ch2);
  } else {
    return ch1 == ch2;
  }
};
static_assert(TOUPPER_EQUALS_PREDICATE('a', 'A'));
static_assert(TOUPPER_EQUALS_PREDICATE('1', '1'));
static_assert(!TOUPPER_EQUALS_PREDICATE('1', '2'));
static_assert(!TOUPPER_EQUALS_PREDICATE('a', 'Z'));
/**
 * Case Insensitive strings equal
 * @param str1 string 1
 * @param str2 string 2
 * @return returns true if both strings are equal regardless of case
 */
[[maybe_unused]] [[nodiscard]] constexpr static bool
i_equals(const std::span<const char> str1, const std::span<const char> str2)
{
  return std::ranges::equal(str1, str2, TOUPPER_EQUALS_PREDICATE);
}
static_assert(i_equals( std::string_view (""), std::string_view ("")));
static_assert(i_equals( std::string_view ("ab"), std::string_view ("AB")));
static_assert(i_equals( std::string_view ("abC"), std::string_view ("ABC")));
static_assert(i_equals( std::string_view ("123"), std::string_view ("123")));
static_assert(!i_equals( std::string_view (""), std::string_view ("a")));
static_assert(!i_equals( std::string_view ("abd"), std::string_view ("ABC")));
static_assert(!i_equals( std::string_view ("123"), std::string_view ("124")));
}
#endif// VIIIARCHIVE_COMPARE_HPP
