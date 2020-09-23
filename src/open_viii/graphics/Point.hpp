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
#include <concepts>
#include <iostream>
namespace open_viii::graphics {
template<typename dimT> requires(std::integral<dimT> || std::floating_point<dimT>) struct Point
{
private:
  mutable dimT m_x{};
  mutable dimT m_y{};

public:
  Point() = default;
  Point(const dimT &in_x, const dimT &in_y) noexcept : m_x(in_x), m_y(in_y){};
  friend auto operator<=>(const Point<dimT> &left, const Point<dimT> &right) noexcept = default;
  auto operator<=>(const Point<dimT> &right) const noexcept = default;

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
  [[nodiscard]] const auto &x() const noexcept
  {
    return m_x;
  }

  /**
   * Y coordinate.
   * @return y
   */
  [[nodiscard]] const auto &y() const noexcept
  {
    return m_y;
  }

  /**
   *
   * @param x is new X coordinate.
   * @return x
   */
  const auto &x(const dimT &x) const noexcept
  {
    return m_x = x;
  }

  /**
   *
   * @param y is new Y coordinate.
   * @return y
   */
  const auto &y(const dimT &y) const noexcept
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
    return { input.m_x != 0 ? m_x / input.m_x : 0, input.m_x != 0 ? m_y / input.m_y : 0 };
  }
  Point<dimT> operator*(const Point<dimT> &input) const noexcept
  {
    return { m_x * input.m_x, m_y * input.m_y };
  }
  friend std::ostream &operator<<(std::ostream &os, const Point<dimT> &input)
  {
    return os << '{' << input.m_x << ", " << input.m_y << '}';
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_POINT_HPP
