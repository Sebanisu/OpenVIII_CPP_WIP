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
#ifndef VIIIARCHIVE_ZZZ_H
#define VIIIARCHIVE_ZZZ_H
#include <string>
#include "FI.h"
#include "OpenVIII/Tools/Tools.h"
#include <filesystem>
#include <set>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include "FIFLFS.h"
#include "FileData.h"
namespace open_viii::archive {
struct [[maybe_unused]] ZZZ
{


private:
  // stored at top of zzz file to tell how many files are stored inside
  // unsigned int count_{};
  // file data inside zzz file.
  std::vector<FileData> m_data{};
  std::filesystem::path m_path{};

public:
  constexpr static auto EXT = ".zzz";
  [[maybe_unused]] [[nodiscard]] const auto &data() const noexcept { return m_data; }
  [[maybe_unused]] [[nodiscard]] const auto &path() const noexcept { return m_path; }
  ZZZ(const ZZZ &) = default;
  ZZZ &operator=(const ZZZ &) = default;
  ZZZ(ZZZ &&) = default;
  ZZZ &operator=(ZZZ &&) = default;
  ~ZZZ() = default;
  constexpr ZZZ() = default;
  explicit ZZZ(const std::filesystem::path &path)
  {
    if (!(path.has_extension() && Tools::i_equals(path.extension().string(), EXT)) || !std::filesystem::exists(path)) {
      return;
    }

    std::uint32_t count{};
    auto fp = std::ifstream(path, std::ios::binary | std::ios::in);
    if (!fp.is_open()) {
      fp.close();
      return;
    }
    m_path = path;
    Tools::read_val(fp, count);
    m_data.reserve(count);
    for (auto i = 0U; fp.is_open() && !fp.eof() && i < count; i++) {
      if ((m_data.emplace_back(fp).empty())) {
        std::cerr << "empty element detected and removed\n";
        m_data.pop_back();
      }
    }
    std::sort(m_data.begin(), m_data.end(), [](const FileData &left, const FileData &right) {
      const auto &right_string = right.get_path_string();
      const auto &left_string = left.get_path_string();
      const auto &right_size = std::size(right_string);
      const auto &left_size = std::size(left_string);
      if (left_size == right_size) {
        return left_string < right_string;
      }
      return left_size < right_size;
    });
    fp.close();
    m_data.shrink_to_fit();
  }
  void save_entry(const FileData &item, const std::string_view &strPath) const
  {
    auto buffer = FS::get_entry(m_path, item, 0U);
    std::cout << '{' << buffer.size() << ", " << strPath << "}\n";
    Tools::write_buffer(buffer, strPath);
  }
  void test() const
  {

    FIFLFS archive{};
    {
      auto beg = m_data.begin();
      auto end = m_data.end();
      for (auto cur = beg; cur < end; cur++) {
        // const auto &item : data_
        const auto &item = *cur;
        const auto &[strPath, zzzOffset, zzzSize] = item.get_tuple();
        {
          const auto &next = cur + 1 < end && (FIFLFS<true>::check_extension((*(cur + 1)).get_path_string()) != 0)
                               ? *(cur + 1)
                               : FileData();
          const auto &prev = cur - 1 > beg && (FIFLFS<true>::check_extension((*(cur - 1)).get_path_string()) != 0)
                               ? *(cur - 1)
                               : FileData();
          // getting a prev and next element to check vs cur item. To make sure at least 2 of them match so we don't add
          // an orphan to the FIFLFS archive.
          std::cout << '{' << zzzOffset << ", " << zzzSize << ", " << strPath << "}\n";
          if ((FIFLFS<true>::check_extension(strPath) != 0)
              && ((!next.empty()
                    && FIFLFS<true>::get_base_name(strPath) == FIFLFS<true>::get_base_name(next.get_path_string()))
                  ||

                  (!prev.empty()
                    && FIFLFS<true>::get_base_name(strPath) == FIFLFS<true>::get_base_name(prev.get_path_string())))) {

            std::filesystem::path fs_path(strPath);
            {
              // char retVal = archive.try_add_nested(path_, 0U, fsPath, item);
              TryAddT ret_val = archive.try_add(m_path, fs_path, item.offset(), item.uncompressed_size());
              if (ret_val == TryAddT::added_to_archive) {
                continue;
              }
              if (ret_val == TryAddT::archive_full) {
                archive.test();
                archive = {};
                continue;
              }
            }
          }
        }
        save_entry(item, strPath);
      }
    }
  }
  [[nodiscard]] static std::vector<std::pair<std::string, open_viii::archive::ZZZ>> get_files_from_path(
    const std::string_view path)
  {
    const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;

    std::vector<std::pair<std::string, open_viii::archive::ZZZ>> tmp{};
    tmp.reserve(2);// main and other
    int i{};
    for (const auto &file_entry : std::filesystem::directory_iterator(path, options)) {
      if (!(file_entry.path().has_extension() && Tools::i_equals(file_entry.path().extension().string(), EXT))) {
        continue;
      }
      // todo check for language codes to choose correct files

      auto &pair = tmp.emplace_back(std::piecewise_construct,
        std::forward_as_tuple(file_entry.path().filename().stem().string()),
        std::forward_as_tuple(file_entry));
      if (std::empty(pair.first)) {
        pair.first = "__" + std::to_string(i++);
      }
      std::transform(pair.first.begin(), pair.first.end(), pair.first.begin(), ::toupper);
    }
    tmp.shrink_to_fit();
    return tmp;
  }
  static void test_pair(const std::pair<std::string_view, open_viii::archive::ZZZ> &pair)
  {
    const auto &[name, zzz] = pair;
    std::cout << '{' << name << ", " << zzz.m_path << "}\n";
    zzz.test();
    // testFLPath(paths.FL(),paths.FI());
  }

  [[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const ZZZ &data)
  {
    return os << '{' << data.path().stem().string() << " zzz {" << std::size(data.data())
              << " File Entries from : " << data.path() << "}}";
  }
  [[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const std::optional<ZZZ> &data)
  {

    if (data.has_value()) {
      return os << data.value();
    }
    return os;
  }
  [[nodiscard]] std::vector<std::pair<unsigned int, std::string>> get_all_entries_data(
    const std::initializer_list<std::string_view> &filename) const
  {
    unsigned int i{};
    std::vector<std::pair<unsigned int, std::string>> vector{};
    for (const open_viii::archive::FileData &dataItem : data()) {
      {
        auto pathString = dataItem.get_path_string();
        if (open_viii::Tools::i_find_any(pathString, filename)) {
          vector.emplace_back(std::make_pair(i, pathString));
        }
      }
      i++;
    }
    return vector;
  }
  template<typename lambdaT>
  requires(std::invocable<lambdaT, std::vector<char>, std::string>) void execute_on(
    const std::initializer_list<std::string_view> &filename,
    const lambdaT &lambda)
  {
    for (const open_viii::archive::FileData &dataItem : data()) {
      {
        auto pathString = dataItem.get_path_string();
        if (open_viii::Tools::i_find_any(pathString, filename)) {
          lambda(FS::get_entry(m_path, dataItem), pathString);
        }
      }
    }
  }
};

}// namespace open_viii::Archive
#endif// VIIIARCHIVE_ZZZ_H