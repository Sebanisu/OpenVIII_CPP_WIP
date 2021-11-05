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
#ifndef VIIIARCHIVE_FIFLFS_HPP
#define VIIIARCHIVE_FIFLFS_HPP
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

template<bool HasNested = false>
struct FIFLFS;// forward declare for concepts.
template<typename lambdaT>
concept executable_buffer_path
  = std::invocable<lambdaT, std::vector<char>, std::string>;
template<typename lambdaT>
concept executable_buffer_path_fi
  = std::invocable<lambdaT, std::vector<char>, std::string, FI>;
template<typename lambdaT>
concept executable_fiflfs_sans_nested = std::invocable<lambdaT, FIFLFS<false>>;

template<typename lambdaT>
concept executable_common_sans_nested
  = executable_buffer_path<lambdaT> || executable_buffer_path_fi<lambdaT>;

template<typename lambdaT>
concept executable_common_nested = executable_common_sans_nested<
  lambdaT> || executable_fiflfs_sans_nested<lambdaT>;

template<typename lambdaT>
concept filter_paths = std::is_invocable_r_v<bool, lambdaT, std::string>;

template<typename srcT>
concept path_or_range
  = std::convertible_to<srcT, std::filesystem::path> || std::ranges::
    contiguous_range<srcT>;

template<typename lambdaT>
concept executable_on_pair_of_int_string
  = std::invocable<lambdaT, std::pair<unsigned int, std::string>>;

