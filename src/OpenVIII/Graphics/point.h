//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_POINT_H
#define VIIIARCHIVE_POINT_H
namespace OpenVIII::Graphics {
template<typename dimT> struct point
{
private:
  mutable dimT X_{};
  mutable dimT Y_{};

public:
  /**
   * X coordinate.
   * @return x
   */
  [[nodiscard]] auto X() const { return X_; }

  /**
   * Y coordinate.
   * @return y
   */
  [[nodiscard]] auto Y() const { return Y_; }

  /**
   *
   * @param x is new X coordinate.
   * @return x
   */
  [[nodiscard]] auto X(const dimT &x) const { return X_ = x; }

  /**
   *
   * @param y is new Y coordinate.
   * @return y
   */
  [[nodiscard]] auto Y(const dimT &y) const { return Y_ = y; }


  friend std::ostream &operator<<(std::ostream &os, const point<dimT> &input)
  {
    return os << '{' << input.X_ << ", " << input.Y_ << '}';
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_POINT_H
