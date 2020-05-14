//
// Created by pcvii on 4/27/2020.
//

#include "../External/lz4/lib/lz4.h"
#include <vector>
#include <cstring>
#include <string_view>
#ifndef VIIICPPTEST_L4Z_H
#define VIIICPPTEST_L4Z_H
namespace OpenVIII::Compression {
struct L4Z
{
  template<typename dstT = std::vector<char>>
  [[nodiscard]] [[maybe_unused]] static dstT Decompress(const char *srcData,const size_t &srcSize, const size_t &dstSize = 0)
  {
    dstT dst{};
    dst.resize(dstSize);
    auto outSize = LZ4_decompress_safe(srcData, dst.data(), static_cast<int>(srcSize), static_cast<int>(dst.size()));
    if (outSize <= 0) {
      return {};
    }
    if (static_cast<unsigned>(outSize) != dstSize) { dst.resize(static_cast<unsigned long>(outSize));
    }
    return dst;
  }

  template<typename dstT = std::vector<char>, typename srcT = std::vector<char>>
  [[nodiscard]] [[maybe_unused]] static dstT Decompress(const srcT & src, const size_t &dstSize = 0)
  { //todo replace with std::span.
    Decompress<dstT>(src.data(),src.size(), dstSize);
  }
};
}// namespace OpenVIII::Compression
#endif// VIIICPPTEST_L4Z_H
