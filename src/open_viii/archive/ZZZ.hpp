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
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
namespace open_viii::archive {
template<typename T>
concept execute_binary_function
  = std::invocable<T, std::vector<char>, std::string>;
template<typename T>
concept execute_unary_function = std::invocable<T, FileData>;
template<typename T>
concept execute_zzz = execute_binary_function<T> || execute_unary_function<T>;

std::vector<char>
  operator+(
    const open_viii::archive::FileData &dataItem,
    const std::filesystem::path        &path)
{
  return FS::get_entry(path, dataItem);
}
std::vector<char>
  operator+(
    const std::filesystem::path        &path,
    const open_viii::archive::FileData &dataItem)
{
  return FS::get_entry(path, dataItem);
}
class ZZZ_Paths
{
public:
  class ZZZFileFetcher
  {
  public:
    using value_type = std::vector<char>;

    ZZZFileFetcher(FileData file_data, const std::filesystem::path &path)
      : m_file_data(std::move(file_data)), m_path(path)
    {}

    ZZZFileFetcher(const ZZZFileFetcher &other)
      : ZZZFileFetcher(other.m_file_data, other.m_path)
    {}
    ZZZFileFetcher() : ZZZFileFetcher({}, tmp_path) {}

    ZZZFileFetcher &
      operator=(const ZZZFileFetcher &other)
    {
      if (this != &other) {
        m_file_data = other.m_file_data;
        m_path      = other.m_path;
      }
      return *this;
    }
    operator value_type()
    {
      return get();
    };
    operator const FileData &() const
    {
      return m_file_data;
    }
    FileData
      get_file_info() const
    {
      return m_file_data;
    }
    const std::filesystem::path &
      path() const
    {
      return m_path.get();
    }
    value_type
      get() const
    {
      return get_file_info() + m_path.get();
    }

  private:
    static inline const std::filesystem::path           tmp_path{};
    FileData                                            m_file_data{};
    std::reference_wrapper<const std::filesystem::path> m_path;
  };
  struct sentinel
  {
  };

  class iterator
  {
  public:
    using value_type        = ZZZFileFetcher;
    using difference_type   = std::ptrdiff_t;
    // using pointer           = const value_type *;
    // using reference         = const value_type &;
    using iterator_category = std::random_access_iterator_tag;

    iterator() : iterator(tmp_path, tmp_offsets, tmp_sizes, {}) {}
    iterator(
      const std::filesystem::path               &path,
      const std::vector<std::istream::off_type> &offsets,
      const std::vector<std::uint32_t>          &sizes,
      std::size_t                                index = 0)
      : m_path(path), m_offsets(offsets), m_sizes(sizes), m_index(index)
    {
      assert(m_offsets.get().size() + 1U == m_sizes.get().size());
      [[maybe_unused]] const auto checkone
        = [](const std::filesystem::path &in) {
            std::error_code ec;
            const bool      found = std::filesystem::exists(in, ec);
            if (ec) {
              std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                        << ec.value() << ": " << ec.message() << ec.value()
                        << " - path: " << in << std::endl;
              ec.clear();
            }
            return found;
          };
      [[maybe_unused]] const auto checktwo
        = [](const std::filesystem::path &in) {
            std::error_code ec;
            const bool      found = std::filesystem::is_regular_file(in, ec);
            if (ec) {
              std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                        << ec.value() << ": " << ec.message() << ec.value()
                        << " - path: " << in << std::endl;
              ec.clear();
            }
            return found;
          };
      assert(checkone(m_path));
      assert(checktwo(m_path));
    }

    value_type
      operator*() const
    {
      if (m_index < m_sizes.get().size()) {
        return seek();
      }
      return {};
    }

    value_type
      operator[](difference_type n) const
    {
      return *(*this + n);
    }

    iterator &
      operator++()
    {
      ++m_index;
      return *this;
    }

    iterator
      operator++(int)
    {
      iterator tmp(*this);
               operator++();
      return tmp;
    }

