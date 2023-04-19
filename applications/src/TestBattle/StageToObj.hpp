//
// Created by pcvii on 3/28/2023.
//

#ifndef OPENVIII_CPP_WIP_STAGETOOBJ_HPP
#define OPENVIII_CPP_WIP_STAGETOOBJ_HPP
#include "open_viii/battle/stage/Geometry.hpp"
#include "open_viii/battle/stage/X.hpp"
#include <filesystem>
#include <iostream>
namespace open_viii::battle::stage::StageToObj {
/**
 * @brief Writes material information to the output MTL file.
 * @param output Output file stream for the MTL file.
 * @param basename Base name of the material.
 * @param palette_id Palette ID for the material.
 */
inline void
  write_material_mtl(
    std::ofstream     &output,
    const std::string &basename,
    int                palette_id)
{
  output << "newmtl " << basename << "_" << palette_id << std::endl;
  output << "map_Kd " << basename << "_" << palette_id << "_x.png" << std::endl;
}

/**
 * @brief Writes material usage information to the output OBJ file.
 * @param output Output file stream for the OBJ file.
 * @param basename Base name of the material.
 * @param palette_id Palette ID for the material.
 */
inline void
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
inline void
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
inline void
  write_vertices_to_obj(const Geometry &self, std::ofstream &obj_file)
{
  const float scale = 128.F;
  for (const auto vertice : self.vertices) {
    obj_file << "v " << static_cast<float>(vertice.x()) / scale << " "
             << static_cast<float>(vertice.y()) / scale << " "
             << static_cast<float>(vertice.z()) / scale << "\n";
  }
}
[[nodiscard]] inline auto
  convert_uv(
    const graphics::Point<std::uint8_t> uv,
    const std::uint8_t                  texture_page,
    const graphics::Tim                &tim) -> graphics::Point<float>
{
  const float texPageWidth = 128.F;

  return {
    ((static_cast<float>(uv.x())
      + (static_cast<float>(texture_page) * texPageWidth)))
      / static_cast<float>(tim.width()),
    static_cast<float>(tim.height())// flip the Y coord.
      - (static_cast<float>(uv.y()) / static_cast<float>(tim.height()))
  };
}

inline void
  write_uvs_to_obj(
    std::ofstream       &obj_file,
    const Geometry      &self,
    const graphics::Tim &tim)
{// Write UVs
  for (const auto &triangle : self.triangles) {
    for (const auto uv : triangle.uvs()) {
      const auto new_uv = convert_uv(uv, triangle.texture_page(), tim);
      obj_file << "vt " << new_uv.x() << " " << new_uv.y() << "\n";
    }
  }
  for (const auto &quad : self.quads) {
    for (const auto uv : quad.uvs()) {
      const auto new_uv = convert_uv(uv, quad.texture_page(), tim);
      obj_file << "vt " << new_uv.x() << " " << new_uv.y() << "\n";
    }
  }
}

inline void
  write_cluts_to_mtl(
    std::ofstream     &mtl_file,
    const Geometry    &self,
    const std::string &image_base_name)
{
  std::vector<uint8_t> cluts{};
  for (const auto &tmp : self.triangles) {
    cluts.push_back(tmp.clut());
  }
  for (const auto &tmp : self.quads) {
    cluts.push_back(tmp.clut());
  }
  const auto remove_range = std::ranges::unique(cluts);
  cluts.erase(remove_range.begin(), remove_range.end());
  std::ranges::sort(cluts);
  for (auto clut : cluts) {
    write_material_mtl(mtl_file, image_base_name, clut);
  }
}

inline void
  write_triangle_to_obj(
    std::ofstream             &obj_file,
    const Triangle            &triangle,
    std::array<std::size_t, 3> uv_index)
{
  obj_file << "f " << triangle.face_indice<0>() + 1 << "/" << uv_index[0] << " "
           << triangle.face_indice<1>() + 1 << "/" << uv_index[1] << " "
           << triangle.face_indice<2>() + 1 << "/" << uv_index[2] << "\n";
};
inline auto
  write_triangles_to_obj(
    std::ofstream     &obj_file,
    const Geometry    &self,
    const std::string &image_base_name) -> std::size_t
{
  std::size_t uv_index = { 1 };// Write triangle faces
  for (const auto &triangle : self.triangles) {
    write_material_obj(obj_file, image_base_name, triangle.clut());
    // order of Triangle, 0,1,2
    // order of UV 1,2,0, note! different order!!
    write_triangle_to_obj(
      obj_file,
      triangle,
      { uv_index + 1, uv_index + 2, uv_index });
    uv_index += 3;
  }
  return uv_index;
}
inline void
  write_quads_to_obj(
    std::ofstream     &obj_file,
    const Geometry    &self,
    const std::string &image_base_name,
    std::size_t        uv_index)
{// Write quad faces
  for (const auto &quad : self.quads) {
    write_material_obj(obj_file, image_base_name, quad.clut());
    const auto triangles = quad_to_triangles(quad);
    // Triangle 1: 0, 1, 3
    // order of UV 0, 1, 3
    write_triangle_to_obj(
      obj_file,
      triangles[0],
      { uv_index, uv_index + 1, uv_index + 3 });
    // Triangle 2: 0, 2, 3
    // order of UV 0, 2, 3
    write_triangle_to_obj(
      obj_file,
      triangles[1],
      { uv_index, uv_index + 2, uv_index + 3 });
    uv_index += 4;
  }
}
inline auto
  create_mtl_file(
    const Geometry              &self,
    const std::filesystem::path &obj_file_name,
    const std::string           &image_base_name) -> std::filesystem::path
{
  auto mtl_name = obj_file_name;
  mtl_name.replace_extension(".mtl");
  std::ofstream mtl_file(mtl_name);
  write_cluts_to_mtl(mtl_file, self, image_base_name);
  return mtl_name;
}
/**
 * @brief Exports geometry data to an OBJ file.
 * @param self The Geometry object containing the model data.
 * @param file_name The output file path for the OBJ file.
 * @param image_base_name The base name of the image file.
 * @param tim The graphics::Tim object containing texture information.
 */
inline void
  export_geometry_to_obj(
    const Geometry              &self,
    const std::filesystem::path &file_name,
    const std::string           &image_base_name,
    const graphics::Tim         &tim)
{
  create_directory_if_needed(file_name.parent_path());
  const std::filesystem::path mtl_name = create_mtl_file(self, file_name, image_base_name);
  std::ofstream obj_file(file_name);
  obj_file << "mtllib " << mtl_name.filename().string() << std::endl;
  std::cout << "saving " << file_name.string() << std::endl;
  std::cout << "saving " << file_name.string() << std::endl;
  write_vertices_to_obj(self, obj_file);
  write_uvs_to_obj(obj_file, self, tim);
  write_quads_to_obj(
    obj_file,
    self,
    image_base_name,
    write_triangles_to_obj(obj_file, self, image_base_name));
  obj_file.close();
}

/**
 * @brief Exports battle stage X file geometry data to an OBJ file.
 * @param self The battle stage X object containing the model data.
 */
inline void
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
}// namespace open_viii::battle::stage::StageToObj
#endif// OPENVIII_CPP_WIP_STAGETOOBJ_HPP
