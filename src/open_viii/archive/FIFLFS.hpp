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
#include "FI.hpp"
#include "FL.hpp"
#include "FS.hpp"
#include <algorithm>
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

enum class TryAddT { not_part_of_archive, added_to_archive, archive_full };
template<bool HasNested = false> struct FIFLFS
{
private:
  template<std::ranges::contiguous_range T> struct [[maybe_unused]] Grouping
  {
  private:
    mutable std::filesystem::path m_path{};
    mutable std::size_t m_offset{};
    mutable std::size_t m_size{};// if forced otherwise 0;
    mutable T m_data{};
    mutable std::string m_base{};
    mutable std::filesystem::path m_nested_path{};

  public:
    [[nodiscard]] std::filesystem::path &path() const noexcept
    {
      return m_path;
    }
    [[nodiscard]] std::size_t &offset() const noexcept
    {
      return m_offset;
    }
    /**
     * Size of file
     * @return size_t
     */
    [[nodiscard]] std::size_t &size() const noexcept
    {
      if (m_size == 0U) {
        if (!std::ranges::empty(data())) {
          m_size = std::ranges::size(data());
        } else if (std::filesystem::exists(path())) {
          m_size = std::filesystem::file_size(path());
        }
      }
      return m_size;
    }
    [[nodiscard]] T &data() const noexcept
    {
      return m_data;
    }
    [[nodiscard]] std::string &base() const noexcept
    {
      return m_base;
    }
    [[nodiscard]] std::filesystem::path &nested_path() const noexcept
    {
      return m_nested_path;
    }

    //    void path(std::filesystem::path && value) const noexcept{ m_path = value; }
    //    void offset(std::size_t && value) const noexcept{ m_offset = value; }
    //    void size(std::size_t && value) const noexcept{ m_size = value; }// if forced otherwise 0;
    //    void data(T && value) const noexcept{ m_data = value; }
    //    void base(std::string && value) const noexcept{ m_base = value; }
    //    void nested_path(std::filesystem::path && value) const noexcept{ m_nested_path = value; }


    void path(const std::filesystem::path &value) const noexcept
    {
      m_path = value;
    }
    void offset(const std::size_t &value) const noexcept
    {
      m_offset = value;
    }
    void size(const std::size_t &value) const noexcept
    {
      m_size = value;
    }
    void data(T &&value) const noexcept
    {
      m_data = std::move(value);
    }
    void base(const std::string &value) const noexcept
    {
      m_base = value;
    }
    void nested_path(const std::filesystem::path &value) const noexcept
    {
      m_nested_path = value;
    }

    //    // Assigns basename and returns it.
    [[maybe_unused]] std::string get_base_name() const noexcept
    {
      return m_base = FIFLFS::get_base_name(m_path);
    }
    explicit operator bool() const
    {
      return (!std::ranges::empty(m_path) && std::filesystem::exists(m_path)) || !std::ranges::empty(m_data)
             || (!std::ranges::empty(m_path) && (m_offset > 0 || m_size > 0));
    }
  };
  Grouping<std::vector<char>> m_fi{};
  Grouping<std::vector<char>> m_fs{};
  Grouping<std::basic_string<char>> m_fl{};// this is char because the file contains strings.
  mutable size_t m_count{};
  void get_count() const noexcept
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

  [[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const FIFLFS &data)
  {
    const auto str = [](auto fiflfs) {
      return std::ranges::empty(fiflfs.nested_path()) ? fiflfs.path() : fiflfs.path() / fiflfs.nested_path();
    };
    return os << '{' << data.get_base_name() << " {" << data.m_count << " File Entries from : " << str(data.m_fi)
              << ", " << str(data.m_fl) << ", " << str(data.m_fs) << "}}";
  }

  [[nodiscard]] archive::FI get_entry_by_index(const unsigned int &id) const
  {
    if (m_count == 0 || id < m_count) {
      if (!std::ranges::empty(m_fi.data())) {
        return archive::FI(m_fi.data(), id, m_fi.offset());
      }
      return archive::FI(m_fi.path(), id, m_fi.offset());
    }
    return {};
  }


  explicit operator bool() const noexcept
  {
    return all_set();
  }
  /*
   * 0 = didn't add
   * 1 = added
   * 2 = added and all set
   * */

  TryAddT try_add(const std::filesystem::path &existing_file_path,
    const std::filesystem::path &nested_path = "",
    size_t offset = 0U,
    size_t size = 0U) const
  {
    const auto set = [&existing_file_path, &offset, &nested_path, &size, this](auto &ds) {
      ds.path(existing_file_path);
      ds.offset(offset);
      ds.size(size);
      ds.nested_path(nested_path);
      if (nested_path.has_stem()) {
        ds.base(get_base_name(nested_path));
      } else {
        ds.get_base_name();
      }
    };
    const auto i = nested_path.has_extension() ? check_extension(nested_path) : check_extension(existing_file_path);
    if (i >= 1 && i <= 3) {
      switch (i) {
      case 1: {
        set(m_fl);
        break;
      }
      case 2: {
        set(m_fs);
        break;
      }
      case 3: {
        set(m_fi);
        get_count();
        break;
      }
      }
      compare_base_names();
      return all_set() ? TryAddT::archive_full : TryAddT::added_to_archive;
    }

    return TryAddT::not_part_of_archive;
  }
  /**
   * Try and Add Nested data. There are 3 parts to an archive so it this adds one and goes to next one till all 3 are
   * added.
   * @tparam srcT Can be an container of data, or it will be a filesystem path to a file with the data.
   * @tparam datT Data struct with offset, size and compression type.
   * @param src The data or path.
   * @param srcOffset Bytes skipped at the beginning of data or file.
   * @param fileEntry
   * @param fi The Data struct.
   * @return char of 0,1,2, 0 not part of archive, 1 part added, 2 being archive full.;
   */
  template<typename srcT, FI_Like datT = archive::FI>
  requires(std::convertible_to<srcT, std::filesystem::path> || std::ranges::contiguous_range<srcT>) TryAddT
    try_add_nested(const srcT &src,
      const size_t src_offset,
      const std::filesystem::path &file_entry,
      const datT &fi) const
  {

    const auto set = [&file_entry](auto &ds) {
      ds.path(file_entry);
      ds.offset(0U);// the offset is 0 because we are getting the truncated data below.
      ds.get_base_name();
    };
    const auto i = check_extension(file_entry);
    if (i >= 1 && i <= 3) {
      switch (i) {
      case 1: {
        set(m_fl);
        m_fl.data(FS::get_entry<std::string>(src, fi, src_offset));
        FL::clean_buffer(m_fl.data());
        break;
      }
      case 2: {
        set(m_fs);
        m_fs.data(FS::get_entry(src, fi, src_offset));
        break;
      }

      case 3: {
        set(m_fi);
        m_fi.data(FS::get_entry(src, fi, src_offset));
        get_count();
        break;
      }
      }
      compare_base_names();
      return all_set() ? TryAddT::archive_full : TryAddT::added_to_archive;
    }
    return TryAddT::not_part_of_archive;
  }
  //  void
  //    saveIMG(const std::vector<char> &buffer, const std::string_view &path, const std::string_view &root = "tmp")
  //    const
  //  {
  //    if (std::ranges::size(buffer) > 0) {
  //      auto dir = std::filesystem::path(root);
  //      auto filename = dir / path;
  //      if (filename.has_extension()) {
  //        if (tools::i_equals(filename.extension().string(), ".lzs")) {
  //          graphics::lzs(buffer).Save(filename.string() + ".ppm");
  //        }
  //
  //        if (tools::i_equals(filename.extension().string(), ".tim")) {
  //          const auto timV = graphics::tim(buffer);
  //          std::cout << timV;
  //          timV.Save(filename.string() + ".ppm");
  //        }
  //        if (tools::i_equals(filename.extension().string(), ".tdw")) {
  //          const auto timV = graphics::tdw(buffer);
  //          std::cout << timV.TIM();
  //          timV.TIM().Save(filename.string() + ".ppm");
  //        }
  //
  //        if (tools::i_equals(filename.extension().string(), ".tex")) {
  //          graphics::tex(buffer).Save(filename.string() + ".ppm");
  //        }
  //      }
  //    }
  //  }

  template<typename dstT = std::vector<char>, FI_Like fiT> dstT get_entry_buffer(const fiT &fi) const
  {
    if (!std::ranges::empty(m_fs.data())) {
      return FS::get_entry<dstT>(m_fs.data(), fi, m_fs.offset());
    } else {
      return FS::get_entry<dstT>(m_fs.path(), fi, m_fs.offset());
    }
  }
  void test() const
  {
    //    if (!std::filesystem::exists(m_fl.path())) {
    //      std::cout << "nested file!\n";
    //    }
    //    std::cout << *this << '\n';
    //    std::cout << "Getting Filenames from : " << m_fl.path() << '\n';
    FIFLFS archive{};
    using namespace std::string_view_literals;
    auto items = archive::FL::get_all_entries_data(m_fl.path(), m_fl.data(), m_fl.offset(), m_fl.size(), m_count, {});
    for (const auto &item : items) {
      const auto &[id, strVirtualPath] = item;
      // std::cout << "try_add_nested: {" << id << ", " << strVirtualPath << "}\n";

      FI_Like auto fi = get_entry_by_index(id);
      {
        TryAddT retVal = [this, &archive, &fi, &strVirtualPath]() {
          std::filesystem::path virtualPath(strVirtualPath);
          if (!std::ranges::empty(m_fs.data())) {
            return archive.try_add_nested(m_fs.data(), m_fs.offset(), virtualPath, fi);
          }

          if (fi.compression_type() == CompressionTypeT::none) {
            auto localRetVal =
              archive.try_add(m_fs.path(), virtualPath, m_fs.offset() + fi.offset(), fi.uncompressed_size());
            //            if (localRetVal != TryAddT::not_part_of_archive) {
            //              std::cout << virtualPath.filename() << " is uncompressed pointing at location in actual
            //              file!\n";
            //            }
            return localRetVal;
          }
          return archive.try_add_nested(
            m_fs.path(), m_fs.offset(), virtualPath, fi);// when path is sent a different function is used later.
        }();
        if (retVal == TryAddT::added_to_archive) {
          continue;
        }
        if (retVal == TryAddT::archive_full) {
          archive.test();
          archive = {};
          continue;
        }
      }
      {
        const auto buffer = get_entry_buffer(fi);
        if (buffer.empty()) {
          //          std::cout << '{' << id << ", "
          //                    << "Empty!"
          //                    << ", " << strVirtualPath << "}" << fi << std::endl;
          if (!(fi.uncompressed_size() == 0 && fi.compression_type() == CompressionTypeT::none)) {
            exit(EXIT_FAILURE);
          }
        }
        if (fi.uncompressed_size() != buffer.size()) {
          exit(EXIT_FAILURE);
        }
        //        std::cout << '{' << id << ", " << buffer.size() << ", " << strVirtualPath << "}" << std::endl;
        Tools::write_buffer(buffer, strVirtualPath);
        // saveIMG(buffer, strVirtualPath);
      }
    }
  }
  static auto get_files_from_path(const std::filesystem::path &path)
  {
    const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;
    std::vector<std::pair<std::string, open_viii::archive::FIFLFS<true>>> tmp{};
    constexpr auto default_size = 6U;// battle, field, magic, main, menu, world
    tmp.reserve(default_size);
    auto archive = open_viii::archive::FIFLFS<true>();
    for (const auto &file_entry : std::filesystem::directory_iterator(path, options)) {
      if (archive.try_add(file_entry) != TryAddT::not_part_of_archive) {
        if (archive.all_set()) {// todo confirm basename matches right now i'm assuming the 3 files are together.
          // todo check for language codes to choose correct files
          // auto key = archive.get_base_name();
          auto bn = archive.get_base_name();
          tmp.emplace_back(
            std::piecewise_construct, std::forward_as_tuple(std::move(bn)), std::forward_as_tuple(std::move(archive)));
          archive = {};
        }
      }
    }
    tmp.shrink_to_fit();// if there is more than 6 it'll collapse the vector
    return tmp;
  }
  auto static check_extension(const std::filesystem::path &path)
  {
    return Tools::i_ends_with_any(
      path.string(), { open_viii::archive::FL::EXT, open_viii::archive::FS::EXT, open_viii::archive::FI::EXT });
  }


  void compare_base_names() const
  {
    if ((m_fl.base() == m_fs.base() && m_fi.base() == m_fs.base()) || std::empty(m_fl.base()) || std::empty(m_fi.base())
        || std::empty(m_fs.base())) {
      return;
    }
    if (m_fl.base() != m_fs.base() || m_fi.base() != m_fs.base()) {
      if (m_fl.base() == m_fi.base()) {
        std::cerr << "base name mismatch FS Data: " << m_fs.path() << '\n';
        std::cerr << "not matching:" << m_fl.path() << ", " << m_fi.path() << '\n';
        // m_fs = {};
        exit(EXIT_FAILURE);
      }
    } else if (m_fi.base() != m_fl.base()) {
      if (m_fl.base() == m_fs.base()) {
        std::cerr << "base name mismatch FL Data: " << m_fl.path() << "\n";
        std::cerr << "not matching:" << m_fl.base() << ", " << m_fs.base() << '\n';
        // m_fi = {};
        exit(EXIT_FAILURE);
      } else if (m_fi.base() == m_fs.base()) {
        std::cerr << "base name mismatch FI Data: " << m_fi.path() << "\n";
        std::cerr << "not matching:" << m_fi.path() << ", " << m_fs.path() << '\n';
        // m_fl = {};
        exit(EXIT_FAILURE);
      }
    } else {
      std::cerr << "No basename matched!\n";
      exit(EXIT_FAILURE);
    }
  }

  [[nodiscard]] std::string static get_base_name(const std::filesystem::path &path)
  {
    if (path.string().empty()) {
      return {};
    }
    auto name = path.filename().stem().string();
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);
    return name;
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
        return open_viii::archive::FS::get_entry<outT>(m_fs.path(), fi, m_fs.offset());
      }
      return open_viii::archive::FS::get_entry<outT>(m_fs.data(), fi, m_fs.offset());
    }();
  }
  [[nodiscard]] std::string get_full_path(const std::string_view &filename) const
  {
    return get_entry_id_and_path(filename).second;
  }
  [[nodiscard]] auto get_entry_id_and_path(const std::string_view &filename) const
  {
    if (std::ranges::empty(m_fl.data())) {
      return open_viii::archive::FL::get_entry(m_fl.path(), { filename }, m_fl.offset(), m_fl.size(), m_count);
    }
    return open_viii::archive::FL::get_entry_data(
      m_fl.path(), m_fl.data(), { filename }, m_fl.offset(), m_fl.size(), m_count);
  }
  template<typename outT = std::vector<char>> [[nodiscard]] outT get_entry_data(const std::string_view &filename) const
  {
    const auto &[id, path] = get_entry_id_and_path(filename);
    return get_entry_data<outT>(get_entry_by_index(id));
  }
  [[nodiscard]] std::vector<std::pair<unsigned int, std::string>> get_vector_of_indexs_and_files(
    const std::initializer_list<std::string_view> &filename) const
  {
    return archive::FL::get_all_entries_data(m_fl.path(), m_fl.data(), m_fl.offset(), m_fl.size(), m_count, filename);
  }

  template<typename lambdaT>
  requires(std::invocable<lambdaT, std::vector<char>, std::string>) void execute_on(
    const std::initializer_list<std::string_view> &filename,
    const lambdaT &lambda) const
  {

    std::vector<std::jthread> threads{};
    const auto results = get_vector_of_indexs_and_files(filename);
    std::ranges::for_each(
      results | std::views::filter([this](const std::pair<unsigned int, std::string> &pair) -> bool {
        return check_extension(pair.second)
               == 0;// prevent from running on nested archives. We have another function for those.
      }),
      [this, &lambda, &threads](const std::pair<unsigned int, std::string> &pair) {
        auto fi = get_entry_by_index(pair.first);
        threads.emplace_back(
          [&lambda](std::vector<char> &&buffer, std::string &&path) {
            lambda(std::move(buffer), std::move(path));
          },
          get_entry_buffer(fi),
          std::string(pair.second));
        if (threads.size() > 16) {
          threads.front().join();
          threads.erase(threads.begin());
        }
        // lambda(get_entry_buffer(fi), pair.second);
      });
  }

  [[nodiscard]] std::vector<std::pair<std::string, std::vector<std::pair<unsigned int, std::string>>>>
    get_all_nested_entries_data([[maybe_unused]] const std::initializer_list<std::string_view> &filename)
  {
    if constexpr (!HasNested) {
      return {};
    }
    std::vector<std::pair<std::string, std::vector<std::pair<unsigned int, std::string>>>> vector{};
    const std::vector<std::pair<unsigned int, std::string>> fls = get_vector_of_indexs_and_files({ archive::FL::EXT });
    const std::string &basename = get_base_name();
    for (const auto &fl : fls) {
      const auto fi = get_entry_by_index(fl.first);
      const auto buffer = get_entry_buffer<std::string>(fi);
      auto results = archive::FL::get_all_entries_data({}, buffer, 0, 0, 0, filename);
      if (!std::ranges::empty(results)) {
        vector.emplace_back(std::make_pair(basename + "::" + get_base_name(fl.second), std::move(results)));
      }
    }
    return vector;
  }
  template<typename lambdaT>
  requires((std::invocable<lambdaT, FIFLFS<false>> || std::invocable<lambdaT, std::vector<char>, std::string>)&&HasNested) void execute_with_nested(
    const std::initializer_list<std::string_view> &filename,
    const lambdaT lambda,
    const std::initializer_list<std::string_view> &nested_filename = {}) const
  {
    FIFLFS<false> archive{};

    const auto items =
      archive::FL::get_all_entries_data(m_fl.path(), m_fl.data(), m_fl.offset(), m_fl.size(), m_count, filename);

    std::vector<std::jthread> threads{};
    threads.reserve(std::ranges::size(items) / 3);
    std::for_each(std::execution::seq,
      items.cbegin(),
      items.cend(),
      [this, &threads, &archive, &lambda, &nested_filename](const auto &item) {
        const auto &[id, strVirtualPath] = item;
        const FI_Like auto fi = get_entry_by_index(id);

        TryAddT retVal = [this, &archive, &fi, &strVirtualPath, &lambda]() {
          std::filesystem::path virtualPath(strVirtualPath);
          if (!std::ranges::empty(m_fs.data())) {
            return archive.try_add_nested(m_fs.data(), m_fs.offset(), virtualPath, fi);
          }

          if (fi.compression_type() == CompressionTypeT::none) {
            auto localRetVal =
              archive.try_add(m_fs.path(), virtualPath, m_fs.offset() + fi.offset(), fi.uncompressed_size());
            //            if (localRetVal != TryAddT::not_part_of_archive) {
            //              std::cout << virtualPath.filename() << " is uncompressed pointing at location in actual
            //              file!\n";
            //            }
            return localRetVal;
          }
          return archive.try_add_nested(
            m_fs.path(), m_fs.offset(), virtualPath, fi);// when path is sent a different function is used later.
        }();
        if (retVal == TryAddT::archive_full) {
          if constexpr (std::invocable<lambdaT, std::vector<char>, std::string>) {
            threads.emplace_back(
              [&lambda](const FIFLFS<false> archive_copy, const std::initializer_list<std::string_view> filename_copy) {
                archive_copy.execute_on(filename_copy, lambda);
              },
              archive,
              nested_filename);
          } else if constexpr (std::invocable<lambdaT, FIFLFS<false>>) {
            threads.emplace_back(
              [&lambda](const FIFLFS<false> archive_copy) {
                lambda(archive_copy);
              },
              archive);
          }
          if (threads.size() > 16) {
            threads.front().join();
            threads.erase(threads.begin());
          }
          archive = {};
        }
      });
  }

  [[nodiscard]] TryAddT
    get_fiflfs(auto &archive, const std::uint32_t id, const std::string_view &str_virtual_path) const
  {
    FI_Like auto fi = get_entry_by_index(id);

    return [this, &archive, &fi, &str_virtual_path]() {
      std::filesystem::path virtualPath(str_virtual_path);
      if (!std::ranges::empty(m_fs.data())) {
        return archive.try_add_nested(m_fs.data(), m_fs.offset(), virtualPath, fi);
      }

      if (fi.compression_type() == CompressionTypeT::none) {
        auto localRetVal =
          archive.try_add(m_fs.path(), virtualPath, m_fs.offset() + fi.offset(), fi.uncompressed_size());
        //        if (localRetVal != TryAddT::not_part_of_archive) {
        //          std::cout << virtualPath.filename() << " is uncompressed pointing at location in actual file!\n";
        //        }
        return localRetVal;
      }
      return archive.try_add_nested(
        m_fs.path(), m_fs.offset(), virtualPath, fi);// when path is sent a different function is used later.
    }();
  }
  [[nodiscard]] FIFLFS<false> get_fiflfs(const std::initializer_list<std::string_view> &filename) const
  {
    FIFLFS<false> archive{};
    const auto items =
      archive::FL::get_all_entries_data(m_fl.path(), m_fl.data(), m_fl.offset(), m_fl.size(), m_count, filename);
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
    FIFLFS<false> archive{};

    const auto items =
      archive::FL::get_all_entries_data(m_fl.path(), m_fl.data(), m_fl.offset(), m_fl.size(), m_count, filename);
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
