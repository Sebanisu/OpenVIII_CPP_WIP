/**
 * @file StageToObj.hpp
 * @brief Utilities for exporting FFVIII stage geometry to Wavefront OBJ.
 */
#ifndef OPENVIII_CPP_WIP_STAGETOOBJ_HPP
#define OPENVIII_CPP_WIP_STAGETOOBJ_HPP
#include "open_viii/battle/stage/Geometry.hpp"
#include "open_viii/battle/stage/X.hpp"
#include <filesystem>
#include <iostream>
/**
 * @namespace open_viii::battle::stage::StageToObj
 * @brief OBJ export helpers for FFVIII battle stage conversion.
 */
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
  std::error_code error_code{};
  std::filesystem::create_directories(file_name.parent_path(), error_code);
  if (error_code) {
    std::cout << std::flush;
    std::cerr << __FILE__ << ":" << __LINE__ << " - " << error_code.value()
              << ": " << error_code.message() << " - " << file_name
              << std::endl;
    error_code.clear();
  }
}

/**
 * @brief Write vertices to the OBJ file.
 * @param stage The Geometry object containing the model data.
 * @param obj_file The output OBJ file stream.
 */
inline void
  write_vertices_to_obj(const Geometry &stage, std::ofstream &obj_file)
{
  const float scale = 128.F;
  for (const auto vertice : stage.vertices) {
    obj_file << "v " << static_cast<float>(vertice.x) / scale << " "
             << static_cast<float>(vertice.y) / scale << " "
             << static_cast<float>(vertice.z) / scale << "\n";
  }
}

/**
 * @brief Converts TIM texture UV coordinates into normalized OBJ UV coordinates.
 *
 * Applies texture-page offsets and converts PlayStation TIM coordinates into
 * normalized floating-point UV coordinates suitable for Wavefront OBJ export.
 * The V coordinate is vertically flipped to match OBJ texture orientation.
 *
 * @param uv_cord Original TIM UV coordinates.
 * @param texture_page Texture page index used by the polygon.
 * @param tim Source TIM texture metadata.
 *
 * @return Normalized OBJ UV coordinates.
 */
[[nodiscard]] inline auto
  convert_uv(
    const graphics::Point<std::uint8_t> uv_cord,
    const std::uint8_t                  texture_page,
    const graphics::Tim                &tim) -> graphics::Point<float>
{
  const float texPageWidth = 128.F;

  return {
    ((static_cast<float>(uv_cord.x())
      + (static_cast<float>(texture_page) * texPageWidth)))
      / static_cast<float>(tim.width()),
    static_cast<float>(tim.height())// flip the Y coord.
      - (static_cast<float>(uv_cord.y()) / static_cast<float>(tim.height()))
  };
}

/**
 * @brief Writes normalized UV coordinates to an OBJ file.
 *
 * Exports UV coordinates for all triangle and quad polygons contained in the
 * stage geometry as Wavefront OBJ texture vertices (`vt`).
 *
 * @param obj_file Output OBJ file stream.
 * @param stage Geometry containing textured polygons.
 * @param tim Source TIM texture metadata.
 */
inline void
  write_uvs_to_obj(
    std::ofstream       &obj_file,
    const Geometry      &stage,
    const graphics::Tim &tim)
{// Write UVs
  for (const auto &triangle : stage.triangles) {
    for (const auto uv_cord : triangle.uvs()) {
      const auto new_uv = convert_uv(uv_cord, triangle.texture_page(), tim);
      obj_file << "vt " << new_uv.x() << " " << new_uv.y() << "\n";
    }
  }
  for (const auto &quad : stage.quads) {
    for (const auto uv_cord : quad.uvs()) {
      const auto new_uv = convert_uv(uv_cord, quad.texture_page(), tim);
      obj_file << "vt " << new_uv.x() << " " << new_uv.y() << "\n";
    }
  }
}

/**
 * @brief Writes material definitions for all unique CLUT values.
 *
 * Collects unique CLUT identifiers from triangle and quad polygons and writes
 * corresponding material definitions to the MTL output stream.
 *
 * @param mtl_file Output MTL file stream.
 * @param stage Geometry containing textured polygons.
 * @param image_base_name Base filename used for generated texture references.
 */
inline void
  write_cluts_to_mtl(
    std::ofstream     &mtl_file,
    const Geometry    &stage,
    const std::string &image_base_name)
{
  std::vector<uint8_t> cluts{};
  for (const auto &tmp : stage.triangles) {
    cluts.push_back(tmp.clut());
  }
  for (const auto &tmp : stage.quads) {
    cluts.push_back(tmp.clut());
  }
  const auto remove_range = std::ranges::unique(cluts);
  cluts.erase(remove_range.begin(), remove_range.end());
  std::ranges::sort(cluts);
  for (auto clut : cluts) {
    write_material_mtl(mtl_file, image_base_name, clut);
  }
}

/**
 * @brief Writes a single triangle face entry to an OBJ file.
 *
 * Writes a Wavefront OBJ face (`f`) entry using vertex indices from the
 * triangle geometry and the supplied UV coordinate indices.
 *
 * @param obj_file Output OBJ file stream.
 * @param triangle Triangle geometry to export.
 * @param uv_index OBJ texture coordinate indices associated with the triangle.
 */
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

