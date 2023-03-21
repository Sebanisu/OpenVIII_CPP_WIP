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
   * @brief The uncompressed size of the data represented by this block.
   *
   * This field stores the size of the block's data after decompression. Note
   * that the compressed size of the data is not explicitly stored and must be
   * inferred based on other factors such as the size of the block header.
   */
  std::uint32_t    m_uncompressed_size{};

  /**
   * @brief The offset of the block within the compressed file.
   *
   * This field stores the offset in bytes from the beginning of the compressed
   * file where this block's compressed data can be found.
   */
  std::uint32_t    m_offset{};

  /**
   * @brief The compression algorithm used to compress this block's data.
   *
   * This field indicates the type of compression used to compress the data in
   * this block. The compression type is specified by a value from the
   * `CompressionTypeT` enum, which can be one of the following:
   * - `none` (0): no compression (uncompressed)
   * - `lzss` (1): LZSS compression
   * - `lz4` (2): LZ4 compression
   */
  CompressionTypeT m_compression_type{};

  /**
   * \brief Reads a FI object from a binary file at a given offset.
   *
   * \param path The path to the file to read from.
   * \param offset The offset in bytes from the beginning of the file to read
   * from. \return The FI object read from the file, or an empty FI object if an
   * error occurs.
   */
  static auto
    read_from_file(
      const std::filesystem::path &path,
      std::ifstream::pos_type      offset) -> FI
  {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
      std::cerr << ("Failed to open file: " + path.string()) << std::endl;
      return {};
    }
    file.seekg(offset, std::ios::beg);

    FI fi{};
    file >> fi;
    return fi;
  }
  /**
   * @brief Reads a fixed-size `FI` struct from a `std::span` of char data.
   *
   * This function takes a `std::span` of char data and reads a fixed-size `FI`
   * struct from it, starting at the specified offset. The `data` span is first
   * truncated to a subspan starting at the given offset and spanning `FI::SIZE`
   * bytes. The subspan's data is then copied into a `std::array<char,
   * FI::SIZE>` buffer using `std::ranges::copy`. Finally, the contents of the
   * buffer are converted into an `FI` struct using `std::bit_cast`.
   *
   * @param data The `std::span` of char data to read from.
   * @param offset The offset in bytes from the start of the `data` span to
   * start reading from.
   *
   * @returns The `FI` struct read from the specified position in the `data`
   * span.
   */
  constexpr static auto
    read_from_memory(std::span<const char> data, std::size_t offset) -> FI
  {
    data = data.subspan(offset, FI::SIZE);
    std::array<char, FI::SIZE> byte_data{};
    std::ranges::copy(data, byte_data.begin());
    return std::bit_cast<FI>(byte_data);
  }

