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
#ifndef VIIIARCHIVE_FILEDATA_HPP
#define VIIIARCHIVE_FILEDATA_HPP
#include "open_viii/Concepts.hpp"
#include "tl/input.hpp"
#include "tl/string.hpp"
namespace open_viii::archive {
/**
 * FileData is an FI Like that contains the filepath, offset and size.
 */
struct [[maybe_unused]] FileData
{
private:
  /**
   * The size of the string is defined before the raw char array.
   */
  std::string   m_filename{};
  /**
   * offset to file
   */
  std::uint64_t m_offset{};
  /**
   * size of the file
   */
  std::uint32_t m_size{};

public:
  constexpr FileData() = default;
  /**
   * Piecemeal constructor
   * @param filename path to file
   * @param offset path to
   * @param size
   */
  [[maybe_unused]] FileData(
    decltype(m_filename)              filename,
    const std::unsigned_integral auto offset,
    const std::unsigned_integral auto size)
    : m_filename(tl::string::replace_slashes(std::move(filename))),
      m_offset(static_cast<decltype(m_offset)>(offset)),
      m_size(static_cast<decltype(m_size)>(size))
  {
    tl::string::replace_slashes(m_filename);
  }
  [[maybe_unused]] FileData(
    const std::unsigned_integral auto offset,
    const std::unsigned_integral auto size)
    : FileData(std::string(), offset, size)
  {}
  [[nodiscard]] bool
    empty() const noexcept
  {
    return m_size == 0 || m_filename.empty();
  }
  explicit FileData(
    tl::read::input                   input,
    const std::unsigned_integral auto string_length)
    : m_filename(tl::string::replace_slashes(
      input.output<decltype(m_filename)>(std::string(string_length, '\0')))),
      m_offset(input.output<decltype(m_offset)>()),
      m_size(input.output<decltype(m_size)>())
  {}
  explicit FileData(tl::read::input input)
    : FileData(input, input.output<std::uint32_t>())
  {}
  explicit FileData(std::istream &fp) : FileData(tl::read::input(&fp, true)) {}
  explicit FileData(std::span<const char> *const fp)
    : FileData(tl::read::input(fp, true))
  {}
  explicit FileData(std::span<const char> fp)
    : FileData(tl::read::input(&fp, true))
  {}
  template<FI_Like fiT>
    requires(!std::is_same_v<fiT, FileData>)
  constexpr explicit FileData(const fiT &fi)
    : m_offset{ static_cast<decltype(m_offset)>(fi.offset()) }, m_size{
        static_cast<decltype(m_size)>(fi.uncompressed_size())
      }
  {
    if (fi.compression_type() != CompressionTypeT::none) {
      throw std::invalid_argument(
        "Compression Type must be none as FileData doesn't store compressed "
        "data");
    }
  }
  template<FI_Like fiT>
    requires(!std::is_same_v<fiT, FileData>)
  explicit FileData(std::string filename, const fiT &fi)
    : m_filename(tl::string::replace_slashes(std::move(filename))),
      m_offset{ static_cast<decltype(m_offset)>(fi.offset()) }, m_size{
        static_cast<decltype(m_size)>(fi.uncompressed_size())
      }
  {
    if (fi.compression_type() != CompressionTypeT::none) {
      throw std::invalid_argument(
        "Compression Type must be none as FileData doesn't store compressed "
        "data");
    }
  }
  /** size of this file entry in the zzz file.
   * This is for appending a new entry. I don't know if it works.
   * @todo used in tests no where else. might want to make this a free function
   * of the tests.
   * @return
   */
  [[maybe_unused]] [[nodiscard]] constexpr auto
    total_size() const noexcept
  {
    return sizeof(unsigned int) + std::ranges::size(m_filename)
         + sizeof(m_offset) + sizeof(m_size);
  }
  /**
   * gets path as a std::filesystem::path
   */
  [[maybe_unused]] [[nodiscard]] auto
    get_path() const
  {
    return std::filesystem::path(m_filename);
  }
  //  // get size of file
  //  [[maybe_unused]] [[nodiscard]] constexpr auto size() const noexcept
  //  {
  //    return m_size;
  //  }
  /**
   * Compression type required to match FI like concept.
   * @return no compression
   */
  [[maybe_unused]] [[nodiscard]] static constexpr auto
    compression_type() noexcept
  {
    return CompressionTypeT::none;
  }
  /**
   * alias for Size that should mirror FI
   */
  [[maybe_unused]] [[nodiscard]] constexpr auto
    uncompressed_size() const noexcept
  {
    return m_size;
  }
  /**
   * get offset of file
   */
  [[maybe_unused]] [[nodiscard]] constexpr auto
    offset() const noexcept
  {
    return m_offset;
  }

