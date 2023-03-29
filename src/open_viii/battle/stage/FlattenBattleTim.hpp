//
// Created by pcvii on 3/28/2023.
//

#ifndef OPENVIII_CPP_WIP_FLATTENBATTLETIM_HPP
#define OPENVIII_CPP_WIP_FLATTENBATTLETIM_HPP
#include "open_viii/battle/stage/X.hpp"
#include "open_viii/graphics/Point.hpp"
#include "open_viii/graphics/Tim.hpp"
namespace open_viii::battle::stage::FlattenBattleTim {
// Function to convert UV coordinates to pixel positions
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

bool
  is_point_in_triangle(
    const graphics::Point<float>                &p,
    const std::array<graphics::Point<float>, 3> &triangle)
{
  const auto &a = triangle[0];
  const auto &b = triangle[1];
  const auto &c = triangle[2];
  double      alpha, beta, gamma;

  double      det
    = (b.y() - c.y()) * (a.x() - c.x()) + (c.x() - b.x()) * (a.y() - c.y());

  alpha
    = ((b.y() - c.y()) * (p.x() - c.x()) + (c.x() - b.x()) * (p.y() - c.y()))
    / det;
  beta = ((c.y() - a.y()) * (p.x() - c.x()) + (a.x() - c.x()) * (p.y() - c.y()))
       / det;
  gamma = 1.0 - alpha - beta;

  return alpha >= 0 && beta >= 0 && gamma >= 0;
}
// float
//   area(float x0, float y0, float x1, float y1, float x2, float y2)
//{
//   return static_cast<float>(
//     abs((x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1)) / 2.F));
// }
//
//// float
////   area(const std::array<graphics::Point<float>, 3> &t)
////{
////   return abs(
////     (t[0].x() * (t[1].y() - t[2].y()) + t[1].x() * (t[2].y() - t[0].y())
////      + t[2].x() * (t[0].y() - t[1].y()))
////     / 2.0);
//// }
// bool
//   near_equal(float a, float b, float tolerance = 1e-6)
//{
//   return std::abs(a - b) <= tolerance;
// }
// inline bool
//   is_point_in_triangle(
//     float                                        x,
//     float                                        y,
//     const std::array<graphics::Point<float>, 3> &triangle)
//{
//
//   float A = area(
//     triangle[0].x(),
//     triangle[0].y(),
//     triangle[1].x(),
//     triangle[1].y(),
//     triangle[2].x(),
//     triangle[2].y());
//   float A1 = area(
//     x,
//     y,
//     triangle[1].x(),
//     triangle[1].y(),
//     triangle[2].x(),
//     triangle[2].y());
//   float A2 = area(
//     triangle[0].x(),
//     triangle[0].y(),
//     x,
//     y,
//     triangle[2].x(),
//     triangle[2].y());
//   float A3 = area(
//     triangle[0].x(),
//     triangle[0].y(),
//     triangle[1].x(),
//     triangle[1].y(),
//     x,
//     y);
//   return near_equal(A, A1 + A2 + A3);
//   //  auto sign = [](
//   //                const graphics::Point<float> &p1,
//   //                const graphics::Point<float> &p2,
//   //                const graphics::Point<float> &p3) {
//   //    return (p1.x() - p3.x()) * (p2.y() - p3.y())
//   //         - (p2.x() - p3.x()) * (p1.y() - p3.y());
//   //  };
//   //
//   //  bool has_neg = false;
//   //  bool has_pos = false;
//   //
//   //  for (int i = 0; i < 3; ++i) {
//   //    float val = sign(
//   //      { x, y },
//   //      triangle[static_cast<std::uint32_t>(i)],
//   //      triangle[static_cast<std::uint32_t>((i + 1) % 3)]);
//   //    if (val < 0)
//   //      has_neg = true;
//   //    if (val > 0)
//   //      has_pos = true;
//   //  }
//
//   // return !(has_neg && has_pos);
// }

void
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
                                  const Triangle & triangle,
                                  std::uint32_t  y,
                                  std::uint32_t  x,
                                  bool          &found) {
//    std::uint32_t texture_page = x / 128U;
//    if (triangle.texture_page() != texture_page) {
//      return;
//    }
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
