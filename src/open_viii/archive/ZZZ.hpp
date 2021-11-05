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
#ifndef VIIIARCHIVE_ZZZ_HPP
#define VIIIARCHIVE_ZZZ_HPP
#include "FI.hpp"
#include "FIFLFS.hpp"
#include "FileData.hpp"
#include "open_viii/tools/Tools.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <utility>
namespace open_viii::archive {
/**
 * ZZZ file archive from FF8 remaster.
 * @see https://github.com/myst6re/qt-zzz
 */
struct [[maybe_unused]] ZZZ
{
private:
  std::filesystem::path m_path{};
  /**
   * uint32_t count_{}; fallowed by array of file data
   */
  std::vector<FileData> m_data{};
  [[nodiscard]] auto
    load_data_from_file() const
  {
    std::vector<FileData> data{};
    if (
      m_path.has_extension()
      && tools::i_equals(m_path.extension().string(), EXT)
      && std::filesystem::exists(m_path)) {
      tools::read_from_file(
        [&data](std::istream &fp) {
          auto count{ tools::read_val<uint32_t>(fp) };
          data.reserve(count);
          while (!fp.eof() && count-- != 0U) {
            if ((data.emplace_back(fp).empty())) {
              std::cerr << "empty element detected and removed\n";
              data.pop_back();
            }
          }
        },
        m_path);
    }
    return data;
  }
  void
    sort_data()
  {
    std::ranges::sort(m_data, [](const FileData &left, const FileData &right) {
      const auto right_string = right.get_path_string();
      const auto left_string  = left.get_path_string();
      const auto right_size   = std::ranges::size(right_string);
      const auto left_size    = std::ranges::size(left_string);
      if (left_size == right_size) {
        return left_string < right_string;// clangtidy wants < to be nullptr
      }
      return left_size < right_size;
    });
    m_data.shrink_to_fit();
  }

public:
  constexpr static auto EXT = std::string_view(".zzz");
  [[maybe_unused]] [[nodiscard]] const auto &
    data() const noexcept
  {
    return m_data;
  }
  [[maybe_unused]] [[nodiscard]] const auto &
    path() const noexcept
  {
    return m_path;
  }
  constexpr ZZZ() = default;
  explicit ZZZ(std::filesystem::path path)
    : m_path(std::move(path)), m_data(load_data_from_file())
  {
    sort_data();
  }
  [[nodiscard]] static std::vector<
    std::pair<std::string, open_viii::archive::ZZZ>>
    get_files_from_path(const std::filesystem::path &path)
  {
    std::vector<std::pair<std::string, open_viii::archive::ZZZ>> tmp{};
    tmp.reserve(2);// main and other
    tools::execute_on_directory(
      path,
      {},
      { EXT },
      [&tmp, i = 0](const std::filesystem::path &file_entry) mutable {
        // todo check for language codes to choose correct files
        // this is done for the archives.
        auto &pair = tmp.emplace_back(
          std::piecewise_construct,
          std::forward_as_tuple(tools::get_base_name(file_entry)),
          std::forward_as_tuple(file_entry));
        if (std::empty(pair.first)) {
          pair.first = "__" + std::to_string(i++);
        }
      });
    tmp.shrink_to_fit();
    return tmp;
  }

  //  [[nodiscard]] std::vector<std::pair<unsigned int, std::string>>
  //    get_vector_of_indexes_and_files(
  //      const std::initializer_list<std::string_view> &filename) const
  //  {
  //    unsigned int i{};
  //    std::vector<std::pair<unsigned int, std::string>> vector{};
  //    for (const open_viii::archive::FileData &data_item : data()) {
  //      {
  //        auto path_string = data_item.get_path_string();
  //        if (open_viii::tools::i_find_any(path_string, filename)) {
  //          vector.emplace_back(std::make_pair(i, path_string));
  //        }
  //      }
  //      i++;
  //    }
  //    return vector;
  //  }
  using default_filter_lambda = decltype([](auto &&) {
    return true;
  });
  template<
    std::invocable<std::vector<char>, std::string> BinaryFunctionT,
    typename FilterT = default_filter_lambda>
  void
    execute_on(
      const std::initializer_list<std::string_view> &filename,
      BinaryFunctionT                              &&binary_function,
      FilterT                                      &&filter_lambda = {}) const
  {
    std::ranges::for_each(
      data(),
      [&binary_function, &filename, this, &filter_lambda](
        const open_viii::archive::FileData &dataItem) {
        auto pathString = dataItem.get_path_string();
        if (open_viii::tools::i_find_any(pathString, filename)) {
          if (filter_lambda(pathString)) {
            binary_function(
              FS::get_entry(m_path, dataItem),
              std::string(pathString));
          }
        }
      });
  }
  using default_lambda = decltype([](auto &&, auto &&) {});
  template<
    typename lambdaT = default_lambda,
    typename FilterT = default_filter_lambda>
  requires((std::invocable<lambdaT, FIFLFS<false>> || std::invocable<lambdaT, std::vector<char>, std::string>)) void execute_with_nested(
    [[maybe_unused]] const std::initializer_list<std::string_view> & = {},
    [[maybe_unused]] lambdaT                                      && = {},
    [[maybe_unused]] const std::initializer_list<std::string_view> & = {},
    FilterT                                                       && = {}) const
  {
    // only nested archives are handled in the other functions. maybe delete
    // this.
  }
  explicit operator bool() const
  {
    return !std::ranges::empty(m_data);
  }
};
inline std::ostream &
  operator<<(std::ostream &os, const ZZZ &data)
{
  return os << '{' << data.path().stem().string() << " zzz {"
            << std::ranges::size(data.data())
            << " File Entries from : " << data.path() << "}}";
}
inline std::ostream &
  operator<<(std::ostream &os, const std::optional<ZZZ> &data)
{
  if (data.has_value()) {
    return os << data.value();
  }
  return os;
}
}// namespace open_viii::archive
#endif// VIIIARCHIVE_ZZZ_HPP