//
// Created by pcvii on 4/27/2020.
//

#include "../External/lz4/lib/lz4.h"
#include <vector>
#include <cstring>
#include "L4Z.hpp"
auto OpenVIII::Compression::L4Z::Decompress(const std::vector<char> &data, const size_t &size)
{
  constexpr const auto minSize = 8U;
  constexpr const auto skipSize = 4U;
  constexpr const auto capacityInterval = 1000U;

  auto result = std::vector<char>();
  if (data.empty() || data.size() <= minSize) { return result; }

  unsigned int capacity{};
  memcpy(&capacity, data.data() + skipSize, sizeof(capacity));
  if (size > 0) {
    result.reserve(size);
  } else {
    result.reserve(static_cast<unsigned long>(capacity) * capacityInterval);
  }

  auto outSize = LZ4_decompress_safe(data.data() + minSize,
    result.data(),
    static_cast<int>(data.size() - minSize),
    static_cast<int>(capacity * capacityInterval));
  if (outSize < 0) { return std::vector<char>(); }
  result.resize(static_cast<unsigned long>(outSize));
  return result;
}
