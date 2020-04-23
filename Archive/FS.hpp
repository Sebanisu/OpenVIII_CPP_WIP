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
#include "../Compression/LZSS.hpp"

namespace OpenVIII::Archive {
struct FS
{
private:
  template<typename T, typename T2, typename T3> static void cpy(const T &start, const T2 &dst, const T3 &length)
  {
    std::copy(start, start + static_cast<long>(length), dst);
  }

public:
  static constexpr const auto Ext = std::string_view(".FS");


  [[nodiscard]] static auto
    GetEntry(const std::filesystem::path &path, const OpenVIII::Archive::FI &fi, const size_t &offset)
  {
    if (fi.UncompressedSize() == 0) { return std::vector<unsigned char>(); }
    std::ifstream fp = std::ifstream(path, std::ios::in | std::ios::binary);
    // if compressed will keep decompressing till get size
    // size compressed isn't quite known with out finding the offset of the next file and finding difference.
    fp.seekg(static_cast<long>(offset + fi.Offset()), std::ios::beg);
    static const auto readBuffer = [&fp](const auto &s) {
      auto buf = std::vector<unsigned char>(s);
      auto tmp = std::vector<char>(s);
      fp.read(tmp.data(), s);// TODO change to bitcast in cpp 20 or read another way?
      memcpy(buf.data(), tmp.data(), s);// memcpy will be optimized away. It is safer than casting.
      return buf;
    };

    static const auto readVal = [&fp](auto &v) {
      auto tmp = std::array<char, sizeof(v)>();
      fp.read(tmp.data(), sizeof(v));
      memcpy(&v, tmp.data(), sizeof(v));// memcpy will be optimized away. It is safer than casting.
      // TODO change to bitcast in cpp 20 or read another way?
    };

    std::vector<unsigned char> buffer;
    unsigned int compSize{ 0 };
    switch (fi.CompressionType()) {
    case TCompressionType::None:
      buffer = readBuffer(fi.UncompressedSize());
      fp.close();
      return buffer;
    case TCompressionType::LZSS:
      readVal(compSize);
      buffer = readBuffer(compSize);
      fp.close();
      return Compression::LZSS::Decompress(buffer, fi.UncompressedSize());
    case TCompressionType::LZ4:
      // todo add support for LZ4
      fp.close();
      return std::vector<unsigned char>();
    }
    return std::vector<unsigned char>();
  }
  [[nodiscard]] static auto
    GetEntry(const std::vector<unsigned char> &data, const OpenVIII::Archive::FI &fi, const size_t &offset)
  {
    if (fi.UncompressedSize() == 0) { return std::vector<unsigned char>(); }

    std::vector<unsigned char> buffer;
    const auto &iterator = data.begin() + fi.Offset() + static_cast<long>(offset);
    const auto &ptr = data.data() + fi.Offset() + static_cast<long>(offset);
    // ptr and iterator.base() would be the same. sadly msvc doesn't have a .base()
    // usages of memcpy can be replaced with std::bitcast in cpp20
    unsigned int compSize{ 0 };
    switch (fi.CompressionType()) {
    case TCompressionType::None:
      if (iterator + fi.UncompressedSize() > data.end()) { break; }
      buffer = std::vector<unsigned char>(fi.UncompressedSize());
      cpy(iterator, buffer.begin(), fi.UncompressedSize());
      return buffer;
    case TCompressionType::LZSS:
      if (iterator + sizeof(compSize) > data.end()) { break; }
      std::memcpy(&compSize, ptr, sizeof(compSize));
      if (iterator + compSize > data.end()) { break; }
      buffer = std::vector<unsigned char>(compSize);
      cpy(iterator + sizeof(compSize), buffer.begin(), compSize);
      return Compression::LZSS::Decompress(buffer, fi.UncompressedSize());
    case TCompressionType::LZ4:
      // todo add support for LZ4
      return std::vector<unsigned char>();
    }
    return std::vector<unsigned char>();
  }
};
}// namespace OpenVIII::Archive
#endif// !OPENVIII_FS_H
