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
#include "open_viii/Concepts.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <concepts>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <istream>
#include <ranges>
#include <span>
#include <string>
#include <thread>
#include <type_traits>
namespace open_viii::Tools {
constexpr static auto toupper_equals_predicate = [](const auto &ch1,
                                                   const auto &ch2) -> bool {
  return ::toupper(ch1) == ::toupper(ch2);
};

static void for_each_two(std::ranges::forward_range auto one,
  std::ranges::forward_range auto two,
  auto lambda)
{
  auto itt_one = one.begin();
  auto itt_two = two.begin();
  while (itt_one < one.end() && itt_two < two.end()) {
    lambda(*itt_one, *itt_two);
    ++itt_one;
    ++itt_two;
  }
}
/**
 * Workaround there is no way to currently to print a utf8 string... streams are
 * char only pretty much.
 * @param s8 utf8 string_view
 * @return char string_view.
 */
static std::string_view u8_to_sv(const std::u8string_view &s8)
{
  return { reinterpret_cast<const char *>(s8.data()), s8.size() };
}

/**
 * Workaround there is no way to currently to print a utf8 string... streams are
 * char only pretty much.
 * @param s8 utf8 string_view
 * @return char string
 */
[[maybe_unused]] static std::string u8_to_s(const std::u8string_view &s8)
{
  auto sv = u8_to_sv(s8);
  return { sv.begin(), sv.end() };
}


template<is_trivially_copyable_and_default_constructible trivialType,
  auto sizeOfType>
[[nodiscard]] static trivialType read_val(std::istream &fp)
{
  trivialType item{};
  std::array<char, sizeOfType> tmp{};
  fp.read(tmp.data(), tmp.size());
  if constexpr (requires { std::ranges::data(item); }) {

    memcpy(std::ranges::data(item),
      std::ranges::data(tmp),
      sizeof(*std::ranges::data(item)) * std::ranges::size(item));
  } else {
    memcpy(&item, std::ranges::data(tmp), sizeOfType);
  }
  return item;
}
template<is_trivially_copyable_and_default_constructible trivialType>
[[nodiscard]] static trivialType read_val(std::istream &fp)
{
  return read_val<trivialType, sizeof(trivialType)>(fp);
}

template<is_trivially_copyable trivialType>
static void read_val(std::istream &fp, trivialType &item)
{
  std::array<char, sizeof(item)> tmp{};
  fp.read(tmp.data(), tmp.size());

    memcpy(&item, std::ranges::data(tmp), sizeof(item));

}

template<typename trivialType>
requires(requires(
  trivialType item) { std::ranges::data(item); })
static void read_val(std::istream &fp, trivialType &item)
{
//  if constexpr( requires(trivialType t){std::is_same_v<char,decltype(*std::ranges::data(t))>;})
//  {}
// if container is type char could just write directly to it.
  const auto size = sizeof(*std::ranges::data(item)) * std::ranges::size(item);
  std::vector<char> tmp(size);

  fp.read(std::ranges::data(tmp), static_cast<long>(size));

    memcpy(std::ranges::data(item),
           std::ranges::data(tmp),
           size);

}

template<typename trivialType>
requires(is_trivially_copyable_and_default_constructible<trivialType>)
  [[nodiscard]] static trivialType read_val(const std::span<const char> &span)
{
  trivialType item{};
  memcpy(&item, std::ranges::data(span), sizeof(item));
  return item;
}


template<is_trivially_copyable trivialType>
static void read_val(const std::span<const char> &span, trivialType &item)
{
  const auto tmp{ span.subspan(0, sizeof(trivialType)) };


    memcpy(&item, std::ranges::data(tmp), sizeof(item));
}


template<has_data_and_size trivialType>
static void read_val(const std::span<const char> &span, trivialType &item, std::size_t size = 0U)
{
  const auto element_size = sizeof(*std::ranges::data(item));
  if constexpr (requires(trivialType i) { i.resize(0); }) {
    if (size != 0U) {
      item.resize(size);
    } else {
      item.resize(std::ranges::size(span) / element_size);
    }
  }
  memcpy(std::ranges::data(item),
         std::ranges::data(span),
         element_size * std::ranges::size(item));
}

template<is_default_constructible_has_data_and_size trivialType>
[[nodiscard]] static trivialType
read_val(const std::span<const char> &span, std::size_t size = 0U)
{
  trivialType item{};
  read_val(span,item,size);
  return item;
}
template<std::ranges::contiguous_range dstT = std::vector<char>,
  std::integral sizeT>
[[maybe_unused]] static auto read_buffer(std::istream &fp, const sizeT &s)
{
  dstT buf{};
  if (s <= 0) {
    return buf;
  }
  buf.resize(static_cast<std::size_t>(s));
  fp.read(std::ranges::data(buf), s);
  return buf;
}

template<typename dstT = std::vector<char>>
[[maybe_unused]] static auto read_buffer(std::istream &fp)
{
  fp.seekg(0, std::ios::end);// seek to end;
  const auto size_of_file = fp.tellg();// read filesize
  fp.seekg(0, std::ios::beg);// seek to beginning
  return read_buffer<dstT>(
    fp, static_cast<long>(size_of_file));// read entire file
}
template<std::ranges::contiguous_range dstT = std::vector<char>>
[[maybe_unused]] static dstT read_file(const std::filesystem::path &path)
{
  dstT buffer{};
  read_buffer(
    [&buffer](std::istream &fp) {
      buffer = read_buffer<dstT>(fp);
    },
    path);


  return buffer;
}

template<typename lambdaT>
requires(std::invocable<lambdaT, std::istream &>)
  [[maybe_unused]] static bool read_buffer(
    const lambdaT &lambda, const std::filesystem::path &path)
{
  std::error_code ec{};
  if (!std::filesystem::exists(path, ec)) {
    return false;
  }
  if (ec) {
    std::cerr << ec.message() << std::endl;
    ec.clear();
    return false;
  }
  auto fp = std::ifstream{};
  for (;;) {
    fp.open(path, std::ios::in | std::ios::binary);
    if (fp.is_open()) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  std::cout << (std::string("Loading: \t\"") + path.string()
                + std::string("\"\n"));
  lambda(fp);
  fp.close();
  return true;
}

template<typename lambdaT>
requires(std::invocable<lambdaT, std::ostream &>)
  [[maybe_unused]] static bool write_buffer(const lambdaT &lambda,
    const std::string_view &path,
    const std::string_view &root = "tmp")
{
  auto dir = std::filesystem::path(root);
  auto filename = dir / path;
  std::filesystem::create_directories(filename.parent_path());

  auto fp = std::ofstream{};
  for (;;) {
    fp.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    if (fp.is_open()) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  //    if (fp.is_open()) {
  std::cout << (std::string("Saving: \t\"") + filename.string()
                + std::string("\"\n"));
  lambda(fp);
  fp.close();
  //    } else {
  //      std::cout << (std::string("Failed to Open for saving: \"") +
  //      filename.string() + std::string("\"\n")); return false;
  //    }
  return true;
}

[[maybe_unused]] static bool write_buffer(
  [[maybe_unused]] const std::span<const char> &buffer,
  const std::string_view &path,
  const std::string_view &root = "tmp")
{
  if (std::ranges::empty(buffer)) {
    // std::cout << (std::string("Buffer is empty: \"")+ std::string(path) +
    // std::string("\"\n"));
    return false;
  }
  return write_buffer(
    [&buffer](std::ostream &fp) {
      fp.write(std::ranges::data(buffer),
        static_cast<long>(std::ranges::size(buffer)));
    },
    path,
    root);
}

/*
Case Insensitive strings equal
*/
[[maybe_unused]] [[nodiscard]] constexpr static bool i_equals(
  const std::string_view &str1, const std::string_view &str2)
{
  return std::ranges::equal(str1, str2, toupper_equals_predicate);
}

// Replace all of one char to another char. Arguments will be static_cast to the
// type of string char used in haystack. I use this to make the separator on
// windows or linux matches the strings.
template<typename needleType, typename replacementType, std::ranges::range T>
requires(std::is_convertible_v<replacementType, needleType>
    &&std::is_convertible_v<needleType, replacementType>)
  [[maybe_unused]] constexpr static void replace_all(T &haystack,
    const needleType &needle,
    const replacementType &replacement) noexcept
{
  static_assert(std::is_integral<needleType>::value
                  && std::is_integral<replacementType>::value,
    "Should be dealing with chars so is integral should cover that.");
  if (haystack.empty()) {
    return;
  }
  //
  //    if (static_cast<std::intmax_t>(needle)
  //        == static_cast<std::intmax_t>(replacement)) {// casting because
  //        worried if the types don't match.
  //      return;
  //    }
  std::ranges::transform(haystack,
    std::ranges::begin(haystack),
    [needle, replacement](const auto &input) {
      // handle when T2 doesn't match T by casting to match input;
      return input == static_cast<decltype(input)>(needle)
               ? static_cast<decltype(input)>(replacement)
               : input;
      // windows uses a wchar_t instead of char. So I need to static cast to
      // make them match though might need at least one value to be char. I'm
      // unsure what would happen if we were in a unicode mode.
    });
}

template<typename T, std::ranges::contiguous_range T2>
[[maybe_unused]] constexpr static bool any_of(
  const T &needle, const T2 &haystack)
{
  return std::ranges::any_of(haystack, [&needle](const auto &value) -> bool {
    return value == needle;
  });
}
[[maybe_unused]] constexpr static void replace_slashes(std::string &haystack)
{
  std::ranges::replace(
    haystack, '\\', std::filesystem::path::preferred_separator);
}

[[maybe_unused]] [[nodiscard]] static constexpr auto i_find(
  const std::string_view &haystack, const std::string_view &needle)
{
  if (std::ranges::size(haystack) >= std::ranges::size(needle)) {
    // clang tidy wants to make this a pointer. Bad idea.
    const auto last = std::search(haystack.begin(),
      haystack.end(),
      needle.begin(),
      needle.end(),
      toupper_equals_predicate);
    return last != haystack.end();
  }
  return false;
}

template<std::ranges::contiguous_range rangeT>
[[maybe_unused]] [[nodiscard]] static auto i_find_any(
  const std::string_view &haystack, const rangeT &needles)
{
  return std::ranges::empty(needles)
         || std::ranges::any_of(
           needles, [&haystack](const auto &needle) -> bool {
             return i_find(haystack, needle);
           });
}

/**
 * Will find and execute lambda on the dir if path_contains a valid value.
 * @tparam lambdaT
 * @param dir
 * @param path_contains list of valid values {} = all.
 * @param lambda f(std::filesystem:: path)
 */
template<typename lambdaT>
requires(std::invocable<lambdaT, std::filesystem::path>)
  [[maybe_unused]] static void execute_on_directories(
    const std::filesystem::path &dir,
    std::initializer_list<std::string_view> path_contains,
    const lambdaT &lambda)
{
  std::ranges::for_each(std::filesystem::recursive_directory_iterator(dir),
    [&path_contains, &lambda](const auto &item) {
      if (std::filesystem::is_directory(item)
          && Tools::i_find_any(item.path().string(), path_contains)) {
        lambda(item.path());
      }
    });
}

/**
 *
 * @tparam lambdaT the type of the invocable (lambda) that accepts the path as
 * an argument.
 * @param dir what directory to search
 * @param filenames any values required to be inside filename.
 * @param extensions any values required to be in the extension.
 * @param lambda accepts path as an argument.
 */
template<typename lambdaT>
requires(std::invocable<lambdaT, std::filesystem::path>)
  [[maybe_unused]] static void execute_on_i_path_extension_ends_with_any(
    const std::filesystem::path &dir,
    std::initializer_list<std::string_view> filenames,
    std::initializer_list<std::string_view> extensions,
    const lambdaT &lambda)
{
  std::ranges::for_each(std::filesystem::recursive_directory_iterator(dir),
    [&extensions, &filenames, &lambda](const auto &item) {
      if (!std::filesystem::is_regular_file(item.path())
          || !item.path().has_extension()
          || !i_ends_with_any(item.path().extension().string(), extensions)
          || !i_find_any(item.path().stem().string(), filenames)) {
        return;
      }
      lambda(item.path());
    });
}
/**
 * Get matching of paths.
 * @param dir path you want to search.
 * @param filenames any values required to be inside filename.
 * @param extensions any values required to be in the extension.
 * @return vector of all the matching paths.
 */
[[maybe_unused]] [[nodiscard]] static auto i_path_extension_ends_with_any(
  const std::filesystem::path &dir,
  std::initializer_list<std::string_view> filenames,
  std::initializer_list<std::string_view> extensions)
{
  std::vector<std::filesystem::path> out{};
  std::ranges::copy_if(std::filesystem::recursive_directory_iterator(dir),
    std::back_insert_iterator(out),
    [&extensions, &filenames](const auto &item) -> bool {
      if (!std::filesystem::is_regular_file(item.path())
          || !item.path().has_extension()
          || !i_find_any(item.path().extension().string(), extensions)
          || !i_find_any(item.path().stem().string(), filenames)) {
        return false;// clang tidy says this is redundant. :( Well screw you
                     // clang tidy I want to print out matches.
      }
      std::cout << "Found file: " << item.path().string() << '\n';
      return true;
    });
  return out;
}
[[maybe_unused]] [[nodiscard]] static constexpr auto i_ends_with(
  const std::string_view &haystack, const std::string_view &ending)
{
  [[maybe_unused]] const auto ending_view = ending | std::views::reverse;
  [[maybe_unused]] const auto haystack_view =
    haystack | std::views::reverse
    | std::views::take(std::ranges::size(ending));
  return std::ranges::size(haystack) >= std::ranges::size(ending)
         && std::ranges::equal(
           ending_view, haystack_view, toupper_equals_predicate);
}
[[maybe_unused]] [[nodiscard]] static bool i_starts_with(
  const std::string_view &haystack, const std::string_view &starting)
{
  [[maybe_unused]] const auto haystack_view =
    haystack | std::views::take(std::ranges::size(starting));
  return std::ranges::size(haystack) >= std::ranges::size(starting)
         && std::ranges::equal(
           starting, haystack_view, toupper_equals_predicate);
}
template<std::ranges::contiguous_range needleT>
[[maybe_unused]] [[nodiscard]] static size_t i_ends_with_any(
  const std::string_view &haystack, const needleT &needles)
{
  // std::cout << haystack <<std::endl;
  size_t i{};
  if (std::ranges::any_of(needles, [&haystack, &i](const auto &needle) -> bool {
        ++i;
        return i_ends_with(haystack, needle);
      })) {
    return i;// return index of found +1;
  }
  return size_t{};// return 0 if not found;
}

[[maybe_unused]] [[nodiscard]] static auto i_ends_with_any(
  const std::string_view &haystack,
  const std::initializer_list<std::string_view> &needles)
{
  // std::cout << haystack <<std::endl;
  size_t i{};
  if (std::ranges::any_of(
        needles, [&haystack, &i](const std::string_view &needle) -> bool {
          ++i;
          return i_ends_with(haystack, needle);
        })) {
    return i;// return index of found +1;
  }
  return size_t{};// return 0 if not found;
}
[[maybe_unused]] [[nodiscard]] static auto i_starts_with_any(
  const std::string_view &haystack,
  const std::initializer_list<std::string_view> &needles)
{
  size_t i{};
  if (std::ranges::any_of(needles, [&haystack, &i](const auto &needle) -> bool {
        ++i;
        return i_starts_with(haystack, needle);
      })) {
    return i;// return index of found +1;
  }
  return size_t{};// return 0 if not found;
}
[[maybe_unused]] [[nodiscard]] static auto i_find_all(
  const std::string_view &haystack,
  const std::initializer_list<std::string_view> &needles)
{
  return std::ranges::all_of(needles, [&haystack](const auto &needle) -> bool {
    return i_find(haystack, needle);
  });
}

template<std::unsigned_integral intT, std::invocable<intT, intT> lambdaT>
static void for_each_xy(
  const intT &max_x, const intT &max_y, const lambdaT &lambda)
{
  for (intT y{}; y < max_x; y++) {
    for (intT x{}; x < max_y; x++) {
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
template<std::unsigned_integral intT, std::invocable<intT, intT> lambdaT>
static void for_each_xy(const intT &max_xy, const lambdaT &lambda)
{
  for_each_xy(max_xy, max_xy, lambda);
}
/**
 * @see https://godbolt.org/z/qdqeP3
 */
template<std::integral retT = std::uint64_t>
static consteval retT largest_bit_value()
{
  constexpr auto bits_per_byte = 8U;
  return sizeof(retT) * bits_per_byte;
}
template<std::integral retT = std::uint64_t>
static consteval retT largest_bit_value(int i)
{
  if constexpr (std::signed_integral<retT>) {
    using unsigned_retT = typename std::make_unsigned<retT>::type;
    return static_cast<retT>(largest_bit_value<unsigned_retT>(i));
  } else {
    retT return_value{};
    constexpr auto max = largest_bit_value<retT>();
    // std::cout << max<< '\n';
    if (max < static_cast<unsigned>(i)) {
      i = static_cast<signed>(max);
    }
    while (i-- > 0) {
      return_value =
        static_cast<retT>(static_cast<retT>(return_value << 1U) | 1U);
      // std::cout << i+1 << '\t' << return_value << '\n';
    }
    return return_value;
  }
}
/**
 * @see https://godbolt.org/z/qdqeP3
 */
template<std::integral retT = std::uint64_t>
static consteval retT flip_bits(retT i)
{
  if constexpr (std::signed_integral<retT>) {
    using unsigned_retT = typename std::make_unsigned<retT>::type;
    return static_cast<retT>(flip_bits<unsigned_retT>(i));
  } else {
    constexpr auto max = std::numeric_limits<retT>::max();
    return static_cast<retT>(max - i);
  }
}
/**
 *
 * @tparam bit_count number of bits in mask
 * @return smallest integral that holds largest bitmask.
 * @see https://godbolt.org/z/djG1fz
 */
template<int bit_count> static consteval auto get_mask()
{
  constexpr auto bit_count_8 = 8;
  constexpr auto bit_count_16 = 16;
  constexpr auto bit_count_32 = 32;
  if constexpr (bit_count <= 0) {
    return uint8_t(0U);
  } else if constexpr (bit_count >= 1 && bit_count <= bit_count_8) {
    return largest_bit_value<std::uint8_t>(bit_count);
  } else if constexpr (bit_count > bit_count_8 && bit_count <= bit_count_16) {
    return largest_bit_value<std::uint16_t>(bit_count);
  } else if constexpr (bit_count > bit_count_16 && bit_count <= bit_count_32) {
    return largest_bit_value<std::uint32_t>(bit_count);
  } else if constexpr (bit_count > bit_count_32 /* && bit_count <= 64*/) {
    return largest_bit_value<std::uint64_t>(bit_count);
  }
}
template<typename lambdaT>
requires(std::invocable<lambdaT, std::filesystem::path>)
  [[maybe_unused]] static void execute_on_directory(
    const std::filesystem::path &dir,
    std::initializer_list<std::string_view> filenames,
    std::initializer_list<std::string_view> extensions,
    const lambdaT &lambda)
{

  std::ranges::for_each(std::filesystem::directory_iterator(dir),
    [&filenames, &extensions, &lambda](const auto &item) {
      const auto path = item.path();
      //        std::cout<<"here \"" << path.string() <<"\""<<std::endl;
      //           std::cout << std::filesystem::is_regular_file(item) <<' '<<
      //           path.has_extension() << ' '<<
      //           i_ends_with_any(path.extension().string(), extensions)  << '
      //           '<< i_find_any(path.stem().string(), filenames)<<std::endl;
      if (!std::filesystem::is_regular_file(item) || !path.has_extension()
          || !i_ends_with_any(path.extension().string(), extensions)
          || !i_find_any(path.stem().string(), filenames)) {
        return;
      }
      lambda(item);
    });
}
/**
 * use std::to_string and pad the value.
 * @param value initial integer value.
 * @param total_length default {}, sets the desired length
 * @param pad_character default 0
 * @return string value of number padded.
 * @see
 * https://stackoverflow.com/questions/53475501/how-to-zero-pre-fill-for-stdto-string-function
 */
template<std::integral intT>
std::string to_string_with_padding(const intT &value,
  const std::size_t total_length = {},
  const char pad_character = '0')
{
  auto str = std::to_string(value);
  if (str.length() < total_length)
    str.insert(
      str.front() == '-' ? 1 : 0, total_length - str.length(), pad_character);
  return str;
}
}// namespace open_viii::Tools
#endif// !VIITOOLS_H
