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

#ifndef VIIIARCHIVE_RECTANGLE_HPP
#define VIIIARCHIVE_RECTANGLE_HPP
#include "open_viii/graphics/Point.hpp"
#include <iostream>
namespace open_viii::graphics {
/**
 * @struct open_viii::graphics::rectangle
 * @tparam dimT Number type.
 * @brief 2D Container that holds top left height and width.
 */
template<typename dimT> requires(std::integral<dimT> || std::floating_point<dimT>) struct Rectangle
{
private:
  mutable Point<dimT> m_top_left{};
  mutable Point<dimT> m_width_height{};

public:
  Rectangle() = default;
  Rectangle(const dimT &in_x, const dimT &in_y, const dimT &in_width, const dimT &in_height) noexcept
    : m_top_left{ in_x, in_y }, m_width_height{ in_width, in_height }
  {}
  Rectangle(const Point<dimT> &xy, const Point<dimT> &hw) noexcept : m_top_left{ xy }, m_width_height{ hw } {}
  //  friend auto operator==(const Rectangle<dimT> &left, const Rectangle<dimT> &right) noexcept
  //  {
  //    return left.m_top_left == right.m_top_left && left.m_width_height == right.m_width_height;
  //  }
  friend auto operator<=>(const Rectangle<dimT> &left, const Rectangle<dimT> &right) noexcept = default;
  auto operator<=>(const Rectangle<dimT> &right) const noexcept = default;
  /**
   * @return Left coordinate.
   */
  [[maybe_unused]] [[nodiscard]] const auto &left() const noexcept { return m_top_left.x(); }
  /**
   * @return Left coordinate.
   */
  [[nodiscard]] const auto &x() const noexcept { return m_top_left.x(); }

  /**
   *
   * @param x is new Left coordinate.
   * @return Left coordinate.
   *
   */
  const auto &x(const dimT &x) const noexcept { return m_top_left.x(x); }
  /**
   *
   * @param x is new Left coordinate.
   * @return Left coordinate.
   */
  const auto &left(const dimT &x) const noexcept { return m_top_left.x(x); }
  /**
   * @return Top coordinate.
   */
  [[maybe_unused]] [[nodiscard]] const auto &top() const noexcept { return m_top_left.y(); }
  /**
   * @return Top coordinate.
   */
  [[nodiscard]] const auto &y() const noexcept { return m_top_left.y(); }
  /**
   *
   * @param y is new Top coordinate.
   * @return Top coordinate.
   */
  const auto &y(const dimT &y) const noexcept { return m_top_left.y(y); }

  /**
   * @return Right coordinate.
   */
  [[nodiscard]] auto right() const noexcept { return m_top_left.x() + m_width_height.y(); }

  /**
   * @return Set Right coordinate.
   */
  auto right(const dimT right) const noexcept
  {
    if (right >= m_top_left.x()) {
      return m_width_height.X(m_top_left.x() - right);
    }
    return m_width_height.X(right - m_top_left.x());
  }

  /**
   * @return Set Bottom coordinate.
   */
  [[maybe_unused]] auto bottom(const dimT bottom) const noexcept
  {
    if (bottom >= m_top_left.y()) {
      return m_width_height.Y(m_top_left.y() - bottom);
    }
    return m_width_height.Y(bottom - m_top_left.y());
  }
  /**
   * @return Bottom coordinate.
   */
  [[maybe_unused]] [[nodiscard]] auto bottom() const noexcept { return m_top_left.y() + m_width_height.y(); }
  /**
   * @return Width.
   */
  [[nodiscard]] const auto &width() const noexcept { return m_width_height.x(); }

  /**
   *
   * @param width is new Width value.
   * @return Width.
   */
  const auto &width(const dimT &width) const noexcept { return m_width_height.x(width); }
  /**
   * @return Height.
   */
  [[nodiscard]] const auto &height() const noexcept { return m_width_height.y(); }
  /**
   *
   * @param height is new Height value.
   * @return Height.
   */
  const auto &height(const dimT &height) const noexcept { return m_width_height.y(height); }

  [[nodiscard]] std::size_t area() const noexcept
  {
    return static_cast<std::size_t>(m_width_height.x()) * static_cast<std::size_t>(m_width_height.y());
  }
  friend std::ostream &operator<<(std::ostream &os, const Rectangle<dimT> &input)
  {
    return os << "{(X, Y) = " << input.m_top_left << ", (Width, Height) = " << input.m_width_height << '}';
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_RECTANGLE_HPP