    iterator &
      operator--()
    {
      --m_index;
      return *this;
    }

    iterator
      operator--(int)
    {
      iterator tmp(*this);
               operator--();
      return tmp;
    }

    iterator &
      operator+=(difference_type n)
    {
      m_index
        = static_cast<std::size_t>(static_cast<difference_type>(m_index) + n);
      return *this;
    }

    friend iterator
      operator+(iterator it, difference_type n)
    {
      return it += n;
    }

    friend iterator
      operator+(difference_type n, iterator it)
    {
      return it += n;
    }

    iterator &
      operator-=(difference_type n)
    {
      m_index
        -= static_cast<std::size_t>(static_cast<difference_type>(m_index) - n);
      return *this;
    }

    friend iterator
      operator-(iterator it, difference_type n)
    {
      return it -= n;
    }

    friend difference_type
      operator-(iterator const &lhs, iterator const &rhs)
    {
      return static_cast<difference_type>(lhs.m_index)
           - static_cast<difference_type>(rhs.m_index);
    }

    friend difference_type
      operator-(sentinel const &, iterator const &rhs)
    {
      return static_cast<difference_type>(rhs.m_sizes.get().size())
           - static_cast<difference_type>(rhs.m_index);
    }
    friend difference_type
      operator-(iterator const &lhs, sentinel const &)
    {
      return static_cast<difference_type>(lhs.m_index)
           - static_cast<difference_type>(lhs.m_sizes.get().size());
    }

    bool
      operator==(const iterator &other) const
    {
      return m_index == other.m_index && m_path == other.m_path
          && &m_offsets.get() == &other.m_offsets.get()
          && &m_sizes.get() == &other.m_sizes.get();
    }
    bool
      operator==(sentinel) const
    {
      return m_index == m_sizes.get().size();
    }

    bool
      operator!=(const iterator &other) const
    {
      return !(*this == other);
    }
    bool
      operator!=(sentinel other) const
    {
      return !(*this == other);
    }

    bool
      operator<(const iterator &other) const
    {
      return m_path == other.m_path
          && &m_offsets.get() == &other.m_offsets.get()
          && &m_sizes.get() == &other.m_sizes.get() && m_index < other.m_index;
    }

    bool
      operator<=(const iterator &other) const
    {
      return *this == other || *this < other;
    }

    bool
      operator>(const iterator &other) const
    {
      return !(*this <= other);
    }

    bool
      operator>=(const iterator &other) const
    {
      return !(*this < other);
    }
    bool
      operator<(const sentinel &) const
    {
      return m_index < m_sizes.get().size();
    }

    bool
      operator<=(const sentinel &other) const
    {
      return *this == other || *this < other;
    }

    bool
      operator>(const sentinel &other) const
    {
      return !(*this <= other);
    }

    bool
      operator>=(const sentinel &other) const
    {
      return !(*this < other);
    }

  private:
    [[nodiscard]] value_type
      seek() const
    {
      std::ifstream file(m_path.get(), std::ios::binary);
      if (!file) {
        std::cerr << ("Failed to open file: " + m_path.get().string())
                  << std::endl;
        return {};
      }
      file.seekg(m_offsets.get().operator[](m_index), std::ios::beg);
      std::string current;
      current.resize(m_sizes.get().operator[](m_index));
      file.read(
        current.data(),
        static_cast<std::streamsize>(m_sizes.get().operator[](m_index)));
      tl::string::replace_slashes(current);

      std::array<char, sizeof(std::uint64_t)> offset_tmp{};
      file.read(
        std::ranges::data(offset_tmp),
        static_cast<std::istream::off_type>(sizeof(std::uint64_t)));

      std::array<char, sizeof(std::uint32_t)> size_tmp{};
      file.read(
        std::ranges::data(size_tmp),
        static_cast<std::istream::off_type>(sizeof(std::uint32_t)));

      return { { std::move(current),
                 std::bit_cast<std::uint64_t>(offset_tmp),
                 std::bit_cast<std::uint32_t>(size_tmp) },
               m_path };
    }

