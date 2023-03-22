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
namespace open_viii::graphics {
/**
 * @struct open_viii::graphics::timClutHeader
 * @ingroup graphics
 * @brief Color Lookup Table (CLUT) Header
 */
struct TimClutHeader
{
private:
  /**
   * @brief X value must be divisible by 4.
   */
  static constexpr auto                        XDIVISABLE_BY{ 4U };

  /**
   * @brief Maximum Y value is 511.
   */
  static constexpr auto                        MAX_Y{ 511U };

  /**
   * @brief Valid width values; some non-standard width values exist, so this
   * isn't used. 4-bit can only read up to 16 values and 8-bit can only read up
   * to 256 values.
   */
  [[maybe_unused]] static constexpr std::array VALID_WIDTH = { 16U, 256U };

  TimImageHeader                               m_image_header{};

public:
  /**
   * @brief Default constructor.
   */
  constexpr TimClutHeader() = default;

  /**
   * @brief Constructor with TimImageHeader parameter.
   * @param in_image_header The TimImageHeader instance.
   */
  explicit constexpr TimClutHeader(TimImageHeader in_image_header)
    : m_image_header(in_image_header)
  {}

  /**
   * @brief Constructor with size and rectangle parameters.
   * @param in_size The total header and data size in bytes.
   * @param in_rect The dimensions of the data.
   */
  constexpr TimClutHeader(
    std::uint32_t            in_size,
    Rectangle<std::uint16_t> in_rect)
    : TimClutHeader(TimImageHeader(in_size, in_rect))
  {}

  /**
   * @brief Default three-way comparison.
   */
  constexpr auto
    operator<=>(const TimClutHeader &) const
    = default;

  /**
   * @brief Get the dimensions of the color lookup table.
   *        Typically, the width = number of colors, and height = number of
   * color lookup tables. Sometimes, if there are only 16 colors (4bpp), there
   * are multiple groups of 16 in the table.
   * @return The dimensions of the color lookup table.
   */
  [[nodiscard]] constexpr auto
    rectangle() const
  {
    return m_image_header.rectangle();
  }

  /**
   * @brief Get the total size of the color lookup table including the header.
   * @return The size in bytes.
   */
  [[nodiscard]] constexpr auto
    size() const
  {
    return m_image_header.size();
  };

  /**
   * @brief Get the total size of the color lookup table data without the
   * header.
   * @return The size in bytes.
   */
  [[nodiscard]] constexpr auto
    data_size() const
  {
    return m_image_header.data_size();
  }

  /**
   * @brief Check if the X and Y of the rectangle are valid values.
   *        Width is usually the number of colors and should be 16 or 256.
   * @return true if valid.
   */
  [[nodiscard]] constexpr bool
    check() const
  {
    return m_image_header.rectangle().x() % XDIVISABLE_BY == 0
        && m_image_header.rectangle().y() <= MAX_Y && m_image_header.check();
  }

  /**
   * @brief Implicit conversion to bool, calls check() method.
   * @return true if check() passes.
   */
  [[nodiscard]] explicit constexpr operator bool() const
  {
    return check();
  }
  /**
   * @brief Overloaded ostream insertion operator to output TimClutHeader
   * information.
   * @param os The output stream to insert the TimClutHeader information into.
   * @param input The TimClutHeader instance to be output.
   * @return A reference to the output stream with the TimClutHeader information
   * inserted.
   */
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
