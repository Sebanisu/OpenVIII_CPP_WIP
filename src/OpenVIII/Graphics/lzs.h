//
// Created by pcvii on 8/26/2020.
//

#ifndef VIIIARCHIVE_LZS_H
#define VIIIARCHIVE_LZS_H
#include "OpenVIII/Graphics/color/color16.h"
#include "rectangle.h"
#include "OpenVIII/Compression/LZSS.h"
#include <span>
#include <cstring>
#include <vector>
#include "ppm.h"
namespace OpenVIII::Graphics {
/**
 * lzs images are LZSS images with a small header (X,Y,H,W) and 16bit colors.
 * @see https://github.com/myst6re/deling/blob/master/FF8Image.cpp#L30
 */
struct [[maybe_unused]] lzs
{
private:
  rectangle<std::uint16_t> rectangle_{};
  std::vector<color16> colors{};

public:
  [[maybe_unused]] explicit lzs(std::span<const char> buffer)
  {
    std::uint32_t compSize{};
    std::memcpy(&compSize, std::ranges::data(buffer), sizeof(std::uint32_t));
    if (compSize + sizeof(std::uint32_t) != std::ranges::size(buffer)) {
      std::cout << "wrong size: " << compSize << ", " << std::ranges::size(buffer) << '\n';
    }
    buffer = buffer.subspan(sizeof(std::uint32_t), compSize);// skip the size value.
    [[maybe_unused]] auto size = std::size(buffer);
    auto uncompressed = Compression::LZSS::Decompress(buffer);
    std::span<const char> adj = uncompressed;
    std::memcpy(&rectangle_, std::ranges::data(adj), sizeof(rectangle_));
    std::cout << "size of uncompressed before: " << std::ranges::size(adj) << ", new size: ";
    adj = adj.subspan(sizeof(rectangle_));// static_cast<std::size_t>(rectangle_.Width()) *
                                          // static_cast<std::size_t>(rectangle_.Height())*sizeof(color16)
    std::cout << std::ranges::size(adj) << '\n';
    std::cout << rectangle_ << '\n';
    std::cout << sizeof(color16) << '\n';
    colors.resize(rectangle_.Height() * rectangle_.Width());
    std::cout << std::ranges::size(colors) << ", " << std::ranges::size(adj) / sizeof(color16) << '\n';
    std::memcpy(std::ranges::data(colors),
      std::ranges::data(adj),
      std::min(std::ranges::size(colors) * sizeof(color16), std::ranges::size(adj)));
  }
  [[maybe_unused]] void Save(std::string_view filename) const
  {
    ppm::save(colors, rectangle_.Width(), rectangle_.Height(), filename);
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_LZS_H
