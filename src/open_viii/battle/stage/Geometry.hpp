//
// Created by pcvii on 11/17/2020.
//
#ifndef VIIIARCHIVE_GEOMETRY_HPP
#define VIIIARCHIVE_GEOMETRY_HPP
#include "GeometryHeader1.hpp"
#include "GeometryHeader2.hpp"
#include "open_viii/graphics/Tim.hpp"
#include "open_viii/graphics/Vertice.hpp"
#include "Shapes.hpp"
// #include <format>
namespace open_viii::battle::stage {
/**
 * @brief Represents a geometry object in the battle stage.
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Geometry
 */
struct Geometry
{
public:
  GeometryHeader1
    geometry_header1{};///< @brief First header of the geometry object.
  std::vector<graphics::Vertice<std::int16_t>>
    vertices{};///< @brief Vector of vertices.

  GeometryHeader2
    geometry_header2{};///< @brief Second header of the geometry object.
  std::vector<Triangle> triangles{};///< @brief Vector of triangles.
  std::vector<Quad>     quads{};    ///< @brief Vector of quads.

  Geometry() = default;
  explicit Geometry(const char *const buffer_begin, std::span<const char> span)
  {
    geometry_header1 = tools::read_val<GeometryHeader1>(span);

    std::cout << "\t\t\t Number of Vertices: "
              << geometry_header1.number_vertices() << std::endl;
    if (!geometry_header1.test()) {
      return;
    }
    vertices = tools::read_vals<graphics::Vertice<std::int16_t>>(
      span,
      geometry_header1.number_vertices());

    const auto padding = calc_pad(std::distance(buffer_begin, span.data()));
    span = span.subspan(static_cast<std::span<const char>::size_type>(padding));

    geometry_header2 = tools::read_val<GeometryHeader2>(span);

    std::cout << "\t\t\t Number of Triangles: "
              << geometry_header2.triangle_count() << std::endl;
    std::cout << "\t\t\t Number of Quads: " << geometry_header2.quad_count()
              << std::endl;

    triangles
      = tools::read_vals<Triangle>(span, geometry_header2.triangle_count());
    quads = tools::read_vals<Quad>(span, geometry_header2.quad_count());
    quads.reserve(geometry_header2.quad_count());
  }
  /**
   * @brief Calculate the padding after reading vertices.
   * @param position The current position in bytes.
   * @return The number of bytes to skip.
   */
  template<std::integral numT>
  static numT
    calc_pad(numT position)
  {
    return static_cast<numT>((position % 4) + 4);
  }
};
}// namespace open_viii::battle::stage

#endif// VIIIARCHIVE_GEOMETRY_HPP
