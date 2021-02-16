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
#include <cassert>
#include <filesystem>
#include <fstream>
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
constexpr static void clean_string(std::string &input,
                                   const bool & skip_fixed = true) noexcept
{
  if (std::ranges::size(input) > 4) {
    if (tools::i_starts_with(input, "c:")) {
      input.erase(0, 3);// remove c:\ from the start of the strings.
    }
    if (skip_fixed) {
      while (input.back() == '\r') {
        input.pop_back();
      }// remove the carriage return character
      tools::replace_slashes(input);
    }
  }
}
constexpr const static auto EXT = std::string_view(".FL");
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
[[nodiscard]] static std::vector<std::pair<unsigned int, std::string>>
  get_all_entries_data(
    const std::filesystem::path &                  path,
    const std::string &                            data,
    const size_t &                                 offset,
    const size_t &                                 size   = 0U,
    const size_t &                                 count  = 0U,
    const std::initializer_list<std::string_view> &needle = {},
    const size_t &                                 limit  = 0U)
{
  // TODO break this code up
  std::vector<std::pair<unsigned int, std::string>> vector{};
  const auto                                        process =
    [&limit, &count, &size, &vector, &offset, &needle, &data](auto &cont) {
      {// Get length
        cont.seekg(0, std::ios::end);
        auto length = cont.tellg();
        // Goto Offset
        if (!cont.seekg(static_cast<long>(offset))) {
          // Error on failure
          std::cerr << "failed to seek to offset: " << offset
                    << "; length: " << length << ";\n";
          exit(EXIT_FAILURE);
        }
      }
      if (count > 0) {
        if (limit > 0 && count > limit) {
          vector.reserve(limit);
        } else {
          vector.reserve(count);
        }
      }
      // id numerical order is same order as fi data. So need to keep the id so
      // we can reference the fi correctly.
      {
        std::basic_string<char> inner_path;
        for (unsigned int id = 0;
             (count == 0U || std::ranges::size(vector) < count)
               && (size == 0U
                   || static_cast<std::size_t>(cont.tellg()) < size + offset)
               && [&inner_path, &cont]() -> bool {
               if (cont.seekg(3, std::ios::cur)) {
                 /* skip c:\ */
                 return static_cast<bool>(std::getline(cont, inner_path));
               }
               return false;
             }();
             id++) {
          if (!tools::i_find_any(inner_path, needle)) {
            continue;
          }
          // https://youtu.be/oTMSgI1XjF8?t=1727
          clean_string(
            vector
              .emplace_back(std::piecewise_construct,
                            std::forward_as_tuple(id),
                            std::forward_as_tuple(std::move(inner_path)))
              .second,
            std::empty(data));
          inner_path = {};
        }
      }
    };
  // sort the strings. to make it easier to choose the correct string first.
  // shorter length and then what ever str < str2 does.
  if (!std::empty(data) && data.front() != '\0') {
    auto ss = std::stringstream(data);
    process(ss);
  } else {
    tools::read_from_file(process, path);
  }
  std::ranges::sort(vector, [](const auto &left, const auto &right) {
    if (left.second.length() == right.second.length()) {
      return left.second < right.second;
    }
    return left.second.length() < right.second.length();
  });
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
[[nodiscard]] static auto
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
[[nodiscard]] static auto
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
 static std::string clean_buffer(std::string buffer)
{
  // remove carriage returns
  std::erase(buffer, '\r');
  // buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'),
  // buffer.end());
  // change slashes to preferred
  tools::replace_slashes(buffer);
  return buffer;
}
}// namespace open_viii::archive::fl
#endif// !VIIIARCHIVE_FL_HPP