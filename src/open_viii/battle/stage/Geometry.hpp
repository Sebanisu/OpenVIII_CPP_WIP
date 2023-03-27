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
  template<typename T>
  static T
    read_val(std::span<const char> &span)
  {
    std::array<char, sizeof(T)> tmp{};
    std::ranges::copy(span.subspan(0, sizeof(T)), tmp.begin());
    span = span.subspan(sizeof(T));
    return std::bit_cast<T>(tmp);
  }
  template<typename T, std::unsigned_integral numT>
  static std::vector<T>
    read_vals(std::span<const char> &span, numT count)
  {
    std::vector<T> return_val{};
    return_val.reserve(count);
    auto iota = std::views::iota(numT{}, count);
    std::ranges::transform(iota, std::back_inserter(return_val), [&span](numT) {
      return read_val<T>(span);
    });
    return return_val;
  }
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
    const auto m_model_offsets = read_vals<std::uint32_t>(span, model_count);
    auto       m_model_pointers
      = m_model_offsets
      | std::views::transform([&](std::uint32_t offset) -> const char * {
          return model_group_ptr + offset;
        });
    span = std::span(m_model_pointers.front(), model_group_end_ptr);

    m_geometry_header1 = read_val<GeometryHeader1>(span);

    std::cout << "\t\t\t Number of Vertices: "
              << m_geometry_header1.number_vertices() << std::endl;
    if (!m_geometry_header1.test()) {
      return;
    }
    m_vertices = read_vals<graphics::Vertice<std::int16_t>>(
      span,
      m_geometry_header1.number_vertices());

    const auto padding = calc_pad(std::distance(buffer_begin, span.data()));
    span               = span.subspan(padding);

    m_geometry_header2 = read_val<GeometryHeader2>(span);

    std::cout << "\t\t\t Number of Triangles: "
              << m_geometry_header2.triangle_count() << std::endl;
    std::cout << "\t\t\t Number of Quads: " << m_geometry_header2.quad_count()
              << std::endl;

    m_triangles
      = read_vals<Triangle>(span, m_geometry_header2.triangle_count());
    m_quads = read_vals<Quad>(span, m_geometry_header2.quad_count());
    m_quads.reserve(m_geometry_header2.quad_count());
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