enum class fiflfsT
{
  none,
  fl,
  fs,
  fi,
};
enum class TryAddT
{
  not_part_of_archive,
  added_to_archive,
  archive_full
};
template<bool HasNested>
struct FIFLFS
{
private:
  Grouping<std::vector<char>> m_fi{};
  Grouping<std::vector<char>> m_fs{};
  Grouping<std::basic_string<char>>
         m_fl{};// this is char because the file contains strings.
  size_t m_count{};
  void
    get_count() noexcept
  {
    m_count = FI::get_count(m_fi.size());
  }

public:
  [[maybe_unused]] [[nodiscard]] constexpr auto
    count() const noexcept
  {
    return m_count;
  }
  [[maybe_unused]] [[nodiscard]] const auto &
    fi() const noexcept
  {
    return m_fi;
  }
  [[maybe_unused]] [[nodiscard]] const auto &
    fs() const noexcept
  {
    return m_fs;
  }
  [[maybe_unused]] [[nodiscard]] const auto &
    fl() const noexcept
  {
    return m_fl;
  }
  [[nodiscard]] bool
    all_set() const
  {
    return m_fi && m_fs && m_fl;
  }
  FIFLFS() = default;
  FIFLFS(std::filesystem::path src)
  {
    const auto parent_path = src.parent_path();
    std::cout << "Parent Path: " << parent_path << std::endl;
    const auto stem = src.stem().string();
    std::cout << "Archive Name: " << stem << std::endl;
    open_viii::tools::execute_on_directory(
      parent_path,
      { stem },
      { FI::EXT, FS::EXT, fl::EXT },
      [this](const std::filesystem::path &path) {
        std::string   cur_path = path.string();
        const TryAddT result   = try_add(
            FI(static_cast<std::uint32_t>(std::filesystem::file_size(path)), 0U),
            path,
            path);
        switch (result) {
        case TryAddT::added_to_archive:
          std::cout << "added: " << path << '\n';
          break;
        case TryAddT::archive_full:
          std::cout << "added: " << path << '\n';
          std::cout << "Archive Loaded\n";
          break;
        case TryAddT::not_part_of_archive:
          std::cout << "not added: " << path << '\n';
          break;
        }
      });
  }
  template<std::unsigned_integral T>
  [[nodiscard]] archive::FI
    get_entry_by_index(const T id) const
  {
    if (m_count == 0 || id < m_count) {
      if (!std::ranges::empty(m_fi.data())) {
        return FI(m_fi.data(), id, m_fi.offset());
        // tools::read_val<archive::FI>(m_fi.data(), offset);
        // return archive::FI(m_fi.data(), id, m_fi.offset());
      }
      return FI(m_fi.path(), id, m_fi.offset());
      // tools::read_value_from_file<archive::FI>(m_fi.path(), offset);
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
  template<path_or_range srcT, FI_Like datT = archive::FI>
  TryAddT
    try_add_nested(
      const srcT                  &src,
      const size_t                 src_offset,
      const std::filesystem::path &file_entry,
      const datT                  &fi)
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
  template<std::ranges::contiguous_range dstT = std::vector<char>, FI_Like fiT>
  dstT
    get_entry_buffer(const fiT &fi) const
  {
    if (!std::ranges::empty(m_fs.data())) {
      return FS::get_entry<dstT>(m_fs.data(), fi, m_fs.offset());
    }
    else {
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
  void
    compare_base_names() const
  {
    if (
      (m_fl.base() == m_fs.base() && m_fi.base() == m_fs.base())
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
    }
    else if (m_fi.base() != m_fl.base()) {
      if (m_fl.base() == m_fs.base()) {
        std::cerr << "base name mismatch FL Data: " << m_fl.path() << "\n";
        std::cerr << "not matching:" << m_fl.base() << ", " << m_fs.base()
                  << '\n';
        // m_fi = {};
        exit(EXIT_FAILURE);
      }
      else if (m_fi.base() == m_fs.base()) {
        std::cerr << "base name mismatch FI Data: " << m_fi.path() << "\n";
        std::cerr << "not matching:" << m_fi.path() << ", " << m_fs.path()
                  << '\n';
        // m_fl = {};
        exit(EXIT_FAILURE);
      }
    }
    else {
      std::cerr << "No basename matched!\n";
      exit(EXIT_FAILURE);
    }
  }
  [[nodiscard]] std::string
    get_base_name() const
  {
    for (const auto &path : { m_fi.base(), m_fl.base(), m_fs.base() }) {
      if (!std::ranges::empty(path)) {
        return path;
      }
    }
    return {};
  }
  template<std::ranges::contiguous_range outT = std::vector<char>, FI_Like fiT>
  [[nodiscard]] outT
    get_entry_data(const fiT &fi) const
  {
    return [this, &fi]() {
      if (std::ranges::empty(m_fs.data())) {
        return open_viii::archive::FS::get_entry<outT>(
          m_fs.path(),
          fi,
          m_fs.offset());
      }
      return open_viii::archive::FS::get_entry<outT>(
        m_fs.data(),
        fi,
        m_fs.offset());
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
        m_fl.path(),
        { filename },
        m_fl.offset(),
        m_fl.size(),
        m_count);
    }
    return open_viii::archive::fl::get_entry(
      m_fl.path(),
      m_fl.data(),
      { filename },
      m_fl.offset(),
      m_fl.size(),
      m_count);
  }
  template<std::ranges::contiguous_range outT = std::vector<char>>
  [[nodiscard]] outT
    get_entry_data(const std::string_view &filename) const
  {
    const auto &[id, path] = get_entry_id_and_path(filename);
    return get_entry_data<outT>(get_entry_by_index(id));
  }

  template<std::ranges::contiguous_range outT = std::vector<char>>
  [[nodiscard]] outT
    get_entry_data(
      const std::initializer_list<std::string_view> &filenames,
      std::string *const                             out_name = nullptr,
      std::size_t *const out_name_pos                         = nullptr) const
  {
    if (out_name_pos != nullptr) {
      *out_name_pos = 0U;
    }
    using pair = decltype(get_entry_id_and_path(""));
    pair       tmp{};
    const auto it
      = std::ranges::find_if(filenames, [&tmp, this](std::string_view str) {
          tmp = get_entry_id_and_path(str);
          return tmp != pair{};
        });
    if (it != filenames.end()) {
      if (out_name_pos != nullptr) {
        *out_name_pos
          = static_cast<std::size_t>(std::distance(filenames.begin(), it));
      }
      const auto &[id, path] = tmp;
      if (out_name != nullptr) {
        *out_name = path;
      }
      return get_entry_data<outT>(get_entry_by_index(id));
    }
    return {};
  }

  [[nodiscard]] std::vector<std::pair<unsigned int, std::string>>
    get_vector_of_indexes_and_files(
      const std::initializer_list<std::string_view> &filename) const
  {
    return archive::fl::get_all_entries(
      m_fl.path(),
      m_fl.data(),
      m_fl.offset(),
      m_fl.size(),
      m_count,
      filename);
  }
  template<executable_on_pair_of_int_string lambdaT>
  void
    for_each_sans_nested(
      const std::vector<std::pair<unsigned int, std::string>> &results,
      lambdaT                                                &&process) const
  {
    if constexpr (HasNested) {
      std::ranges::for_each(
        results
          | std::views::filter(
            [this](const std::pair<unsigned int, std::string> &pair) -> bool {
              return check_extension(pair.second)
                  == fiflfsT::none;// prevent from running on nested archives.
                                   // We have another function for those.
            }),
        process);
    }
    else {
      std::ranges::for_each(results, process);
    }
  }
  static constexpr auto default_filter_lambda = [](auto &&) {
    return true;
  };
  template<
    executable_common_sans_nested lambdaT,
    filter_paths                  filterT = decltype(default_filter_lambda)>
  void
    execute_on(
      const std::initializer_list<std::string_view> &filename,
      lambdaT                                      &&lambda,
      filterT                                      &&filter_lambda = {}) const
  {
    const auto results = get_vector_of_indexes_and_files(filename);
    const auto process = [this, &lambda, &filter_lambda](
                           const std::pair<unsigned int, std::string> &pair) {
      if (filter_lambda(pair.second)) {
        auto fi = get_entry_by_index(pair.first);
        if constexpr (executable_buffer_path<lambdaT>) {

          lambda(get_entry_buffer(fi), pair.second);
        }
        else if constexpr (executable_buffer_path_fi<lambdaT>) {
          lambda(get_entry_buffer(fi), pair.second, fi);
        }
      }
    };
    for_each_sans_nested(results, process);
  }
  [[nodiscard]] std::vector<std::string>
    map_data() const
  {
    if constexpr (!HasNested) {
      return {};
    }
    else {
      std::vector<std::string> list{};
      FIFLFS<false>            archive = {};
      const auto               items   = get_all_items_from_fl({ "mapdata" });
      (void)std::any_of(
        std::execution::seq,
        items.cbegin(),
        items.cend(),
        [&archive, this, &list](const auto &item) {
          if (fill_archive_lambda(archive)(item)) {
            const auto raw_list
              = archive.template get_entry_data<std::string>("maplist");
            // archive::fl::get_all_entries(raw_list,0,0,0)
            std::stringstream ss{ raw_list };
            std::string       tmp{};
            while (std::getline(ss, tmp)) {
              while (!tmp.empty() && tmp.back() == '\r') {
                tmp.pop_back();
              }
              if (!tmp.empty()) {
                list.emplace_back(std::move(tmp));
              }
            }
            return true;
          }
          return false;
        });
      return list;
    }
  }

  auto
    fill_archive_lambda(FIFLFS<false> &archive) const
  {
    return [this, &archive](const auto &item) -> bool {
      const auto &[id, strVirtualPath] = item;
      const FI_Like auto    fi         = get_entry_by_index(id);
      std::filesystem::path virtualPath(strVirtualPath);
      const TryAddT         retVal = [&]() {
        if (fi.compression_type() == CompressionTypeT::none) {
          return archive.try_add(
                    FileData(
                      static_cast<unsigned long>(m_fs.offset() + fi.offset()),
                      fi.uncompressed_size()),
                    m_fs.path(),
                    virtualPath);
        }
        return archive
          .try_add_nested(m_fs.path(), m_fs.offset(), virtualPath, fi);
      }();
      return retVal == TryAddT::archive_full;
    };
  }
  auto
    get_all_items_from_fl(
      const std::initializer_list<std::string_view> &filename) const
  {
    return archive::fl::get_all_entries(
      m_fl.path(),
      m_fl.data(),
      m_fl.offset(),
      m_fl.size(),
      m_count,
      filename);
  }
  template<
    executable_common_nested lambdaT,
    filter_paths             filterT = decltype(default_filter_lambda)>
  void
    execute_with_nested(
      const std::initializer_list<std::string_view> &filename,
      lambdaT                                      &&lambda,
      const std::initializer_list<std::string_view> &nested_filename = {},
      filterT                                      &&filter_lambda   = {},
      bool                                           limit_once = false) const
  {
    if constexpr (!HasNested) {
      return;
    }
    else {
      FIFLFS<false> archive = {};
      const auto    items   = get_all_items_from_fl(filename);
      const auto    pFunction
        = [&lambda, &nested_filename, &archive, this, &filter_lambda](
            const auto &item) -> bool {
        if (filter_lambda(item.second) && fill_archive_lambda(archive)(item)) {
          if constexpr (executable_common_sans_nested<lambdaT>) {
            archive.execute_on(nested_filename, lambda, filter_lambda);
          }
          else if constexpr (executable_fiflfs_sans_nested<lambdaT>) {
            lambda(std::move(archive));
          }
          archive = {};
          return true;
        }
        return false;
      };
      if (!limit_once)
        std::for_each(
          std::execution::seq,
          items.cbegin(),
          items.cend(),
          pFunction);
      else {
        (void)std::any_of(
          std::execution::seq,
          items.cbegin(),
          items.cend(),
          pFunction);
      }
    }
  }
  template<FI_Like fiT>
  [[nodiscard]] TryAddT
    try_add(
      fiT                          fi_like,
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
};

[[nodiscard]] inline TryAddT
  get_fiflfs(
    const FIFLFS<true>    &source,
    FIFLFS<false>         &archive,
    const std::uint32_t    id,
    const std::string_view str_virtual_path)
{
  FI fi = source.get_entry_by_index(id);
  return [&source, &archive, &fi, &str_virtual_path]() {
    std::filesystem::path virtualPath(str_virtual_path);
    if (!std::ranges::empty(source.fs().data())) {
      return archive.try_add_nested(
        source.fs().data(),
        source.fs().offset(),
        virtualPath,
        fi);
    }
    if (fi.compression_type() == CompressionTypeT::none) {
      auto localRetVal = archive.try_add(
        FileData(source.fs().offset() + fi.offset(), fi.uncompressed_size()),
        source.fs().path(),
        virtualPath);
      //        if (localRetVal != TryAddT::not_part_of_archive) {
      //          std::cout << virtualPath.filename() << " is uncompressed
      //          pointing at location in actual file!\n";
      //        }
      return localRetVal;
    }
    return archive.try_add_nested(
      source.fs().path(),
      source.fs().offset(),
      virtualPath,
      fi);// when path is sent a different function is used later.
  }();
}
[[nodiscard]] inline FIFLFS<false>
  get_fiflfs(
    const FIFLFS<true>                            &source,
    const std::initializer_list<std::string_view> &filename)
{
  FIFLFS<false> archive{};
  const auto    items = archive::fl::get_all_entries(
       source.fl().path(),
       source.fl().data(),
       source.fl().offset(),
       source.fl().size(),
       source.count(),
       filename);
  for (const auto &[id, strVirtualPath] : items) {
    TryAddT tryAddT = get_fiflfs(source, archive, id, strVirtualPath);
    if (tryAddT == TryAddT::archive_full) {
      break;
    }
  }
  return archive;
}
[[nodiscard]] inline auto
  find_match(
    const std::vector<std::filesystem::path> &paths,
    const std::filesystem::path              &in_path)
{
  return std::ranges::find_if(
    paths,
    [&in_path](const std::filesystem::path &new_path) {
      return new_path.has_filename()
          && in_path.filename() == new_path.filename();
    });
}

[[nodiscard]] inline bool
  any_matches(
    const std::vector<std::filesystem::path> &paths,
    const std::filesystem::path              &in_path)
{
  return find_match(paths, in_path) != std::ranges::end(paths);
}

[[nodiscard]] inline bool
  any_matches(
    const std::vector<std::filesystem::path>                 &paths,
    const std::vector<std::pair<std::uint32_t, std::string>> &pairs)
{
  return std::ranges::any_of(pairs, [&paths](const auto &pair) {
    const auto in_path = std::filesystem::path(pair.second);
    return in_path.has_filename() && any_matches(paths, in_path);
  });
}
/**
 * create a copy of the archive with the files that have the same filename
 * replaced with a new one. rest are copied with no changes.
 * @param source original archive
 * @param paths paths to new files.
 * @return paths to new archive files.
 */
template<bool Nested = false>
[[nodiscard]] inline std::vector<std::filesystem::path>
  replace_files(
    const FIFLFS<Nested>                     &source,
    const std::vector<std::filesystem::path> &paths)
{

  std::vector<std::filesystem::path> r{};
  auto pairs = source.get_vector_of_indexes_and_files({});

  // make sure there is a match.
  if (!any_matches(paths, pairs)) {
    return r;
  }
  std::ranges::sort(pairs);
  const auto &fi_name = source.fi().nested_path_or_path().filename();
  const auto &fl_name = source.fl().nested_path_or_path().filename();
  const auto &fs_name = source.fs().nested_path_or_path().filename();
  std::cout << "Destination FI: " << fi_name << '\n';
  std::cout << "Destination FL: " << fl_name << '\n';
  std::cout << "Destination FS: " << fs_name << '\n';
  if (fi_name.empty() || fl_name.empty() || fs_name.empty()) {
    return r;
  }
  const auto   &temp = std::filesystem::temp_directory_path();
  std::ofstream fs_fi(
    temp / fi_name,
    std::ios::out | std::ios::binary | std::ios::trunc);
  std::ofstream fs_fl(
    temp / fl_name,
    std::ios::out | std::ios::binary | std::ios::trunc);
  std::ofstream fs_fs(
    temp / fs_name,
    std::ios::out | std::ios::binary | std::ios::trunc);
  std::ranges::for_each(pairs, [&](const auto &pair) {
    const auto &[i, in_path]         = pair;
    const FI               source_fi = source.get_entry_by_index(i);
    //    switch (source_fi.compression_type()) {
    //    case open_viii::CompressionTypeT::none:
    //      std::puts("\t Not Compressed");
    //      break;
    //    case open_viii::CompressionTypeT::lzss:
    //      std::puts("\t LZSS Compressed");
    //      break;
    //    case open_viii::CompressionTypeT::lz4:
    //      std::puts("\t LZ4 Compressed");
    //      break;
    //    }

    // match found use the new file
    open_viii::archive::FI fi = [&]([[maybe_unused]] const auto &in_path_ref) {
      if (auto match = find_match(paths, in_path_ref);
          match != std::ranges::end(paths)) {
        std::cout << "Updated: " << pair.second << std::endl;
        return open_viii::archive::append_entry(
          fs_fs,
          *match,
          source_fi.compression_type());
      }
      return open_viii::archive::append_entry(
        fs_fs,
        source.get_entry_data(source_fi),
        // CompressionTypeT::none);
        source_fi.compression_type());
    }(in_path);
    open_viii::archive::append_entry(fs_fi, fi);
    open_viii::archive::append_entry(fs_fl, std::filesystem::path(in_path));
    //    std::puts("\t\tAdded File");
  });
  r.emplace_back(temp / fi_name);
  r.emplace_back(temp / fl_name);
  r.emplace_back(temp / fs_name);
  return r;
}
inline auto
  get_fiflfs_entries(
    const FIFLFS<true>                            &source,
    const std::initializer_list<std::string_view> &filename)
{
  std::vector<FIFLFS<false>> out{};
  const auto                 items = archive::fl::get_all_entries(
                    source.fl().path(),
                    source.fl().data(),
                    source.fl().offset(),
                    source.fl().size(),
                    source.count(),
                    filename);
  for (const auto &[id, strVirtualPath] : items) {
    const TryAddT t
      = get_fiflfs(source, out.emplace_back(), id, strVirtualPath);
    if (t != TryAddT::archive_full) {
      out.pop_back();
    }
  }
  return out;
}
template<bool has_nested>
inline std::ostream &
  operator<<(std::ostream &os, const FIFLFS<has_nested> &data)
{
  const auto str = [](auto fiflfs) {
    return std::ranges::empty(fiflfs.nested_path())
           ? fiflfs.path()
           : fiflfs.path() / fiflfs.nested_path();
  };
  return os << '{' << data.get_base_name() << " {" << data.count()
            << " File Entries from : " << str(data.fi()) << ", "
            << str(data.fl()) << ", " << str(data.fs()) << "}}";
}
}// namespace open_viii::archive
#endif// !VIIIARCHIVE_FIFLFS_HPP
