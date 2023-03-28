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
  static void
    write_material_mtl(
      std::ofstream     &output,
      const std::string &basename,
      int                palette_id)
  {
    output << "newmtl " << basename << "_" << palette_id << std::endl;
    output << "map_Kd " << basename << "_" << palette_id << "_x.png"
           << std::endl;
  }

  static void
    write_material_obj(
      std::ofstream     &output,
      const std::string &basename,
      int                palette_id)
  {
    output << "usemtl " << basename << "_" << palette_id << std::endl;
  }
  void
    export_mesh_to_obj(
      const std::filesystem::path &file_name,
      const std::string           &image_base_name,
      const graphics::Tim         &tim) const
  {
    std::error_code ec{};
    std::filesystem::create_directories(file_name.parent_path(), ec);
    if (ec) {
      std::cerr << __FILE__ << ":" << __LINE__ << " - " << ec.value() << ": "
                << ec.message() << " - " << file_name << std::endl;
      ec.clear();
    }
    auto mtl_name = file_name;
    mtl_name.replace_extension(".mtl");
    std::ofstream obj_file(file_name);
    std::ofstream mtl_file(mtl_name);
    obj_file << "mtllib " << mtl_name.filename().string() << std::endl;
    // puts(std::format("saving {}", file_name.string()).c_str());
    std::cout << "saving " << file_name.string() << std::endl;
    std::vector<std::uint8_t> cluts{};
    std::uint16_t             max_value = {};
    for (const auto &tmp : m_triangles) {
      cluts.push_back(tmp.clut());
      for (const auto &face : tmp.face_indices()) {
        if (face + 1 > max_value) {
          max_value = face + 1;
        }
      }
    }

    for (const auto &tmp : m_quads) {
      cluts.push_back(tmp.clut());
      for (const auto &face : tmp.face_indices()) {
        if (face + 1 > max_value) {
          max_value = face + 1;
        }
      }
    }
    const auto remove_range = std::ranges::unique(cluts);
    cluts.erase(remove_range.begin(), remove_range.end());

    for (auto clut : cluts) {
      write_material_mtl(mtl_file, image_base_name, clut);
    }

    // Write vertices
    for (const auto vertice : m_vertices) {//| std::views::take(max_value)
      //      obj_file
      //        << std::format("v {} {} {}\n", vertice.x(), vertice.y(),
      //        vertice.z());
      const float scale = 128.F;
      obj_file << "v " << vertice.x() / scale << " " << vertice.y() / scale
               << " " << vertice.z() / scale << "\n";
    }

    const auto convert_uv =
      [&tim](graphics::Point<std::uint8_t> uv, std::uint8_t texture_page) {
        //      float U = (float)uv.x() / (float)(tim.width() * 2.F) +
        //      ((float)texture_page/(tim.width() * 2.F)); float V =
        //      (float)uv.y() / (float)(tim.height() * 2.F);
        // return graphics::Point<float>(U,V);
        const float texPageWidth = 128.F;

        return graphics::Point<float>(
          ((static_cast<float>(uv.x())
            + (static_cast<float>(texture_page) * texPageWidth)))
            / static_cast<float>(tim.width()),
          static_cast<float>(tim.height())
            - (static_cast<float>(uv.y()) / static_cast<float>(tim.height())));
      };
    // Write UVs
    for (const auto &triangle : m_triangles) {
      for (const auto uv : triangle.uvs()) {
        const auto new_uv = convert_uv(uv, triangle.texture_page());
        obj_file << "vt " << new_uv.x() << " " << new_uv.y() << "\n";
      }
    }
    for (const auto &quad : m_quads) {
      for (const auto uv : quad.uvs()) {
        const auto new_uv = convert_uv(uv, quad.texture_page());
        obj_file << "vt " << new_uv.x() << " " << new_uv.y() << "\n";
      }
    }

    const auto write_triangle
      = [&obj_file](
          const Triangle                             &triangle,
          [[maybe_unused]] std::array<std::size_t, 3> uv_index) {
          //      obj_file << std::format(
          //        "f {0}/{3} {1}/{4} {2}/{5}\n",
          //        triangle.face_indice<0>(),
          //        triangle.face_indice<1>(),
          //        triangle.face_indice<2>(),
          //        uv_index[0],
          //        uv_index[1],
          //        uv_index[2]);
          obj_file << "f " << triangle.face_indice<0>() + 1 << "/"
                   << uv_index[0] << " " << triangle.face_indice<1>() + 1 << "/"
                   << uv_index[1] << " " << triangle.face_indice<2>() + 1 << "/"
                   << uv_index[2] << "\n";
        };
    std::size_t i{ 1 };
    // Write triangle faces
    for (const auto &triangle : m_triangles) {
      write_material_obj(obj_file, image_base_name, triangle.clut());
      write_triangle(triangle, { i + 1, i + 2, i });
      i += 3;
    }

    // Write quad faces
    for (const auto &quad : m_quads) {
      write_material_obj(obj_file, image_base_name, quad.clut());
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