    static const inline std::filesystem::path               tmp_path{};
    static const inline std::vector<std::istream::off_type> tmp_offsets{};
    static const inline std::vector<std::uint32_t>          tmp_sizes{};
    std::reference_wrapper<const std::filesystem::path>     m_path;
    std::reference_wrapper<const std::vector<std::istream::off_type>> m_offsets;
    std::reference_wrapper<const std::vector<std::uint32_t>>          m_sizes;
    std::size_t                                                       m_index;
  };

  static_assert(std::movable<iterator>);
  static_assert(std::copyable<iterator>);
  static_assert(std::input_iterator<iterator>);
  static_assert(std::forward_iterator<iterator>);
  static_assert(std::sentinel_for<iterator, iterator>);
  static_assert(std::sentinel_for<sentinel, iterator>);
  static_assert(std::sized_sentinel_for<iterator, iterator>);
  static_assert(std::sized_sentinel_for<sentinel, iterator>);
  static_assert(std::bidirectional_iterator<iterator>);
  static_assert(std::random_access_iterator<iterator>);
  ZZZ_Paths() = default;
  ZZZ_Paths(
    std::filesystem::path               path,
    std::vector<std::istream::off_type> offsets,
    std::vector<std::uint32_t>          sizes)
    : m_path(std::move(path)), m_offsets(offsets), m_sizes(sizes)
  {}

  iterator
    begin() const
  {
    return iterator(m_path, m_offsets, m_sizes);
  }

  sentinel
    end() const
  {
    return sentinel{};
  }

  iterator
    cbegin() const
  {
    return iterator(m_path, m_offsets, m_sizes);
  }

  sentinel
    cend() const
  {
    return sentinel{};
  }

  const std::filesystem::path &
    path() const
  {
    return m_path;
  }
  std::istream::off_type
    last_offset() const
  {
    return m_offsets.back();
  }
  bool
    empty() const
  {
    return std::ranges::empty(m_sizes);
  }
  std::size_t
    size() const
  {
    return std::ranges::size(m_sizes);
  }

private:
  std::filesystem::path               m_path{};
  std::vector<std::istream::off_type> m_offsets{};
  std::vector<std::uint32_t>          m_sizes{};
};

/**
 * ZZZ file archive from FF8 remaster.
 * @see https://github.com/myst6re/qt-zzz
 */
struct [[maybe_unused]] ZZZ
{
private:
  // uint32_t count_{}; //fallowed by array of file data
  ZZZ_Paths m_paths{};
  ZZZ_Paths
    load_data_from_file(std::filesystem::path m_path)
  {
    std::vector<std::istream::off_type> m_offsets      = {};
    std::vector<std::uint32_t>          m_string_sizes = {};
    m_offsets.push_back(
      static_cast<std::istream::off_type>(sizeof(uint32_t) * 2));
    std::error_code ec{};
    const bool      found = m_path.has_extension()
                    && tools::i_equals(m_path.extension().string(), EXT)
                    && std::filesystem::exists(m_path, ec);
    if (ec) {
      std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                << ec.value() << ": " << ec.message() << ec.value()
                << " - path: " << m_path << std::endl;
      ec.clear();
    }
    if (found) {
      tools::read_from_file(
        [&](std::istream &fp) {
          auto count{ tools::read_val<uint32_t>(fp) };
          if (count == 0) {
            return;
          }
          m_offsets.reserve(count + 1);
          m_string_sizes.reserve(count);

          for (; count != 0; --count) {
            // get string length
            std::array<char, sizeof(std::uint32_t)> tmp{};
            fp.read(
              std::ranges::data(tmp),
              static_cast<std::istream::off_type>(sizeof(uint32_t)));
            auto string_size = std::bit_cast<std::uint32_t>(tmp);
            m_string_sizes.push_back(string_size);

            // seek to next entry
            fp.seekg(
              static_cast<std::istream::off_type>(
                sizeof(std::uint64_t) + sizeof(std::uint32_t) + string_size),
              std::ios::cur);
            m_offsets.push_back(
              fp.tellg()
              + static_cast<std::istream::off_type>(sizeof(std::uint32_t)));
          }
          m_offsets.back()
            -= static_cast<std::istream::off_type>(sizeof(std::uint32_t));
        },
        m_path);
    }
    return { std::move(m_path),
             std::move(m_offsets),
             std::move(m_string_sizes) };
  }

public:
  constexpr static auto EXT = std::string_view(".zzz");
  [[maybe_unused]] [[nodiscard]] const auto &
    path() const noexcept
  {
    return m_paths.path();
  }
  ZZZ() = default;
  explicit ZZZ(std::filesystem::path path)
    : m_paths(load_data_from_file(std::move(path)))
  {}
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

