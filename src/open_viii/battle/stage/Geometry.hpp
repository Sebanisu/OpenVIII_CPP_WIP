//
// Created by pcvii on 11/17/2020.
//
#ifndef VIIIARCHIVE_GEOMETRY_HPP
#define VIIIARCHIVE_GEOMETRY_HPP
#include "GeometryHeader1.hpp"
#include "GeometryHeader2.hpp"
#include "open_viii/graphics/Vertice.hpp"
#include "Shapes.hpp"
namespace open_viii::battle::stage {
/**
 * @brief Represents a geometry object in the battle stage.
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Geometry
 */
struct Geometry
{
public:
  GeometryHeader1
    m_geometry_header1{};///< @brief First header of the geometry object.
  std::vector<graphics::Vertice<std::int16_t>>
    m_vertices{};///< @brief Vector of vertices.

  GeometryHeader2
    m_geometry_header2{};///< @brief Second header of the geometry object.
  std::vector<Triangle> m_triangles{};///< @brief Vector of triangles.
  std::vector<Quad>     m_quads{};    ///< @brief Vector of quads.

  Geometry() = default;
  explicit Geometry(const char *const buffer_begin, std::span<const char> span)
  {
    {
      std::array<char, sizeof(GeometryHeader1)> tmp{};
      std::ranges::copy(span, tmp.begin());
      m_geometry_header1 = std::bit_cast<GeometryHeader1>(tmp);
      span               = span.subspan(sizeof(GeometryHeader1));
    }
    std::cout << "\t\t\t Number of Vertices: "
              << m_geometry_header1.number_vertices() << std::endl;
    if (!m_geometry_header1.test()) {
      return;
    }
    m_vertices.reserve(m_geometry_header1.number_vertices());
    for ([[maybe_unused]] const int i : std::views::iota(
           std::uint16_t{},
           m_geometry_header1.number_vertices())) {
      std::array<char, sizeof(graphics::Vertice<std::int16_t>)> tmp{};
      std::ranges::copy(span, tmp.begin());
      m_vertices.push_back(std::bit_cast<graphics::Vertice<std::int16_t>>(tmp));
      span = span.subspan(sizeof(graphics::Vertice<std::int16_t>));
    }
    {
      const auto padding = (std::distance(buffer_begin, span.data())) % 4 + 4;
      span               = span.subspan(padding);
    }
    {
      std::array<char, sizeof(GeometryHeader2)> tmp{};
      std::ranges::copy(span, tmp.begin());
      m_geometry_header2 = std::bit_cast<GeometryHeader2>(tmp);
      span               = span.subspan(sizeof(GeometryHeader2));
    }
    std::cout << "\t\t\t Number of Triangles: "
              << m_geometry_header2.triangle_count() << std::endl;
    std::cout << "\t\t\t Number of Quads: " << m_geometry_header2.quad_count()
              << std::endl;
    m_triangles.reserve(m_geometry_header2.triangle_count());
    for ([[maybe_unused]] const int i : std::views::iota(
           std::uint16_t{},
           m_geometry_header2.triangle_count())) {
      std::array<char, sizeof(Triangle)> tmp{};
      std::ranges::copy(span, tmp.begin());
      m_triangles.push_back(std::bit_cast<Triangle>(tmp));
      span = span.subspan(sizeof(Triangle));
    }
    m_quads.reserve(m_geometry_header2.quad_count());
    for ([[maybe_unused]] const int i :
         std::views::iota(std::uint16_t{}, m_geometry_header2.quad_count())) {
      std::array<char, sizeof(Quad)> tmp{};
      std::ranges::copy(span, tmp.begin());
      m_quads.push_back(std::bit_cast<Quad>(tmp));
      span = span.subspan(sizeof(Quad));
    }
  }
  /**
   * @brief Calculate the padding after reading vertices.
   * @param position The current position in bytes.
   * @return The number of bytes to skip.
   */
  static long
    calc_pad(long position)
  {
    return (position % 4) + 4;
  }
};
}// namespace open_viii::battle::stage

#endif// VIIIARCHIVE_GEOMETRY_HPP
