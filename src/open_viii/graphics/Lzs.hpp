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
#ifndef VIIIARCHIVE_LZS_HPP
#define VIIIARCHIVE_LZS_HPP
#include "Png.hpp"
#include "Ppm.hpp"
#include "Rectangle.hpp"
#include "open_viii/compression/LZSS.hpp"
#include "open_viii/graphics/color/Color16.hpp"
#include <cstring>
#include <span>
#include <vector>
namespace open_viii::graphics {
/**
 * lzs images are LZSS images with a small header (X,Y,H,W) and 16bit colors.
 * @see https://github.com/myst6re/deling/blob/master/FF8Image.cpp#L30
 */
struct [[maybe_unused]] Lzs
{
private:
  Rectangle<std::uint16_t> m_rectangle{};
  std::vector<Color16<ColorLayoutT::ABGR>>     m_colors{};

public:
  [[maybe_unused]] explicit Lzs(std::span<const char> buffer)
  {
    {
      std::uint32_t comp_size{};
      size_t        sz32 = sizeof(std::uint32_t);
      if (sz32 > std::ranges::size(buffer)) {
        return;
      }
      std::memcpy(&comp_size, std::ranges::data(buffer), sz32);
      if (comp_size + sz32 != std::ranges::size(buffer)) {
        std::cout << "wrong size: " << comp_size << ", "
                  << std::ranges::size(buffer) << '\n';
        return;
      }
      buffer = buffer.subspan(sz32, comp_size);// skip the size value.
    }
    {
      auto uncompressed           = compression::LZSS::decompress(buffer);
      std::span<const char> adj   = uncompressed;
      size_t                szrec = sizeof(m_rectangle);
      if (szrec > std::ranges::size(adj)) {
        return;
      }
      std::memcpy(&m_rectangle, std::ranges::data(adj), szrec);
      std::cout << "size of uncompressed before: " << std::ranges::size(adj)
                << ", new size: ";
      adj = adj.subspan(szrec);
      std::cout << std::ranges::size(adj) << '\n';
      std::cout << m_rectangle << '\n';
      static constexpr size_t sz16 = sizeof(Color16<ColorLayoutT::ABGR>);
      std::cout << sz16 << '\n';
      const size_t max_bytes = std::ranges::size(adj) / sz16;
      const size_t area      = m_rectangle.area();
      size_t       min_size  = std::min(max_bytes, area) * sz16;
      if (min_size == 0) {
        m_rectangle = {};
        return;
      }
      m_colors.resize(min_size);
      std::cout << std::ranges::size(m_colors) << ", " << area << '\n';
      std::memcpy(std::ranges::data(m_colors),
                  std::ranges::data(adj),
                  min_size);
    }
  }
  [[maybe_unused]] void
    save(const std::string_view &filename) const
  {
    Ppm::save(m_colors, m_rectangle.width(), m_rectangle.height(), filename);
    Png::save(m_colors, m_rectangle.width(), m_rectangle.height(), filename,std::string(filename));
  }
  friend std::ostream &
    operator<<(std::ostream &os, const Lzs &l)
  {
    return os << '{' << l.m_rectangle << "}\n";
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_LZS_HPP