  using default_filter_lambda = decltype([](auto &&) {
    return true;
  });
  template<execute_zzz FunctionT, typename FilterT = default_filter_lambda>
  void
    execute_on(
      const std::initializer_list<std::string_view> &filename,
      FunctionT                                    &&function,
      FilterT                                      &&filter_lambda = {}) const
  {
    if constexpr (execute_unary_function<FunctionT>) {
      std::ranges::for_each(
        m_paths,
        [&function, &filename, &filter_lambda](
          const open_viii::archive::FileData &dataItem) {
          auto pathString = dataItem.get_path_string();
          if (open_viii::tools::i_find_any(pathString, filename)) {
            if (filter_lambda(pathString)) {
              function(dataItem);
            }
          }
        });
    }
    else if constexpr (execute_binary_function<FunctionT>) {
      std::ranges::for_each(
        m_paths,
        [&function, &filename, &filter_lambda, this](
          const open_viii::archive::FileData &dataItem) {
          auto pathString = dataItem.get_path_string();
          if (open_viii::tools::i_find_any(pathString, filename)) {
            if (filter_lambda(pathString)) {
              function(m_paths.path() + dataItem, std::string(pathString));
            }
          }
        });
    }
  }
  using default_lambda = decltype([](auto &&, auto &&) {});
  template<
    typename lambdaT = default_lambda,
    typename FilterT = default_filter_lambda>
    requires(
      (std::invocable<lambdaT, FIFLFS<false>>
       || std::invocable<lambdaT, std::vector<char>, std::string>))
  void
    execute_with_nested(
      [[maybe_unused]] const std::initializer_list<std::string_view> & = {},
      [[maybe_unused]] lambdaT                                      && = {},
      [[maybe_unused]] const std::initializer_list<std::string_view> & = {},
      FilterT && = {}) const
  {
    // only nested archives are handled in the other functions. maybe delete
    // this.
  }
  explicit operator bool() const
  {
    return !std::ranges::empty(m_paths);
  }
  bool
    empty() const
  {
    return m_paths.empty();
  }

  std::size_t
    size() const
  {
    return m_paths.size();
  }

  ZZZ_Paths::iterator
    begin() const
  {
    return m_paths.begin();
  }
  ZZZ_Paths::iterator::value_type
    operator[](std::size_t i) const
  {
    return (
      begin().operator[](static_cast<ZZZ_Paths::iterator::difference_type>(i)));
  }
  ZZZ_Paths::iterator::value_type
    operator[](std::string_view needle) const
  {
    for (auto item : m_paths) {
      if (open_viii::tools::i_find(
            item.get_file_info().get_path_string_view(),
            needle)) {
        return item;
      }
    }
    return {};
  }
  ZZZ_Paths::sentinel
    end() const
  {
    return m_paths.end();
  }
};
inline std::ostream &
  operator<<(std::ostream &os, const ZZZ &data)
{
  assert(data.path().has_stem());
  return os << '{' << data.path().stem().string() << " zzz {" << data.size()
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
static_assert(std::ranges::sized_range<ZZZ>);
static_assert(std::ranges::random_access_range<ZZZ>);
}// namespace open_viii::archive
#endif// VIIIARCHIVE_ZZZ_HPP