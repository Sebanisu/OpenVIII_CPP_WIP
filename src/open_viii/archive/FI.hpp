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
#ifndef VIIIARCHIVE_FI_HPP
#define VIIIARCHIVE_FI_HPP
#include "open_viii/CompressionTypeT.hpp"
#include "open_viii/Concepts.hpp"
#include "open_viii/tools/Tools.hpp"
#include "tl/read.hpp"
#include "tl/write.hpp"
namespace open_viii::archive {
/**
 * FI is the file index for the FL and FS files.
 */
struct FI
{
private:
  /**
   * Uncompressed Size
   */
  std::uint32_t    m_uncompressed_size{};
  /**
   * Offset
   */
  std::uint32_t    m_offset{};
  /**
   * Compression Type
   */
  CompressionTypeT m_compression_type{};

  static FI
    read_from_file(
      const std::filesystem::path &path,
      std::ifstream::pos_type      offset)
  {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
      std::cerr << ("Failed to open file: " + path.string()) << std::endl;
      return {};
    }
    file.seekg(offset, std::ios::beg);

    std::array<char, sizeof(FI)> offset_tmp{};
    file.read(
      std::ranges::data(offset_tmp),
      static_cast<std::istream::off_type>(sizeof(FI)));
    return std::bit_cast<FI>(offset_tmp);
  }
  static FI
    read_from_memory(std::span<const char> data, std::size_t offset)
  {
    data = data.subspan(offset, FI::SIZE);
    return *std::bit_cast<FI *>(data.data());
  }

public:
  /**
   * Expected size of the FI struct, used for a static assert after.
   */
  static constexpr std::size_t SIZE = 12U;
  /**
   * File extension for FI
   */
  static constexpr auto        EXT  = std::string_view(".fi");
  /**
   * Uncompressed size in bytes.
   */
  [[nodiscard]] constexpr auto
    uncompressed_size() const noexcept -> std::uint32_t
  {
    return m_uncompressed_size;
  }
  /**
   * Offset is distance from beginning in bytes.
   */
  [[nodiscard]] constexpr auto
    offset() const noexcept -> std::uint32_t
  {
    return m_offset;
  }
  /**
   * Compression type (uncompressed, lzss, or lz4).
   */
  [[nodiscard]] constexpr auto
    compression_type() const noexcept -> CompressionTypeT
  {
    return m_compression_type;
  }
  /**
   * Enable default constructor.
   */
  constexpr FI() noexcept = default;
  /**
   * Constructor takes a FI like object and copies the data out of it.
   * @tparam fiT Fi Like object type
   * @param fi Fi Like object
   * @note There may be data loss. This object stores things as std::uint32_t.
   * FileData object uses std::uint64_t. Also FileData has a string for file
   * path. This doesn't store the path.
   */
  template<FI_Like fiT>
    requires(!std::is_same_v<std::decay_t<fiT>, FI>)
  constexpr explicit FI(const fiT &fi) noexcept
    : m_uncompressed_size{ static_cast<decltype(m_uncompressed_size)>(
      fi.uncompressed_size()) },
      m_offset{ static_cast<decltype(m_offset)>(fi.offset()) },
      m_compression_type{ static_cast<decltype(m_compression_type)>(
        fi.compression_type()) }
  {}
  /**
   * Piecemeal constructor takes the 3 values stored in this object.
   * @param uncompressed_size bytes of the entry after being uncompressed.
   * @param offset bytes from start of FS file.
   * @param compression_type compressed or not. And which compression it's
   * using.
   */
  constexpr FI(
    const std::uint32_t    &uncompressed_size,
    const std::uint32_t    &offset,
    const CompressionTypeT &compression_type = CompressionTypeT::none) noexcept
    : m_uncompressed_size{ uncompressed_size }, m_offset{ offset },
      m_compression_type{ compression_type }
  {}
  /**
   * Read FI from a span at offset.
   * @param data span containing all the FI entries
   * @param offset location from start where the desired entry is.
   */
  constexpr FI(std::span<const char> data, std::size_t offset)
    : FI(read_from_memory(data, offset))
  {}
  /**
   * Read FI[id] from a span at offset.
   * @param data span containing all the FI entries
   * @param id is the FI entry number starting at 0.
   * @param offset to entry 0.
   */
  constexpr FI(std::span<const char> data, std::size_t id, std::size_t offset)
    : FI(read_from_memory(data, get_fi_entry_offset(id, offset)))
  {}

