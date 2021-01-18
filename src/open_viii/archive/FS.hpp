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
  FI_Like fiT = FI>
static dstT get_entry(
  const std::filesystem::path &path, const fiT &fi, const size_t &offset = 0U)
{
  if (fi.uncompressed_size() == 0) {
    return {};
  }
  auto ofp =tools::open_file(path);
  if(ofp.has_value() && ofp->is_open()) {
    // if compressed will keep decompressing till get size
    // size compressed isn't quite known with out finding the offset of the next
    // file and finding difference.
    ofp->seekg(static_cast<long>(offset + fi.offset()), std::ios::beg);

    switch (fi.compression_type()) {
    case CompressionTypeT::none: {
      return tools::read_val<dstT>(*ofp, fi.uncompressed_size());
    }
    case CompressionTypeT::lzss: {
      unsigned int compSize{ 0 };
      tools::read_val(*ofp, compSize);
      dstT buffer = tools::read_val<dstT>(*ofp, compSize);
      return compression::LZSS::decompress<dstT>(
        buffer, fi.uncompressed_size());
    }
    case CompressionTypeT::lz4: {
      // L4Z header contains size of total section as uint32, 4 byte string
      // the size of the compressed data is the first value minus 8. the second
      // value is something i'm unsure of
      const auto sectSize = tools::read_val<std::uint32_t>(*ofp);
      constexpr static auto skipSize = 8U;
      ofp->seekg(skipSize, std::ios::cur);
      const auto compSize = sectSize - skipSize;
      dstT buffer = tools::read_val<dstT>(*ofp, compSize);


      return compression::l4z::decompress<dstT>(
        buffer.data(), compSize, fi.uncompressed_size());
    }
    }
    ofp->close();
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
  FI_Like fiT = FI>
static dstT get_entry(
  std::span<const char> data, const fiT &fi, const size_t &offset)
{
  // it shouldn't be empty
  if (data.empty()) {
    return {};
  }
  // todo if we get a span stream we maybe can combine the two functions into 1.
  // stringstream only works with strings. and strstream is depreciated. It
  // should use the other one. Though that is only the case for FIFLFS archives.
  if (fi.uncompressed_size() == 0) {
    return {};
  }
  data = data.subspan(fi.offset() + offset);
  // todo usages of memcpy can be replaced with std::bitcast in cpp20

  switch (fi.compression_type()) {
  case CompressionTypeT::none: {
    if (fi.uncompressed_size() > std::ranges::size(data)) {
      break;
    }
    data = data.subspan(0, fi.uncompressed_size());
    return { std::ranges::cbegin(data), std::ranges::end(data) };
  }
  case CompressionTypeT::lzss: {
    if (sizeof(std::uint32_t) > std::ranges::size(data)) {
      break;
    }

    const auto compSize = tools::read_val<std::uint32_t>(data);
    if (compSize + sizeof(compSize) > std::ranges::size(data)) {
      break;
    }
    return compression::LZSS::decompress<dstT>(
      data.subspan(sizeof(compSize), compSize), fi.uncompressed_size());
  }
  case CompressionTypeT::lz4: {
    // L4Z header contains size of total section as uint32, 4 byte string and
    // another uint32. the size of the compressed data is the first value
    // minus 8. the second value is something i'm unsure of
    if (sizeof(std::uint32_t) > std::ranges::size(data)) {
      break;
    }

    std::uint32_t sectSize = tools::read_val<std::uint32_t>(data);
    // std::memcpy(&sectSize, std::ranges::data(data), sizeof(sectSize));
    if (sectSize > std::ranges::size(data)) {
      break;
    }
    constexpr static auto skipBytes = 8U;
    const auto compSize = sectSize - skipBytes;
    data = data.subspan(sizeof(sectSize) + skipBytes, compSize);

    return compression::l4z::decompress<dstT>(data, fi.uncompressed_size());
  }
  }
  return {};
}
}// namespace open_viii::archive::FS
#endif// !VIIIARCHIVE_FS_HPP
