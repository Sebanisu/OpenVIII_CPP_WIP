//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_RECTANGLE_H
#define VIIIARCHIVE_RECTANGLE_H
#include "OpenVIII/Graphics/point.h"
namespace OpenVIII::Graphics {
/**
 * @struct OpenVIII::Graphics::rectangle
 * @tparam dimT Number type.
 * @brief 2D Container that holds top left height and width.
 */
template<typename dimT> struct rectangle
{
private:
  mutable point<dimT> TopLeft_{};
  mutable point<dimT> WidthHeight_{};

public:
  /**
   * @return Left coordinate.
   */
  [[maybe_unused]] [[nodiscard]] auto Left() const { return TopLeft_.X(); }
  /**
   * @return Left coordinate.
   */
  [[nodiscard]] auto X() const { return TopLeft_.X(); }

  /**
   *
   * @param x is new Left coordinate.
   * @return Left coordinate.
   */
  [[nodiscard]] auto X(const dimT &x) const { return TopLeft_.X(x); }
  /**
   *
   * @param x is new Left coordinate.
   * @return Left coordinate.
   */
  [[nodiscard]] auto Left(const dimT &x) const { return TopLeft_.X(x); }
  /**
   * @return Top coordinate.
   */
  [[nodiscard]] auto Top() const { return TopLeft_.Y(); }
  /**
   * @return Top coordinate.
   */
  [[nodiscard]] auto Y() const { return TopLeft_.Y(); }
  /**
   *
   * @param y is new Top coordinate.
   * @return Top coordinate.
   */
  [[nodiscard]] auto Y(const dimT &y) const { return TopLeft_.Y(y); }

  /**
   * @return Right coordinate.
   */
  [[nodiscard]] auto Right() const { return TopLeft_.X() + WidthHeight_.X(); }
  /**
   * @return Bottom coordinate.
   */
  [[nodiscard]] auto Bottom() const { return TopLeft_.Y() + WidthHeight_.Y(); }
  /**
   * @return Width.
   */
  [[nodiscard]] auto Width() const { return WidthHeight_.X(); }

  /**
   *
   * @param width is new Width value.
   * @return Width.
   */
  [[nodiscard]] auto Width(const dimT &width) const { return WidthHeight_.X(width); }
  /**
   * @return Height.
   */
  [[nodiscard]] auto Height() const { return WidthHeight_.Y(); }
  /**
   *
   * @param height is new Height value.
   * @return Height.
   */
  [[nodiscard]] auto Height(const dimT &height) const { return WidthHeight_.Y(height); }

  friend std::ostream &operator<<(std::ostream &os, const rectangle<dimT> &input)
  {
    return os << "{(X, Y) = " << input.TopLeft_ << ", (Width, Height) = " << input.WidthHeight_ << '}';
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_RECTANGLE_H
