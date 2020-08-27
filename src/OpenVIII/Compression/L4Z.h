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

#include <vector>
#include "lz4/lib/lz4.h"

#ifndef VIIICOMPRESSION_L4Z_H
#define VIIICOMPRESSION_L4Z_H
namespace OpenVIII::Compression {
struct L4Z
{
  template<typename dstT = std::vector<char>>
  [[nodiscard]] [[maybe_unused]] static dstT
    Decompress(const char *srcData, const size_t &srcSize, const size_t &dstSize = 0)
  {
    dstT dst{};
    dst.resize(dstSize);
    auto outSize = LZ4_decompress_safe(srcData, dst.data(), static_cast<int>(srcSize), static_cast<int>(dst.size()));
    if (outSize <= 0) {
      return {};
    }
    if (static_cast<unsigned>(outSize) != dstSize) {
      dst.resize(static_cast<unsigned long>(outSize));
    }
    return dst;
  }

  template<typename dstT = std::vector<char>, std::ranges::range srcT = std::vector<char>>
  [[nodiscard]] [[maybe_unused]] static dstT Decompress(const srcT &src, const size_t &dstSize = 0)
  {// todo replace with std::span.
    return Decompress<dstT>(std::ranges::data(src), std::ranges::size(src), dstSize);
  }
};
}// namespace OpenVIII::Compression
#endif// VIIICOMPRESSION_L4Z_H
