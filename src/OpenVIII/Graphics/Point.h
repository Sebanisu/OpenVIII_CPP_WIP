//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_POINT_H
#define VIIIARCHIVE_POINT_H
#include <iostream>
#include <concepts>
namespace open_viii::graphics {
template<std::integral dimT>struct Point
{
private:
  mutable dimT m_x{};
  mutable dimT m_y{};

public:
  /**
   * X coordinate.
   * @return x
   */
  [[nodiscard]] auto x() const { return m_x; }

  /**
   * Y coordinate.
   * @return y
   */
  [[nodiscard]] auto y() const { return m_y; }

  /**
   *
   * @param x is new X coordinate.
   * @return x
   */
  [[nodiscard]] auto x(const dimT &x) const { return m_x = x; }

  /**
   *
   * @param y is new Y coordinate.
   * @return y
   */
  [[nodiscard]] auto y(const dimT &y) const { return m_y = y; }


  friend std::ostream &operator<<(std::ostream &os, const Point<dimT> &input)
  {
    return os << '{' << input.m_x << ", " << input.m_y << '}';
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_POINT_H
