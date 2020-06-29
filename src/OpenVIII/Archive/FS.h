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
#ifndef VIIIARCHIVE_FS_H
#define VIIIARCHIVE_FS_H

#include "FI.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <memory>
#include <utility>
#include <iterator>
#include <cstring>

#include "../../../External/VIIICompression/LZSS.h"
#include "../../../External/VIIICompression/L4Z.h"
#include "../../../External/VIIITools/Tools.h"

namespace OpenVIII::Archive {
static size_t largestCompressedBuffer{};
struct FS
{
public:
  static constexpr const auto Ext = std::string_view(".FS");

  template<typename dstT = std::vector<char>, typename datT = OpenVIII::Archive::FI>
  static dstT GetEntry(const std::filesystem::path &path, const datT &fi, const size_t &offset)
  {
    if (fi.UncompressedSize() == 0) {
      return {};
    }
    std::ifstream fp = std::ifstream(path, std::ios::in | std::ios::binary);
    // if compressed will keep decompressing till get size
    // size compressed isn't quite known with out finding the offset of the next file and finding difference.
    fp.seekg(static_cast<long>(offset + fi.Offset()), std::ios::beg);
    //    static const auto readBuffer = [&fp](const auto &s) {
    //      auto buf = std::vector<char>(s);
    ////      auto tmp = std::vector<char>(s);
    //      fp.read(buf.data(), s);// TODO change to bitcast in cpp 20 or read another way?
    ////      memcpy(buf.data(), tmp.data(), s);// memcpy will be optimized away. It is safer than casting.
    //      return buf;
    //    };

    //    static const auto readVal = [&fp](auto &v) {
    //      auto tmp = std::array<char, sizeof(v)>();
    //      fp.read(tmp.data(), sizeof(v));
    //      memcpy(&v, tmp.data(), sizeof(v));// memcpy will be optimized away. It is safer than casting.
    //      // TODO change to bitcast in cpp 20 or read another way?
    //    };


    switch (fi.CompressionType()) {
    case CompressionTypeT::None: {
      return Tools::ReadBuffer<dstT>(fp, fi.UncompressedSize());
    }
    case CompressionTypeT::LZSS: {
      unsigned int compSize{ 0 };
      Tools::ReadVal(fp, compSize);
      if (compSize > largestCompressedBuffer)
        largestCompressedBuffer = compSize;
      dstT buffer = Tools::ReadBuffer<dstT>(fp, compSize);
      return Compression::LZSS::Decompress<dstT>(buffer, fi.UncompressedSize());
    }
    case CompressionTypeT::LZ4: {
      unsigned int sectSize{ 0 };
      // L4Z header contains size of total section as uint32, 4 byte string
      // the size of the compressed data is the first value minus 8. the second value is something i'm unsure of
      Tools::ReadVal(fp, sectSize);
      constexpr static auto skipSize = 8U;
      fp.seekg(skipSize, std::ios::cur);
      const auto compSize = sectSize - skipSize;
      dstT buffer = Tools::ReadBuffer<dstT>(fp, compSize);

      if (compSize > largestCompressedBuffer) {
        largestCompressedBuffer = compSize;
      }
      return Compression::L4Z::Decompress<dstT>(buffer.data(), compSize, fi.UncompressedSize());
    }
    }
    fp.close();
    return {};
  }
  template<typename dstT = std::vector<char>>
  static dstT GetEntry(const std::vector<char> &data, const OpenVIII::Archive::FI &fi, const size_t &offset)
  {
    // it shouldn't be empty
    if (data.empty()) {
      return {};
    }
    // todo do I need this version of the function? I think at least for nested entries. zzz shouldn't use this.
    // It should use the other one. Though that is only the case for FIFLFS archives.
    if (fi.UncompressedSize() == 0) {
      return {};
    }

    auto iterator = data.begin() + fi.Offset() + static_cast<long>(offset);
    auto ptr = data.data() + fi.Offset() + static_cast<long>(offset);
    // ptr and iterator.base() would be the same. sadly msvc doesn't have a .base()
    // todo usages of memcpy can be replaced with std::bitcast in cpp20

    switch (fi.CompressionType()) {
    case CompressionTypeT::None: {
      if (iterator + fi.UncompressedSize() > data.end()) {
        break;
      }
      // dstT dst = {};
      // dst.reserve(fi.UncompressedSize());
      // std::copy(iterator, iterator + fi.UncompressedSize(), std::back_inserter(dst)); //todo do I need this copy?
      return { iterator, iterator + fi.UncompressedSize() };
    }
    case CompressionTypeT::LZSS: {
      unsigned int compSize{ 0 };
      if (iterator + sizeof(compSize) > data.end()) {
        break;
      }
      std::memcpy(&compSize, ptr, sizeof(compSize));
      if (compSize > largestCompressedBuffer) {
        largestCompressedBuffer = compSize;
      }
      iterator += sizeof(compSize);
      if (iterator + compSize > data.end()) {
        break;
      }
      ptr += sizeof(compSize);
      std::string_view span(
        ptr, compSize);// i think this works then I don't need to copy the data from one vector to another.
      // buffer.reserve(compSize);
      // std::copy(iterator, iterator + compSize, std::back_inserter(buffer));// todo replace with std::span in cpp20.
      return Compression::LZSS::Decompress<dstT>(span, fi.UncompressedSize());
    }
    case CompressionTypeT::LZ4: {
      unsigned int sectSize{ 0 };
      // L4Z header contains size of total section as uint32, 4 byte string and another uint32.
      // the size of the compressed data is the first value minus 8. the second value is something i'm unsure of
      if (iterator + sizeof(sectSize) > data.end()) {
        break;
      }
      std::memcpy(&sectSize, ptr, sizeof(sectSize));
      ptr += sizeof(sectSize);
      if (iterator + sectSize + sizeof(sectSize) > data.end()) {
        break;
      }
      constexpr static auto skipBytes = 8U;
      ptr += skipBytes;
      const auto compSize = sectSize - skipBytes;
      if (compSize > largestCompressedBuffer) {
        largestCompressedBuffer = compSize;
      }
      return Compression::L4Z::Decompress<dstT>(ptr, compSize, fi.UncompressedSize());
    }
    }
    return {};
  }
};
}// namespace OpenVIII::Archive
#endif// !VIIIARCHIVE_FS_H
