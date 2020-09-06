// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef VIITOOLS_H
#define VIITOOLS_H

#include <string>
#include <algorithm>
#include <fstream>
#include <istream>
#include <filesystem>
#include <cassert>
#include <ranges>
#include "OpenVIII/concepts.h"
namespace open_viii {
struct [[maybe_unused]] Tools
{

  template<Number T, Number rT> static constexpr T clamp(const T &input, const rT &min, const rT &max)
  {
    static_assert((std::is_integral_v<T> && std::is_integral_v<rT>)
                  || (std::is_floating_point_v<T> && std::is_floating_point_v<rT>));
    if (input > max) {
      return static_cast<T>(max);
    }
    if (input < min) {
      return static_cast<T>(min);
    }
    return input;
  }
  static std::string_view u8tosv(const std::u8string_view &s8)
  {
    return { reinterpret_cast<const char *>(s8.data()), s8.size() };
  }
  [[maybe_unused]] static std::string u8tos(const std::u8string_view &s8)
  {
    auto sv = u8tosv(s8);
    return { sv.begin(), sv.end() };
  }
  template<typename trivialType> [[maybe_unused]] static void read_val(std::istream &fp, trivialType &item)
  {
    static_assert(std::is_trivial_v<trivialType>);
    std::array<char, sizeof(item)> tmp{};
    fp.read(tmp.data(), tmp.size());
    memcpy(&item, tmp.data(), sizeof(item));
  }// namespace open_viii

