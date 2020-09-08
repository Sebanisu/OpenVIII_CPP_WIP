//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_POINT_H
#define VIIIARCHIVE_POINT_H
#include <iostream>
#include <concepts>
namespace open_viii::graphics {
template<std::integral dimT> struct Point
{
private:
  mutable dimT m_x{};
  mutable dimT m_y{};

public:
  Point() = default;
  Point(const dimT &in_x, const dimT &in_y) noexcept : m_x(in_x), m_y(in_y){};
  friend auto operator==(const Point<dimT> &left, const Point<dimT> &right) noexcept
  {
    return left.m_x == right.m_x && left.m_y == right.m_y;
  }
  friend auto operator<(const Point<dimT> &left, const Point<dimT> &right) noexcept
  {
    return left.m_x < right.m_x && left.m_y < right.m_y;
  }
  friend auto operator>(const Point<dimT> &left, const Point<dimT> &right) noexcept
  {
    return left.m_x > right.m_x && left.m_y > right.m_y;
  }
  /**
   * X coordinate.
   * @return x
   */
  [[nodiscard]] const auto &x() const noexcept { return m_x; }

  /**
   * Y coordinate.
   * @return y
   */
  [[nodiscard]] const auto &y() const noexcept { return m_y; }

  /**
   *
   * @param x is new X coordinate.
   * @return x
   */
  [[nodiscard]] const auto &x(const dimT &x) const noexcept { return m_x = x; }

  /**
   *
   * @param y is new Y coordinate.
   * @return y
   */
  [[nodiscard]] const auto &y(const dimT &y) const noexcept { return m_y = y; }


  friend std::ostream &operator<<(std::ostream &os, const Point<dimT> &input)
  {
    return os << '{' << input.m_x << ", " << input.m_y << '}';
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_POINT_H
