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
 */
template<
  is_default_constructible_has_data_size_resize outputT = std::vector<char>>
static outputT
  get_entry_lz4(tl::read::input &input, const std::uint32_t uncompressed_size)
{// L4Z header contains size of total section as uint32, 4 byte string
  // the size of the compressed data is the first value minus 8. the second
  // value is maybe the uint32 value returned at compression time.
  constexpr static auto skipSize = 8;
  const auto            sectSize = input.template output<uint32_t>();
  const auto            compSize = sectSize - skipSize;
  outputT               buffer =
    input.seek(skipSize, std::ios::cur).template output<outputT>(compSize);
  return compression::l4z::decompress<outputT>(
    buffer.data(), compSize, uncompressed_size);
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
    return get_entry_lz4<outputT>(input, fi.uncompressed_size());
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
// todo I need a version for a std::ostream and a version for a std::vector;
static void
  append(std::vector<char> &output, const std::span<const char> &input)
{
  output.insert(std::ranges::end(output),
                std::ranges::begin(input),
                std::ranges::end(input));
}
template<typename inputT>
// todo I need a version for a std::ostream and a version for a std::vector;
requires(std::is_trivially_copyable_v<inputT> && !requires(inputT t) {
  // prevents things that std::span could take from going here
  std::span<const char>(t);
}) static void append(std::vector<char> &output, const inputT &input)
{
  std::array<char, sizeof(inputT)> input_as_bytes{};
  std::memcpy(std::data(input_as_bytes), &input, sizeof(inputT));
  append(output, input_as_bytes);
}
// todo I need a version for a std::ostream and a version for a std::vector;
static void
  append_lzss(std::vector<char> &output, const std::span<const char> &input)
{
  std::vector<char> new_comp_data = compression::LZSS::compress(input);
  append(output, static_cast<uint32_t>(std::ranges::size(new_comp_data)));
  append(output, new_comp_data);
}
// todo I need a version for a std::ostream and a version for a std::vector;
static void
  append_l4z(std::vector<char> &output, const std::span<const char> &input)
{
  static constexpr auto lz4           = std::string_view("4ZL_", 4U);
  std::vector<char>     new_comp_data = compression::l4z::compress(input);
  const auto            compressed_size =
    static_cast<uint32_t>(std::ranges::size(new_comp_data));
  append(output, compressed_size + 8U);
  append(output, lz4);
  append(output, static_cast<std::uint32_t>(std::ranges::size(input)));
  append(output, new_comp_data);
}
// todo I need a version for a std::ostream and a version for a std::vector;
template<FI_Like fiT = FI>
static fiT
  append_entry(std::vector<char> &         output,
               const std::span<const char> input,
               const CompressionTypeT compression_type = CompressionTypeT::none)
{
  using uncompressed_size_t = std::decay_t<decltype(fiT().uncompressed_size())>;
  using offset_t            = std::decay_t<decltype(fiT().offset())>;
  fiT return_value{ static_cast<uncompressed_size_t>(std::ranges::size(input)),
                    static_cast<offset_t>(std::ranges::size(output)),
                    compression_type };
  switch (compression_type) {
  case CompressionTypeT::none: {
    append(output, input);
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
  throw;
}
template<FI_Like fiT = FI>
static fiT
  append_entry(std::vector<char> &         output,
               const std::span<const char> input,
               const fiT                   in_fi)
{
  return append_entry(output, input, in_fi.compression_type());
}
}// namespace open_viii::archive::FS
#endif// !VIIIARCHIVE_FS_HPP