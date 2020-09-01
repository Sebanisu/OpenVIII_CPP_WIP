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
    {
      std::uint32_t compSize{};
      size_t sz32 = sizeof(std::uint32_t);
      if (sz32 > std::ranges::size(buffer)) {
        return;
      }
      std::memcpy(&compSize, std::ranges::data(buffer), sz32);
      if (compSize + sz32 != std::ranges::size(buffer)) {
        std::cout << "wrong size: " << compSize << ", " << std::ranges::size(buffer) << '\n';
        return;
      }
      buffer = buffer.subspan(sz32, compSize);// skip the size value.
    }
    {
      auto uncompressed = Compression::LZSS::Decompress(buffer);
      std::span<const char> adj = uncompressed;
      size_t szrec = sizeof(rectangle_);
      if (szrec > std::ranges::size(adj)) {
        return;
      }
      std::memcpy(&rectangle_, std::ranges::data(adj), szrec);
      std::cout << "size of uncompressed before: " << std::ranges::size(adj) << ", new size: ";
      adj = adj.subspan(szrec);
      std::cout << std::ranges::size(adj) << '\n';
      std::cout << rectangle_ << '\n';
      std::cout << sizeof(color16) << '\n';
      const size_t maxbytes = std::ranges::size(adj) / sizeof(color16);
      const size_t area = static_cast<size_t>(rectangle_.Height()) * static_cast<size_t>(rectangle_.Width());
      size_t minSize = std::min(maxbytes, area) * sizeof(color16);
      if (minSize == 0) {
        rectangle_ = {};
        return;
      }
      colors.resize(minSize);
      std::cout << std::ranges::size(colors) << ", " << area << '\n';
      std::memcpy(std::ranges::data(colors), std::ranges::data(adj), minSize);
    }
  }
  [[maybe_unused]] void Save(const std::string_view &filename) const
  {
    ppm::save(colors, rectangle_.Width(), rectangle_.Height(), filename);
  }
  friend std::ostream &operator<<(std::ostream &os, const lzs &l) { return os << '{' << l.rectangle_ << "}\n"; }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_LZS_H
