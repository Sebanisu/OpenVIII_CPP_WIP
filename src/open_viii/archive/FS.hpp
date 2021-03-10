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
static constexpr auto EXT = std::string_view(".FS");
template<is_default_constructible_has_data_size_resize dstT = std::vector<char>,
         FI_Like                                       fiT  = FI>
static dstT
  get_entry(tl::read::input input, const fiT fi, const std::size_t offset = 0U)
{
  input.seek(static_cast<long>(offset + fi.offset()),std::ios::beg);
  // if compressed will keep decompressing till get size
  // size compressed isn't quite known with out finding the offset of the next
  // file and finding difference.
  switch (fi.compression_type()) {
  case CompressionTypeT::none: {
    return input.template output<dstT>(fi.uncompressed_size());
  }
  case CompressionTypeT::lzss: {
    const auto compSize = input.template output<std::uint32_t>();
    dstT       buffer   = input.template output<dstT>(compSize);
    return compression::LZSS::decompress<dstT>(buffer, fi.uncompressed_size());
  }
  case CompressionTypeT::lz4: {
    // L4Z header contains size of total section as uint32, 4 byte string
    // the size of the compressed data is the first value minus 8. the second
    // value is something i'm unsure of
    constexpr static auto skipSize = 8U;
    const auto            sectSize = input.template output<std::uint32_t>();
    const auto            compSize = sectSize - skipSize;
    dstT buffer = input.seek(skipSize, std::ios::cur).template output<dstT>(compSize);
    return compression::l4z::decompress<dstT>(
      buffer.data(), compSize, fi.uncompressed_size());
  }
  }
  throw;
}
/**
 * get file entry and decompress it
 * @tparam dstT type being returned
 * @tparam fiT type of FI or FileData that contains offset, size, compression.
 * @param path to file
 * @param fi FI or FileData
 * @param offset to file data in bytes
 * @return uncompressed file
 */
template<is_default_constructible_has_data_size_resize dstT = std::vector<char>,
         FI_Like                                       fiT  = FI>
static dstT
  get_entry(const std::filesystem::path &path,
            const fiT &                  fi,
            const size_t &               offset = 0U)
{
  if (fi.uncompressed_size() == 0) {
    return {};
  }
  auto ofp = tl::read::open_file(path);
  if (ofp.has_value() && ofp->is_open()) {
    return get_entry<dstT>(tl::read::input(&*ofp, true), fi, offset);
  }
  return {};
}
/**
 * get file entry and decompress it
 * @tparam dstT type being returned
 * @tparam fiT type of FI or FileData that contains offset, size, compression.
 * @param data buffer with embedded file
 * @param fi FI or FileData
 * @param offset to file data in bytes
 * @return uncompressed file
 */
template<is_default_constructible_has_data_size_resize dstT = std::vector<char>,
         FI_Like                                       fiT  = FI>
static dstT
  get_entry(std::span<const char> data, const fiT &fi, const size_t &offset)
{
  // it shouldn't be empty
  if (data.empty() || fi.uncompressed_size() == 0) {
    return {};
  }
  return get_entry<dstT>(tl::read::input(data, true), fi, offset);
}
}// namespace open_viii::archive::FS
#endif// !VIIIARCHIVE_FS_HPP