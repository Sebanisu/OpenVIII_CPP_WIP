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



  [[nodiscard]] auto load_data_from_file() const
  {
    std::vector<FileData> data{};
    if (m_path.has_extension()
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

  void sort_data()
  {
    std::ranges::sort(m_data,
                      [](const FileData &left, const FileData &right) {
                             const auto right_string = right.get_path_string();
                             const auto left_string = left.get_path_string();
                             const auto right_size = std::ranges::size(right_string);
                             const auto left_size = std::ranges::size(left_string);
                             if (left_size == right_size) {
                               return left_string < right_string;
                             }
                             return left_size < right_size;
                      });
    m_data.shrink_to_fit();
  }
public:
  constexpr static auto EXT = ".zzz";
  [[maybe_unused]] [[nodiscard]] const auto &data() const noexcept
  {
    return m_data;
  }
  [[maybe_unused]] [[nodiscard]] const auto &path() const noexcept
  {
    return m_path;
  }
  ZZZ(const ZZZ &) = default;
  ZZZ &operator=(const ZZZ &) = default;
  ZZZ(ZZZ &&) = default;
  ZZZ &operator=(ZZZ &&) = default;
  ~ZZZ() = default;
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
    const std::filesystem::directory_options options =
      std::filesystem::directory_options::skip_permission_denied;

    std::vector<std::pair<std::string, open_viii::archive::ZZZ>> tmp{};
    tmp.reserve(2);// main and other
    int i{};
    for (const auto &file_entry :
      std::filesystem::directory_iterator(path, options)) {
      if (!(file_entry.path().has_extension()
            && tools::i_equals(file_entry.path().extension().string(), EXT))) {
        continue;
      }
      // todo check for language codes to choose correct files

      auto &pair = tmp.emplace_back(std::piecewise_construct,
        std::forward_as_tuple(file_entry.path().filename().stem().string()),
        std::forward_as_tuple(file_entry));
      if (std::empty(pair.first)) {
        pair.first = "__" + std::to_string(i++);
      }
      std::transform(
        pair.first.begin(), pair.first.end(), pair.first.begin(), ::toupper);
    }
    tmp.shrink_to_fit();
    return tmp;
  }

  [[nodiscard]] friend std::ostream &operator<<(
    std::ostream &os, const ZZZ &data)
  {
    return os << '{' << data.path().stem().string() << " zzz {"
              << std::ranges::size(data.data())
              << " File Entries from : " << data.path() << "}}";
  }
  [[nodiscard]] friend std::ostream &operator<<(
    std::ostream &os, const std::optional<ZZZ> &data)
  {

    if (data.has_value()) {
      return os << data.value();
    }
    return os;
  }
  [[nodiscard]] std::vector<std::pair<unsigned int, std::string>>
    get_vector_of_indexes_and_files(
      const std::initializer_list<std::string_view> &filename) const
  {
    unsigned int i{};
    std::vector<std::pair<unsigned int, std::string>> vector{};
    for (const open_viii::archive::FileData &data_item : data()) {
      {
        auto path_string = data_item.get_path_string();
        if (open_viii::tools::i_find_any(path_string, filename)) {
          vector.emplace_back(std::make_pair(i, path_string));
        }
      }
      i++;
    }
    return vector;
  }
  template<typename lambdaT>
  requires(std::invocable<lambdaT,
    std::vector<char>,
    std::string>) void execute_on(const std::initializer_list<std::string_view>
                                    &filename,
    const lambdaT &lambda)
  {
    for (const open_viii::archive::FileData &dataItem : data()) {
      {
        auto pathString = dataItem.get_path_string();
        if (open_viii::tools::i_find_any(pathString, filename)) {
          lambda(FS::get_entry(m_path, dataItem), std::string(pathString));
        }
      }
    }
  }
};

}// namespace open_viii::archive
#endif// VIIIARCHIVE_ZZZ_HPP