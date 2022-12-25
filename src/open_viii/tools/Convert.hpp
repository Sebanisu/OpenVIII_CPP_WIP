//
// Created by pcvii on 2/15/2021.
//
#ifndef VIIIARCHIVE_CONVERT_HPP
#define VIIIARCHIVE_CONVERT_HPP
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <ranges>
#include <string>
namespace open_viii::tools {
/**
 * Workaround there is no way to currently to print a utf8 string... streams are
 * char only pretty much.
 * @param s8 utf8 string_view
 * @return char string_view.
 * @todo needs tests?
 */
[[maybe_unused]] [[nodiscard]] inline std::string_view
  u8_to_sv(const std::u8string_view &s8)
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
[[maybe_unused]] [[nodiscard]] inline std::string
  u8_to_s(const std::u8string_view &s8)
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
static constexpr auto upper = [](auto ch) {
  if (std::is_constant_evaluated()) {
    constexpr char upper_offset = 'a' - 'A';
    // this is really basic but should cover everything we do.
    return static_cast<decltype(ch)>(
      (ch >= 'a' && ch <= 'z') ? ch - upper_offset : ch);
  }
  return static_cast<decltype(ch)>(::toupper(ch));
};
static_assert(upper('a') == 'A');
static_assert(upper('a') != 'Z');
/**
 * use std::to_string and pad the value.
 * @param value initial integer value.
 * @param total_length default {}, sets the desired length
 * @param pad_character default 0
 * @return string value of number padded.
 * @see
 * https://stackoverflow.com/questions/53475501/how-to-zero-pre-fill-for-stdto-string-function
 * @todo add tests
 */
template<std::integral intT>
[[nodiscard]] inline std::string
  to_string_with_padding(
    const intT       &value,
    const std::size_t total_length  = {},
    const char        pad_character = '0')
{
  auto str = std::to_string(value);
  if (str.length() < total_length)
    str.insert(
      str.front() == '-' ? 1 : 0,
      total_length - str.length(),
      pad_character);
  return str;
}
[[maybe_unused]] [[nodiscard]] inline std::string
  get_base_name(const std::filesystem::path &path)
{
  if (path.string().empty()) {
    return {};
  }
  auto name = path.filename().stem().string();
  std::ranges::transform(name, std::ranges::begin(name), upper);
  return name;
}
}// namespace open_viii::tools
#endif// VIIIARCHIVE_CONVERT_HPP