//
// Created by pcvii on 2/15/2021.
//
#ifndef VIIIARCHIVE_CONVERT_HPP
#define VIIIARCHIVE_CONVERT_HPP
#include <algorithm>
#include <cctype>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
namespace open_viii::tools {
/**
 * Workaround there is no way to currently to print a utf8 string... streams are
 * char only pretty much.
 * @param s8 utf8 string_view
 * @return char string_view.
 * @todo needs tests?
 */
[[maybe_unused]] static std::string_view u8_to_sv(const std::u8string_view &s8)
{
  return { reinterpret_cast<const char *>(s8.data()), s8.size() };
}
/**
 * Workaround there is no way to currently to print a utf8 string... streams are
 * char only pretty much.
 * @param s8 utf8 string_view
 * @return char string
 * @todo needs tests?
 */
[[maybe_unused]] static std::string u8_to_s(const std::u8string_view &s8)
{
  auto sv = u8_to_sv(s8);
  return { sv.begin(), sv.end() };
}
/**
 * Convert character to uppercase version
 * @param ch lowercase character and such.
 * @return uppercase character
 * @note ::toupper is not constexpr
 * @see https://en.cppreference.com/w/cpp/string/byte/toupper
 */
static constexpr auto upper(int ch)
{
  if (std::is_constant_evaluated()) {
    constexpr char upper_offset = 'a' - 'A';
    // this is really basic but should cover everything we do.
    return (ch >= 'a' && ch <= 'z') ? ch - upper_offset : ch;
  }
  return ::toupper(ch);
}
static_assert(upper('a') == 'A');
static_assert(upper('a') != 'Z');
/**
 * if input char matches needle return replacement
 * @param input character
 * @param needle character
 * @param replacement character
 * @note windows uses a wchar_t instead of char. So I need to static cast to
 * make them match though might need at least one value to be char. I'm unsure
 * what would happen if we were in a unicode mode.
 */
template<std::integral needleType, std::integral replacementType>
requires(
  std::is_convertible_v<replacementType, needleType> &&std::is_convertible_v<
    needleType,
    replacementType>) static constexpr auto transform_char(const auto &input,
                                                           const needleType
                                                             &needle,
                                                           const replacementType
                                                             &replacement)
{
  return input == static_cast<decltype(input)>(needle)
           ? static_cast<decltype(input)>(replacement)
           : input;
}
static_assert(transform_char('\\', '\\', '/') == '/');
static_assert(transform_char('/', '\\', '/') == '/');
static_assert(transform_char('b', 'b', 'a') == 'a');
static_assert(transform_char('/', '/', '\\') != '/');
/**
 * Replace all of one char to another char.
 * @param haystack out parameter leading to the string value you want to change
 * @param needle char searched for
 * @param replacement new char value
 * @note needle and replacement will be static_cast to the type of string char
 * used in haystack. I use this to make the separator on windows or linux
 * matches the strings.
 * @todo add test because this can't be constexpr
 */
template<std::integral needleType, std::integral replacementType>
[[maybe_unused]] static void
  replace_all(const std::span<char>  haystack,
              const needleType &     needle,
              const replacementType &replacement) noexcept
{
  if (haystack.empty()) {
    return;
  }
  std::ranges::transform(haystack,
                         std::ranges::begin(haystack),
                         [needle, replacement](const auto input) {
                           transform_char(input, needle, replacement);
                         });
}
}// namespace open_viii::tools
#endif// VIIIARCHIVE_CONVERT_HPP
