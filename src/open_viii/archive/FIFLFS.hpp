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
#ifndef VIIIARCHIVE_FIFLm_fsH
#define VIIIARCHIVE_FIFLm_fsH
#include "Grouping.hpp"
#include <algorithm>
#include <concepts>
#include <execution>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <thread>
#include <utility>
namespace open_viii::archive {
enum class fiflfsT {
  none,
  fl,
  fs,
  fi,
};
enum class TryAddT { not_part_of_archive, added_to_archive, archive_full };
template<bool HasNested = false> struct FIFLFS
{
private:
  Grouping<std::vector<char>> m_fi{};
  Grouping<std::vector<char>> m_fs{};
  Grouping<std::basic_string<char>>
         m_fl{};// this is char because the file contains strings.
  size_t m_count{};
  void   get_count() noexcept
  {
    m_count = FI::get_count(m_fi.size());
  }

public:
  [[maybe_unused]] [[nodiscard]] const auto &fi() const noexcept
  {
    return m_fi;
  }
  [[maybe_unused]] [[nodiscard]] const auto &fs() const noexcept
  {
    return m_fs;
  }
  [[maybe_unused]] [[nodiscard]] const auto &fl() const noexcept
  {
    return m_fl;
  }
  [[nodiscard]] bool all_set() const
  {
    return m_fi && m_fs && m_fl;
  }
  FIFLFS() = default;
  [[nodiscard]] friend std::ostream &operator<<(std::ostream &os,
                                                const FIFLFS &data)
  {
    const auto str = [](auto fiflfs) {
      return std::ranges::empty(fiflfs.nested_path())
               ? fiflfs.path()
               : fiflfs.path() / fiflfs.nested_path();
    };
    return os << '{' << data.get_base_name() << " {" << data.m_count
              << " File Entries from : " << str(data.m_fi) << ", "
              << str(data.m_fl) << ", " << str(data.m_fs) << "}}";
  }
  [[nodiscard]] archive::FI get_entry_by_index(const unsigned int &id) const
  {
    if (m_count == 0 || id < m_count) {
      const auto offset = archive::get_fi_entry_offset(id, m_fi.offset());
      if (!std::ranges::empty(m_fi.data())) {
        return tools::read_val<archive::FI>(m_fi.data(), offset);
        // return archive::FI(m_fi.data(), id, m_fi.offset());
      }
      return tools::read_value_from_file<archive::FI>(m_fi.path(), offset);
      // return archive::FI(m_fi.path(), id, m_fi.offset());
    }
    return {};
  }
  explicit operator bool() const noexcept
  {
    return all_set();
  }
  /**
   * Try and Add Nested data. There are 3 parts to an archive so it this
   adds
   * one and goes to next one till all 3 are added.
   * @tparam srcT Can be an container of data, or it will be a filesystem
   path
   * to a file with the data.
   * @tparam datT Data struct with offset, size and compression type.
   * @param src The data or path.
   * @param srcOffset Bytes skipped at the beginning of data or file.
   * @param fileEntry
   * @param fi The Data struct.
   * @return char of 0,1,2, 0 not part of archive, 1 part added, 2 being
   archive
   * full.;
   */
  template<typename srcT, FI_Like datT = archive::FI>
  requires(std::convertible_to<
             srcT,
             std::filesystem::path> || std::ranges::contiguous_range<srcT>)
    TryAddT try_add_nested(const srcT &                 src,
                           const size_t                 src_offset,
                           const std::filesystem::path &file_entry,
                           const datT &                 fi)
  {
    const fiflfsT i = check_extension(file_entry);
    switch (i) {
    case fiflfsT::none:
      return TryAddT::not_part_of_archive;
    case fiflfsT::fl: {
      m_fl = decltype(m_fl)(
        fl::clean_buffer(FS::get_entry<std::string>(src, fi, src_offset)),
        "",
        file_entry);
      break;
    }
    case fiflfsT::fs: {
      m_fs = decltype(m_fs)(FS::get_entry(src, fi, src_offset), "", file_entry);
      break;
    }
    case fiflfsT::fi: {
      m_fi = decltype(m_fi)(FS::get_entry(src, fi, src_offset), "", file_entry);
      get_count();
      break;
    }
    }
    compare_base_names();
    return all_set() ? TryAddT::archive_full : TryAddT::added_to_archive;
  }
  template<typename dstT = std::vector<char>, FI_Like fiT>
  dstT get_entry_buffer(const fiT &fi) const
  {
    if (!std::ranges::empty(m_fs.data())) {
      return FS::get_entry<dstT>(m_fs.data(), fi, m_fs.offset());
    } else {
      return FS::get_entry<dstT>(m_fs.path(), fi, m_fs.offset());
    }
  }
  auto static check_extension(const std::filesystem::path &path)
  {
    return static_cast<fiflfsT>(tools::i_ends_with_any_get_offset(
      path.string(),
      std::array{ open_viii::archive::fl::EXT,
                  open_viii::archive::FS::EXT,
                  open_viii::archive::FI::EXT }));
  }
  void compare_base_names() const
  {
    if ((m_fl.base() == m_fs.base() && m_fi.base() == m_fs.base())
        || std::empty(m_fl.base()) || std::empty(m_fi.base())
        || std::empty(m_fs.base())) {
      return;
    }
    if (m_fl.base() != m_fs.base() || m_fi.base() != m_fs.base()) {
      if (m_fl.base() == m_fi.base()) {
        std::cerr << "base name mismatch FS Data: " << m_fs.path() << '\n';
        std::cerr << "not matching:" << m_fl.path() << ", " << m_fi.path()
                  << '\n';
        // m_fs = {};
        exit(EXIT_FAILURE);
      }
    } else if (m_fi.base() != m_fl.base()) {
      if (m_fl.base() == m_fs.base()) {
        std::cerr << "base name mismatch FL Data: " << m_fl.path() << "\n";
        std::cerr << "not matching:" << m_fl.base() << ", " << m_fs.base()
                  << '\n';
        // m_fi = {};
        exit(EXIT_FAILURE);
      } else if (m_fi.base() == m_fs.base()) {
        std::cerr << "base name mismatch FI Data: " << m_fi.path() << "\n";
        std::cerr << "not matching:" << m_fi.path() << ", " << m_fs.path()
                  << '\n';
        // m_fl = {};
        exit(EXIT_FAILURE);
      }
    } else {
      std::cerr << "No basename matched!\n";
      exit(EXIT_FAILURE);
    }
  }
  [[nodiscard]] std::string get_base_name() const
  {
    for (const auto &path : { m_fi.base(), m_fl.base(), m_fs.base() }) {
      if (!std::ranges::empty(path)) {
        return path;
      }
    }
    return {};
  }
  template<std::ranges::contiguous_range outT = std::vector<char>, FI_Like fiT>
  [[nodiscard]] outT get_entry_data(const fiT &fi) const
  {
    return [this, &fi]() {
      if (std::ranges::empty(m_fs.data())) {
        return open_viii::archive::FS::get_entry<outT>(
          m_fs.path(), fi, m_fs.offset());
      }
      return open_viii::archive::FS::get_entry<outT>(
        m_fs.data(), fi, m_fs.offset());
    }();
  }
  [[nodiscard]] std::string
    get_full_path(const std::string_view &filename) const
  {
    return get_entry_id_and_path(filename).second;
  }
  [[nodiscard]] auto
    get_entry_id_and_path(const std::string_view &filename) const
  {
    if (std::ranges::empty(m_fl.data())) {
      return open_viii::archive::fl::get_entry(
        m_fl.path(), { filename }, m_fl.offset(), m_fl.size(), m_count);
    }
    return open_viii::archive::fl::get_entry_data(m_fl.path(),
                                                  m_fl.data(),
                                                  { filename },
                                                  m_fl.offset(),
                                                  m_fl.size(),
                                                  m_count);
  }
  template<typename outT = std::vector<char>>
  [[nodiscard]] outT get_entry_data(const std::string_view &filename) const
  {
    const auto &[id, path] = get_entry_id_and_path(filename);
    return get_entry_data<outT>(get_entry_by_index(id));
  }
  [[nodiscard]] std::vector<std::pair<unsigned int, std::string>>
    get_vector_of_indexes_and_files(
      const std::initializer_list<std::string_view> &filename) const
  {
    return archive::fl::get_all_entries_data(
      m_fl.path(), m_fl.data(), m_fl.offset(), m_fl.size(), m_count, filename);
  }
  template<typename lambdaT>
  requires(std::invocable<
           lambdaT,
           std::vector<char>,
           std::string>) void execute_on(const std::
                                           initializer_list<std::string_view>
                                             &          filename,
                                         const lambdaT &lambda) const
  {
    const auto results = get_vector_of_indexes_and_files(filename);
    std::ranges::for_each(
      results
        | std::views::filter(
          [this](const std::pair<unsigned int, std::string> &pair) -> bool {
            return check_extension(pair.second)
                   == fiflfsT::none;// prevent from running on nested archives.
                                    // We have another function for those.
          }),
      [this, &lambda](const std::pair<unsigned int, std::string> &pair) {
        auto fi = get_entry_by_index(pair.first);
        lambda(get_entry_buffer(fi), pair.second);
      });
  }
  template<typename lambdaT>
  requires((std::invocable<lambdaT, FIFLFS<false>> || std::invocable<lambdaT, std::vector<char>, std::string>)) void execute_with_nested(
    const std::initializer_list<std::string_view> &filename,
    const lambdaT                                  lambda,
    const std::initializer_list<std::string_view> &nested_filename = {}) const
  {
    if (!HasNested) {
      return;
    }
    FIFLFS<false> archive{};
    const auto    items = archive::fl::get_all_entries_data(
      m_fl.path(), m_fl.data(), m_fl.offset(), m_fl.size(), m_count, filename);
    std::for_each(
      std::execution::seq,
      items.cbegin(),
      items.cend(),
      [this, &archive, &lambda, &nested_filename](const auto &item) {
        const auto &[id, strVirtualPath] = item;
        const FI_Like auto    fi         = get_entry_by_index(id);
        std::filesystem::path virtualPath(strVirtualPath);
        const TryAddT         retVal = [&]() {
          if (fi.compression_type() == CompressionTypeT::none) {
            return archive.try_add(
              FileData(m_fs.offset() + fi.offset(), fi.uncompressed_size()),
              m_fs.path(),
              virtualPath);
          }
          return archive.try_add_nested(
            m_fs.path(), m_fs.offset(), virtualPath, fi);
        }();
        if (retVal == TryAddT::added_to_archive) {
          std::cout << "Added:\t" << virtualPath << '\n';
          //          if(static_cast<bool>(archive)) {
          //            retVal = TryAddT::archive_full;
          //          }
        }
        if (retVal == TryAddT::archive_full) {
          std::cout << "Full:\t" << archive.get_base_name() << '\n';
          if constexpr (std::
                          invocable<lambdaT, std::vector<char>, std::string>) {
            archive.template execute_on(nested_filename, lambda);
          } else if constexpr (std::invocable<lambdaT, FIFLFS<false>>) {
            lambda(archive);
          }
          archive = {};
        }
      });
  }
  template<FI_Like fiT>
  [[nodiscard]] TryAddT try_add(fiT                          fi_like,
                                const std::filesystem::path &parent_path,
                                const std::filesystem::path &child_path)
  {
    switch (check_extension(child_path)) {
    case fiflfsT::none:
      return TryAddT::not_part_of_archive;
    case fiflfsT::fi:
      m_fi = decltype(m_fi)(fi_like, parent_path, child_path);
      get_count();
      break;
    case fiflfsT::fl:
      m_fl = decltype(m_fl)(fi_like, parent_path, child_path);
      break;
    case fiflfsT::fs:
      m_fs = decltype(m_fs)(fi_like, parent_path, child_path);
      break;
    }
    return all_set() ? TryAddT::archive_full : TryAddT::added_to_archive;
  }
  [[nodiscard]] TryAddT
    get_fiflfs(auto &                  archive,
               const std::uint32_t     id,
               const std::string_view &str_virtual_path) const
  {
    FI_Like auto fi = get_entry_by_index(id);
    return [this, &archive, &fi, &str_virtual_path]() {
      std::filesystem::path virtualPath(str_virtual_path);
      if (!std::ranges::empty(m_fs.data())) {
        return archive.try_add_nested(
          m_fs.data(), m_fs.offset(), virtualPath, fi);
      }
      if (fi.compression_type() == CompressionTypeT::none) {
        auto localRetVal = archive.try_add(
          FileData(m_fs.offset() + fi.offset(), fi.uncompressed_size()),
          m_fs.path(),
          virtualPath);
        //        if (localRetVal != TryAddT::not_part_of_archive) {
        //          std::cout << virtualPath.filename() << " is uncompressed
        //          pointing at location in actual file!\n";
        //        }
        return localRetVal;
      }
      return archive.try_add_nested(
        m_fs.path(),
        m_fs.offset(),
        virtualPath,
        fi);// when path is sent a different function is used later.
    }();
  }
  [[nodiscard]] FIFLFS<false>
    get_fiflfs(const std::initializer_list<std::string_view> &filename) const
  {
    FIFLFS<false> archive{};
    if constexpr (!HasNested) {
      return archive;
    }
    const auto items = archive::fl::get_all_entries_data(
      m_fl.path(), m_fl.data(), m_fl.offset(), m_fl.size(), m_count, filename);
    for (const auto &[id, strVirtualPath] : items) {
      TryAddT tryAddT = get_fiflfs(archive, id, strVirtualPath);
      if (tryAddT == TryAddT::archive_full) {
        break;
      }
    }
    return archive;
  }
  [[nodiscard]] std::vector<FIFLFS<false>> get_fiflfs_entries(
    const std::initializer_list<std::string_view> &filename) const
  {
    std::vector<FIFLFS<false>> out{};
    FIFLFS<false>              archive{};
    const auto                 items = archive::fl::get_all_entries_data(
      m_fl.path(), m_fl.data(), m_fl.offset(), m_fl.size(), m_count, filename);
    for (const auto &[id, strVirtualPath] : items) {
      switch (get_fiflfs(archive, id, strVirtualPath)) {
      default:
        break;
      case TryAddT::archive_full:
        out.emplace_back(std::move(archive));
        archive = {};
        break;
      }
    }
    return out;
  }
};
}// namespace open_viii::archive
#endif// !VIIIARCHIVE_FIFLm_fsH
