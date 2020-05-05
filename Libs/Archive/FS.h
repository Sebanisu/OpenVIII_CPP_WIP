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


  [[nodiscard]] static auto
    GetEntry(const std::filesystem::path &path, const OpenVIII::Archive::FI &fi, const size_t &offset)
  {
    if (fi.UncompressedSize() == 0) { return std::vector<char>(); }
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

    std::vector<char> buffer;
    unsigned int compSize{ 0 };
    switch (fi.CompressionType()) {
    case TCompressionType::None:
      buffer = Tools::ReadBuffer(fp, fi.UncompressedSize());
      fp.close();
      return buffer;
    case TCompressionType::LZSS:
      Tools::ReadVal(fp, compSize);
      buffer = Tools::ReadBuffer(fp, compSize);
      fp.close();
      return Compression::LZSS::Decompress(buffer, fi.UncompressedSize());
    case TCompressionType::LZ4:
      // todo add support for LZ4
      fp.close();
      return std::vector<char>();
    }
    return std::vector<char>();
  }
  [[nodiscard]] static auto
    GetEntry(const std::vector<char> &data, const OpenVIII::Archive::FI &fi, const size_t &offset)
  {
    if (fi.UncompressedSize() == 0) { return std::vector<char>(); }

    std::vector<char> buffer;
    const auto &iterator = data.begin() + fi.Offset() + static_cast<long>(offset);
    const auto &ptr = data.data() + fi.Offset() + static_cast<long>(offset);
    // ptr and iterator.base() would be the same. sadly msvc doesn't have a .base()
    // todo usages of memcpy can be replaced with std::bitcast in cpp20
    unsigned int compSize{ 0 };
    switch (fi.CompressionType()) {
    case TCompressionType::None:
      if (iterator + fi.UncompressedSize() > data.end()) { break; }
      buffer = std::vector<char>(fi.UncompressedSize());
      std::copy(iterator, iterator + fi.UncompressedSize(), buffer.begin());
      return buffer;
    case TCompressionType::LZSS:
      if (iterator + sizeof(compSize) > data.end()) { break; }
      std::memcpy(&compSize, ptr, sizeof(compSize));
      if (iterator + compSize > data.end()) { break; }
      buffer = std::vector<char>(compSize);
      std::copy(iterator + sizeof(compSize), iterator + sizeof(compSize) + compSize, buffer.begin());
      return Compression::LZSS::Decompress(buffer, fi.UncompressedSize());
    case TCompressionType::LZ4:
      // todo add support for LZ4
      return Compression::L4Z::Decompress(data.data()+offset,data.size()-offset,fi.UncompressedSize());
      //return std::vector<char>();
    }
    return std::vector<char>();
  }
};
}// namespace OpenVIII::Archive
#endif// !OPENVIII_FS_H
