//
// Created by pcvii on 4/27/2020.
//

#include "../External/lz4/lib/lz4.h"
#include <vector>
#include <cstring>
#include <string_view>
#include "L4Z.h"
std::vector<char> OpenVIII::Compression::L4Z::Decompress(const char *data, const size_t &dataSize, const size_t &size)
{
  auto result = std::vector<char>(size);
  auto outSize = LZ4_decompress_safe(data, result.data(), static_cast<int>(dataSize), static_cast<int>(result.size()));
  if (outSize <= 0) {
    return std::vector<char>();
  }
  if (static_cast<unsigned>(outSize) != size) {
    result.resize(static_cast<unsigned long>(outSize));
  }
  return result;
}
