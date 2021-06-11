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
#ifndef VIIIARCHIVE_TIMCLUTHEADER_HPP
#define VIIIARCHIVE_TIMCLUTHEADER_HPP
#include "TimImageHeader.hpp"
#include <compare>
namespace open_viii::graphics {
/**
 * @struct open_viii::graphics::timClutHeader
 * @ingroup graphics
 * @brief Color Lookup Table Header
 */
struct TimClutHeader
{
private:
  /**
   * X value must be divisible by 4, the spec says 16 this but theres some
   * values that aren't standard.
   * @brief X value must be divisible by 4;
   */
  static constexpr auto                        XDIVISABLE_BY{ 4U };
  /**
   * @brief Y max value is 511
   */
  static constexpr auto                        MAX_Y{ 511U };
  /**
   * 4 bit can only read up to 16 values and 8 bit can only read up to 256
   * values. But There are larger other sizes. The game uses
   * @brief Valid width values; some non-standard width values exist so this
   * isn't used.
   */
  [[maybe_unused]] static constexpr std::array VALID_WIDTH = { 16U, 256U };
  TimImageHeader                               m_image_header{};

public:
  constexpr TimClutHeader() = default;
  explicit constexpr TimClutHeader(TimImageHeader in_image_header)
    : m_image_header(in_image_header)
  {}
  constexpr TimClutHeader(std::uint32_t            in_size,
                          Rectangle<std::uint16_t> in_rect)
    : TimClutHeader(TimImageHeader(in_size, in_rect))
  {}
  constexpr auto
    operator<=>(const TimClutHeader &) const = default;
  /**
   * Typically the width = number of colors, and height = number of color lookup
   * tables. Sometimes if there is only 16 colors (4bpp) there is multiple
   * groups of 16 in the table.
   * @brief Dimensions of the color lookup table.
   */
  [[nodiscard]] constexpr auto
    rectangle() const
  {
    return m_image_header.rectangle();
  }
  /**
   * Total size of Color Lookup Table including header.
   * @brief Size in bytes.
   */
  [[nodiscard]] constexpr auto
    size() const
  {
    return m_image_header.size();
  };
  /**
   * Total size of Color Lookup Table data without header.
   * @brief Size in bytes.
   */
  [[nodiscard]] constexpr auto
    data_size() const
  {
    return m_image_header.data_size();
  }
  /**
   * Test X and Y of rectangle to see if they are valid values.
   * Width is usually number of colors should be 16 or 256.
   * @return returns true if valid
   */
  [[nodiscard]] constexpr bool
    check() const
  {
    return m_image_header.rectangle().x() % XDIVISABLE_BY == 0
        && m_image_header.rectangle().y() <= MAX_Y && m_image_header.check();
  }
  [[nodiscard]] explicit constexpr operator bool() const
  {
    return check();
  }
  friend std::ostream &
    operator<<(std::ostream &os, const TimClutHeader &input)
  {
    return os << input.m_image_header
              << " {Colors:" << input.m_image_header.rectangle().width()
              << ", Tables: " << input.m_image_header.rectangle().height()
              << '}';
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TIMCLUTHEADER_HPP
