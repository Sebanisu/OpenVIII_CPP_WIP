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
  std::vector<Color16>     m_colors{};

public:
  /**
   *
   * @param buffer
   */
  [[maybe_unused]] explicit Lzs(std::span<const char> buffer)
  {
    if (sizeof(std::uint32_t) > std::ranges::size(buffer)) {
      return;
    }
    std::vector<char> uncompressed = uncompress(buffer);
    buffer                         = uncompressed;
    if (sizeof(m_rectangle) > std::ranges::size(buffer)) {
      return;
    }
    m_rectangle = tools::read_val<Rectangle<std::uint16_t>>(buffer);
    buffer = buffer.subspan(sizeof(m_rectangle));

    static constexpr size_t sz16 = sizeof(Color16);
    std::cout << sz16 << '\n';
    const size_t max_bytes = std::ranges::size(buffer) / sz16;
    const size_t area      = m_rectangle.area();
    size_t       min_size  = std::min(max_bytes, area) * sz16;
    if (min_size == 0) {
      m_rectangle = {};
      return;
    }
    tools::read_val(buffer,m_colors,min_size);
  }
  std::vector<char> uncompress(std::span<const char> &buffer) const
  {
    uint32_t comp_size = tools::read_val<uint32_t>(buffer);
    buffer = buffer.subspan(sizeof(uint32_t), comp_size);// skip the size value.
    auto uncompressed = compression::LZSS::decompress(buffer);
    return uncompressed;
  }
  [[maybe_unused]] void save(const std::string_view &filename) const
  {
    Ppm::save(m_colors, m_rectangle.width(), m_rectangle.height(), filename);
  }
  friend std::ostream &operator<<(std::ostream &os, const Lzs &l)
  {
    return os << '{' << l.m_rectangle << "}\n";
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_LZS_HPP
