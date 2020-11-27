//
// Created by pcvii on 11/23/2020.
//

#ifndef VIIIARCHIVE_SHAPES_HPP
#define VIIIARCHIVE_SHAPES_HPP
#include "Quad.hpp"
#include "Triangle.hpp"
namespace open_viii::graphics {
/**
 * run operation on uvs and return a uv.
 * @tparam current should be 0 at start, it is a placeholder for which value we are looking at.
 * @tparam lambdaT lambda that takes two points and returns a point.
 * @param lambda lambda function
 * @return return point
 */
template<std::size_t current = 0U,
  Point_Like pointT = graphics::Point<std::uint8_t>,
  Shape_Like shapeT,
  TakesTwoPointsReturnsPoint<pointT> lambdaT>
requires(current < shapeT::COUNT) [[nodiscard]] constexpr static pointT
  for_each_uv(const shapeT &shape, const lambdaT &lambda)
{
  if constexpr (current + 1U < shapeT::COUNT) {
    return std::invoke(lambda, shape.template uv<current>(), for_each_uv<current + 1>(shape, lambda));
  } else {
    return shape.template uv<current>();
  }
}
/**
//   * find max_uv
//   * @return return the max UV value. More exactly it'll be the max U and max V values.
//   */
template<Point_Like pointT = graphics::Point<std::uint8_t>, Shape_Like shapeT>
[[nodiscard]] constexpr static pointT max_uv(const shapeT &shape)
{
  return for_each_uv(shape, [](const pointT &a, const pointT &b) {
    return graphics::max(a, b);
  });
}
/**
 * find min_uv
 * @return return the min UV value. More exactly it'll be the min U and min V values.
 */
template<Point_Like pointT = graphics::Point<std::uint8_t>, Shape_Like shapeT>
[[nodiscard]] constexpr static pointT min_uv(const shapeT &shape)
{
  return for_each_uv(shape, [](const pointT &a, const pointT &b) {
    return graphics::min(a, b);
  });
}
/**
 * returns a rectangle containing the triangle.
 * @return
 */
[[nodiscard]] constexpr static auto rectangle(const Shape_Like auto &shape)
{
  const auto min_uv_value = min_uv(shape);
  return Rectangle(min_uv_value, max_uv(shape) - min_uv_value);
}
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_SHAPES_HPP
