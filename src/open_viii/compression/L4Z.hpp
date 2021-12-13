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
#ifndef VIIICOMPRESSION_L4Z_H
#define VIIICOMPRESSION_L4Z_H
#include "lib/lz4.h"
#include "open_viii/Concepts.hpp"
#include <span>
namespace open_viii::compression::l4z {
/**
 * Extract char buffer using LZ4 decompressor.
 * @tparam dstT returning type.
 * @param src_data pointer to source char buffer.
 * @param src_size (cast to int) size of src data.
 * @param dst_size (cast to int) size of dst data.
 * @return uncompressed char buffer.
 */
template<
  typename dstT = std::vector<char>,
  std::integral srcSizeT,
  std::integral dstSizeT>
[[nodiscard]] [[maybe_unused]] static dstT
  decompress(
    const char     *src_data,
    const srcSizeT &src_size,
    const dstSizeT &dst_size)
{
  if (src_size < 0 || dst_size < 0) {
    return {};
  }
  dstT dst{};
  dst.resize(static_cast<std::size_t>(dst_size));
  auto outSize = LZ4_decompress_safe(
    src_data,
    std::ranges::data(dst),
    static_cast<int>(src_size),
    static_cast<int>(std::ranges::size(dst)));
  if (outSize <= 0) {
    return {};
  }
  if (static_cast<unsigned>(outSize) != dst_size) {
    dst.resize(static_cast<unsigned long>(outSize));
  }
  return dst;
}
/**
 * Extract char buffer using LZ4 decompressor.
 * @tparam dstT returning type.
 * @param src source char buffer
 * @param dst_size (cast to int) size of dst data.
 * @return uncompressed char buffer.
 */
template<typename dstT = std::vector<char>, std::integral dstSizeT>
[[nodiscard]] [[maybe_unused]] static dstT
  decompress(const std::span<const char> &src, const dstSizeT &dst_size)
{
  return decompress<dstT>(
    std::ranges::data(src),
    std::ranges::size(src),
    dst_size);
}
template<typename dstT = std::vector<char>>
static dstT
  compress(const std::span<const char> &src)
{
  dstT       dst{};
  const auto srcSize = static_cast<int>(std::ranges::size(src));
  const auto dstSize = LZ4_compressBound(srcSize);
  dst.resize(static_cast<std::size_t>(dstSize));
  const auto size_wrote = LZ4_compress_default(
    std::ranges::data(src),
    std::ranges::data(dst),
    srcSize,
    dstSize);
  if (size_wrote <= 0) {
    return {};
  }
  dst.resize(static_cast<std::size_t>(size_wrote));
  return dst;
}
}// namespace open_viii::compression::l4z
#endif// VIIICOMPRESSION_L4Z_H
