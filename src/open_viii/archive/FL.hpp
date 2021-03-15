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
#ifndef VIIIARCHIVE_FL_HPP
#define VIIIARCHIVE_FL_HPP
#include "open_viii/tools/Tools.hpp"
#include "tl/string.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <optional>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace open_viii::archive::fl {
/**
 * FL files contain internal file structure paths. As a flat text file. This
 * class is used to search the strings for a filename or grab all the entries.
 * The entry will be a string paired with an int that is the line number. This
 * is used to ID the FI entries.
 */

/**
 * Remove the C:\ from the start, remove the \r from the end, and change \ to
 * the correct slash. added skipFixed if data is set then i probably fixed
 * slashes already.
 * @param input updates this string
 * @param skipFixed if false skip removing the \r from end and skip replacing
 * slashes.
 * @return void
 */
constexpr static void
  clean_path_string(std::string &input, const bool &skip_fixed = true) noexcept
{
  if (std::ranges::size(input) > 4) {
    tl::string::remove_drive_letter(input);
    if (skip_fixed) {
      tl::string::remove_carriage_return_from_end(input);
      tl::string::replace_slashes(input);
    }
  }
}

/**
 * Remove the C:\ from the start, remove the \r from the end, and change \ to
 * the correct slash. added skipFixed if data is set then i probably fixed
 * slashes already.
 * @param input updates this string
 * @param skipFixed if false skip removing the \r from end and skip replacing
 * slashes.
 * @return modified input
 */
[[maybe_unused]] static std::string
  clean_path_string(std::string &&input,
                    const bool &  skip_fixed = false) noexcept
{
  clean_path_string(input, skip_fixed);
  return std::move(input);
}

/**
 * File extension
 */
constexpr const static auto EXT = std::string_view(".FL");

/**
 * Decide how much to reserve based on known count or a set limit.
 * @param count computed max count
 * @param limit manual limit placed
 * @return 0 or count or limit;
 */
static std::size_t
  get_max(const std::size_t &count, const std::size_t &limit)
{
  if (count != 0U) {
    if (limit != 0U && count > limit) {
      return limit;
    } else {
      return count;
    }
  }
  return {};
}
static std::vector<std::pair<std::uint32_t, std::string>> &
  sort_entries(std::vector<std::pair<std::uint32_t, std::string>> &&vector)
{// sort the strings. to make it easier to choose the correct string first.
  // shorter length and then what ever str < str2 does.
  std::ranges::sort(vector,
                    [](const std::pair<std::uint32_t, std::string> &left,
                       const std::pair<std::uint32_t, std::string> &right) {
                      const std::string &ls = std::get<1>(left);
                      const std::string &rs = std::get<1>(right);
                      if (std::ranges::size(ls) == std::ranges::size(rs)) {
                        return (ls <=> rs) == std::strong_ordering::less;
                      }
                      return std::ranges::size(ls) < std::ranges::size(rs);
                    });
  return vector;
}
[[nodiscard]] static std::vector<std::pair<std::uint32_t, std::string>>
  get_all_entries_data(
    const tl::read::input &                        cont,
    const size_t &                                 offset,
    const size_t &                                 size   = 0U,
    const size_t &                                 count  = 0U,
    const std::initializer_list<std::string_view> &needle = {},
    const size_t &                                 limit  = 0U)
{
  std::vector<std::pair<std::uint32_t, std::string>> vector{};
  cont.seek(static_cast<std::intmax_t>(offset), std::ios::beg);
  std::size_t max = get_max(count, limit);
  if (max != 0) {
    vector.reserve(max);
  }
  // id numerical order is same order as fi data. So need to keep the id so
  // we can reference the fi correctly.
  {
    for (std::uint32_t id = 0; (count == 0U || std::ranges::size(vector) < max)
                               && (size == 0U || (cont.tell() < size + offset));
         ++id) {
      const std::string inner_path = clean_path_string(cont.get_line());
      if (std::ranges::empty(inner_path)) {
        break;
      }
      if (!tools::i_find_any(inner_path, needle)) {
        continue;
      }

      // https://youtu.be/oTMSgI1XjF8?t=1727
      vector.emplace_back(std::piecewise_construct,
                          std::forward_as_tuple(id),
                          std::forward_as_tuple(std::move(inner_path)));
    }
  }
  return sort_entries(std::move(vector));
}
/**
 * Get All entries sorted from file or data buffer.
 * @param path filename path.
 * @param data buffer of bytes.
 * @param offset bytes from start of data to start looking.
 * @param size of FL file if known; 0 == unlimited
 * @param count expected number of matches; calculated from FI file size / 12; 0
 * == unlimited
 * @param needle possible string matches; {} == all
 * @param limit max matches; 0 == unlimited
 * @return matches
 */
