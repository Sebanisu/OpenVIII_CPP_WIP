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
#ifndef VIIIARCHIVE_FS_HPP
#define VIIIARCHIVE_FS_HPP
#include "FI.hpp"
#include "open_viii/compression/L4Z.hpp"
#include "open_viii/compression/LZSS.hpp"
#include "open_viii/tools/Tools.hpp"
#include "tl/input.hpp"
#include "tl/read.hpp"
#include "tl/write.hpp"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <span>
#include <utility>
namespace open_viii::archive::FS {
/**
 * File Source
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/PC_Media#.fs_.28File_Source.29
 */

/**
 * Extension
 */
static constexpr auto EXT = std::string_view(".FS");

/**
 * Get entry and uncompress via lzss
 * @note not meant to be used directly
 * @tparam outputT type to be returned: example std::string, std::vector<char>
 * @tparam fiT FI or FI compatible type.
 * @param input buffer adapter that holds a std::span<char> or a std::istream
 * @param uncompressed_size number of bytes expected to expand to.
 * @return output() filled with uncompressed data.
 * @note lzss doesn't need the uncompressed size to extract the data but it's
 * used to reserve the memory before uncompressing.
 */
template<
  is_default_constructible_has_data_size_resize outputT = std::vector<char>>
static outputT
  get_entry_lzss(tl::read::input &input, const std::uint32_t uncompressed_size)
{
  const auto compSize = input.template output<uint32_t>();
  outputT    buffer   = input.template output<outputT>(compSize);
  return compression::LZSS::decompress<outputT>(buffer, uncompressed_size);
}

/**
 * Get entry and uncompress via lz4
 * @note not meant to be used directly
 * @tparam outputT type to be returned: example std::string, std::vector<char>
 * @tparam fiT FI or FI compatible type.
 * @param input buffer adapter that holds a std::span<char> or a std::istream
 * @param uncompressed_size number of bytes expected to expand to.
 * @return output() filled with uncompressed data.
 * @note L4Z header contains size of total section as uint32, 4 byte string, and
 * a uint32 of the uncompressed size.
 */
template<
  is_default_constructible_has_data_size_resize outputT = std::vector<char>>
static outputT
  get_entry_lz4(tl::read::input &input)
{
  constexpr static auto skip_size       = 8;
  const auto            section_size    = input.template output<uint32_t>();
  const std::uint32_t   compressed_size = section_size - skip_size;
  const std::uint32_t   uncompressed_size =
    input.seek(4, std::ios::cur).template output<uint32_t>();
  outputT buffer = input.template output<outputT>(compressed_size);
  return compression::l4z::decompress<outputT>(
    buffer.data(), compressed_size, uncompressed_size);
}

/**
 * Get entry and uncompress via lz4
 * @tparam outputT type to be returned: example std::string, std::vector<char>
 * @tparam fiT FI or FI compatible type.
 * @param input buffer adapter that holds a std::span<char> or a std::istream
 * @param fi contains compression type, uncompressed size, and offset
 * @param offset additional offset value added to fi.offset()
 * @return output() filled with uncompressed data.
 */
template<
  is_default_constructible_has_data_size_resize outputT = std::vector<char>,
  FI_Like                                       fiT     = FI>
[[nodiscard]] static outputT
  get_entry(tl::read::input input, const fiT fi, const std::size_t offset = 0U)
{
  input.seek(static_cast<long>(offset + fi.offset()), std::ios::beg);
  // if compressed will keep decompressing till get size
  // size compressed isn't quite known with out finding the offset of the next
  // file and finding difference.
  switch (fi.compression_type()) {
  case CompressionTypeT::none: {
    return input.template output<outputT>(fi.uncompressed_size());
  }
  case CompressionTypeT::lzss: {
    return get_entry_lzss<outputT>(input, fi.uncompressed_size());
  }
  case CompressionTypeT::lz4: {
    return get_entry_lz4<outputT>(input);
  }
  }
  throw;
}

/**
 * get file entry and decompress it
 * @tparam outputT type being returned
 * @tparam fiT type of FI or FileData that contains offset, size, compression.
 * @param path to file
 * @param fi FI or FileData
 * @param offset to file data in bytes
 * @return uncompressed file
 */
template<
  is_default_constructible_has_data_size_resize outputT = std::vector<char>,
  FI_Like                                       fiT     = FI>
[[nodiscard]] static outputT
  get_entry(const std::filesystem::path &path,
            const fiT &                  fi,
            const size_t &               offset = 0U)
{
  if (fi.uncompressed_size() == 0) {
    return {};
  }
  auto ofp = tl::read::open_file(path);
  if (ofp.has_value() && ofp->is_open()) {
    return get_entry<outputT>(tl::read::input(&*ofp, true), fi, offset);
  }
  return {};
}

/**
 * get file entry and decompress it
 * @tparam outputT type being returned
 * @tparam fiT type of FI or FileData that contains offset, size, compression.
 * @param data buffer with embedded file
 * @param fi FI or FileData
 * @param offset to file data in bytes
 * @return uncompressed file
 */
template<
  is_default_constructible_has_data_size_resize outputT = std::vector<char>,
  FI_Like                                       fiT     = FI>
[[nodiscard]] static outputT
  get_entry(std::span<const char> data, const fiT &fi, const size_t offset = 0U)
{
  // it shouldn't be empty
  if (data.empty() || fi.uncompressed_size() == 0) {
    return {};
  }
  return get_entry<outputT>(tl::read::input(data, true), fi, offset);
}
}// namespace open_viii::archive::FS
namespace open_viii::archive {

/**
 * Append lzss compressed entry
 * @tparam T type of output buffer.
 * @param output buffer to write to.
 * @param input buffer to read from.
 */
template<is_insertable_or_ostream T>
static void
  append_lzss(T &output, const std::span<const char> &input)
{
  std::vector<char> new_comp_data = compression::LZSS::compress(input);
  tl::write::append(output,
                    static_cast<uint32_t>(std::ranges::size(new_comp_data)));
  tl::write::append(output, new_comp_data);
}

/**
 * Append lz4 compressed entry
 * @tparam T type of output buffer.
 * @param output buffer to write to.
 * @param input buffer to read from.
 */
template<is_insertable_or_ostream T>
static void
  append_l4z(T &output, const std::span<const char> &input)
{
  static constexpr auto lz4           = std::string_view("4ZL_", 4U);
  std::vector<char>     new_comp_data = compression::l4z::compress(input);
  const auto            compressed_size =
    static_cast<uint32_t>(std::ranges::size(new_comp_data));
  tl::write::append(output, compressed_size + 8U);
  tl::write::append(output, lz4);
  tl::write::append(output,
                    static_cast<std::uint32_t>(std::ranges::size(input)));
  tl::write::append(output, new_comp_data);
}

/**
 * Append entry to FS and get back FI like data where it is in the FS file.
 * @tparam T type of output buffer.
 * @tparam fiT FI like data type returned
 * @param output buffer to write to.
 * @param input buffer to read from.
 * @param compression_type desired compression value: none, lzss, lz4
 * @return return FI like data.
 */
template<is_insertable_or_ostream T, FI_Like fiT = FI>
static fiT
  append_entry(T &                         output,
               const std::span<const char> input,
               const CompressionTypeT compression_type = CompressionTypeT::none)
{
  using uncompressed_size_t = std::decay_t<decltype(fiT().uncompressed_size())>;
  using offset_t            = std::decay_t<decltype(fiT().offset())>;
  fiT return_value{ static_cast<uncompressed_size_t>(std::ranges::size(input)),
                    static_cast<offset_t>(tl::utility::get_position(output)),
                    compression_type };
  switch (compression_type) {
  case CompressionTypeT::none: {
    tl::write::append(output, input);
    return return_value;
    break;
  }
  case CompressionTypeT::lzss: {
    append_lzss(output, input);
    return return_value;
    break;
  }
  case CompressionTypeT::lz4: {
    append_l4z(output, input);
    return return_value;
    break;
  }
  }
  throw;// it's an error to get here.
}

/**
 * Append entry to FS and get back FI like data where it is in the FS file.
 * @tparam in_fiT FI like data type incoming
 * @tparam out_fiT FI like data type returned
 * @tparam T type of output buffer.
 * @param output buffer to write to.
 * @param input buffer to read from.
 * @param in_fi incoming FI it's just grabbing the compression type from it.
 * @return return FI like data.
 */
template<FI_Like in_fiT = FI, FI_Like out_fiT = FI, is_insertable_or_ostream T>
static out_fiT
  append_entry(T &output, const std::span<const char> input, const in_fiT in_fi)
{
  return append_entry(output, input, in_fi.compression_type());
}
}// namespace open_viii::archive
#endif// !VIIIARCHIVE_FS_HPP