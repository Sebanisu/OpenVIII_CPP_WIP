//
// Created by pcvii on 2/15/2021.
//
#ifndef VIIIARCHIVE_COMPARE_HPP
#define VIIIARCHIVE_COMPARE_HPP
#include "Convert.hpp"
#include <algorithm>
#include <array>
#include <ranges>
#include <span>
namespace open_viii::tools {

    template<typename t1,typename t2>
    concept can_upper = requires(t1 c1, t2 c2)
    {
        upper(c1) == upper(c2);
    };

/**
 * Check if two characters are equal case insensitive
 * @param ch1 character 1
 * @param ch2 character 2
 * @return true if both are the same or after to uppercase are both same.
 */
constexpr static auto TOUPPER_EQUALS_PREDICATE = [](const auto &ch1,
                                                    const auto &ch2) -> bool {
  if constexpr (can_upper<decltype(ch1),decltype(ch2)>) {
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
[[maybe_unused]] constexpr static auto
  i_equals = [](const std::string_view str1, const std::string_view str2) ->bool
{
  return std::ranges::equal(str1, str2, TOUPPER_EQUALS_PREDICATE);
};
static_assert(i_equals(std::string_view(""), std::string_view("")));
static_assert(i_equals(std::string_view("ab"), "AB"));
static_assert(i_equals(std::string_view("ab"), std::string_view("AB")));
static_assert(i_equals(std::string_view("abC"), std::string_view("ABC")));
static_assert(i_equals(std::string_view("123"), std::string_view("123")));
static_assert(!i_equals(std::string_view(""), std::string_view("a")));
static_assert(!i_equals(std::string_view("abd"), std::string_view("ABC")));
static_assert(!i_equals(std::string_view("123"), std::string_view("124")));
/**
 * Find needle in span of chars haystack
 * @param haystack
 * @param needle
 * @return true if both values
 */
static constexpr auto i_find = [](const std::string_view haystack,
                                  const std::string_view needle) -> bool {
  if (std::ranges::empty(haystack) || std::ranges::empty(needle)
      || std::ranges::size(haystack) < std::ranges::size(needle)) {
    return false;
  }
  // clang tidy wants to make this a pointer. Bad idea.
  // it's not a pointer in msvc.
  const auto last = std::search(haystack.begin(),
                                haystack.end(),
                                needle.begin(),
                                needle.end(),
                                TOUPPER_EQUALS_PREDICATE);
  return last != haystack.end();
};
static_assert(i_find(std::string_view("abc"), std::string_view("b")));
static_assert(i_find(std::string_view("abc"), "b"));
static_assert(!i_find(std::string_view("abc"), std::string_view("")));
static_assert(!i_find(std::string_view(""), std::string_view("")));
static_assert(!i_find(std::string_view(""), std::string_view("b")));
static_assert(!i_find(std::string_view("abc"), std::string_view("0")));
/**
 * see if starting is at beginning of haystack case insensitive
 * @param haystack
 * @param starting needle to be at start of string
 * @return if the start of haystack contains starting return true
 */
static constexpr auto i_starts_with =
  [](const std::string_view haystack, const std::string_view starting) -> bool {
  return std::ranges::size(haystack) >= std::ranges::size(starting)
         && i_equals(starting, haystack.substr(0, std::ranges::size(starting)));
};
static_assert(i_starts_with(std::string_view("12345"), std::string_view("1")));
static_assert(i_starts_with(std::string_view("12345"), "1"));
static_assert(!i_starts_with(std::string_view("12345"), std::string_view("5")));
/**
 * see if ending is at end of haystack case insensitive
 * @param haystack
 * @param ending needle to be at end of string
 * @return if the start of haystack contains ending return true
 */
static constexpr auto i_ends_with = [](const std::string_view haystack,
                                       const std::string_view ending) -> bool {
  return i_starts_with(haystack.substr(std::ranges::size(haystack)
                                       - std::min(std::ranges::size(haystack),
                                                  std::ranges::size(ending))),
                       ending);
};
static_assert(i_ends_with(
  std::string_view("ff8/Data/eng/FIELD/model/main_chr/d000.mch"), ".mch"));
static_assert(i_ends_with(std::string_view("12345"), std::string_view("5")));
static_assert(!i_ends_with(std::string_view("12345"), std::string_view("1")));
/**
 * Generic with any, returns true if the lambda ever returns true.
 * @param haystack
 * @param needles
 * @param lambda
 * @return
 */
[[maybe_unused]] [[nodiscard]] static constexpr bool
  with_any(const std::string_view        haystack,
           const std::ranges::range auto needles,
           const auto &                  lambda)
{
  return std::ranges::empty(needles)
         || std::ranges::any_of(
           needles,
           [&lambda, &haystack](const std::string_view needle) -> bool {
             return lambda(haystack, needle);
           });
}
/**
 * run i_find till with multiple needles till it finds a match
 * @param haystack
 * @param needles range of needles
 * @return true if it finds a match
 */
[[maybe_unused]] [[nodiscard]] static constexpr bool
  i_find_any(const std::string_view        haystack,
             const std::ranges::range auto needles)
{
  return with_any(haystack, needles, i_find);
}
static_assert(i_find_any(std::string_view("haystack.a"),
                         std::array{ std::string_view(".a"),
                                     std::string_view(".b") }));
static_assert(!i_find_any(std::string_view("haystack.d"),
                          std::array{ std::string_view(".a"),
                                      std::string_view(".b") }));
/**
 * run i_starts till with multiple needles till it finds a match
 * @param haystack
 * @param needles range of needles
 * @return true if it finds a match
 */
[[maybe_unused]] [[nodiscard]] static constexpr bool
  i_starts_with_any(const std::string_view        haystack,
                    const std::ranges::range auto needles)
{
  return with_any(haystack, needles, i_starts_with);
}
static_assert(i_starts_with_any(std::string_view("haystack.a"),
                                std::array{ std::string_view(".a"),
                                            std::string_view("h") }));
static_assert(i_starts_with_any(std::string_view("haystack.a"),
                                std::array{ ".a", "h" }));
static_assert(!i_starts_with_any(std::string_view("haystack.d"),
                                 std::array{ std::string_view(".d"),
                                             std::string_view(".b") }));
/**
 * run i_starts till with multiple needles till it finds a match
 * @param haystack
 * @param needles range of needles
 * @return true if it finds a match
 */
[[maybe_unused]] [[nodiscard]] static constexpr bool
  i_ends_with_any(const std::string_view        haystack,
                  const std::ranges::range auto needles)
{
  return with_any(haystack, needles, i_ends_with);
}
static_assert(i_ends_with_any(std::string_view("haystack.d"),
                              std::array{ std::string_view(".d"),
                                          std::string_view(".b") }));
static_assert(i_ends_with_any(std::string_view("haystack.d"),
                              std::array{ ".d", ".b" }));
static_assert(!i_ends_with_any(std::string_view("haystack.a"),
                               std::array{ std::string_view(".d"),
                                           std::string_view("h") }));
/**
 * get the offset where it finds a match.
 * @param haystack
 * @param needles
 * @param lambda
 * @return return offset +1 or 0. 0 is no match.
 */
[[maybe_unused]] [[nodiscard]] static constexpr std::size_t
  with_any_get_offset(const std::string_view        haystack,
                      const std::ranges::range auto needles,
                      const auto &                  lambda)
{
  std::size_t i = 0U;
  return with_any(haystack,
                  needles,
                  [&lambda, &i](const std::string_view inside_haystack,
                                const std::string_view needle) {
                    ++i;
                    return lambda(inside_haystack, needle);
                  })
           ? i
           : 0U;
}
static_assert(with_any_get_offset(std::string_view("haystack.a"),
                                  std::array{ std::string_view(".a"),
                                              std::string_view(".b") },
                                  i_find)
              == 1U);
static_assert(with_any_get_offset(std::string_view("haystack.a"),
                                  std::array{ ".a", ".b" },
                                  i_find)
              == 1U);
/**
 * run i_find till with multiple needles till it finds a match
 * @param haystack
 * @param needles range of needles
 * @return true if it finds a match
 */
[[maybe_unused]] [[nodiscard]] static constexpr std::size_t
  i_find_any_get_offset(const std::string_view        haystack,
                        const std::ranges::range auto needles)
{
  return with_any_get_offset(haystack, needles, i_find);
}
static_assert(i_find_any_get_offset(std::string_view("haystack.a"),
                                    std::array{ std::string_view(".a"),
                                                std::string_view(".b") })
              == 1U);
static_assert(i_find_any_get_offset(std::string_view("haystack.a"),
                                    std::array{ ".a", ".b" })
              == 1U);
static_assert(i_find_any_get_offset(std::string_view("haystack.a"),
                                    std::array{ std::string_view(".b"),
                                                std::string_view(".a") })
              == 2U);
static_assert(i_find_any_get_offset(std::string_view("haystack.a"),
                                    std::array{ std::string_view("0"),
                                                std::string_view("1") })
              == 0U);
static_assert(i_find_any_get_offset(std::string_view("haystack.a"),
                                    std::initializer_list<std::string_view>{})
              == 0U);
/**
 * run i_starts till with multiple needles till it finds a match
 * @param haystack
 * @param needles range of needles
 * @return true if it finds a match
 */
[[maybe_unused]] [[nodiscard]] static constexpr std::size_t
  i_starts_with_any_get_offset(const std::string_view        haystack,
                               const std::ranges::range auto needles)
{
  return with_any_get_offset(haystack, needles, i_starts_with);
}
static_assert(i_starts_with_any_get_offset(std::string_view("haystack.a"),
                                           std::array{ std::string_view(".a"),
                                                       std::string_view("h") })
              == 2U);
static_assert(i_starts_with_any_get_offset(std::string_view("haystack.a"),
                                           std::array{ ".a", "h" })
              == 2U);
static_assert(i_starts_with_any_get_offset(std::string_view("haystack.d"),
                                           std::array{ std::string_view(".d"),
                                                       std::string_view(".b") })
              == 0U);
/**
 * run i_starts till with multiple needles till it finds a match
 * @param haystack
 * @param needles range of needles
 * @return true if it finds a match
 */
[[maybe_unused]] [[nodiscard]] static constexpr std::size_t
  i_ends_with_any_get_offset(const std::string_view        haystack,
                             const std::ranges::range auto needles)
{
  return with_any_get_offset(haystack, needles, i_ends_with);
}
static_assert(i_ends_with_any_get_offset(std::string_view("haystack.d"),
                                         std::array{ std::string_view(".d"),
                                                     std::string_view(".b") })
              == 1U);
static_assert(i_ends_with_any_get_offset(std::string_view("haystack.d"),
                                         std::array{ ".d", ".b" })
              == 1U);
static_assert(i_ends_with_any_get_offset(std::string_view("haystack.b"),
                                         std::array{ std::string_view(".d"),
                                                     std::string_view(".b") })
              == 2U);
/**
 * Generic with ALL, returns true if the lambda always returns true.
 * @param haystack
 * @param needles
 * @param lambda
 * @return
 */
[[maybe_unused]] [[nodiscard]] static constexpr bool
  with_all(const std::string_view        haystack,
           const std::ranges::range auto needles,
           const auto &                  lambda)
{
  return std::ranges::empty(needles)
         || std::ranges::all_of(
           needles,
           [&lambda, &haystack](const std::string_view needle) -> bool {
             return lambda(haystack, needle);
           });
}
/**
 * run i_find till with multiple needles till it finds a match
 * @param haystack
 * @param needles range of needles
 * @return true if it finds a match
 */
[[maybe_unused]] [[nodiscard]] static constexpr bool
  i_find_all(const std::string_view        haystack,
             const std::ranges::range auto needles)
{
  return with_all(haystack, needles, i_find);
}
static_assert(i_find_all(std::string_view("haystack.a"),
                         std::array{ std::string_view(".a"),
                                     std::string_view("stack") }));
static_assert(i_find_all(std::string_view("haystack.a"),
                         std::array{ ".a", "stack" }));
static_assert(!i_find_all(std::string_view("haystack.d"),
                          std::array{ std::string_view(".a"),
                                      std::string_view(".b") }));
/**
 * run i_starts till with multiple needles till it finds a match
 * @param haystack
 * @param needles range of needles
 * @return true if it finds a match
 */
[[maybe_unused]] [[nodiscard]] static constexpr bool
  i_starts_with_all(const std::string_view        haystack,
                    const std::ranges::range auto needles)
{
  return with_all(haystack, needles, i_starts_with);
}
static_assert(i_starts_with_all(std::string_view("haystack.a"),
                                std::array{ std::string_view("hay"),
                                            std::string_view("haystack") }));
static_assert(i_starts_with_all(std::string_view("haystack.a"),
                                std::array{ "hay", "haystack" }));
static_assert(!i_starts_with_all(std::string_view("haystack.d"),
                                 std::array{ std::string_view(".d"),
                                             std::string_view(".b") }));
/**
 * run i_starts till with multiple needles till it finds a match
 * @param haystack
 * @param needles range of needles
 * @return true if it finds a match
 */
[[maybe_unused]] [[nodiscard]] static constexpr bool
  i_ends_with_all(const std::string_view        haystack,
                  const std::ranges::range auto needles)
{
  return with_all(haystack, needles, i_ends_with);
}
static_assert(i_ends_with_all(std::string_view("haystack.d"),
                              std::array{ std::string_view(".d"),
                                          std::string_view("stack.d") }));
static_assert(i_ends_with_all(std::string_view("haystack.d"),
                              std::array{ ".d", "stack.d" }));
static_assert(!i_ends_with_all(std::string_view("haystack.a"),
                               std::array{ std::string_view(".d"),
                                           std::string_view("h") }));
constexpr auto search(const std::span<const char> haystack,
                      const std::span<const char> needle)
{
  if (std::ranges::empty(needle) || std::ranges::empty(haystack))
    return std::ranges::end(haystack);
  return std::search(std::ranges::begin(haystack),
                     std::ranges::end(haystack),
                     std::ranges::begin(needle),
                     std::ranges::end(needle));
}
constexpr auto search(const std::span<const char> haystack, const char *needle)
{
  return search(haystack, std::string_view(needle));
}
constexpr bool search_bool(const std::span<const char> haystack,
                           const std::span<const char> needle)
{
  return search(haystack, needle) != std::ranges::end(haystack);
}
constexpr auto search_bool(const std::span<const char> haystack,
                           const char *                needle)
{
  return search_bool(haystack, std::string_view(needle));
}
static_assert(search_bool(std::string_view("abc"), std::string_view("b")));
static_assert(search_bool(std::string_view("abc"), "b"));
static_assert(search_bool("abc", std::string_view("b")));
static_assert(!search_bool(std::string_view("abc"), std::string_view("")));
static_assert(!search_bool(std::string_view(""), std::string_view("")));
static_assert(!search_bool(std::string_view(""), std::string_view("b")));
static_assert(!search_bool(std::string_view("abc"), std::string_view("0")));
}// namespace open_viii::tools
#endif// VIIIARCHIVE_COMPARE_HPP
