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
#include "open_viii/Concepts.hpp"
#include "open_viii/graphics/Point.hpp"
namespace open_viii::graphics {
/**
 * @struct open_viii::graphics::rectangle
 * @tparam dimT Number type.
 * @brief 2D Container that holds top left height and width.
 */
template<Number dimT>
struct Rectangle
{
private:
  Point<dimT> m_top_left{};
  Point<dimT> m_width_height{};

public:
  constexpr Rectangle() = default;
  constexpr auto
    operator<=>(const Rectangle<dimT> &right) const noexcept = default;
  constexpr Rectangle(const Point<dimT> &xy, const Point<dimT> &hw)
    : m_top_left(xy), m_width_height(hw)
  {}
  template<Number T>
  requires(!std::is_same_v<T, dimT>) constexpr explicit Rectangle(
    const Rectangle<T> &r)
    : Rectangle(
      static_cast<Point<dimT>>(r.top_left()),
      static_cast<Point<dimT>>(r.width_height()))
  {}
  constexpr Rectangle(
    const dimT &in_x,
    const dimT &in_y,
    const dimT &in_width,
    const dimT &in_height) noexcept
    : Rectangle(Point<dimT>(in_x, in_y), Point<dimT>(in_width, in_height))
  {}
  /**
   * @return Left coordinate.
   */
  [[maybe_unused]] [[nodiscard]] constexpr auto
    left() const noexcept
  {
    return x();
  }
  /**
   * @return Left coordinate.
   */
  [[nodiscard]] constexpr auto
    x() const noexcept
  {
    return m_top_left.x();
  }
  /**
   *
   * @param x is new Left coordinate.
   * @return Left coordinate.
   *
   */
  void
    x(const dimT &in_x) noexcept
  {
    m_top_left.x(in_x);
  }
  /**
   *
   * @param x is new Left coordinate.
   * @return Left coordinate.
   */
  void
    left(const dimT &in_x) const noexcept
  {
    x(in_x);
  }
  /**
   * @return Top coordinate.
   */
  [[maybe_unused]] [[nodiscard]] constexpr auto
    top() const noexcept
  {
    return y();
  }
  /**
   * @return Top coordinate.
   */
  [[nodiscard]] constexpr auto
    y() const noexcept
  {
    return m_top_left.y();
  }
  /**
   *
   * @param y is new Top coordinate.
   * @return Top coordinate.
   */
  void
    y(const dimT &in_y) noexcept
  {
    m_top_left.y(in_y);
  }
  /**
   *
   * @param y is new Top coordinate.
   * @return Top coordinate.
   */
  void
    top(const dimT &in_y) noexcept
  {
    y(in_y);
  }
  /**
   * @return Right coordinate.
   */
  [[nodiscard]] constexpr auto
    right() const noexcept
  {
    return m_top_left.x() + m_width_height.y();
  }
  /**
   * @return Set Right coordinate.
   */
  void
    right(const dimT right) noexcept
  {
    if (right >= m_top_left.x()) {
      m_width_height.X(m_top_left.x() - right);
    }
    m_width_height.X(right - m_top_left.x());
  }
  /**
   * @return Set Bottom coordinate.
   */
  void
    bottom(const dimT bottom) noexcept
  {
    if (bottom >= m_top_left.y()) {
      m_width_height.Y(m_top_left.y() - bottom);
    }
    m_width_height.Y(bottom - m_top_left.y());
  }
  /**
   * @return Bottom coordinate.
   */
  [[maybe_unused]] [[nodiscard]] constexpr auto
    bottom() const noexcept
  {
    return m_top_left.y() + m_width_height.y();
  }
  /**
   * @return Width.
   */
  [[nodiscard]] constexpr auto
    width() const noexcept
  {
    return m_width_height.x();
  }
  /**
   *
   * @param width is new Width value.
   * @return Width.
   */
  void
    width(const dimT &in_width) noexcept
  {
    return m_width_height.x(in_width);
  }
  /**
   * @return Height.
   */
  [[nodiscard]] constexpr auto
    height() const noexcept
  {
    return m_width_height.y();
  }
  /**
   *
   * @param height is new Height value.
   * @return Height.
   */
  void
    height(dimT in_height) const noexcept
  {
    m_width_height.y(in_height);
  }
  [[nodiscard]] constexpr auto
    area() const noexcept
  {
    return m_width_height.area();
  }
  constexpr Point<dimT>
    top_left() const noexcept
  {
    return m_top_left;
  }
  constexpr Point<dimT>
    width_height() const noexcept
  {
    return m_width_height;
  }
  constexpr auto
    operator*(const dimT &scaler) const
  {
    auto r = *this;
    r.m_top_left *= scaler;
    r.m_width_height *= scaler;
    return r;
  }
  constexpr auto
    operator+(const dimT &scaler) const
  {
    auto r = *this;
    r.m_top_left += scaler;
    r.m_width_height += scaler;
    return r;
  }
  constexpr auto
    operator-(const dimT &scaler) const
  {
    auto r = *this;
    r.m_top_left -= scaler;
    r.m_width_height -= scaler;
    return r;
  }
  constexpr auto
    operator/(const dimT &scaler) const
  {
    auto r = *this;
    r.m_top_left /= scaler;
    r.m_width_height /= scaler;
    return r;
  }

  constexpr auto
    operator*(const Rectangle<dimT> &scaler) const
  {
    auto r = *this;
    r.m_top_left *= scaler.m_top_left;
    r.m_width_height *= scaler.m_width_height;
    return r;
  }
  constexpr auto
    operator+(const Rectangle<dimT> &scaler) const
  {
    auto r = *this;
    r.m_top_left += scaler.m_top_left;
    r.m_width_height += scaler.m_width_height;
    return r;
  }
  constexpr auto
    operator-(const Rectangle<dimT> &scaler) const
  {
    auto r = *this;
    r.m_top_left -= scaler.m_top_left;
    r.m_width_height -= scaler.m_width_height;
    return r;
  }
  constexpr auto
    operator/(const Rectangle<dimT> &scaler) const
  {
    auto r = *this;
    r.m_top_left /= scaler.m_top_left;
    r.m_width_height /= scaler.m_width_height;
    return r;
  }
};
template<typename dimT>
inline std::ostream &
  operator<<(std::ostream &os, const Rectangle<dimT> &input)
{
  return os << "{(X, Y) = " << input.top_left()
            << ", (Width, Height) = " << input.width_height() << '}';
}
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_RECTANGLE_HPP
