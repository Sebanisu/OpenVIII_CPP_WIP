//
// Created by pcvii on 4/27/2020.
//

#include "../External/lz4/lib/lz4.h"
#include <vector>
#include <cstring>
#include <string_view>
#include "L4Z.h"
std::vector<char> OpenVIII::Compression::L4Z::Decompress(const char * data,const size_t & dataSize, const size_t &size)
{
  //constexpr const auto minSize = 8U;
  //constexpr const auto skipSize = 4U;
  //constexpr const auto capacityInterval = 1000U;
auto test = std::string_view(data,dataSize);
  //if (data.empty() || data <= minSize) { return result; }
  //if(dataSize<minSize) { return std::vector<char>(); }
  //unsigned int capacity{};
  //memcpy(&capacity, data + skipSize, sizeof(capacity));
  //size_t s;
//  if (size > 0) {
    //s= size;
//  } else {
//    s = static_cast<unsigned long>(capacity) * capacityInterval;
//  }
  auto result = std::vector<char>(size);
  auto outSize = LZ4_decompress_safe(data,result.data()+12, static_cast<int>(dataSize-12), static_cast<int>(result.size()));
  if (outSize < 0) {
    return std::vector<char>();
  }
  result.resize(static_cast<unsigned long>(outSize));
  return result;
}
