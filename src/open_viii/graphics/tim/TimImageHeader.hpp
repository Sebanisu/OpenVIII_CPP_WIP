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
#ifndef VIIIARCHIVE_TIMIMAGEHEADER_HPP
#define VIIIARCHIVE_TIMIMAGEHEADER_HPP
#include "open_viii/graphics/Rectangle.hpp"
#include <cstdint>
namespace open_viii::graphics {
/**
 * Base Header for the following raw 16bit color image data.
 */
struct TimImageHeader
{
private:
  std::uint32_t            m_size{};
  Rectangle<std::uint16_t> m_rectangle{};

public:
  /**
   * Total Header and Data size
   * @brief size in bytes
   */
  auto size() const
  {
    return m_size;
  }
  /**
   * Dimensions of data.
   */
  auto rectangle() const
  {
    return m_rectangle;
  }
  /**
   * Total size of Color Lookup Table data without header.
   * @brief Size in bytes.
   */
  [[nodiscard]] auto data_size() const
  {
    return m_size - sizeof(TimImageHeader);
  };
  friend std::ostream &operator<<(std::ostream &os, const TimImageHeader &input)
  {
    return os << '{' << input.size() << " bytes, " << input.m_rectangle << '}';
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TIMIMAGEHEADER_HPP
