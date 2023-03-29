//
// Created by pcvii on 3/28/2023.
//

#ifndef OPENVIII_CPP_WIP_FLATTENBATTLETIM_HPP
#define OPENVIII_CPP_WIP_FLATTENBATTLETIM_HPP
#include "open_viii/battle/stage/X.hpp"
#include "open_viii/graphics/Point.hpp"
#include "open_viii/graphics/Tim.hpp"
namespace open_viii::battle::stage::FlattenBattleTim {

/**
 * @brief Converts UV coordinates to pixel positions
 * @param uv The UV coordinates to convert
 * @param texture_page The texture page the UV coordinates belong to
 * @return The pixel position as a graphics::Point<float> object
 */
inline graphics::Point<float>
  uv_to_pixel_position(
    const graphics::Point<std::uint8_t> &uv,
    std::uint8_t                         texture_page)
{
  const float texPageWidth = 128.F;

  return graphics::Point<float>(
    ((static_cast<float>(uv.x())
      + (static_cast<float>(texture_page) * texPageWidth))),
    (static_cast<float>(uv.y())));
}

/**
 * @brief Determines if a point is within a triangle, with optional margin
 * @param p The point to check
 * @param triangle A std::array of triangle vertices as graphics::Point<float>
 * @param margin The margin to be used for the check (optional, default is
 * 0.25F)
 * @return true if the point is inside the triangle, false otherwise
 */
inline bool
  is_point_in_triangle(
    const graphics::Point<float>                &p,
    const std::array<graphics::Point<float>, 3> &triangle,
    float                                        margin = 0.25F)
{
  // Calculate the bounding box of the triangle
  float min_x
    = std::min(std::min(triangle[0].x(), triangle[1].x()), triangle[2].x())
    - margin;
  float max_x
    = std::max(std::max(triangle[0].x(), triangle[1].x()), triangle[2].x())
    + margin;
  float min_y
    = std::min(std::min(triangle[0].y(), triangle[1].y()), triangle[2].y())
    - margin;
  float max_y
    = std::max(std::max(triangle[0].y(), triangle[1].y()), triangle[2].y())
    + margin;

  // If the point is outside the bounding box, it can't be inside the triangle
  if (p.x() < min_x || p.x() > max_x || p.y() < min_y || p.y() > max_y) {
    return false;
  }

  // Check if the point is inside the triangle
  double alpha, beta, gamma;
  double det
    = (triangle[1].y() - triangle[2].y()) * (triangle[0].x() - triangle[2].x())
    + (triangle[2].x() - triangle[1].x()) * (triangle[0].y() - triangle[2].y());
  alpha = ((triangle[1].y() - triangle[2].y()) * (p.x() - triangle[2].x())
           + (triangle[2].x() - triangle[1].x()) * (p.y() - triangle[2].y()))
        / det;
  beta = ((triangle[2].y() - triangle[0].y()) * (p.x() - triangle[2].x())
          + (triangle[0].x() - triangle[2].x()) * (p.y() - triangle[2].y()))
       / det;
  gamma = 1.0 - alpha - beta;

  return alpha >= -margin && beta >= -margin && gamma >= -margin;
}

/**
 * @brief Extracts the used colors in a battle stage texture
 * @param self The X object containing the battle stage data
 * @return void, but saves a PNG file with used colors at a specified path
 */
inline void
  extract_used_colors(const X &self)
{
  const graphics::Tim                               &tim = self.tim();
  std::array<std::vector<graphics::Color16ABGR>, 16> source_colors;
  std::ranges::transform(
    std::views::iota(0, 16),
    source_colors.begin(),
    [&tim](auto i) {
      return tim.get_colors<graphics::Color16ABGR>(
        static_cast<std::uint16_t>(i));
    });

  std::vector<graphics::Color32RGBA> used_colors(
    tim.width() * tim.height(),
    graphics::Color32RGBA{});

  const auto process_triangle = [&tim, &source_colors, &used_colors](
                                  const Triangle &triangle,
                                  std::uint32_t   y,
                                  std::uint32_t   x,
                                  bool           &found) {
    std::array<graphics::Point<float>, 3> triangle_uvs;
    for (std::uint32_t i = 0; const auto &uv : triangle.uvs()) {
      triangle_uvs[i] = uv_to_pixel_position(uv, triangle.texture_page());
      ++i;
    }

    if (is_point_in_triangle(
          { static_cast<float>(x), static_cast<float>(y) },
          triangle_uvs)) {
      std::uint32_t index = y * tim.width() + x;
      used_colors[index]  = static_cast<graphics::Color32RGBA>(
        source_colors[triangle.clut()][index]);
      found = true;
      return;
    }
  };

  for (const auto &geometries : self.geometries()) {
    for (const auto &geometry : geometries.nested_geometries) {
      for (std::uint32_t y = 0; y < tim.height(); ++y) {
        for (std::uint32_t x = 0; x < tim.width(); ++x) {
          bool found = false;
          for (const Triangle &triangle : geometry.triangles) {
            process_triangle(triangle, y, x, found);
            if (found)
              break;
          }

          if (!found) {
            for (const auto &quad : geometry.quads) {
              const auto triangles = quad_to_triangles(quad);
              for (const auto &triangle : triangles) {
                process_triangle(triangle, y, x, found);
                if (found)
                  break;
              }
              if (found)
                break;
            }
          }
        }
      }
    }
  }

  auto path = "tmp" / std::filesystem::path(self.path());
  path.replace_extension(".png");
  std::cout << "saving " << std::filesystem::absolute(path).string()
            << std::endl;
  graphics::Png::save(used_colors, tim.width(), tim.height(), path);
}
}// namespace open_viii::battle::stage::FlattenBattleTim

#endif// OPENVIII_CPP_WIP_FLATTENBATTLETIM_HPP
