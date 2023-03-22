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
namespace open_viii::graphics {
/**
 * @brief Base header for the following raw 16-bit color image data.
 */
struct TimImageHeader
{
private:
  std::uint32_t            m_size{};
  Rectangle<std::uint16_t> m_rectangle{};

public:
  /**
   * @brief Default constructor.
   */
  constexpr TimImageHeader() = default;

  /**
   * @brief Constructor with size and rectangle parameters.
   * @param in_size The total header and data size in bytes.
   * @param in_rect The dimensions of the data.
   */
  constexpr TimImageHeader(
    std::uint32_t            in_size,
    Rectangle<std::uint16_t> in_rect)
    : m_size(in_size), m_rectangle(in_rect)
  {}

  /**
   * @brief Default three-way comparison.
   */
  constexpr auto
    operator<=>(const TimImageHeader &) const
    = default;

  /**
   * @brief Get the total header and data size in bytes.
   * @return The value of m_size.
   */
  [[nodiscard]] constexpr auto
    size() const
  {
    return m_size;
  }

  /**
   * @brief Get the dimensions of the data.
   * @return The value of m_rectangle.
   */
  [[nodiscard]] constexpr auto
    rectangle() const
  {
    return m_rectangle;
  }

  /**
   * @brief Get the total size of the color lookup table data without the
   * header.
   * @return The size in bytes.
   */
  [[nodiscard]] constexpr auto
    data_size() const
  {
    return m_size - sizeof(TimImageHeader);
  };

  /**
   * @brief Check whether the width and height of the rectangle are greater than
   * 0.
   * @return true if the check passes.
   */
  [[nodiscard]] constexpr bool
    check() const
  {
    return rectangle().width() > 0 && rectangle().height() > 0;
  }

  /**
   * @brief Implicit conversion to bool, calls check() method.
   * @return true if Check test passes.
   */
  [[nodiscard]] explicit constexpr operator bool() const
  {
    return check();
  }
};

/**
 * @brief Overloaded ostream operator for TimImageHeader.
 * @param os Output stream.
 * @param input TimImageHeader instance to output.
 * @return Modified output stream.
 */
inline std::ostream &
  operator<<(std::ostream &os, const TimImageHeader &input)
{
  return os << '{' << input.size() << " bytes, " << input.rectangle() << '}';
}

}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TIMIMAGEHEADER_HPP