  template<typename dstT = std::vector<char>, typename sizeT>
  [[maybe_unused]] static auto read_buffer(std::istream &fp, const sizeT &s)
  {
    static_assert(std::is_integral_v<sizeT>, "Integral Required");
    assert(s > 0);
    dstT buf{};
    buf.resize(s);
    fp.read(buf.data(), s);
    return buf;
  }
  template<typename lambdaT>
  requires(std::invocable<lambdaT, std::ostream &>) [[maybe_unused]] static bool write_buffer(const lambdaT &lambda,
    const std::string_view &path,
    const std::string_view &root = "tmp")
  {
    auto dir = std::filesystem::path(root);
    auto filename = dir / path;
    std::filesystem::create_directories(filename.parent_path());

    auto fp = std::ofstream(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    if (fp.is_open()) {
      std::cout << "Saving: " << filename << '\n';
      try {
        lambda(fp);
      } catch (const std::out_of_range &oor) {
        std::cerr << "Out of Range error: " << oor.what() << '\n';
      }
      fp.close();
    } else {
      std::cout << "Failed to Open for saving: " << filename << '\n';
    }
    return true;
  }

  [[maybe_unused]] static bool write_buffer(const std::span<const char> &buffer,
    const std::string_view &path,
    const std::string_view &root = "tmp")
  {
    if (std::ranges::empty(buffer)) {
      return false;
    }
    return write_buffer(
      [&buffer](
        std::ostream &fp) { fp.write(std::ranges::data(buffer), static_cast<long>(std::ranges::size(buffer))); },
      path,
      root);
  }

  /*
  Case Insensitive strings equal
  */
  [[maybe_unused]] [[nodiscard]] constexpr static bool i_equals(const std::string_view &str1,
    const std::string_view &str2)
  {
    const constexpr auto i_equal = [](const auto &c1, const auto &c2) { return ::toupper(c1) == ::toupper(c2); };
    if (str1.length() != str2.length()) {
      return false;
    }
    for (size_t i = 0; i < str1.length(); i++) {
      if (!i_equal(str1.at(i), str2.at(i))) {
        return false;
      }
    }
    return true;
    // return std::equal(str1.begin(), str1.end(), str2.begin(), iEqual);
    // todo std::equal constexpr in cpp20
  }

  // Replace all of one char to another char. Arguments will be static_cast to the type of string char used in haystack.
  // I use this to make the separator on windows or linux matches the strings.
  template<typename needleType, typename replacementType>
  [[maybe_unused]] constexpr static void
    replace_all(std::string &haystack, const needleType &needle, const replacementType &replacement) noexcept
  {
    static_assert(std::is_integral<needleType>::value && std::is_integral<replacementType>::value,
      "Should be dealing with chars so is integral should cover that.");
    if (haystack.empty()) {
      return;
    }

    if (static_cast<std::intmax_t>(needle)
        == static_cast<std::intmax_t>(replacement)) {// casting because worried if the types don't match.
      return;
    }
    const auto replace = [needle, replacement](const auto &input) {
      // handle when T2 doesn't match T by casting to match input;
      return input == static_cast<decltype(input)>(needle) ? static_cast<decltype(input)>(replacement) : input;
      // windows uses a wchar_t instead of char. So I need to static cast to make them match
      // though might need at least one value to be char. I'm unsure what would happen if we were in
      // a unicode mode.
    };
    std::transform(haystack.begin(), haystack.end(), haystack.begin(), replace);
  }

  template<typename T, typename T2> [[maybe_unused]] constexpr static bool any_of(const T &needle, const T2 &haystack)
  {
    for (const auto i : haystack) {// in cpp 20 std::any_of can be used instead.
      if (i == needle) {
        return true;
      }
    }
    return false;
  }
  [[maybe_unused]] constexpr static void replace_slashes(std::string &haystack)
  {
    replace_all(haystack, '\\', std::filesystem::path::preferred_separator);
  }
  /*
  Find Case Insensitive Sub String in a given substring (version returns location in string and allows offset)
  */
  //[[nodiscard]] inline static size_t iFind(std::string haystack, std::string needle, size_t offset = 0)
  //{
  //	// Convert complete given String to lower case
  //	std::transform(haystack.begin(), haystack.end(), haystack.begin(), ::toupper);
  //	// Convert complete given Sub String to lower case
  //	std::transform(needle.begin(), needle.end(), needle.begin(), ::toupper);
  //	// Find sub string in given string
  //	return haystack.find(needle, offset);
  //}


  // Find Case Insensitive Sub String in a given substring

  [[maybe_unused]] [[nodiscard]] inline static auto i_find(const std::string_view &haystack,
    const std::string_view &needle)
  {
    if (std::size(haystack) >= std::size(needle)) {
      return std::search(haystack.begin(),
               haystack.end(),
               needle.begin(),
               needle.end(),
               [](const auto &ch1, const auto &ch2) { return ::toupper(ch1) == ::toupper(ch2); })
             != haystack.end();
      // todo make constexpr in cpp 20
    }
    return false;
  }
  template<std::ranges::contiguous_range rangeT>
  [[maybe_unused]] [[nodiscard]] inline static auto i_find_any(const std::string_view &haystack, const rangeT &needles)
  {
    for (const auto &needle : needles) {
      if (i_find(haystack, needle)) {
        return true;
      }
    }
    return false;
  }
  [[maybe_unused]] [[nodiscard]] inline static auto i_ends_with(const std::string_view &haystack,
    const std::string_view &ending)
  {
    return std::size(haystack) >= std::size(ending)
           && std::equal(ending.rbegin(), ending.rend(), haystack.rbegin(), [](const auto &ch1, const auto &ch2) {
                return ::toupper(ch1) == ::toupper(ch2);
              });
  }
  [[maybe_unused]] [[nodiscard]] inline static auto i_starts_with(const std::string_view &haystack,
    const std::string_view &starting)
  {
    return std::size(haystack) >= std::size(starting)
           && std::equal(starting.begin(), starting.end(), haystack.begin(), [](const auto &ch1, const auto &ch2) {
                return ::toupper(ch1) == ::toupper(ch2);
              });
  }
  [[maybe_unused]] [[nodiscard]] inline static auto i_ends_with_any(const std::string_view &haystack,
    const std::initializer_list<std::string_view> &endings)
  {
    {
      size_t i{};
      for (const auto &ending : endings) {
        i++;
        if (i_ends_with(haystack, ending)) {
          return i;// return index of found +1;
        }
      }
    }
    return size_t{};// return 0 if not found;
  }

  [[maybe_unused]] [[nodiscard]] inline static auto i_starts_with_any(const std::string_view &haystack,
    const std::initializer_list<std::string_view> &endings)
  {
    {
      size_t i{};
      for (const auto &ending : endings) {
        i++;
        if (i_starts_with(haystack, ending)) {
          return i;// return index of found +1;
        }
      }
    }
    return size_t{};// return 0 if not found;
  }
  [[maybe_unused]] [[nodiscard]] inline static auto i_find_all(const std::string_view &haystack,
    const std::initializer_list<std::string_view> &needles)
  {
    for (const auto &needle : needles) {
      if (!i_find(haystack, needle)) {
        return false;
      }
    }
    return true;
  }
};
}// namespace open_viii
#endif// !VIITOOLS_H
