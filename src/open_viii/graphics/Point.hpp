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
#include <iostream>
#include <numeric>
namespace open_viii::graphics {
template<Number dimT>
struct Point
{
private:
  dimT m_x{};
  dimT m_y{};

public:
  using value_type  = dimT;
  constexpr Point() = default;
  constexpr Point(const dimT &in_x, const dimT &in_y) noexcept
    : m_x(in_x), m_y(in_y){};
  constexpr auto
    operator<=>(const Point<dimT> &right) const noexcept
    = default;
  template<typename T>
    requires(
      (std::integral<T> || std::floating_point<T>) && !std::is_same_v<T, dimT>)
  explicit Point(const Point<T> &r)
  {
    m_x = static_cast<dimT>(r.x());
    m_y = static_cast<dimT>(r.y());
  }
  [[nodiscard]] constexpr auto
    abs() const noexcept
  {
    if constexpr (std::signed_integral<dimT> || std::floating_point<dimT>) {
      return Point<dimT>(
        static_cast<dimT>(std::abs(m_x)),
        static_cast<dimT>(std::abs(m_y)));
    }
    else {
      return *this;
    }
  }
  /**
   * X coordinate.
   * @return x
   */
  [[nodiscard]] constexpr auto
    x() const noexcept
  {
    return m_x;
  }
  /**
   * Y coordinate.
   * @return y
   */
  [[nodiscard]] constexpr auto
    y() const noexcept
  {
    return m_y;
  }
  /**
   *
   * @param x is new X coordinate.
   * @return x
   */
  void
    x(const dimT &x) noexcept
  {
    m_x = x;
  }
  /**
   *
   * @param y is new Y coordinate.
   * @return y
   */
  void
    y(const dimT &y) noexcept
  {
    m_y = y;
  }
  /**
   * assuming x and y are width and height
   * @return area;
   */
  [[nodiscard]] constexpr auto
    area() const noexcept
  {
    if constexpr (std::unsigned_integral<dimT>) {
      return static_cast<std::size_t>(m_x) * static_cast<std::size_t>(m_y);
    }
    else if constexpr (std::signed_integral<dimT>) {
      return static_cast<std::intmax_t>(m_x) * static_cast<std::intmax_t>(m_y);
    }
    else if constexpr (std::floating_point<dimT>) {
      return m_x * m_y;
    }
  }
  constexpr Point<dimT>
    operator/=(const Point<dimT> &input) noexcept
  {
    m_x
      = (input.m_x != dimT{ 0 } ? static_cast<dimT>(m_x / input.m_x) : static_cast<dimT>(0));
    m_y
      = (input.m_y != dimT{ 0 } ? static_cast<dimT>(m_y / input.m_y) : static_cast<dimT>(0));
    return *this;
  }
  constexpr Point<dimT>
    operator*=(const Point<dimT> &input) noexcept
  {
    m_x = static_cast<dimT>(m_x * input.m_x);
    m_y = static_cast<dimT>(m_y * input.m_y);
    return *this;
  }
  constexpr Point<dimT>
    operator-=(const Point<dimT> &input) noexcept
  {
    m_x = static_cast<dimT>(m_x - input.m_x);
    m_y = static_cast<dimT>(m_y - input.m_y);
    return *this;
  }
  constexpr Point<dimT>
    operator+=(const Point<dimT> &input) noexcept
  {
    m_x = static_cast<dimT>(m_x + input.m_x);
    m_y = static_cast<dimT>(m_y + input.m_y);
    return *this;
  }

  constexpr Point<dimT> &
    operator/=(const dimT &input) noexcept
  {
    m_x
      = (input != dimT{ 0 } ? static_cast<dimT>(m_x / input) : static_cast<dimT>(0));
    m_y
      = (input != dimT{ 0 } ? static_cast<dimT>(m_y / input) : static_cast<dimT>(0));
    return *this;
  }
  constexpr Point<dimT> &
    operator*=(const dimT &input) noexcept
  {
    m_x = static_cast<dimT>(m_x * input);
    m_y = static_cast<dimT>(m_y * input);
    return *this;
  }
  constexpr Point<dimT> &
    operator-=(const dimT &input) noexcept
  {
    m_x = static_cast<dimT>(m_x - input);
    m_y = static_cast<dimT>(m_y - input);
    return *this;
  }
  constexpr Point<dimT> &
    operator+=(const dimT &input) noexcept
  {
    m_x = static_cast<dimT>(m_x + input);
    m_y = static_cast<dimT>(m_y + input);
    return *this;
  }

  [[nodiscard]] constexpr Point<dimT>
    operator/(const Point<dimT> &input) const noexcept
  {
    auto r = *this;
    return r /= input;
  }
  [[nodiscard]] constexpr Point<dimT>
    operator*(const Point<dimT> &input) const noexcept
  {
    auto r = *this;
    return r *= input;
  }
  [[nodiscard]] constexpr Point<dimT>
    operator-(const Point<dimT> &input) const noexcept
  {
    auto r = *this;
    return r -= input;
  }
  [[nodiscard]] constexpr Point<dimT>
    operator+(const Point<dimT> &input) const noexcept
  {
    auto r = *this;
    return r += input;
  }

  [[nodiscard]] constexpr Point<dimT>
    operator/(const dimT &input) const noexcept
  {
    auto r = *this;
    return r /= input;
  }
  [[nodiscard]] constexpr Point<dimT>
    operator*(const dimT &input) const noexcept
  {
    auto r = *this;
    return r *= input;
  }
  [[nodiscard]] constexpr Point<dimT>
    operator-(const dimT &input) const noexcept
  {
    auto r = *this;
    return r -= input;
  }
  [[nodiscard]] constexpr Point<dimT>
    operator+(const dimT &input) const noexcept
  {
    auto r = *this;
    return r += input;
  }

  [[nodiscard]] constexpr Point<dimT>
    with_x(dimT in_x) const noexcept
  {
    return { in_x, m_y };
  }
  [[nodiscard]] constexpr Point<dimT>
    with_y(dimT in_y) const noexcept
  {
    return { m_x, in_y };
  }

  template<typename T>
  [[nodiscard]] constexpr auto
    midpoint(const Point<T> other) const noexcept
  {
    return Point(std::midpoint(m_x, other.x()), std::midpoint(m_y, other.y()));
  }
};
/**
 * compares two points for max.
 * @param lhs another point
 * @return the max
 */
template<Number dimT>
[[nodiscard]] constexpr inline Point<dimT>(
  max)(const Point<dimT> &rhs, const Point<dimT> &lhs) noexcept
{
  return { (std::max)(rhs.x(), lhs.x()), (std::max)(rhs.y(), lhs.y()) };
}
/**
 * compares two points for min.
 * @param lhs another point
 * @return the min
 */
template<Number dimT>
[[nodiscard]] constexpr inline Point<dimT>(
  min)(const Point<dimT> &rhs, const Point<dimT> &lhs) noexcept
{
  return { (std::min)(rhs.x(), lhs.x()), (std::min)(rhs.y(), lhs.y()) };
}
template<typename dimT>
inline std::ostream &
  operator<<(std::ostream &os, const Point<dimT> &input)
{
  return os << '{' << +input.x() << ", " << +input.y() << '}';
}
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_POINT_HPP