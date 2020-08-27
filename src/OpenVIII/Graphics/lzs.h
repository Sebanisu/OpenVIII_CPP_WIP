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
 */
struct [[maybe_unused]] lzs
{
private:
  rectangle<std::uint16_t> rectangle_{};
  std::vector<color16> colors{};

public:
  [[maybe_unused]] explicit lzs(std::span<const char> buffer)
  {
    auto uncompressed = Compression::LZSS::Decompress(buffer);
    std::span<const char> adj = uncompressed;
    std::memcpy(&rectangle_, std::ranges::data(adj), sizeof(rectangle_));
    adj = adj.subspan(
      sizeof(rectangle_), static_cast<std::size_t>(rectangle_.Width()) * static_cast<std::size_t>(rectangle_.Height()));
    colors.resize(std::ranges::size(adj));
    std::memcpy(std::ranges::data(colors), std::ranges::data(adj), std::ranges::size(adj));
  }
  [[maybe_unused]] void Save(std::string_view filename)
  {
    ppm::save(colors, rectangle_.Width(), rectangle_.Height(), filename);
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_LZS_H