  /**
   * Read FI from a path at offset.
   * @param path containing all the FI entries
   * @param offset location from start where the desired entry is.
   */
  template<typename T>
    requires std::same_as<std::filesystem::path, std::remove_cvref_t<T>>
  FI(const T &path, std::size_t offset)
    : FI(read_from_file(path, static_cast<std::ifstream::off_type>(offset)))
  {}
  /**
   * Read FI[id] from a path at offset.
   * @param path containing all the FI entries
   * @param id is the FI entry number starting at 0.
   * @param offset to entry 0.
   */
  template<typename T>
    requires std::same_as<std::filesystem::path, std::remove_cvref_t<T>>
  FI(const T &path, std::size_t id, std::size_t offset)
    : FI(read_from_file(
      path,
      static_cast<std::ifstream::off_type>(get_fi_entry_offset(id, offset))))
  {}
  /**
   * Get count of possible entries based on file_size
   * @param file_size total bytes in a file.
   */
  template<std::unsigned_integral T>
  [[nodiscard]] static constexpr auto
    get_count(const T &file_size) noexcept
  {
    return static_cast<std::size_t>(file_size / SIZE);
  }
  /**
   * Get count of possible entries based on file_size
   * @param path to file containing the FI entries.
   * @return If path doesn't exist returns 0.
   */
  template<typename T>
    requires std::same_as<std::filesystem::path, std::remove_cvref<T>>
  [[maybe_unused]] [[nodiscard]] static std::size_t
    get_count(const T &path)
  {
    std::error_code ec{};
    const bool      found = std::filesystem::exists(path, ec);
    if (ec) {
      std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                << ec.value() << ": " << ec.message() << ec.value()
                << " - path: " << path << std::endl;
      ec.clear();
    }
    if (found) {
      const std::size_t count = get_count(std::filesystem::file_size(path, ec));
      if (ec) {
        std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                  << ec.value() << ": " << ec.message() << ec.value()
                  << " - path: " << path << std::endl;
        ec.clear();
      }
      return count;
    }
    return {};
  }
  /**
   * gets member variables
   * @note required to structured binding support
   */
  template<std::size_t I>
  [[nodiscard]] constexpr auto
    get() const noexcept
  {
    static_assert(I < 3);
    if constexpr (I == 0) {
      return m_uncompressed_size;
    }
    else if constexpr (I == 1) {
      return m_offset;
    }
    else if constexpr (I == 2) {
      return m_compression_type;
    }
  }
  /**
   * find the offset in file where FI entry is located
   * @param id, count from 0 to N of FI entries to skip.
   * @param offset, the number of bytes to start of first FI entry
   * @return id * 12 + offset
   */
  template<std::integral T, std::integral U>
  [[nodiscard]] static constexpr U
    get_fi_entry_offset(T id, U offset = {})
  {
    return static_cast<U>(
      static_cast<U>(id * static_cast<T>(FI::SIZE)) + offset);
  }

  [[nodiscard]] constexpr FI
    with_compression_type(CompressionTypeT in_compression_type)
  {
    return FI(m_uncompressed_size, m_offset, in_compression_type);
  }
  [[nodiscard]] constexpr FI
    with_uncompression_size(std::uint32_t in_uncompressed_size)
  {
    return FI(in_uncompressed_size, m_offset, m_compression_type);
  }

  [[nodiscard]] constexpr FI
    with_offset(std::uint32_t in_offset_size)
  {
    return FI(m_uncompressed_size, in_offset_size, m_compression_type);
  }

  friend std::istream &
    operator>>(std::istream &is, FI &fi)
  {
    std::array<char, 12U> buffer{};
    if (is.read(buffer.data(), 12)) {
      fi = std::bit_cast<FI>(buffer);
    }
    return is;
  }
};
static_assert(sizeof(FI) == FI::SIZE);
static_assert(std::is_trivially_copyable_v<FI>);

/**
 *
 * @tparam T type of output buffer.
 * @param output buffer to write to.
 * @param in_fi incoming FI it's just grabbing the compression type from it.
 */
template<is_insertable_or_ostream T>
inline void
  append_entry(T &output, const FI in_fi)
{
  tl::write::append(output, in_fi);
}
/**
 * Dump values to ostream.
 * @param os out stream value
 * @param data FI entry
 */
inline std::ostream &
  operator<<(std::ostream &os, const FI &data)
{
  os << '{' << data.uncompressed_size() << ", " << data.offset() << ", "
     << static_cast<std::uint32_t>(data.compression_type()) << '}';
  return os;
}
class FIFileRange
{
public:
  class iterator
  {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = FI;
    using difference_type   = std::ifstream::off_type;
    //    using pointer           = const FI *;
    //    using reference         = const FI &;
    static constexpr difference_type value_size
      = static_cast<difference_type>(sizeof(FI));
    iterator() = default;
    explicit iterator(std::ifstream &ifs, difference_type offset = 0)
      : m_ifs(&ifs), m_offset(offset)
    {}

    iterator &
      operator++()
    {
      m_offset += value_size;
      return *this;
    }

