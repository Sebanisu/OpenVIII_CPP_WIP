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
#include "tl/input.hpp"
#include "tl/read.hpp"
#include "tl/string.hpp"
#include "tl/write.hpp"

namespace open_viii::archive::fl {
/**
 * FL files contain internal file structure paths. As a flat text file. This
 * class is used to search the strings for a filename or grab all the entries.
 * The entry will be a string paired with an int that is the line number. This
 * is used to ID the FI entries.
 * @todo refactor this to reduce parameters and reduce functions.
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
inline constexpr void
  clean_path_string(std::string &input) noexcept
{
  if (std::ranges::size(input) > 4 && input[1] == ':') {
    tl::string::remove_drive_letter(input);
    tl::string::remove_carriage_return_from_end(input);
  }
  tl::string::replace_slashes(input);
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
[[nodiscard]] [[maybe_unused]] inline std::string
  clean_path_string(std::string &&input) noexcept
{
  clean_path_string(input);
  return std::move(input);
}

/**
 * Take out carriage returns and replace slashes.
 * @param in_buffer multi line string of paths.
 * @return cleaned.
 */
[[nodiscard]] [[maybe_unused]] inline std::string
  clean_buffer(std::string &&in_buffer)
{
  return tl::string::replace_slashes(
    tl::string::remove_carriage_return(tl::string::erase_string_from_string(
      std::move(in_buffer),
      std::string_view(R"(c:\)"),
      std::string_view(R"(C:\)"))));
}

/**
 * File extension
 */
static constexpr auto EXT = std::string_view(".fl");

/**
 * Decide how much to reserve based on known count or a set limit.
 * @param count computed max count
 * @param limit manual limit placed
 * @return 0 or count or limit;
 */
[[nodiscard]] [[maybe_unused]] inline constexpr std::size_t
  get_max(const std::size_t &count, const std::size_t &limit)
{
  std::array<std::size_t, 2U> args = { count, limit };
  std::ranges::sort(args);
  for (const auto arg : args) {
    if (arg != 0U)
      return arg;
  }
  return {};
}

/**
 * Sort the strings. to make it easier to choose the correct string first.
 * shorter length and then what ever str < str2 does.
 * @param vector pairs of ints and paths
 */
inline void
  sort_entries(std::span<std::pair<std::uint32_t, std::string>> vector)
{
  std::ranges::sort(
    vector,
    [](
      const std::pair<std::uint32_t, std::string> &left,
      const std::pair<std::uint32_t, std::string> &right) {
      const std::string &ls = std::get<1>(left);
      const std::string &rs = std::get<1>(right);
      if (std::ranges::size(ls) == std::ranges::size(rs)) {
        return ls.compare(rs) < 0;
      }
      return std::ranges::size(ls) < std::ranges::size(rs);
    });
}

/**
 * Sort the strings. to make it easier to choose the correct string first.
 * shorter length and then what ever str < str2 does.
 * @param vector pairs of ints and paths
 * @return sorted vector
 */
[[nodiscard]] [[maybe_unused]] inline auto
  sort_entries(std::vector<std::pair<std::uint32_t, std::string>> &&vector)
{
  sort_entries(vector);
  return std::move(vector);
}

/**
 * Eagerly populate a vector with strings of paths, then sort it.
 * @param cont wrapper on istream or span. For the source data.
 * @param offset to start of data.
 * @param size + offset is the end of the data.
 * @param count max count detected from FI filesize / 12U.
 * @param needle set of strings to search for.
 * @param limit manually set max count.
 * @note If size, limit, count are all 0 it'll read till end of file. Or till it
 * reads an empty line.
 * @todo make needle a predicate lambda.
 */
[[nodiscard]] [[maybe_unused]] inline std::vector<std::string>
  get_all_entry_strings(
    const tl::read::input                         &cont,
    const size_t                                  &offset,
    const size_t                                  &size   = 0U,
    const size_t                                  &count  = 0U,
    const std::initializer_list<std::string_view> &needle = {},
    const size_t                                  &limit  = 0U)
{
  std::vector<std::string> vector{};
  cont.seek(static_cast<std::intmax_t>(offset), std::ios::beg);
  std::size_t max = (get_max)(count, limit);
  if (max != 0U) {
    vector.reserve(max);
  }
  {
    for (; (max == 0U || std::ranges::size(vector) != max)
           && (size == 0U || (cont.tell() < size + offset));) {
      std::string inner_path = clean_path_string(cont.get_line());
      if (std::ranges::empty(inner_path)) {
        break;
      }
      if (!tools::i_find_any(inner_path, needle)) {
        continue;
      }

      vector.emplace_back(std::move(inner_path));
    }
  }
  std::ranges::sort(vector);
  return vector;
}

// Get all entries from the FL file sorted and cleaned.
[[nodiscard]] [[maybe_unused]] inline auto
  get_all_entry_strings(
    const std::filesystem::path                   &path,
    const size_t                                  &offset,
    const size_t                                  &size   = 0U,
    const size_t                                  &count  = 0U,
    const std::initializer_list<std::string_view> &needle = {},
    const size_t                                  &limit  = 0U)
{

  std::vector<std::string> vector{};
  tl::read::from_file(
    [&](std::istream &istream) {
      vector = get_all_entry_strings(
        tl::read::input(&istream, true),
        offset,
        size,
        count,
        needle,
        limit);
    },
    path);
  return vector;
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
[[nodiscard]] [[maybe_unused]] inline std::vector<std::string>
  get_all_entry_strings(
    const std::filesystem::path                   &path,
    const std::string                             &data,
    const std::size_t                             &offset,
    const std::size_t                             &size   = 0U,
    const std::size_t                             &count  = 0U,
    const std::initializer_list<std::string_view> &needle = {},
    const std::size_t                             &limit  = 0U)
{

  if (!std::empty(data) && data.front() != '\0') {
    return get_all_entry_strings(data, offset, size, count, needle, limit);
  }
  else {}
  return get_all_entry_strings(path, offset, size, count, needle, limit);
}

/**
 * Eagerly populate a vector with pairs of (id,path), then sort it.
 * @param cont wrapper on istream or span. For the source data.
 * @param offset to start of data.
 * @param size + offset is the end of the data.
 * @param count max count detected from FI filesize / 12U.
 * @param needle set of strings to search for.
 * @param limit manually set max count.
 * @note If size, limit, count are all 0 it'll read till end of file. Or till it
 * reads an empty line.
 * @todo make needle a predicate lambda.
 */
[[nodiscard]] [[maybe_unused]] inline std::vector<
  std::pair<std::uint32_t, std::string>>
  get_all_entries(
    const tl::read::input                         &cont,
    const size_t                                  &offset,
    const size_t                                  &size   = 0U,
    const size_t                                  &count  = 0U,
    const std::initializer_list<std::string_view> &needle = {},
    const size_t                                  &limit  = 0U)
{
  std::vector<std::pair<std::uint32_t, std::string>> vector{};
  cont.seek(static_cast<std::intmax_t>(offset), std::ios::beg);
  std::size_t max = (get_max)(count, limit);
  if (max != 0U) {
    vector.reserve(max);
  }
  // id numerical order is same order as fi data. So need to keep the id so
  // we can reference the fi correctly.
  {
    for (std::uint32_t id = 0; (max == 0U || std::ranges::size(vector) != max)
                               && (size == 0U || (cont.tell() < size + offset));
         ++id) {
      std::string inner_path = clean_path_string(cont.get_line());
      if (std::ranges::empty(inner_path)) {
        break;
      }
      if (!tools::i_find_any(inner_path, needle)) {
        continue;
      }

      // https://youtu.be/oTMSgI1XjF8?t=1727
      vector.emplace_back(
        std::piecewise_construct,
        std::forward_as_tuple(id),
        std::forward_as_tuple(std::move(inner_path)));
    }
  }
  return sort_entries(std::move(vector));
}
// Get all entries from the FL file sorted and cleaned.
[[nodiscard]] [[maybe_unused]] inline auto
  get_all_entries(
    const std::filesystem::path                   &path,
    const size_t                                  &offset,
    const size_t                                  &size   = 0U,
    const size_t                                  &count  = 0U,
    const std::initializer_list<std::string_view> &needle = {},
    const size_t                                  &limit  = 0U)
{

  std::vector<std::pair<std::uint32_t, std::string>> vector{};
  tl::read::from_file(
    [&](std::istream &istream) {
      vector = get_all_entries(
        tl::read::input(&istream, true),
        offset,
        size,
        count,
        needle,
        limit);
    },
    path);
  return vector;
}

[[nodiscard]] [[maybe_unused]] inline std::vector<
  std::pair<std::uint32_t, std::string>>
  get_all_entries(
    const std::string                             &data,
    const size_t                                  &offset,
    const size_t                                  &size,
    const size_t                                  &count,
    const std::initializer_list<std::string_view> &needle,
    const size_t                                  &limit)
{
  return get_all_entries(
    tl::read::input(data, true),
    offset,
    size,
    count,
    needle,
    limit);
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
[[nodiscard]] [[maybe_unused]] inline std::vector<
  std::pair<std::uint32_t, std::string>>
  get_all_entries(
    const std::filesystem::path                   &path,
    const std::string                             &data,
    const size_t                                  &offset,
    const size_t                                  &size   = 0U,
    const size_t                                  &count  = 0U,
    const std::initializer_list<std::string_view> &needle = {},
    const size_t                                  &limit  = 0U)
{

  if (!std::empty(data) && data.front() != '\0') {
    return get_all_entries(data, offset, size, count, needle, limit);
  }
  else {}
  return get_all_entries(path, offset, size, count, needle, limit);
}

/**
 * Get a single entry that is the first match for needle.
 * @param data contains buffer of chars
 * @param needle is a group of strings to filter the output with.
 * @param offset is the number of bytes to skip.
 * @param size is max number of bytes. 0 is unlimited.
 * @param count is max results returned. 0 is unlimited.
 */
template<typename T>
[[nodiscard]] [[maybe_unused]] inline auto
  get_entry(
    const T                                       &data,
    const std::initializer_list<std::string_view> &needle,
    const size_t                                  &offset = 0U,
    const size_t                                  &size   = 0U,
    const size_t                                  &count  = 0U)
{
  auto vector  = get_all_entries(data, offset, size, count, needle, 1U);
  using valueT = typename decltype(vector)::value_type;
  if (std::empty(vector)) {
    return valueT{};
  }
  return vector.front();
}

/**
 * Get a single entry that is the first match for needle.
 * @param path contains path to file
 * @param data contains buffer of chars
 * @param needle is a group of strings to filter the output with.
 * @param offset is the number of bytes to skip.
 * @param size is max number of bytes. 0 is unlimited.
 * @param count is max results returned. 0 is unlimited.
 */
[[nodiscard]] [[maybe_unused]] inline auto
  get_entry(
    const std::filesystem::path                   &path,
    const std::string                             &data,
    const std::initializer_list<std::string_view> &needle,
    const size_t                                  &offset = 0U,
    const size_t                                  &size   = 0U,
    const size_t                                  &count  = 0U)
{
  if (std::ranges::empty(data)) {
    return get_entry(path, needle, offset, size, count);
  }
  return get_entry(data, needle, offset, size, count);
}
}// namespace open_viii::archive::fl
namespace open_viii::archive {

/**
 * Append FL path to buffer.
 * @tparam T type of output buffer.
 * @param path being wrote.
 * @note path is prepended with c:\ and appended with \r\n.
 */
template<is_insertable_or_ostream T>
inline void
  append_entry(T &output, const std::filesystem::path &path)
{
  using namespace std::string_literals;
  std::string string = "C:\\"s + path.string() + "\r\n"s;
  tl::string::undo_replace_slashes(string);
  tl::write::append(output, string);
}
///**
// * Append FL path to buffer.
// * @tparam T type of output buffer.
// * @param path being wrote.
// * @note path is prepended with c:\ and appended with \r\n.
// */
// template<is_insertable_or_ostream T>
// inline void
//  append_entry(T &output, const std::string_view &path)
//{
//  using namespace std::string_literals;
//  std::string string = "c:\\"s + std::string(path) + "\r\n"s;
//  tl::string::undo_replace_slashes(string);
//  tl::write::append(output, string);
//}
}// namespace open_viii::archive
#endif// !VIIIARCHIVE_FL_HPP