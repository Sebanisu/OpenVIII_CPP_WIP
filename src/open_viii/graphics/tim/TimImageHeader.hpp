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
#include <compare>
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
  constexpr TimImageHeader() = default;
  constexpr TimImageHeader(std::uint32_t            in_size,
                           Rectangle<std::uint16_t> in_rect)
    : m_size(in_size), m_rectangle(in_rect)
  {}
  constexpr auto operator<=>(const TimImageHeader &) const = default;
  /**
   * Total Header and Data size
   * @brief size in bytes
   */
  [[nodiscard]] constexpr auto size() const
  {
    return m_size;
  }
  /**
   * Dimensions of data.
   */
  [[nodiscard]] constexpr auto rectangle() const
  {
    return m_rectangle;
  }
  /**
   * Total size of Color Lookup Table data without header.
   * @brief Size in bytes.
   */
  [[nodiscard]] constexpr auto data_size() const
  {
    return m_size - sizeof(TimImageHeader);
  };
  [[nodiscard]] constexpr bool check() const
  {
    return rectangle().width() > 0
           && rectangle().height() > 0;
  }
  [[nodiscard]] explicit constexpr operator bool() const
  {
    return check();
  }
  friend std::ostream &operator<<(std::ostream &os, const TimImageHeader &input)
  {
    return os << '{' << input.size() << " bytes, " << input.m_rectangle << '}';
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TIMIMAGEHEADER_HPP