    iterator
      operator++(int)
    {
      iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    iterator &
      operator--()
    {
      m_offset -= value_size;
      return *this;
    }

    iterator
      operator--(int)
    {
      iterator tmp(*this);
      --(*this);
      return tmp;
    }
    iterator
      operator+(iterator other) const
    {
      return iterator(*m_ifs, m_offset + other.m_offset);
    }

    iterator
      operator+(difference_type n) const
    {
      return iterator(*m_ifs, m_offset + n * value_size);
    }

    iterator friend
      operator+(difference_type n, iterator other)
    {
      return iterator(*other.m_ifs, other.m_offset + n * value_size);
    }

    iterator
      operator-(difference_type n) const
    {
      return iterator(*m_ifs, m_offset - n * value_size);
    }

    difference_type
      operator-(const iterator &other) const
    {
      return (m_offset - other.m_offset) / value_size;
    }

    iterator &
      operator+=(difference_type n)
    {
      m_offset += n * value_size;
      return *this;
    }

    iterator &
      operator-=(difference_type n)
    {
      m_offset -= n * value_size;
      return *this;
    }

    value_type
      operator*() const
    {
      FI fi = {};
      m_ifs->seekg(m_offset);
      *m_ifs >> fi;
      return fi;
    }
    //    const FI *
    //      operator->() const
    //    {
    //      return &m_fi;
    //    }
    value_type
      operator[](difference_type n) const
    {
      return *(*this + n);
    }

    bool
      operator==(const iterator &other) const
    {
      return m_ifs == other.m_ifs && m_offset == other.m_offset;
    }
    bool
      operator!=(const iterator &other) const
    {
      return !(*this == other);
    }
    bool
      operator<(const iterator &other) const
    {
      return m_offset < other.m_offset;
    }
    bool
      operator<=(const iterator &other) const
    {
      return m_ifs == other.m_ifs && m_offset <= other.m_offset;
    }
    bool
      operator>(const iterator &other) const
    {
      return m_offset > other.m_offset;
    }
    bool
      operator>=(const iterator &other) const
    {
      return m_ifs == other.m_ifs && m_offset >= other.m_offset;
    }

  private:
    std::ifstream *m_ifs{};
    std::streamoff m_offset{};
  };

  // The begin function returns an FIInputIterator that points to the beginning
  // of the file
  iterator
    begin()
  {
    open();
    return iterator(m_file_stream, m_begin);
  }

  // The end function returns an FIInputIterator that points past the end of the
  // file
  iterator
    end()
  {
    open();
    // We need to seek to the end of the file to get the offset

    if (m_end > 0) {
      return iterator(m_file_stream, m_end);
    }
    std::streamoff offset = m_file_stream.seekg(0, std::ios::end).tellg();
    return iterator(m_file_stream, offset);
  }
  iterator::value_type
    operator[](iterator::difference_type n)
  {
    if (!m_file_stream) {
      open();
    }
    FI fi = {};
    m_file_stream.seekg(m_begin + (n * iterator::value_size));
    m_file_stream >> fi;
    return fi;
  }

  FIFileRange(std::filesystem::path in_file_path)
    : m_file_path(std::move(in_file_path))
  {}

  FIFileRange(
    std::filesystem::path     in_file_path,
    iterator::difference_type in_begin,
    iterator::difference_type in_size)
    : m_file_path(std::move(in_file_path)), m_begin(in_begin),
      m_end(in_begin + in_size)
  {}

private:
  void
    open()
  {
    if (!m_file_stream) {
      m_file_stream.open(m_file_path, std::ios::binary);
    }
  }
  std::filesystem::path   m_file_path   = {};
  std::ifstream           m_file_stream = {};
  std::ifstream::off_type m_begin       = {};
  std::ifstream::off_type m_end         = {};
};
static_assert(std::random_access_iterator<FIFileRange::iterator>);
static_assert(std::ranges::random_access_range<FIFileRange>);
class FIMemoryRange
{
public:
  class iterator
  {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = FI;
    using difference_type   = std::ptrdiff_t;
    // using pointer           = const FI *;
    // using reference         = const FI &;
    static constexpr difference_type value_size
      = static_cast<difference_type>(sizeof(FI));
    iterator() = default;
    explicit iterator(
      std::span<const char> in_buffer,
      std::size_t           in_offset = 0)
      : m_buffer_iterator(in_buffer), m_offset(in_offset)
    {}

    iterator &
      operator++()
    {
      m_offset = static_cast<std::size_t>(
        static_cast<difference_type>(m_offset) + value_size);
      return *this;
    }

