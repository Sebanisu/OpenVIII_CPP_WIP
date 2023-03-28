//
// Created by pcvii on 3/28/2023.
//

#ifndef OPENVIII_CPP_WIP_STAGETOOBJ_HPP
#define OPENVIII_CPP_WIP_STAGETOOBJ_HPP
#include "Geometry.hpp"
#include "X.hpp"
#include <filesystem>
#include <iostream>
namespace open_viii::battle::stage {
namespace StageToObj {
  /**
   * @brief Writes material information to the output MTL file.
   * @param output Output file stream for the MTL file.
   * @param basename Base name of the material.
   * @param palette_id Palette ID for the material.
   */
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

  /**
   * @brief Writes material usage information to the output OBJ file.
   * @param output Output file stream for the OBJ file.
   * @param basename Base name of the material.
   * @param palette_id Palette ID for the material.
   */
  static void
    write_material_obj(
      std::ofstream     &output,
      const std::string &basename,
      int                palette_id)
  {
    output << "usemtl " << basename << "_" << palette_id << std::endl;
  }

  /**
   * @brief Handles directory creation and error checking.
   * @param file_name The output file path for the OBJ file.
   */
  static void
    create_directory_if_needed(const std::filesystem::path &file_name)
  {
    std::error_code ec{};
    std::filesystem::create_directories(file_name.parent_path(), ec);
    if (ec) {
      std::cout << std::flush;
      std::cerr << __FILE__ << ":" << __LINE__ << " - " << ec.value() << ": "
                << ec.message() << " - " << file_name << std::endl;
      ec.clear();
    }
  }

  /**
   * @brief Write vertices to the OBJ file.
   * @param self The Geometry object containing the model data.
   * @param obj_file The output OBJ file stream.
   */
  static void
    write_vertices_to_obj(const Geometry &self, std::ofstream &obj_file)
  {
    const float scale = 128.F;
    for (const auto vertice : self.vertices) {
      obj_file << "v " << vertice.x() / scale << " " << vertice.y() / scale
               << " " << vertice.z() / scale << "\n";
    }
  }

  /**
   * @brief Exports geometry data to an OBJ file.
   * @param self The Geometry object containing the model data.
   * @param file_name The output file path for the OBJ file.
   * @param image_base_name The base name of the image file.
   * @param tim The graphics::Tim object containing texture information.
   */
  static void
    export_geometry_to_obj(
      const Geometry              &self,
      const std::filesystem::path &file_name,
      const std::string           &image_base_name,
      const graphics::Tim         &tim)
  {
    create_directory_if_needed(file_name.parent_path());
    auto mtl_name = file_name;
    mtl_name.replace_extension(".mtl");
    std::ofstream obj_file(file_name);
    std::ofstream mtl_file(mtl_name);
    obj_file << "mtllib " << mtl_name.filename().string() << std::endl;
    // puts(std::format("saving {}", file_name.string()).c_str());
    std::cout << "saving " << file_name.string() << std::endl;
    std::vector<std::uint8_t> cluts{};
    std::uint16_t             max_value = {};
    for (const auto &tmp : self.triangles) {
      cluts.push_back(tmp.clut());
      for (const auto &face : tmp.face_indices()) {
        if (face + 1 > max_value) {
          max_value = face + 1;
        }
      }
    }

    for (const auto &tmp : self.quads) {
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

    write_vertices_to_obj(self, obj_file);

    const auto convert_uv =
      [&tim](graphics::Point<std::uint8_t> uv, std::uint8_t texture_page) {
        const float texPageWidth = 128.F;

        return graphics::Point<float>(
          ((static_cast<float>(uv.x())
            + (static_cast<float>(texture_page) * texPageWidth)))
            / static_cast<float>(tim.width()),
          static_cast<float>(tim.height())// flip the Y coord.
            - (static_cast<float>(uv.y()) / static_cast<float>(tim.height())));
      };
    // Write UVs
    for (const auto &triangle : self.triangles) {
      for (const auto uv : triangle.uvs()) {
        const auto new_uv = convert_uv(uv, triangle.texture_page());
        obj_file << "vt " << new_uv.x() << " " << new_uv.y() << "\n";
      }
    }
    for (const auto &quad : self.quads) {
      for (const auto uv : quad.uvs()) {
        const auto new_uv = convert_uv(uv, quad.texture_page());
        obj_file << "vt " << new_uv.x() << " " << new_uv.y() << "\n";
      }
    }

    const auto write_triangle
      = [&obj_file](
          const Triangle                             &triangle,
          [[maybe_unused]] std::array<std::size_t, 3> uv_index) {
          obj_file << "f " << triangle.face_indice<0>() + 1 << "/"
                   << uv_index[0] << " " << triangle.face_indice<1>() + 1 << "/"
                   << uv_index[1] << " " << triangle.face_indice<2>() + 1 << "/"
                   << uv_index[2] << "\n";
        };
    std::size_t i{ 1 };
    // Write triangle faces
    for (const auto &triangle : self.triangles) {
      write_material_obj(obj_file, image_base_name, triangle.clut());
      // order of Triangle, 0,1,2
      // order of UV 1,2,0, note! different order!!
      write_triangle(triangle, { i + 1, i + 2, i });
      i += 3;
    }

    // Write quad faces
    for (const auto &quad : self.quads) {
      write_material_obj(obj_file, image_base_name, quad.clut());
      const auto triangles = quad_to_triangles(quad);
      // Triangle 1: 0, 1, 3
      // order of UV 0, 1, 3
      write_triangle(triangles[0], { i, i + 1, i + 3 });
      // Triangle 2: 0, 2, 3
      // order of UV 0, 2, 3
      write_triangle(triangles[1], { i, i + 2, i + 3 });
      i += 4;
    }
    obj_file.close();
  }

  /**
   * @brief Exports battle stage X file geometry data to an OBJ file.
   * @param self The battle stage X object containing the model data.
   */
  static void
    export_x_to_obj(const X &self)
  {
    if (self.tim().check()) {
      self.tim().save(self.path());
    }
    for (std::size_t i{}; const auto &geometries : self.geometries()) {
      for (std::size_t j{}; const auto &model : geometries.nested_geometries) {
        std::stringstream           ss{};
        const std::filesystem::path x_path = std::filesystem::path(self.path());
        ss << x_path.stem().string() << "_" << i << "_" << j << ".obj";
        export_geometry_to_obj(
          model,
          std::filesystem::absolute("tmp" / x_path.parent_path()) / ss.str(),
          x_path.stem().string(),
          self.tim());
        ++j;
      }
      ++i;
    }
  }
}// namespace StageToObj
}// namespace open_viii::battle::stage
#endif// OPENVIII_CPP_WIP_STAGETOOBJ_HPP