public:
  /**
   * @brief The size of the `FI` struct in bytes.
   *
   * This constant stores the expected size of the `FI` struct, in bytes. It is
   * used in a static assertion to ensure that the size of the `FI` struct
   * matches the expected size at compile time.
   */
  static constexpr auto SIZE = std::size_t{ 12U };

  /**
   * @brief The file extension for files containing `FI` data.
   *
   * This constant stores the file extension for files containing `FI` data. The
   * extension is stored as a `std::string_view` for efficiency and ease of use.
   */
  static constexpr auto EXT  = std::string_view{ ".fi" };
  /**
   * @brief Get the uncompressed size of the data represented by this block.
   *
   * @return The size of the block's data after decompression, in bytes.
   */
  [[nodiscard]] constexpr auto
    uncompressed_size() const noexcept -> std::uint32_t
  {
    return m_uncompressed_size;
  }

  /**
   * @brief Get the offset of the block within the compressed file.
   *
   * @return The offset in bytes from the beginning of the compressed file where
   * this block's compressed data can be found.
   */
  [[nodiscard]] constexpr auto
    offset() const noexcept -> std::uint32_t
  {
    return m_offset;
  }

  /**
   * @brief Get the compression algorithm used to compress this block's data.
   *
   * @return The type of compression used to compress the data in this block, as
   * defined by the `CompressionTypeT` enum. Possible values are `none` (for
   * uncompressed data), `lzss`, or `lz4`.
   */
  [[nodiscard]] constexpr auto
    compression_type() const noexcept -> CompressionTypeT
  {
    return m_compression_type;
  }

  /**
   * @brief Default constructor for FI object.
   *
   * This constructor initializes the FI object with default values.
   */
  constexpr FI() noexcept = default;

  /**
   * @brief Constructor that takes a FI-like object and copies its data into
   * this object.
   *
   * This constructor takes a FI-like object and copies its data into this FI
   * object. The template parameter `fiT` specifies the type of the FI-like
   * object. There may be data loss when converting the data to std::uint32_t
   * values. Note that this object does not store the file path like the
   * FileData object does.
   *
   * @tparam fiT Type of FI-like object
   * @param fi FI-like object to copy data from
   */
  template<FI_Like fiT>
    requires(!std::is_same_v<std::remove_cvref<fiT>, FI>)
  constexpr explicit FI(const fiT &fi) noexcept
    : m_uncompressed_size{ static_cast<decltype(m_uncompressed_size)>(
      fi.uncompressed_size()) },
      m_offset{ static_cast<decltype(m_offset)>(fi.offset()) },
      m_compression_type{ static_cast<decltype(m_compression_type)>(
        fi.compression_type()) }
  {}

  /**
   * @brief Constructor that initializes an FI object with the given values.
   *
   * This constructor takes the uncompressed size, offset, and compression type
   * of the FI object and initializes it with those values.
   *
   * @param uncompressed_size Bytes of the entry after being uncompressed
   * @param offset Bytes from the start of the file system file
   * @param compression_type Indicates whether the data is compressed or
   * uncompressed, and which compression algorithm was used if compressed
   */
  constexpr FI(
    const std::uint32_t    &uncompressed_size,
    const std::uint32_t    &offset,
    const CompressionTypeT &compression_type = CompressionTypeT::none) noexcept
    : m_uncompressed_size{ uncompressed_size }, m_offset{ offset },
      m_compression_type{ compression_type }
  {}

  /**
   * Construct an FI object by reading its binary representation from a span of
   * data.
   * @param data A span containing all the FI entries
   * @param offset The offset from the start of the span where the desired entry
   * is located
   */
  constexpr FI(std::span<const char> data, std::size_t offset)
    : FI(read_from_memory(data, offset))
  {}

  /**
   * Construct an FI object by reading its binary representation from a span of
   * data at a specific index.
   * @param data A span containing all the FI entries
   * @param id The index of the desired FI entry, starting at 0
   * @param offset The offset from the start of the span to the beginning of the
   * FI entries
   */
  constexpr FI(std::span<const char> data, std::size_t id, std::size_t offset)
    : FI(read_from_memory(data, get_fi_entry_offset(id, offset)))
  {}

  /**
   * Construct an FI object by reading its binary representation from a file at
   * a specific offset.
   * @param path The path to the file containing the FI entries
   * @param offset The offset from the start of the file where the desired entry
   * is located
   */
  template<typename T>
    requires std::same_as<std::filesystem::path, std::remove_cvref_t<T>>
  FI(const T &path, std::size_t offset)
    : FI(read_from_file(path, static_cast<std::ifstream::off_type>(offset)))
  {}
  /**
   * @brief Construct an `FI` object by reading the FI[id] entry from a file at
   * the given offset.
   *
   * @tparam T Type of the path parameter. Must be `std::filesystem::path`.
   * @param path Path to the file containing the FI entries.
   * @param id ID of the FI entry to read, starting at 0.
   * @param offset Offset in bytes to the start of the first FI entry in the
   * file.
   * @note The constructed `FI` object will contain data from the specified
   * entry in the file.
   * @warning The file must exist and be readable, otherwise an exception will
   * be thrown.
   */
  template<typename T>
    requires std::same_as<std::filesystem::path, std::remove_cvref_t<T>>
  FI(const T &path, std::size_t id, std::size_t offset)
    : FI(read_from_file(
      path,
      static_cast<std::ifstream::off_type>(get_fi_entry_offset(id, offset))))
  {}

  /**
   * Get the count of possible entries based on the file size.
   * @param file_size Total bytes in a file.
   * @return The count of possible entries based on the file size.
   */
  template<std::unsigned_integral T>
  [[nodiscard]] static constexpr auto
    get_count(const T &file_size) noexcept -> std::size_t
  {
    return static_cast<std::size_t>(file_size / SIZE);
  }

  /**
   * Get the count of possible entries based on the file size of a file
   * containing the FI entries.
   * @param path The path to the file containing the FI entries.
   * @return The count of possible entries based on the file size. If the path
   * does not exist, returns 0.
   */
  template<typename T>
    requires std::same_as<std::filesystem::path, std::remove_cvref<T>>
  [[maybe_unused]] [[nodiscard]] static auto
    get_count(const T &path) -> std::size_t
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
  /** Get the value of the member variable at the specified index. This function
   * is required for structured binding support.
   * @tparam Index of the member variable to retrieve, which must be 0, 1 or 2.
   * @return The value of the member variable at the specified index.
   * @note This function assumes that the object is in a valid state.
   */
  template<std::size_t Index>
  [[nodiscard]] constexpr auto
    get() const noexcept
  {
    static_assert(Index < 3, "Index out of range");
    if constexpr (Index == 0) {
      return m_uncompressed_size;
    }
    else if constexpr (Index == 1) {
      return m_offset;
    }
    else if constexpr (Index == 2) {
      return m_compression_type;
    }
  }
  /**
   * Calculates the offset in the file where the FI entry is located.
   * @tparam T an integral type to represent the ID of the FI entry.
   * @tparam U an integral type to represent the offset from the start of the
   * first FI entry.
   * @param id The ID of the FI entry, counting from 0 to N.
   * @param offset The number of bytes to start of the first FI entry.
   * @return The offset in the file where the FI entry is located.
   */
  template<std::integral T, std::integral U>
  [[nodiscard]] static constexpr U
    get_fi_entry_offset(T id, U offset = {})
  {
    return static_cast<U>(
      static_cast<U>(id * static_cast<T>(FI::SIZE)) + offset);
  }
  /**
   * Return a new FI object with a different compression type.
   * @param in_compression_type The new compression type.
   * @return A new FI object with the same uncompressed size and offset but with
   *         the specified compression type.
   */
  [[nodiscard]] constexpr auto
    with_compression_type(CompressionTypeT in_compression_type) -> FI
  {
    return FI(m_uncompressed_size, m_offset, in_compression_type);
  }

  /**
   * Return a new FI object with a different uncompressed size.
   * @param in_uncompressed_size The new uncompressed size in bytes.
   * @return A new FI object with the specified uncompressed size and the same
   *         offset and compression type.
   */
  [[nodiscard]] constexpr auto
    with_uncompressed_size(std::uint32_t in_uncompressed_size) -> FI
  {
    return FI(in_uncompressed_size, m_offset, m_compression_type);
  }

  /**
   * Return a new FI object with a different offset.
   * @param in_offset_size The new offset in bytes.
   * @return A new FI object with the same uncompressed size and compression
   * type but with the specified offset.
   */
  [[nodiscard]] constexpr auto
    with_offset(std::uint32_t in_offset_size) -> FI
  {
    return FI(m_uncompressed_size, in_offset_size, m_compression_type);
  }

  /**
   * Overload the stream input operator to read the binary data of a FI object
   * from an input stream.
   * @param is the input stream to read from.
   * @param fi the FI object to store the read data.
   * @return the input stream after the read operation.
   */
  friend auto
    operator>>(std::istream &is, FI &fi) -> std::istream &
  {
    std::array<char, 12U> buffer{};
    if (is.read(buffer.data(), 12)) {
      fi = std::bit_cast<FI>(buffer);
    }
    return is;
  }

  /**
   * Dump FI entry values to an output stream.
   *
   * @param os Output stream value.
   * @param data The FI entry to dump.
   * @return Reference to the output stream.
   */
  friend auto
    operator<<(std::ostream &os, const FI &data) -> std::ostream &
  {
    os << '{' << data.uncompressed_size() << ", " << data.offset() << ", "
       << static_cast<std::uint32_t>(data.compression_type()) << '}';
    return os;
  }
};
/**
 * Ensures that the size of the FI object matches its expected size.
 */
static_assert(sizeof(FI) == FI::SIZE);

/**
 * Ensures that the FI object is trivially copyable.
 */
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

}// namespace open_viii::archive
/**
 * @brief Defines the number of arguments for structured bindings support
 *
 * @note Required for structured bindings support
 */
template<>
struct [[maybe_unused]] std::tuple_size<open_viii::archive::FI>
  : std::integral_constant<size_t, 3>
{
};

/**
 * @brief Defines the type of the 1st argument for structured bindings support
 *
 * @note Required for structured bindings support
 */
template<>
struct [[maybe_unused]] std::tuple_element<0, open_viii::archive::FI>
{
  using type = std::uint32_t;
};

/**
 * @brief Defines the type of the 2nd argument for structured bindings support
 *
 * @note Required for structured bindings support
 */
template<>
struct [[maybe_unused]] std::tuple_element<1, open_viii::archive::FI>
{
  using type = std::uint32_t;
};

/**
 * @brief Defines the type of the 3rd argument for structured bindings support
 *
 * @note Required for structured bindings support
 */
template<>
struct [[maybe_unused]] std::tuple_element<2, open_viii::archive::FI>
{
  using type = open_viii::CompressionTypeT;
};

#endif// !VIIIARCHIVE_FI_HPP