/**
 * @brief Writes all triangle polygons to an OBJ file.
 *
 * Exports triangle faces from the stage geometry and assigns materials based
 * on CLUT values. UV indices are generated sequentially to match previously
 * exported texture coordinates.
 *
 * @param obj_file Output OBJ file stream.
 * @param stage Geometry containing triangle polygons.
 * @param image_base_name Base material name used for OBJ material references.
 *
 * @return Next available OBJ UV index after triangle export.
 */
inline auto
  write_triangles_to_obj(
    std::ofstream     &obj_file,
    const Geometry    &stage,
    const std::string &image_base_name) -> std::size_t
{
  std::size_t uv_index = { 1 };// Write triangle faces
  for (const auto &triangle : stage.triangles) {
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

/**
 * @brief Writes all quad polygons to an OBJ file.
 *
 * Converts quads into triangle pairs and exports them as Wavefront OBJ face
 * entries. Materials are assigned using CLUT values associated with each quad.
 *
 * @param obj_file Output OBJ file stream.
 * @param stage Geometry containing quad polygons.
 * @param image_base_name Base material name used for OBJ material references.
 * @param uv_index Starting OBJ UV index for quad texture coordinates.
 */
inline void
  write_quads_to_obj(
    std::ofstream     &obj_file,
    const Geometry    &stage,
    const std::string &image_base_name,
    std::size_t        uv_index)
{// Write quad faces
  for (const auto &quad : stage.quads) {
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


/**
 * @brief Creates an MTL file for exported stage geometry.
 *
 * Generates a material library file alongside the OBJ output and writes
 * material definitions for all unique CLUT values used by the geometry.
 *
 * @param stage Geometry containing textured polygons.
 * @param obj_file_name Target OBJ filename.
 * @param image_base_name Base filename used for material texture references.
 *
 * @return Path to the generated MTL file.
 */
inline auto
  create_mtl_file(
    const Geometry              &stage,
    const std::filesystem::path &obj_file_name,
    const std::string           &image_base_name) -> std::filesystem::path
{
  auto mtl_name = obj_file_name;
  mtl_name.replace_extension(".mtl");
  std::ofstream mtl_file(mtl_name);
  write_cluts_to_mtl(mtl_file, stage, image_base_name);
  return mtl_name;
}

/**
 * @brief Exports stage geometry as a Wavefront OBJ model.
 *
 * Creates the required output directory structure, generates a companion MTL
 * material library, and writes vertices, UV coordinates, and polygon faces to
 * the OBJ file. Geometry is exported using texture and CLUT information from
 * the supplied TIM texture data.
 *
 * @param stage Geometry to export.
 * @param file_name Destination OBJ file path.
 * @param image_base_name Base filename used for generated material references.
 * @param tim TIM texture metadata and image data used for UV normalization.
 */
inline void
  export_geometry_to_obj(
    const Geometry              &stage,
    const std::filesystem::path &file_name,
    const std::string           &image_base_name,
    const graphics::Tim         &tim)
{
  create_directory_if_needed(file_name.parent_path());
  const std::filesystem::path mtl_name
    = create_mtl_file(stage, file_name, image_base_name);
  std::ofstream obj_file(file_name);
  obj_file << "mtllib " << mtl_name.filename().string() << std::endl;
  std::cout << "saving " << file_name.string() << std::endl;
  std::cout << "saving " << file_name.string() << std::endl;
  write_vertices_to_obj(stage, obj_file);
  write_uvs_to_obj(obj_file, stage, tim);
  write_quads_to_obj(
    obj_file,
    stage,
    image_base_name,
    write_triangles_to_obj(obj_file, stage, image_base_name));
  obj_file.close();
}

/**
 * @brief Exports all battle stage geometries from an X archive to OBJ files.
 *
 * Iterates through all geometry groups and nested models contained within the
 * battle stage archive, exporting each model as an individual Wavefront OBJ
 * file. Associated TIM textures are optionally written to disk before geometry
 * export begins.
 *
 * Output filenames are generated using the source stage filename combined with
 * geometry and model indices.
 *
 * @param stage Battle stage archive containing geometry and texture data.
 */
inline void
  export_x_to_obj(const X &stage)
{
  if (stage.tim().check()) {
    stage.tim().save(stage.path());
  }
  std::string                       out_string = {};
  static constexpr std::string_view obj_ext    = ".obj";
  constexpr std::uint32_t           digits     = 10;
  const std::filesystem::path       x_path = std::filesystem::path(stage.path());
  out_string.reserve(
    x_path.stem().string().size() + 2 + (digits * 2) + obj_ext.size());
  for (std::size_t geo_index{}; const auto &geometries : stage.geometries()) {
    for (std::size_t model_index{};
         const auto &model : geometries.nested_geometries) {
      out_string.append(x_path.stem().string())
        .append(1, '_')
        .append(std::to_string(geo_index))
        .append(1, '_')
        .append(std::to_string(model_index))
        .append(obj_ext);
      export_geometry_to_obj(
        model,
        std::filesystem::absolute("tmp" / x_path.parent_path()) / out_string,
        x_path.stem().string(),
        stage.tim());
      ++model_index;
      out_string.clear();
    }
    ++geo_index;
  }
}
}// namespace open_viii::battle::stage::StageToObj
#endif// OPENVIII_CPP_WIP_STAGETOOBJ_HPP
