//
// Created by pcvii on 11/23/2020.
//
#ifndef VIIIARCHIVE_SHAPES_HPP
#define VIIIARCHIVE_SHAPES_HPP
#include "Quad.hpp"
#include "Triangle.hpp"
#include <array>
#include <concepts>
#include <cstdint>
namespace open_viii::graphics {
/**
 * run operation on uvs and return a uv.
 * @tparam current should be 0 at start, it is a placeholder for which value we
 * are looking at.
 * @tparam lambdaT lambda that takes two points and returns a point.
 * @param lambda lambda function
 * @return return point
 */
template<
  std::size_t                        current = 0U,
  Point_Like                         pointT  = graphics::Point<std::uint8_t>,
  Shape_Like                         shapeT,
  TakesTwoPointsReturnsPoint<pointT> lambdaT>
  requires(current < shapeT::COUNT)
[[nodiscard]] constexpr static pointT
  for_each_uv(const shapeT &shape, const lambdaT &lambda)
{
  if constexpr (current + 1U < shapeT::COUNT) {
    return std::invoke(
      lambda,
      shape.template uv<current>(),
      for_each_uv<current + 1>(shape, lambda));
  }
  else {
    return shape.template uv<current>();
  }
}
/**
//   * find max_uv
//   * @return return the max UV value. More exactly it'll be the max U and max
V values.
//   */
template<Point_Like pointT = graphics::Point<std::uint8_t>, Shape_Like shapeT>
[[nodiscard]] constexpr static pointT
  max_uv(const shapeT &shape)
{
  return for_each_uv(shape, [](const pointT &a, const pointT &b) {
    return (graphics::max)(a, b);
  });
}
/**
 * find min_uv
 * @return return the min UV value. More exactly it'll be the min U and min V
 * values.
 */
template<Point_Like pointT = graphics::Point<std::uint8_t>, Shape_Like shapeT>
[[nodiscard]] constexpr static pointT
  min_uv(const shapeT &shape)
{
  return for_each_uv(shape, [](const pointT &a, const pointT &b) {
    return (graphics::min)(a, b);
  });
}
/**
 * returns a rectangle containing the triangle.
 * @return
 */
[[nodiscard]] constexpr static auto
  rectangle(const Shape_Like auto &shape)
{
  const auto min_uv_value = min_uv(shape);
  return Rectangle(min_uv_value, max_uv(shape) - min_uv_value);
}
}// namespace open_viii::graphics
namespace open_viii::battle {
template<typename triangle_type, typename quad_type>
std::array<triangle_type, 2>
  common_quad_to_triangle(const quad_type &quad)
{
  std::array<triangle_type, 2> triangles;
  // Triangle 1: 0, 1, 3
  triangles[0].template face_indice<0>() = quad.template face_indice<0>();
  triangles[0].template face_indice<1>() = quad.template face_indice<1>();
  triangles[0].template face_indice<2>() = quad.template face_indice<3>();

  triangles[0].template uv<0>()          = quad.template uv<0>();
  triangles[0].template uv<1>()          = quad.template uv<1>();
  triangles[0].template uv<2>()          = quad.template uv<3>();
  // Triangle 2: 0, 2, 3
  triangles[1].template face_indice<0>() = quad.template face_indice<0>();
  triangles[1].template face_indice<1>() = quad.template face_indice<2>();
  triangles[1].template face_indice<2>() = quad.template face_indice<3>();

  triangles[1].template uv<0>()          = quad.template uv<0>();
  triangles[1].template uv<1>()          = quad.template uv<2>();
  triangles[1].template uv<2>()          = quad.template uv<3>();
  return triangles;
}
}// namespace open_viii::battle
namespace open_viii::battle::stage {

std::array<Triangle, 2>
  quad_to_triangles(const Quad &quad)
{
  auto triangles              = common_quad_to_triangle<Triangle>(quad);

  // Triangle 1: 0, 1, 3

  triangles[0].clut()         = quad.clut();
  triangles[0].texture_page() = quad.texture_page();// discarding 4 bits.
  triangles[0].raw_hide()     = quad.raw_hide();
  triangles[0].gpu()          = quad.gpu();

  // Triangle 2: 0, 2, 3
  triangles[1].clut()         = quad.clut();
  triangles[1].texture_page() = quad.texture_page();// discarding 4 bits.
  triangles[1].raw_hide()     = quad.raw_hide();
  triangles[1].gpu()          = quad.gpu();

  return triangles;
}

}// namespace open_viii::battle::stage

#endif// VIIIARCHIVE_SHAPES_HPP
