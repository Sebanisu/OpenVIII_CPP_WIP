//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_RECTANGLE_H
#define VIIIARCHIVE_RECTANGLE_H
#include "OpenVIII/Graphics/Point.h"
#include <iostream>
namespace open_viii::graphics {
/**
 * @struct open_viii::graphics::rectangle
 * @tparam dimT Number type.
 * @brief 2D Container that holds top left height and width.
 */
template<typename dimT> struct Rectangle
{
private:
  mutable Point<dimT> m_top_left{};
  mutable Point<dimT> m_width_height{};

public:
  /**
   * @return Left coordinate.
   */
  [[maybe_unused]] [[nodiscard]] auto left() const { return m_top_left.x(); }
  /**
   * @return Left coordinate.
   */
  [[nodiscard]] auto x() const { return m_top_left.x(); }

  /**
   *
   * @param x is new Left coordinate.
   * @return Left coordinate.
   */
  [[nodiscard]] auto x(const dimT &x) const { return m_top_left.x(x); }
  /**
   *
   * @param x is new Left coordinate.
   * @return Left coordinate.
   */
  [[nodiscard]] auto left(const dimT &x) const { return m_top_left.x(x); }
  /**
   * @return Top coordinate.
   */
  [[nodiscard]] auto top() const { return m_top_left.y(); }
  /**
   * @return Top coordinate.
   */
  [[nodiscard]] auto y() const { return m_top_left.y(); }
  /**
   *
   * @param y is new Top coordinate.
   * @return Top coordinate.
   */
  [[nodiscard]] auto y(const dimT &y) const { return m_top_left.y(y); }

  /**
   * @return Right coordinate.
   */
  [[nodiscard]] auto right() const { return m_top_left.x() + m_width_height.y(); }

  /**
   * @return Set Right coordinate.
   */
  [[nodiscard]] auto right(const dimT right) const
  {
    if (right >= m_top_left.x()) {
      return m_width_height.X(m_top_left.x() - right);
    }
    return m_width_height.X(right - m_top_left.x());
  }

  /**
   * @return Set Bottom coordinate.
   */
  [[nodiscard]] auto bottom(const dimT bottom) const
  {
    if (bottom >= m_top_left.y()) {
      return m_width_height.Y(m_top_left.y() - bottom);
    }
    return m_width_height.Y(bottom - m_top_left.y());
  }
  /**
   * @return Bottom coordinate.
   */
  [[nodiscard]] auto bottom() const { return m_top_left.y() + m_width_height.y(); }
  /**
   * @return Width.
   */
  [[nodiscard]] auto width() const { return m_width_height.x(); }

  /**
   *
   * @param width is new Width value.
   * @return Width.
   */
  [[nodiscard]] auto width(const dimT &width) const { return m_width_height.X(width); }
  /**
   * @return Height.
   */
  [[nodiscard]] auto height() const { return m_width_height.y(); }
  /**
   *
   * @param height is new Height value.
   * @return Height.
   */
  [[nodiscard]] auto height(const dimT &height) const { return m_width_height.Y(height); }

  [[nodiscard]] std::size_t area() const
  {
    return static_cast<std::size_t>(m_width_height.x()) * static_cast<std::size_t>(m_width_height.y());
  }
  friend std::ostream &operator<<(std::ostream &os, const Rectangle<dimT> &input)
  {
    return os << "{(X, Y) = " << input.m_top_left << ", (Width, Height) = " << input.m_width_height << '}';
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_RECTANGLE_H
