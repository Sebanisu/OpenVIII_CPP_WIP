#ifndef OPENVIII_FS_H
#define OPENVIII_FS_H

#include "FI.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <memory>
#include <utility>
#include <iterator>
#include <cstring>
#include "Compression/LZSS/LZSS.h"

#include "Compression/L4Z/L4Z.h"
#include "Tools/Tools.h"

namespace OpenVIII::Archive {
struct FS
{
public:
  static constexpr const auto Ext = std::string_view(".FS");

  template<typename dstT = std::vector<char>>
  [[nodiscard]] static dstT
    GetEntry(const std::filesystem::path &path, const OpenVIII::Archive::FI &fi, const size_t &offset)
  {
    if (fi.UncompressedSize() == 0) { return {}; }
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

    dstT buffer;
    unsigned int compSize{ 0 };
    unsigned int sectSize{ 0 };
    switch (fi.CompressionType()) {
    case TCompressionType::None:
      buffer = Tools::ReadBuffer<dstT>(fp, fi.UncompressedSize());
      fp.close();
      return buffer;
    case TCompressionType::LZSS:
      Tools::ReadVal(fp, compSize);
      buffer = Tools::ReadBuffer<dstT>(fp, compSize);
      fp.close();
      return Compression::LZSS::Decompress<dstT>(buffer, fi.UncompressedSize());
    case TCompressionType::LZ4:
      // L4Z header contains size of total section as uint32, 4 byte string
      // the size of the compressed data is the first value minus 8. the second value is something i'm unsure of
      Tools::ReadVal(fp, sectSize);
      auto str = std::array<char, 4>();
      fp.read(str.data(), 4);
      Tools::ReadVal(fp, compSize);
      buffer = Tools::ReadBuffer<dstT>(fp, sectSize - 8);
      fp.close();
      return Compression::L4Z::Decompress<dstT>(buffer.data(), sectSize - 8, fi.UncompressedSize());
    }
    return {};
  }
  template<typename dstT = std::vector<char>>
  [[nodiscard]] static dstT
    GetEntry(const std::vector<char> &data, const OpenVIII::Archive::FI &fi, const size_t &offset)
  {
    if (fi.UncompressedSize() == 0) { return {}; }

    dstT buffer{};
    auto iterator = data.begin() + fi.Offset() + static_cast<long>(offset);
    auto ptr = data.data() + fi.Offset() + static_cast<long>(offset);
    // ptr and iterator.base() would be the same. sadly msvc doesn't have a .base()
    // todo usages of memcpy can be replaced with std::bitcast in cpp20
    unsigned int compSize{ 0 };
    unsigned int sectSize{ 0 };
    switch (fi.CompressionType()) {
    case TCompressionType::None:
      if (iterator + fi.UncompressedSize() > data.end()) {
        // auto umm = data.size() - fi.UncompressedSize();
        break;
      }
      buffer.reserve(fi.UncompressedSize());
      std::copy(iterator, iterator + fi.UncompressedSize(), std::back_inserter(buffer));
      return buffer;
    case TCompressionType::LZSS:
      if (iterator + sizeof(compSize) > data.end()) { break; }
      std::memcpy(&compSize, ptr, sizeof(compSize));
      iterator += sizeof(compSize);
      if (iterator + compSize > data.end()) { break; }
      buffer.reserve(compSize);
      std::copy(iterator, iterator + compSize, std::back_inserter(buffer));// todo replace with std::span in cpp20
      return Compression::LZSS::Decompress<dstT>(buffer, fi.UncompressedSize());
    case TCompressionType::LZ4:
      // L4Z header contains size of total section as uint32, 4 byte string
      // the size of the compressed data is the first value minus 8. the second value is something i'm unsure of
      if (iterator + sizeof(sectSize) > data.end()) { break; }
      std::memcpy(&sectSize, ptr, sizeof(sectSize));
      ptr += sizeof(sectSize);
      if (iterator + sectSize + sizeof(sectSize) > data.end()) { break; }
      auto str = std::string_view(ptr, 4U);
      ptr += std::size(str);
      std::memcpy(&compSize, ptr, sizeof(compSize));
      ptr += sizeof(compSize);
      return Compression::L4Z::Decompress<dstT>(ptr, sectSize - 8, fi.UncompressedSize());
      // return std::vector<char>();
    }
    return {};
  }
};
}// namespace OpenVIII::Archive
#endif// !OPENVIII_FS_H
