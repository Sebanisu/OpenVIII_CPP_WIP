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
  template<typename T>
  T
    read_val(std::span<const char> &span)
  {
    std::array<char, sizeof(T)> tmp{};
    std::ranges::copy(span.subspan(0, sizeof(T)), tmp.begin());
    span = span.subspan(sizeof(T));
    return std::bit_cast<T>(tmp);
  }

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
    const char *const model_group_ptr     = span.data();
    const char *const model_group_end_ptr = span.data() + span.size();

    std::cout << "\t\t Assigned Offset: " << std::hex << std::uppercase
              << std::distance(buffer_begin, span.data()) << std::dec
              << std::nouppercase << std::endl;
    const auto model_count = read_val<std::uint32_t>(span);
    std::cout << "\t\t Model Count: " << model_count << std::endl;
    std::vector<const char *> m_model_pointers{};
    m_model_pointers.reserve(model_count);
    std::ranges::transform(
      std::views::iota(std::uint32_t{}, model_count),
      std::back_inserter(m_model_pointers),
      [&](auto &&) -> const char * {
        return model_group_ptr + read_val<std::uint32_t>(span);
      });
    span = std::span(m_model_pointers.front(), model_group_end_ptr);
    {
      m_geometry_header1 = read_val<GeometryHeader1>(span);
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
      m_vertices.push_back(read_val<graphics::Vertice<std::int16_t>>(span));
    }
    {
      const auto padding = (std::distance(buffer_begin, span.data())) % 4 + 4;
      span               = span.subspan(padding);
    }
    {
      m_geometry_header2 = read_val<GeometryHeader2>(span);
    }
    std::cout << "\t\t\t Number of Triangles: "
              << m_geometry_header2.triangle_count() << std::endl;
    std::cout << "\t\t\t Number of Quads: " << m_geometry_header2.quad_count()
              << std::endl;
    m_triangles.reserve(m_geometry_header2.triangle_count());
    for ([[maybe_unused]] const int i : std::views::iota(
           std::uint16_t{},
           m_geometry_header2.triangle_count())) {
      m_triangles.push_back(read_val<Triangle>(span));
    }
    m_quads.reserve(m_geometry_header2.quad_count());
    for ([[maybe_unused]] const int i :
         std::views::iota(std::uint16_t{}, m_geometry_header2.quad_count())) {
      m_quads.push_back(read_val<Quad>(span));
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
