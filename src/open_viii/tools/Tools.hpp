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
#include "Convert.hpp"
#include "Compare.hpp"
#include "Random.hpp"
#include "Read.hpp"
#include "Write.hpp"
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
#include "For_Each.hpp"
#include "BitOps.hpp"
namespace open_viii::tools {

template<std::invocable<std::filesystem::path> UnaryOperationT,
         typename BinaryOperationT>
static void execute_on_directory(
  const std::filesystem::path &directory,
  UnaryOperationT              unary_function,
  BinaryOperationT             binary_function =
    true) requires(std::
                     is_same_v<
                       BinaryOperationT,
                       bool> || std::is_same_v<std::invoke_result_t<BinaryOperationT, std::filesystem::path>, bool>)
{
  const std::filesystem::directory_options options =
    std::filesystem::directory_options::skip_permission_denied;
  std::ranges::for_each(
    std::filesystem::directory_iterator(directory, options),
    [&unary_function, &binary_function](const auto &item) {
      const auto path = item.path();
      if constexpr (std::is_same_v<BinaryOperationT, bool>) {
        if (!binary_function) {
          return;
        }
      } else if constexpr (std::is_same_v<
                             std::invoke_result_t<BinaryOperationT,
                                                  std::filesystem::path>,
                             bool>) {
        if (!binary_function(path)) {
          return;
        }
      }
      unary_function(path);
    });
}
template<typename lambdaT>
requires(std::invocable<lambdaT, std::filesystem::path>)
  [[maybe_unused]] static void execute_on_directory(
    const std::filesystem::path &           dir,
    std::initializer_list<std::string_view> filenames,
    std::initializer_list<std::string_view> extensions,
    const lambdaT &                         lambda)
{
  execute_on_directory(
    dir, lambda, [&filenames, &extensions](const std::filesystem::path &path) {
      return std::filesystem::is_regular_file(path)
             && (std::ranges::empty(extensions)
                 || (path.has_extension()
                     && i_ends_with_any(path.extension().string(), extensions)))
             && (std::ranges::empty(filenames)
                 || (path.has_stem()
                     && i_find_any(path.stem().string(), filenames)));
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
    const std::filesystem::path &           dir,
    std::initializer_list<std::string_view> path_contains,
    const lambdaT &                         lambda)
{
  execute_on_directory(
    dir, lambda, [&path_contains](const std::filesystem::path &path) {
      return std::filesystem::is_directory(path)
             && tools::i_find_any(path.string(), path_contains);
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
std::string to_string_with_padding(const intT &      value,
                                   const std::size_t total_length  = {},
                                   const char        pad_character = '0')
{
  auto str = std::to_string(value);
  if (str.length() < total_length)
    str.insert(
      str.front() == '-' ? 1 : 0, total_length - str.length(), pad_character);
  return str;
}
[[maybe_unused]] [[nodiscard]] std::string static get_base_name(
  const std::filesystem::path &path)
{
  if (path.string().empty()) {
    return {};
  }
  auto name = path.filename().stem().string();
  std::transform(name.begin(), name.end(), name.begin(), ::toupper);
  return name;
}
}// namespace open_viii::tools
#endif// !VIITOOLS_H