    iterator
      operator++(int)
    {
      iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    iterator &
      operator--()
    {
      m_offset = static_cast<std::size_t>(
        static_cast<difference_type>(m_offset) - value_size);
      return *this;
    }

    iterator
      operator--(int)
    {
      iterator tmp(*this);
      --(*this);
      return tmp;
    }
    iterator
      operator+(iterator other) const
    {
      return iterator(m_buffer_iterator, m_offset + other.m_offset);
    }

    iterator
      operator+(difference_type n) const
    {
      return iterator(
        m_buffer_iterator,
        static_cast<std::size_t>(
          static_cast<difference_type>(m_offset) + n * value_size));
    }

    iterator friend
      operator+(difference_type n, iterator other)
    {
      return iterator(
        other.m_buffer_iterator,
        static_cast<std::size_t>(
          static_cast<difference_type>(other.m_offset) + n * value_size));
    }

    iterator
      operator-(difference_type n) const
    {
      return iterator(
        m_buffer_iterator,
        static_cast<std::size_t>(
          static_cast<difference_type>(m_offset) - n * value_size));
    }

    difference_type
      operator-(const iterator &other) const
    {
      return static_cast<difference_type>(m_offset - other.m_offset)
           / value_size;
    }

    iterator &
      operator+=(difference_type n)
    {
      m_offset = static_cast<std::size_t>(
        static_cast<difference_type>(m_offset) + n * value_size);
      return *this;
    }

    iterator &
      operator-=(difference_type n)
    {
      m_offset = static_cast<std::size_t>(
        static_cast<difference_type>(m_offset) - n * value_size);

      return *this;
    }

    value_type
      operator*() const
    {
      FI         fi  = {};
      const auto tmp = m_buffer_iterator.subspan(m_offset, sizeof(FI));
      std::memcpy(&fi, tmp.data(), tmp.size());
      return fi;
    }
    //    const FI *
    //      operator->() const
    //    {
    //      return &m_fi;
    //    }
    value_type
      operator[](difference_type n) const
    {
      return *(*this + n);
    }

    bool
      operator==(const iterator &other) const
    {
      return same_range(other) && m_offset == other.m_offset;
    }
    bool
      operator!=(const iterator &other) const
    {
      return !(*this == other);
    }
    bool
      operator<(const iterator &other) const
    {
      return m_offset < other.m_offset;
    }
    bool
      operator<=(const iterator &other) const
    {
      return same_range(other) && m_offset <= other.m_offset;
    }
    bool
      operator>(const iterator &other) const
    {
      return m_offset > other.m_offset;
    }
    bool
      operator>=(const iterator &other) const
    {
      return same_range(other) && m_offset >= other.m_offset;
    }

  private:
    bool
      same_range(const iterator &other) const
    {
      // std::ranges::equal(m_buffer_iterator, other.m_buffer_iterator)
      return m_buffer_iterator.data() == other.m_buffer_iterator.data()
          && m_buffer_iterator.size() == other.m_buffer_iterator.size();
    }
    std::span<const char> m_buffer_iterator{};
    std::size_t           m_offset{};
    // FI             m_fi{};
  };

  iterator::value_type
    operator[](std::size_t n) const
  {
    FI         fi  = {};
    const auto tmp = m_buffer.subspan(n * sizeof(FI));
    std::memcpy(&fi, tmp.data(), tmp.size());
    return fi;
  }
  // The begin function returns an FIInputIterator that points to the beginning
  // of the file
  iterator
    begin() const
  {
    return iterator(m_buffer);
  }

  // The end function returns an FIInputIterator that points past the end of the
  // file
  iterator
    end() const
  {
    return iterator(m_buffer, m_buffer.size());
  }

  FIMemoryRange(std::span<const char> in_buffer)
    : m_buffer(std::move(in_buffer))
  {}

private:
  std::span<const char> m_buffer = {};
};
static_assert(std::random_access_iterator<FIMemoryRange::iterator>);
static_assert(std::ranges::random_access_range<FIMemoryRange>);

}// namespace open_viii::archive
/**
 * define number of arguments
 * @note required to structured binding support
 */
template<>
struct [[maybe_unused]] std::tuple_size<open_viii::archive::FI>
  : std::integral_constant<size_t, 3>
{
};
/**
 * type of 1st argument
 * @note required to structured binding support
 */
template<>
struct [[maybe_unused]] std::tuple_element<0, open_viii::archive::FI>
{
  using type = std::uint32_t;
};
/**
 * type of 2nd argument
 * @note required to structured binding support
 */
template<>
struct [[maybe_unused]] std::tuple_element<1, open_viii::archive::FI>
{
  using type = std::uint32_t;
};
/**
 * type of 3rd argument
 * @note required to structured binding support
 */
template<>
struct [[maybe_unused]] std::tuple_element<2, open_viii::archive::FI>
{
  using type = open_viii::CompressionTypeT;
};
#endif// !VIIIARCHIVE_FI_HPP