[[nodiscard]] static std::vector<std::pair<std::uint32_t, std::string>>
  get_all_entries_data(
    const std::filesystem::path &                  path,
    const std::string &                            data,
    const size_t &                                 offset,
    const size_t &                                 size   = 0U,
    const size_t &                                 count  = 0U,
    const std::initializer_list<std::string_view> &needle = {},
    const size_t &                                 limit  = 0U)
{

  std::vector<std::pair<std::uint32_t, std::string>> vector{};
  if (!std::empty(data) && data.front() != '\0') {
    vector = get_all_entries_data(
      tl::read::input(data, true), offset, size, count, needle, limit);
  } else {
    tl::read::from_file(
      [&](std::istream &istream) {
        vector = get_all_entries_data(
          tl::read::input(&istream, true), offset, size, count, needle, limit);
      },
      path);
  }

  return vector;
}
// Get all entries from the FL file sorted and cleaned.
[[maybe_unused]] [[nodiscard]] static auto
  get_all_entries(const std::filesystem::path &                  path,
                  const size_t &                                 offset,
                  const size_t &                                 size   = 0,
                  const size_t &                                 count  = 0,
                  const std::initializer_list<std::string_view> &needle = {})
{
  auto tmp = std::string();
  return get_all_entries_data(path, tmp, offset, size, count, needle);
}
/**
 * Get a single entry that is the first match for needle.
 * @param path contains path to file
 * @param data contains buffer of chars //required to be strings for
 * stringstream
 * @param needle is a group of strings to filter the output with.
 * @param offset is the number of bytes to skip.
 * @param size is max number of bytes. 0 is unlimited.
 * @param count is max results returned. 0 is unlimited.
 * @return
 */
[[maybe_unused]] [[nodiscard]] static auto
  get_entry_data(const std::filesystem::path &                  path,
                 const std::string &                            data,
                 const std::initializer_list<std::string_view> &needle,
                 const size_t &                                 offset = 0U,
                 const size_t &                                 size   = 0U,
                 const size_t &                                 count  = 0U)
{// Maybe should search all entries instead of using this because this is not
 // sorted. Sorting matters when the
  // strings are similar. Though this might be faster if only getting a few
  // files from an archive.
  auto buffer =
    get_all_entries_data(path, data, offset, size, count, needle, 1U);
  if (std::empty(buffer)) {
    return std::make_pair(0U, std::string());
  }
  return buffer.front();
}
// Get a single entry that is the first match for needle.
[[maybe_unused]] [[nodiscard]] static auto
  get_entry(const std::filesystem::path &                  path,
            const std::initializer_list<std::string_view> &needle,
            const size_t &                                 offset = 0U,
            const size_t &                                 size   = 0U,
            const size_t &                                 count  = 0U)
{// Maybe should search all entries instead of using this because this is not
 // sorted. Sorting matters when the strings are similar. Though this might be
 // faster if only getting a few files from an archive.
  auto data = get_all_entries_data(path, "", offset, size, count, needle, 1U);
  if (std::empty(data)) {
    return std::make_pair(0U, std::string());
  }
  return data.front();
}
[[maybe_unused]] static std::string
  clean_buffer(std::string &&in_buffer)
{
  return tl::string::replace_slashes(
    tl::string::remove_carriage_return(std::move(in_buffer)));
}
}// namespace open_viii::archive::fl
#endif// !VIIIARCHIVE_FL_HPP