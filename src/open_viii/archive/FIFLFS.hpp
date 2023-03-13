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
#include <sstream>
#include <string>
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
concept executable_common_nested = executable_common_sans_nested<lambdaT>
                                || executable_fiflfs_sans_nested<lambdaT>;

template<typename lambdaT>
concept filter_paths = std::is_invocable_r_v<bool, lambdaT, std::string>;

template<typename srcT>
concept path_or_range = std::convertible_to<srcT, std::filesystem::path>
                     || std::ranges::contiguous_range<srcT>;

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
  Grouping<std::vector<char>>       m_fi{};
  Grouping<std::vector<char>>       m_fs{};
  // this is char because the file contains strings.
  Grouping<std::basic_string<char>> m_fl{};
  size_t                            m_count{};
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
  // Default constructor
  FIFLFS()                        = default;
  // Copy constructor
  FIFLFS(const FIFLFS &other)     = default;

  // Move constructor
  FIFLFS(FIFLFS &&other) noexcept = default;

  // Copy assignment operator
  FIFLFS &
    operator=(const FIFLFS &other)
    = default;

  // Move assignment operator
  FIFLFS &
    operator=(FIFLFS &&other) noexcept
    = default;

  // Destructor
  ~FIFLFS() noexcept = default;

  FIFLFS(std::filesystem::path src)
  {
    const auto parent_path = src.parent_path();
    // std::cout << "Parent Path: " << parent_path << std::endl;
    const auto stem        = src.stem().string();
    // std::cout << "Archive Name: " << stem << std::endl;
    open_viii::tools::execute_on_directory(
      parent_path,
      { stem },
      { FI::EXT, FS::EXT, fl::EXT },
      [this](const std::filesystem::path &path) {
        std::string     cur_path = path.string();
        std::error_code ec{};
        const auto      count
          = static_cast<std::uint32_t>(std::filesystem::file_size(path, ec));
        if (ec) {
          std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                    << ec.value() << ": " << ec.message() << ec.value()
                    << " - path: " << path << std::endl;
          ec.clear();
        }
        const TryAddT result = try_add(FI(count, 0U), path, path);
        switch (result) {
        case TryAddT::added_to_archive:
          // std::cout << "added: " << path << '\n';
          break;
        case TryAddT::archive_full:
          // std::cout << "added: " << path << '\n';
          // std::cout << "Archive Loaded\n";
          break;
        case TryAddT::not_part_of_archive:
          // std::cout << "not added: " << path << '\n';
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
            [](const std::pair<unsigned int, std::string> &pair) -> bool {
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
    map_data_classic() const
  {
    if constexpr (!HasNested) {
      return {};
    }
    else {
      std::vector<std::string> list{};
      FIFLFS<false>            archive = {};
      const auto               items   = get_all_items_from_fl({ "mapdata" });
      std::ignore
        = std::ranges::any_of(items, [&archive, this, &list](const auto &item) {
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
  [[nodiscard]] std::vector<std::string>
    map_data() const
  {
    if constexpr (!HasNested) {
      return {};
    }
    else {
      std::vector<std::string> list{};
      const auto               items = get_all_items_from_fl({ ".fs" });
      (void)std::transform(
        items.cbegin(),
        items.cend(),
        std::back_inserter(list),
        [](const auto &item) {
          auto fspath = std::filesystem::path(item.second);
          return fspath.filename().stem().string();
        });
      std::sort(list.begin(), list.end());
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

  template<filter_paths filterT = decltype(default_filter_lambda)>
  auto
    get_archive_with_nested(
      const std::initializer_list<std::string_view> &filename,
      filterT                                      &&filter_lambda = {}) const
  {
    FIFLFS<false> archive = {};
    const auto    items   = get_all_items_from_fl(filename);
    std::ignore           = std::ranges::any_of(items, [&](const auto &item) {
      return filter_lambda(item.second) && fill_archive_lambda(archive)(item);
    });
    return archive;
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
      if constexpr (executable_common_sans_nested<lambdaT>) {
        const auto pFunction
          = [&lambda, &nested_filename, &archive, this, &filter_lambda](
              const auto &item) -> bool {
          if (
            filter_lambda(item.second) && fill_archive_lambda(archive)(item)) {
            archive.execute_on(nested_filename, lambda, filter_lambda);
            archive = {};
            return true;
          }
          return false;
        };
        if (!limit_once)
          std::ranges::for_each(items, pFunction);
        else {
          std::ignore = std::ranges::any_of(items, pFunction);
        }
      }
      else if constexpr (executable_fiflfs_sans_nested<lambdaT>) {
        const auto pFunction =
          [&lambda, &archive, this, &filter_lambda](const auto &item) -> bool {
          if (
            filter_lambda(item.second) && fill_archive_lambda(archive)(item)) {
            lambda(std::move(archive));
            archive = {};
            return true;
          }
          return false;
        };
        if (!limit_once)
          std::ranges::for_each(items, pFunction);
        else {
          std::ignore = std::ranges::any_of(items, pFunction);
        }
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
  struct FIFLFS_sentinel
  {
  };
  template<bool is_true>
    requires(is_true)
  class FIFLFS_true_Iterator
  {
  public:
    using value_type        = open_viii::archive::FIFLFS<false>;
    using difference_type   = std::ptrdiff_t;
    using pointer           = open_viii::archive::FIFLFS<false> *;
    using reference         = open_viii::archive::FIFLFS<false> &;
    using iterator_category = std::random_access_iterator_tag;
    using const_iterator    = FIFLFS_true_Iterator<is_true>;

    FIFLFS_true_Iterator(
      const open_viii::archive::FIFLFS<true> &archive,
      std::size_t                             index = 0)
      : m_fiflfs_true(archive), m_map_names(m_fiflfs_true.get().map_data()),
        m_current_index(index)
    {}
    bool
      operator==(const FIFLFS_true_Iterator &other) const
    {
      return &m_fiflfs_true.get() == &other.m_fiflfs_true.get()
          && m_current_index == other.m_current_index
          && m_map_names == other.m_map_names;
    }
    bool
      operator==(const FIFLFS_sentinel) const
    {
      return m_current_index == std::ranges::size(m_map_names);
    }

    FIFLFS_true_Iterator &
      operator+=(difference_type n)
    {
      m_current_index += n;
      return *this;
    }

    friend FIFLFS_true_Iterator
      operator+(FIFLFS_true_Iterator it, difference_type n)
    {
      return it += n;
    }

    friend FIFLFS_true_Iterator
      operator+(difference_type n, FIFLFS_true_Iterator it)
    {
      return it += n;
    }

    FIFLFS_true_Iterator &
      operator-=(difference_type n)
    {
      m_current_index -= n;
      return *this;
    }

    friend FIFLFS_true_Iterator
      operator-(FIFLFS_true_Iterator it, difference_type n)
    {
      return it -= n;
    }

    friend difference_type
      operator-(
        FIFLFS_true_Iterator const &lhs,
        FIFLFS_true_Iterator const &rhs)
    {
      return lhs.m_current_index - rhs.m_current_index;
    }

    value_type
      operator[](difference_type n) const
    {
      return *(*this + n);
    }

    FIFLFS_true_Iterator &
      operator++()
    {
      ++m_current_index;
      return *this;
    }

    FIFLFS_true_Iterator
      operator++(int)
    {
      FIFLFS_true_Iterator old = *this;
      ++(*this);
      return old;
    }
    FIFLFS_true_Iterator &
      operator--()
    {
      --m_current_index;
      return *this;
    }

    FIFLFS_true_Iterator
      operator--(int)
    {
      FIFLFS_true_Iterator old = *this;
      --(*this);
      return old;
    }

    value_type
      operator*() const
    {
      return m_fiflfs_true.get().get_archive_with_nested(
        { m_map_names[m_current_index] },
        [](auto &&) {
          return true;
        });
    }

  private:
    std::reference_wrapper<const FIFLFS<true>> m_fiflfs_true;
    std::vector<std::string>                   m_map_names;
    std::size_t                                m_current_index;
  };

  template<bool is_false>
    requires(!is_false)
  class FIFLFS_false_Iterator
  {
  public:
    using value_type        = std::pair<std::string, std::vector<char>>;
    using difference_type   = std::ptrdiff_t;
    using pointer           = std::pair<std::string, std::vector<char>> *;
    using reference         = std::pair<std::string, std::vector<char>> &;
    using iterator_category = std::random_access_iterator_tag;
    using const_iterator    = FIFLFS_false_Iterator<is_false>;
    FIFLFS_false_Iterator(
      const open_viii::archive::FIFLFS<false> &archive,
      std::size_t                              index = 0)
      : m_fiflfs_false(archive),
        m_pair_id_names(m_fiflfs_false.get().get_all_items_from_fl({})),
        m_current_index(index)
    {}
    bool
      operator==(const FIFLFS_false_Iterator &other) const
    {
      return &m_fiflfs_false.get() == &other.m_fiflfs_false.get()
          && m_current_index == other.m_current_index
          && m_pair_id_names == other.m_pair_id_names;
    }
    bool
      operator==(const FIFLFS_sentinel) const
    {
      return m_current_index == std::ranges::size(m_pair_id_names);
    }

    FIFLFS_false_Iterator &
      operator+=(difference_type n)
    {
      m_current_index += n;
      return *this;
    }

    friend FIFLFS_false_Iterator
      operator+(FIFLFS_false_Iterator it, difference_type n)
    {
      return it += n;
    }

    friend FIFLFS_false_Iterator
      operator+(difference_type n, FIFLFS_false_Iterator it)
    {
      return it += n;
    }

    FIFLFS_false_Iterator &
      operator-=(difference_type n)
    {
      m_current_index -= n;
      return *this;
    }

    friend FIFLFS_false_Iterator
      operator-(FIFLFS_false_Iterator it, difference_type n)
    {
      return it -= n;
    }

    friend difference_type
      operator-(
        FIFLFS_false_Iterator const &lhs,
        FIFLFS_false_Iterator const &rhs)
    {
      return lhs.m_current_index - rhs.m_current_index;
    }

    value_type
      operator[](difference_type n) const
    {
      return *(*this + n);
    }

    FIFLFS_false_Iterator &
      operator++()
    {
      ++m_current_index;
      return *this;
    }

    FIFLFS_false_Iterator
      operator++(int)
    {
      FIFLFS_false_Iterator old = *this;
      ++(*this);
      return old;
    }
    FIFLFS_false_Iterator &
      operator--()
    {
      --m_current_index;
      return *this;
    }

    FIFLFS_false_Iterator
      operator--(int)
    {
      FIFLFS_false_Iterator old = *this;
      --(*this);
      return old;
    }

    value_type
      operator*() const
    {
      const auto &[file_id, file_name] = m_pair_id_names[m_current_index];
      const FI file_info = m_fiflfs_false.get().get_entry_by_index(file_id);
      return { file_name, m_fiflfs_false.get().get_entry_buffer(file_info) };
    }
    //
    //    operator const_iterator() const
    //    {
    //      return *this;
    //    }

  private:
    std::reference_wrapper<const FIFLFS<false>>        m_fiflfs_false;
    std::vector<std::pair<std::uint32_t, std::string>> m_pair_id_names;
    std::size_t                                        m_current_index;
  };

  auto
    begin() const
    requires(HasNested)
  {
    return FIFLFS_true_Iterator<HasNested>{ *this };
  }
  auto
    begin() const
    requires(!HasNested)
  {
    return FIFLFS_false_Iterator<HasNested>{ *this };
  }
  FIFLFS_sentinel
    end() const
  {
    return {};
  }

  auto
    cbegin() const
    requires(HasNested)
  {
    return FIFLFS_true_Iterator<HasNested>{ *this };
  }
  auto
    cbegin() const
    requires(!HasNested)
  {
    return FIFLFS_false_Iterator<HasNested>{ *this };
  }
  FIFLFS_sentinel
    cend() const
  {
    return {};
  }
};
static_assert(std::movable<FIFLFS<false>>);
static_assert(std::movable<FIFLFS<true>>);
}// namespace open_viii::archive
#endif// !VIIIARCHIVE_FIFLFS_HPP
