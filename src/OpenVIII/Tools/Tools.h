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

namespace OpenVIII {
struct [[maybe_unused]] Tools
{
  static std::string_view u8tosv(const std::u8string_view & s8)
  {
    return {reinterpret_cast<const char *>(s8.data()),s8.size()};
  }
  static std::string u8tos(const std::u8string_view & s8)
  { auto sv = u8tosv(s8);
    return {sv.begin(),sv.end()};
  }
  template<typename trivialType> [[maybe_unused]] static void ReadVal(std::istream &fp, trivialType &item)
  {
    static_assert(std::is_trivial_v<trivialType>);
    std::array<char, sizeof(item)> tmp{};
    fp.read(tmp.data(), tmp.size());
    memcpy(&item, tmp.data(), sizeof(item));
  }// namespace OpenVIII

  template<typename dstT = std::vector<char>, typename sizeT>
  [[maybe_unused]] static auto ReadBuffer(std::istream &fp, const sizeT &s)
  {
    static_assert(std::is_integral_v<sizeT>, "Integral Required");
    assert(s > 0);
    dstT buf{};
    buf.resize(s);
    fp.read(buf.data(), s);
    return buf;
  }

  template<typename srcT = std::vector<char>>
  [[maybe_unused]] static bool
    WriteBuffer(const srcT &buffer, const std::string_view &path, const std::string_view &root = "tmp")
  {
    // todo make buffer a std::span
    if (std::empty(buffer)) {
      return false;
    }
    auto dir = std::filesystem::path(root);
    auto filename = dir / path;
    std::filesystem::create_directories(filename.parent_path());
    auto fp = std::ofstream(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    if (fp.is_open()) {
      fp.write(buffer.data(), static_cast<long>(buffer.size()));
    }
    fp.close();
    return true;
  }

  /*
  Case Insensitive strings equal
  */
  [[maybe_unused]] [[nodiscard]] constexpr static bool iEquals(const std::string_view &str1,
    const std::string_view &str2)
  {
    const constexpr auto iEqual = [](const auto &c1, const auto &c2) { return ::toupper(c1) == ::toupper(c2); };
    if (str1.length() != str2.length()) {
      return false;
    }
    for (size_t i = 0; i < str1.length(); i++) {
      if (!iEqual(str1.at(i), str2.at(i))) {
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
    replaceAll(std::string &haystack, const needleType &needle, const replacementType &replacement) noexcept
  {

    static_assert(std::is_integral<needleType>::value && std::is_integral<replacementType>::value,
      "Should be dealing with chars so is integral should cover that.");
    if (haystack.empty()) {
      return;
    }

    if (static_cast<int>(needle) == static_cast<int>(replacement)) {// casting because worried if the types don't match.
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
  [[maybe_unused]] constexpr static void replaceSlashes(std::string &haystack)
  {
    replaceAll(haystack, '\\', std::filesystem::path::preferred_separator);
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

  [[maybe_unused]] [[nodiscard]] inline static auto iFind(const std::string_view &haystack,
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
  [[maybe_unused]] [[nodiscard]] inline static auto iFindAny(const std::string_view &haystack,
    const std::initializer_list<std::string_view> &needles)
  {
    for (const auto &needle : needles) {
      if (iFind(haystack, needle)) {
        return true;
      }
    }
    return false;
  }
  [[maybe_unused]] [[nodiscard]] inline static auto iEndsWith(const std::string_view &haystack,
    const std::string_view &ending)
  {

    return std::size(haystack) >= std::size(ending)
           && std::equal(ending.rbegin(), ending.rend(), haystack.rbegin(), [](const auto &ch1, const auto &ch2) {
                return ::toupper(ch1) == ::toupper(ch2);
              });
  }
  [[maybe_unused]] [[nodiscard]] inline static auto iStartsWith(const std::string_view &haystack,
    const std::string_view &starting)
  {

    return std::size(haystack) >= std::size(starting)
           && std::equal(starting.begin(), starting.end(), haystack.begin(), [](const auto &ch1, const auto &ch2) {
                return ::toupper(ch1) == ::toupper(ch2);
              });
  }
  [[maybe_unused]] [[nodiscard]] inline static auto iEndsWithAny(const std::string_view &haystack,
    const std::initializer_list<std::string_view> &endings)
  {
    {
      size_t i{};
      for (const auto &ending : endings) {
        i++;
        if (iEndsWith(haystack, ending)) {
          return i;// return index of found +1;
        }
      }
    }
    return size_t{};// return 0 if not found;
  }

  [[maybe_unused]] [[nodiscard]] inline static auto iStartsWithAny(const std::string_view &haystack,
    const std::initializer_list<std::string_view> &endings)
  {
    {
      size_t i{};
      for (const auto &ending : endings) {
        i++;
        if (iStartsWith(haystack, ending)) {
          return i;// return index of found +1;
        }
      }
    }
    return size_t{};// return 0 if not found;
  }
  [[maybe_unused]] [[nodiscard]] inline static auto iFindAll(const std::string_view &haystack,
    const std::initializer_list<std::string_view> &needles)
  {
    for (const auto &needle : needles) {
      if (!iFind(haystack, needle)) {
        return false;
      }
    }
    return true;
  }
};
}// namespace OpenVIII
#endif// !VIITOOLS_H