  [[maybe_unused]] [[nodiscard]] FileData
    with_uncompressed_size(std::uint32_t in_size) const noexcept
  {

    auto r   = *this;
    r.m_size = in_size;
    return r;
  }

  [[maybe_unused]] [[nodiscard]] FileData
    with_offset(std::uint64_t in_offset) const noexcept
  {
    auto r     = *this;
    r.m_offset = in_offset;
    return r;
  }

  [[maybe_unused]] [[nodiscard]] auto
    with_path(std::filesystem::path path) const
  {

    auto r       = *this;
    r.m_filename = path.string();
    return r;
  }

  /**
   * gets path as a std::string_view
   */
  [[maybe_unused]] [[nodiscard]] auto
    get_path_string_view() const
  {
    return std::string_view(m_filename);
  }
  /**
   * gets path as a std::string
   */
  [[maybe_unused]] [[nodiscard]] auto
    get_path_string() const
  {
    return m_filename;
  }

  /**
   * gets member variables
   * @note required to structured binding support
   */
  template<std::size_t I>
  [[nodiscard]] auto
    get() const noexcept
  {
    static_assert(I < 3);
    if constexpr (I == 0) {
      return std::string_view(m_filename);
    }
    else if constexpr (I == 1) {
      return m_offset;
    }
    else if constexpr (I == 2) {
      return m_size;
    }
  }
};

/**
 * Append FileData to buffer.
 * @tparam T type of output buffer.
 * @param output buffer to write to.
 * @param fd incoming FI it's just grabbing the compression type from it.
 */
template<is_insertable_or_ostream T>
static void
  append_entry(T &output, const FileData fd)
{
  std::string string = std::string(fd.get_path_string_view());
  tl::string::undo_replace_slashes(string);
  tl::write::append(output, static_cast<std::uint32_t>(std::size(string)));
  tl::write::append(output, string);
  tl::write::append(output, fd.offset(), fd.uncompressed_size());
}

class FileDataOutputIterator
{
public:
  using iterator_category = std::output_iterator_tag;
  using value_type        = FileData;
  using difference_type   = std::ptrdiff_t;
  using pointer           = void;
  using reference         = void;
  explicit FileDataOutputIterator(std::ostream &output_stream)
    : m_output_stream(output_stream)
  {}

  FileDataOutputIterator &
    operator=(const FileData &file_data)
  {
    // Write string_size to output stream
    const std::uint32_t string_size
      = static_cast<std::uint32_t>(file_data.get_path_string_view().size());
    m_output_stream.write(
      std::bit_cast<const char *>(&string_size),
      sizeof(string_size));

    // Write file_path to output stream
    m_output_stream.write(
      file_data.get_path_string_view().data(),
      static_cast<std::streamsize>(file_data.get_path_string_view().size()));

    // Write binary_offset to output stream
    const std::uint64_t offset = file_data.offset();
    m_output_stream.write(std::bit_cast<const char *>(&offset), sizeof(offset));

    // Write binary_size to output stream
    const std::uint32_t size = file_data.uncompressed_size();
    m_output_stream.write(std::bit_cast<const char *>(&size), sizeof(size));

    return *this;
  }

  FileDataOutputIterator &
    operator*()
  {
    return *this;
  }

  FileDataOutputIterator &
    operator++()
  {
    return *this;
  }

  FileDataOutputIterator &
    operator++(int)
  {
    return *this;
  }

private:
  std::ostream &m_output_stream;
};

}// namespace open_viii::archive

/**
 * define number of arguments
 * @note required to structured binding support
 */
template<>
struct [[maybe_unused]] std::tuple_size<open_viii::archive::FileData>
  : std::integral_constant<size_t, 3>
{
};

/**
 * type of 1st argument
 * @note required to structured binding support
 */
template<>
struct [[maybe_unused]] std::tuple_element<0, open_viii::archive::FileData>
{
  using type = std::string_view;
};
/**
 * type of 2nd argument
 * @note required to structured binding support
 */
template<>
struct [[maybe_unused]] std::tuple_element<1, open_viii::archive::FileData>
{
  using type = std::uint64_t;
};
/**
 * type of 3rd argument
 * @note required to structured binding support
 */
template<>
struct [[maybe_unused]] std::tuple_element<2, open_viii::archive::FileData>
{
  using type = std::uint32_t;
};

#endif// VIIIARCHIVE_FILEDATA_HPP