//
// Created by pcvii on 11/17/2020.
//
#ifndef VIIIARCHIVE_GEOMETRY_HPP
#define VIIIARCHIVE_GEOMETRY_HPP
#include "GeometryHeader1.hpp"
#include "GeometryHeader2.hpp"
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
    span = span.subspan(static_cast<std::span<const char>::size_type>(padding));

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

  void
    export_mesh_to_obj(const std::filesystem::path &file_name) const
  {
    std::error_code ec{};
    std::filesystem::create_directories(file_name.parent_path(), ec);
    if (ec) {
      std::cerr << __FILE__ << ":" << __LINE__ << " - " << ec.value() << ": "
                << ec.message() << " - " << file_name << std::endl;
      ec.clear();
    }
    std::ofstream obj_file(file_name);
    // puts(std::format("saving {}", file_name.string()).c_str());
    std::cout << "saving " << file_name.string() << std::endl;
    // Write vertices
    for (const auto vertice : m_vertices) {
      //      obj_file
      //        << std::format("v {} {} {}\n", vertice.x(), vertice.y(),
      //        vertice.z());
      obj_file << "v " << vertice.x() << " " << vertice.y() << " "
               << vertice.z() << "\n";
    }

    // Write UVs
    for (const auto &triangle : m_triangles) {
      for (const auto uv : triangle.uvs()) {
        // obj_file << std::format("vt {} {}\n", uv.x(), uv.y());
        obj_file << "vt " << uv.x() << " " << uv.y() << "\n";
      }
    }
    for (const auto &quad : m_quads) {
      for (const auto uv : quad.uvs()) {
        // obj_file << std::format("vt {} {}\n", uv.x(), uv.y());
        obj_file << "vt " << uv.x() << " " << uv.y() << "\n";
      }
    }

    const auto write_triangle = [&obj_file](
                                  const Triangle            &triangle,
                                  std::array<std::size_t, 3> uv_index) {
      //      obj_file << std::format(
      //        "f {0}/{3} {1}/{4} {2}/{5}\n",
      //        triangle.face_indice<0>(),
      //        triangle.face_indice<1>(),
      //        triangle.face_indice<2>(),
      //        uv_index[0],
      //        uv_index[1],
      //        uv_index[2]);
      obj_file << "f " << triangle.face_indice<0>() << "/" << uv_index[0] << " "
               << triangle.face_indice<1>() << "/" << uv_index[1] << " "
               << triangle.face_indice<2>() << "/" << uv_index[2] << "\n";
    };

    // Write triangle faces
    for (std::size_t i{}; const auto &triangle : m_triangles) {
      write_triangle(triangle, { i, i + 1, i + 2 });
      i += 3;
    }

    // Write quad faces
    for (std::size_t i{ m_triangles.size() * 3U }; const auto &quad : m_quads) {

      const auto triangles = quad_to_triangles(quad);
      // Triangle 1: 0, 1, 3
      write_triangle(triangles[0], { i, i + 1, i + 3 });
      // Triangle 2: 0, 2, 3
      write_triangle(triangles[1], { i, i + 2, i + 3 });
      i += 4;
    }

    obj_file.close();
  }
};
}// namespace open_viii::battle::stage

#endif// VIIIARCHIVE_GEOMETRY_HPP
