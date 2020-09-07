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
private:
  struct IEqualPredicate
  {
  constexpr bool operator()(const auto &ch1, const auto &ch2) const noexcept
  {
    return ::toupper(ch1) == ::toupper(ch2);
  }
  };

public:
  /**
   * Workaround there is no way to currently to print a utf8 string... streams are char only pretty much.
   * @param s8 utf8 string_view
   * @return char string_view.
   */
  static std::string_view u8_to_sv(const std::u8string_view &s8)
  {
    return { reinterpret_cast<const char *>(s8.data()), s8.size() };
  }
  /**
   * Workaround there is no way to currently to print a utf8 string... streams are char only pretty much.
   * @param s8 utf8 string_view
   * @return char string
   */
  [[maybe_unused]] static std::string u8_to_s(const std::u8string_view &s8)
  {
    auto sv = u8_to_sv(s8);
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

  [[maybe_unused]] static bool write_buffer([[maybe_unused]] const std::span<const char> &buffer,
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
    return std::ranges::equal(str1, str2, IEqualPredicate());
  }

  // Replace all of one char to another char. Arguments will be static_cast to the type of string char used in haystack.
  // I use this to make the separator on windows or linux matches the strings.
  template<typename needleType, typename replacementType, std::ranges::range T>
  requires(std::is_convertible_v<replacementType, needleType> &&std::is_convertible_v<needleType, replacementType>)
    [[maybe_unused]] constexpr static void replace_all(T &haystack,
      const needleType &needle,
      const replacementType &replacement) noexcept
  {
    static_assert(std::is_integral<needleType>::value && std::is_integral<replacementType>::value,
      "Should be dealing with chars so is integral should cover that.");
    if (haystack.empty()) {
      return;
    }
    //
    //    if (static_cast<std::intmax_t>(needle)
    //        == static_cast<std::intmax_t>(replacement)) {// casting because worried if the types don't match.
    //      return;
    //    }
    std::ranges::transform(haystack, std::ranges::begin(haystack), [needle, replacement](const auto &input) {
      // handle when T2 doesn't match T by casting to match input;
      return input == static_cast<decltype(input)>(needle) ? static_cast<decltype(input)>(replacement) : input;
      // windows uses a wchar_t instead of char. So I need to static cast to make them match
      // though might need at least one value to be char. I'm unsure what would happen if we were in
      // a unicode mode.
    });
  }

  template<typename T, std::ranges::contiguous_range T2>
  [[maybe_unused]] constexpr static bool any_of(const T &needle, const T2 &haystack)
  {
    return std::ranges::any_of(haystack, [&needle](const auto &value) -> bool { return value == needle; });
  }
  [[maybe_unused]] constexpr static void replace_slashes(std::string &haystack)
  {
    std::ranges::replace(haystack, '\\', std::filesystem::path::preferred_separator);
    // replace_all(haystack, '\\', std::filesystem::path::preferred_separator);
  }

  [[maybe_unused]] [[nodiscard]] static constexpr auto i_find(const std::string_view &haystack,
    const std::string_view &needle)
  {
    if (std::ranges::size(haystack) >= std::ranges::size(needle)) {
      const auto sub_range = std::ranges::search(haystack, needle, IEqualPredicate());
      return std::ranges::size(sub_range) == std::ranges::size(needle);
      // todo make constexpr in cpp 20
    }
    return false;
  }

  template<std::ranges::contiguous_range rangeT>
  [[maybe_unused]] [[nodiscard]] static auto i_find_any(const std::string_view &haystack, const rangeT &needles)
  {
    return std::ranges::any_of(needles, [&haystack](const auto &needle) -> bool { return i_find(haystack, needle); });
  }
  static void search_path(const std::filesystem::path &dir, std::initializer_list<std::string_view> extensions)
  {
    for (const auto &item : std::filesystem::recursive_directory_iterator(dir)) {
      if (!std::filesystem::is_regular_file(item.path()) || i_find_any(item.path().extension().string(), extensions))
        continue;

      // open text file here
      std::cout << "Found file: " << item.path().string() << '\n';
    }
  }
  [[maybe_unused]] [[nodiscard]] static constexpr auto i_ends_with(const std::string_view &haystack,
    const std::string_view &ending)
  {
    const auto ending_view = ending | std::views::reverse;
    const auto haystack_view = haystack | std::views::reverse | std::views::take(std::ranges::size(ending));
    return std::ranges::size(haystack) >= std::ranges::size(ending)
           && std::ranges::equal(ending_view, haystack_view, IEqualPredicate());
  }
  [[maybe_unused]] [[nodiscard]] static auto i_starts_with(const std::string_view &haystack,
    const std::string_view &starting)
  {
    const auto starting_view = starting;
    const auto haystack_view = haystack | std::views::reverse | std::views::take(std::ranges::size(starting));
    return std::ranges::size(haystack) >= std::ranges::size(starting)
           && std::ranges::equal(starting_view, haystack_view, IEqualPredicate());
  }
  [[maybe_unused]] [[nodiscard]] static auto i_ends_with_any(const std::string_view &haystack,
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

  [[maybe_unused]] [[nodiscard]] static auto i_starts_with_any(const std::string_view &haystack,
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
  [[maybe_unused]] [[nodiscard]] static auto i_find_all(const std::string_view &haystack,
    const std::initializer_list<std::string_view> &needles)
  {
    return std::ranges::all_of(needles, [&haystack](const auto &needle) -> bool { return i_find(haystack, needle); });
    //    for (const auto &needle : needles) {
    //      if (!i_find(haystack, needle)) {
    //        return false;
    //      }
    //    }
    //    return true;
  }
};
}// namespace open_viii
#endif// !VIITOOLS_H
