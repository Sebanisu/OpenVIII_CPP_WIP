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

#ifndef VIIIARCHIVE_POINT_HPP
#define VIIIARCHIVE_POINT_HPP
#include "open_viii/Concepts.hpp"
#include <algorithm>
#include <concepts>
#include <iostream>
namespace open_viii::graphics {
template<Number dimT> struct Point
{
private:
  mutable dimT m_x{};
  mutable dimT m_y{};

public:
  Point() = default;
  //  Point(const Point<dimT> &) = default;
  //  Point(Point<dimT> &&) noexcept = default;
  //  Point<dimT>& operator=(const Point<dimT>&) = default;
  //  Point<dimT>& operator=(Point<dimT>&&) noexcept = default;
  //  virtual ~Point<dimT>() = delete;
  // friend auto operator<=>(const Point<dimT> &left, const Point<dimT> &right) noexcept = default;
  auto operator<=>(const Point<dimT> &right) const noexcept = default;
  template<typename T>
  requires((std::integral<T> || std::floating_point<T>)&&!std::is_same_v<T, dimT>) explicit Point(const Point<T> &r)
  {
    m_x = static_cast<dimT>(r.x());
    m_y = static_cast<dimT>(r.y());
  }
  Point(const dimT &in_x, const dimT &in_y) noexcept : m_x(in_x), m_y(in_y){};

  [[nodiscard]] auto abs()
  {
    if constexpr (std::signed_integral<dimT> || std::floating_point<dimT>) {
      return Point<dimT>(static_cast<dimT>(std::abs(m_x)), static_cast<dimT>(std::abs(m_y)));
    } else {
      return *this;
    }
  }
  /**
   * X coordinate.
   * @return x
   */
  [[nodiscard]] auto x() const noexcept
  {
    return m_x;
  }

  /**
   * Y coordinate.
   * @return y
   */
  [[nodiscard]] auto y() const noexcept
  {
    return m_y;
  }

  /**
   *
   * @param x is new X coordinate.
   * @return x
   */
  auto x(const dimT &x) const noexcept
  {
    return m_x = x;
  }

  /**
   *
   * @param y is new Y coordinate.
   * @return y
   */
  auto y(const dimT &y) const noexcept
  {
    return m_y = y;
  }
  /**
   * assuming x and y are width and height
   * @return area;
   */
  auto area() const noexcept
  {
    if constexpr (std::unsigned_integral<dimT>) {
      return static_cast<std::size_t>(m_x) * static_cast<std::size_t>(m_y);
    } else if constexpr (std::signed_integral<dimT>) {
      return static_cast<std::intmax_t>(m_x) * static_cast<std::intmax_t>(m_y);
    } else if constexpr (std::floating_point<dimT>) {
      return m_x * m_y;
    }
  }
  Point<dimT> operator/(const Point<dimT> &input) const noexcept
  {
    return { (input.m_x != static_cast<dimT>(0) ? static_cast<dimT>(m_x / input.m_x) : static_cast<dimT>(0)),
      (input.m_y != static_cast<dimT>(0) ? static_cast<dimT>(m_y / input.m_y) : static_cast<dimT>(0)) };
  }
  Point<dimT> operator*(const Point<dimT> &input) const noexcept
  {
    return { static_cast<dimT>(m_x * input.m_x), static_cast<dimT>(m_y * input.m_y) };
  }

  Point<dimT> operator-(const Point<dimT> &input) const noexcept
  {
    return { static_cast<dimT>(m_x - input.m_x), static_cast<dimT>(m_y - input.m_y) };
  }
  Point<dimT> operator+(const Point<dimT> &input) const noexcept
  {
    return { static_cast<dimT>(m_x + input.m_x), static_cast<dimT>(m_y + input.m_y) };
  }
  friend std::ostream &operator<<(std::ostream &os, const Point<dimT> &input)
  {
    return os << '{' << input.m_x << ", " << input.m_y << '}';
  }
};

/**
 * compares two points for max.
 * @param lhs another point
 * @return the max
 */
template<Number dimT> static Point<dimT> max(const Point<dimT> &rhs, const Point<dimT> &lhs) noexcept
{
  return { std::max(rhs.x(), lhs.x()), std::max(rhs.y(), lhs.y()) };
}

/**
 * compares two points for min.
 * @param lhs another point
 * @return the min
 */
template<Number dimT> static Point<dimT> min(const Point<dimT> &rhs, const Point<dimT> &lhs) noexcept
{
  return { std::min(rhs.x(), lhs.x()), std::min(rhs.y(), lhs.y()) };
}
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_POINT_HPP
