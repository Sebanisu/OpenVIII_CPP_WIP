//
// Created by pcvii on 3/28/2023.
//

#ifndef OPENVIII_CPP_WIP_STAGETOOBJ_HPP
#define OPENVIII_CPP_WIP_STAGETOOBJ_HPP
#include "DatFile.hpp"
#include "open_viii/archive/ZZZ.hpp"
#include <filesystem>
#include <iostream>
#include <optional>
namespace open_viii::battle::dat::DatToObj {

/*
 * begin stupid hacks to fix Rinoa and Ward in remaster. might break when going
 * above 3X upscale if you choose to upscale more crop off dead bottom 128
 * pixels from SE's render. also hack adjusts aspect ratio to handle the eye
 * closing animation frames. in remaster you just need to shift all the uv's to
 * the right when eyes close.
 */
graphics::Point<float>
  calculate_dimensions(
    const std::uint32_t classicWidth,
    const std::uint32_t classicHeight,
    const std::uint32_t width,
    const std::uint32_t height)
{
  const auto x = static_cast<float>(width) / static_cast<float>(classicWidth);
  const auto y = static_cast<float>(height) / static_cast<float>(classicHeight);

  const float scaledHeight = [=]() {
    if (x > y) {
      return static_cast<float>(height) / std::floor(y);
    }
    else {
      return static_cast<float>(height) / std::floor(x);
    }
  }();

  const float newAR = static_cast<float>(width) / static_cast<float>(height);
  const float oldAR = static_cast<float>(classicWidth) / scaledHeight;

  const float adjustedWidth = [=]() {
    if (std::abs(newAR - oldAR) < std::numeric_limits<float>::epsilon()) {
      return static_cast<float>(classicWidth);
    }
    else {
      return scaledHeight * newAR;
    }
  }();

  return { adjustedWidth, scaledHeight };
}

graphics::Point<float>
  calculate_scaling_factors(
    const std::uint32_t classicWidth,
    const std::uint32_t classicHeight,
    const std::uint32_t width,
    const std::uint32_t height)
{
  const auto x = static_cast<float>(width) / static_cast<float>(classicWidth);
  const auto y = static_cast<float>(height) / static_cast<float>(classicHeight);

  const float scaledHeight = [=]() {
    if (x > y) {
      return static_cast<float>(height) / std::floor(y);
    }
    else {
      return static_cast<float>(height) / std::floor(x);
    }
  }();

  const float newAR = static_cast<float>(width) / static_cast<float>(height);
  const float oldAR = static_cast<float>(classicWidth) / scaledHeight;

  const float adjustedWidth = [=]() {
    if (std::abs(newAR - oldAR) < std::numeric_limits<float>::epsilon()) {
      return static_cast<float>(classicWidth);
    }
    else {
      return scaledHeight * newAR;
    }
  }();

  const float uScalingFactor = adjustedWidth / static_cast<float>(classicWidth);
  const float vScalingFactor = scaledHeight / static_cast<float>(classicHeight);

  return { uScalingFactor, vScalingFactor };
}

std::optional<std::uint8_t>
  extractNumberFromStem(const std::string &fileStem)
{
  // Find the last underscore in the string
  std::size_t pos = fileStem.rfind('_');

  // Check if an underscore was found
  if (pos == std::string::npos) {
    return std::nullopt;
  }

  // Extract the substring after the last underscore
  std::string numberStr = fileStem.substr(pos + 1);

  // Check if the substring consists only of digits
  if (
    numberStr.empty()
    || !std::all_of(numberStr.begin(), numberStr.end(), ::isdigit)) {
    return std::nullopt;
  }

  // Convert the substring to an integer
  try {
    return static_cast<std::uint8_t>(std::stoi(numberStr));
  }
  catch (const std::invalid_argument &) {
    return std::nullopt;
  }
  catch (const std::out_of_range &) {
    return std::nullopt;
  }
}

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
    int                texture_id,
    bool               remaster)
{
  output << "newmtl " << basename << "_" << texture_id << std::endl;
  if (remaster) {
    std::string hd_name = basename + "_" + std::to_string(texture_id) + ".png";
    output << "map_Kd " << hd_name << std::endl;
    return;
  }
  std::string name = basename + "_" + std::to_string(texture_id) + "_0_dat.png";
  output << "map_Kd " << name << std::endl;
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
  write_vertices_to_obj(const ObjectData &self, std::ofstream &obj_file)
{
  const float scale = 128.F;
  for (const auto &vertice_data : self.vertice_datas) {
    for (const auto vertice : vertice_data.vertices) {
      obj_file << "v " << static_cast<float>(vertice.y()) / scale << " "
               << static_cast<float>(-vertice.z()) / scale << " "
               << static_cast<float>(vertice.x()) / scale << "\n";
    }
  }
}

[[nodiscard]] inline auto
  convert_uv(
    const graphics::Point<std::uint8_t> uv,
    const graphics::Point<float>       &image_dims,
    [[maybe_unused]] const bool         remaster) -> graphics::Point<float>
{
  // on images that aren't square you have to split the uv in half I think.
  // two images have glitches extra garbage
  return { static_cast<float>(uv.x()) / image_dims.x(),
           image_dims.y()// flip the Y coord.
             - (static_cast<float>(uv.y()) / image_dims.y()) };
}

inline void
  write_uvs_to_obj(
    std::ofstream                                   &obj_file,
    const ObjectData                                &self,
    const std::vector<graphics::Tim>                &tims,
    const std::vector<std::optional<graphics::Png>> &pngs)
{// Write UVs
  for (const auto &triangle : self.triangles) {
    //    if (std::ranges::all_of(triangle.uvs(), [](const auto& uv)-> bool
    //    {return uv == decltype(uv){}; }))
    //    {
    //        continue;
    //    }
    for (const auto uv : triangle.uvs()) {
      std::uint8_t texture_id = triangle.texture_id();
      if (texture_id >= pngs.size()) {
        continue;// todo is this an error?
      }
      const auto new_uv = [&]() {
        if (pngs[texture_id]) {
          const auto &tim = tims[texture_id];
          const auto &png = pngs[texture_id];
          return convert_uv(
                   uv,
                   { static_cast<float>(tim.width()),
                     static_cast<float>(tim.height()) },
                   false)
               * calculate_scaling_factors(
                   tim.width(),
                   tim.height(),
                   png->width(),
                   png->height());
        }
        else {
          const auto &tim = tims[texture_id];
          return convert_uv(
            uv,
            { static_cast<float>(tim.width()),
              static_cast<float>(tim.height()) },
            false);
        }
      }();
      obj_file << "vt " << new_uv.x() << " " << new_uv.y() << "\n";
    }
  }
  for (const auto &quad : self.quads) {
    // if (std::ranges::all_of(quad.uvs(), [](const auto& uv)-> bool {return uv
    // == decltype(uv){}; }))
    //{
    //     continue;
    // }
    for (const auto uv : quad.uvs()) {

      std::uint8_t texture_id = quad.texture_id();
      if (texture_id >= pngs.size()) {
        continue;// todo is this an error?
      }
      const auto new_uv = [&]() {
        if (pngs[texture_id]) {
          const auto &tim = tims[texture_id];
          const auto &png = pngs[texture_id];
          return convert_uv(
                   uv,
                   { static_cast<float>(tim.width()),
                     static_cast<float>(tim.height()) },
                   false)
               * calculate_scaling_factors(
                   tim.width(),
                   tim.height(),
                   png->width(),
                   png->height());
        }
        else {
          const auto &tim = tims[texture_id];
          return convert_uv(
            uv,
            { static_cast<float>(tim.width()),
              static_cast<float>(tim.height()) },
            false);
        }
      }();
      obj_file << "vt " << new_uv.x() << " " << new_uv.y() << "\n";
    }
  }
}

inline void
  write_texture_ids_to_mtl(
    std::ofstream                                   &mtl_file,
    const std::string                               &image_base_name,
    const std::vector<std::optional<graphics::Png>> &pngs)
{
  for (std::uint8_t i{}; const auto &png : pngs) {
    write_material_mtl(mtl_file, image_base_name, i++, png.has_value());
  }
}

inline void
  write_triangle_to_obj(
    std::ofstream             &obj_file,
    const DatTriangle         &triangle,
    std::array<std::size_t, 3> uv_index)
{
  // if (std::ranges::any_of(triangle.indeces(), [](std::uint16_t index) {return
  // index == 0; }))
  //{
  //     return;
  // }
  obj_file << "f " << triangle.face_indice<0>() + 1 << "/" << uv_index[0] << " "
           << triangle.face_indice<1>() + 1 << "/" << uv_index[1] << " "
           << triangle.face_indice<2>() + 1 << "/" << uv_index[2] << "\n";
};
inline auto
  write_triangles_to_obj(
    std::ofstream     &obj_file,
    const ObjectData  &self,
    const std::string &image_base_name) -> std::size_t
{
  std::size_t uv_index = { 1 };// Write triangle faces
  for (const auto &triangle : self.triangles) {
    // if (std::ranges::all_of(triangle.uvs(), [](const auto& uv)-> bool {return
    // uv == decltype(uv){}; }))
    //{
    //     continue;
    // }
    write_material_obj(obj_file, image_base_name, triangle.texture_id());
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
    const ObjectData  &self,
    const std::string &image_base_name,
    std::size_t        uv_index)
{// Write quad faces
  for (const auto &quad : self.quads) {

    // if (std::ranges::all_of(quad.uvs(), [](const auto& uv)-> bool {return uv
    // == decltype(uv){}; }))
    //{
    //     continue;
    // }
    write_material_obj(obj_file, image_base_name, quad.texture_id());
    const auto triangles = quad.get_triangles();
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
    const std::filesystem::path                     &obj_file_name,
    const std::string                               &image_base_name,
    const std::vector<std::optional<graphics::Png>> &pngs)
    -> std::filesystem::path
{
  auto mtl_name = obj_file_name;
  mtl_name.replace_extension(".mtl");
  std::ofstream mtl_file(mtl_name);
  write_texture_ids_to_mtl(mtl_file, image_base_name, pngs);
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
    const ObjectData                                &self,
    const std::filesystem::path                     &file_name,
    const std::string                               &image_base_name,
    const std::vector<graphics::Tim>                &tims,
    const std::vector<std::optional<graphics::Png>> &pngs)
{
  create_directory_if_needed(file_name.parent_path());
  const std::filesystem::path mtl_name
    = create_mtl_file(file_name, image_base_name, pngs);
  std::ofstream obj_file(file_name);
  obj_file << "mtllib " << mtl_name.filename().string() << std::endl;
  std::cout << "saving " << file_name.string() << std::endl;
  write_vertices_to_obj(self, obj_file);
  write_uvs_to_obj(obj_file, self, tims, pngs);
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
  export_dat_to_obj(
    const DatFile                     &self,
    const std::optional<archive::ZZZ> &main_zzz)
{
  std::vector<std::optional<graphics::Png>> pngs = {};
  pngs.resize(self.section_11().m_tims.size());
  const auto dat_path = std::filesystem::path(self.path());
  const auto parent   = dat_path.parent_path();
  const auto stem     = dat_path.stem().string();
  const auto ext      = dat_path.extension().string();
  // todo check for remaster png images here.
  const auto hd_battle_path
    = std::filesystem::path("textures") / "battle.fs" / "hd_new";
  const auto out_path = std::filesystem::path("tmp") / parent;
  create_directory_if_needed(out_path);
  if (main_zzz) {
    for (const auto &main_zzz_fetch : main_zzz.value()) {

      const auto &file_path = main_zzz_fetch.get_file_info().get_path();
      if (
        !file_path.has_extension() || file_path.extension().string() != ".png"
        || !file_path.has_stem() || !file_path.stem().string().starts_with(stem)
        || (file_path.string().find(hd_battle_path.string()) == std::string::npos)) {
        continue;
      }
      const auto out_name = out_path / file_path.filename();
      open_viii::tools::write_buffer(
        main_zzz_fetch.get(),
        out_name.string(),
        "");
      if (const auto maybe_index
          = extractNumberFromStem(file_path.stem().string());
          maybe_index && maybe_index.value() < pngs.size()) {
        pngs[maybe_index.value()] = graphics::Png(out_name);
      }
    }
  }

  for (int                             i = 0;
       const open_viii::graphics::Tim &tim : self.section_11().m_tims) {
    if (tim.check()) {
      std::string output_filename{};
      output_filename.reserve(
        stem.size() + 1 + ext.size()
        + 10);// Optional: reserve space for string, assuming max 10 digits for
              // index
      output_filename.append(stem)
        .append(1, '_')
        .append(std::to_string(i))
        .append(ext);
      std::filesystem::path const output_path = parent / output_filename;
      tim.save(output_path.string());
    }
    ++i;
  }

  for (std::size_t i{};
       const auto &object_data : self.section_2().object_data) {
    std::string output_filename{};
    output_filename.reserve(
      stem.size() + 1 + 4
      + 10);// Optional: reserve space for string, assuming max 10 digits for
            // index
    output_filename.append(stem)
      .append(1, '_')
      .append(std::to_string(i))
      .append(".obj");
    export_geometry_to_obj(
      object_data,
      out_path / output_filename,
      stem,
      self.section_11().m_tims,
      pngs);
    ++i;
  }
}
}// namespace open_viii::battle::dat::DatToObj
#endif// OPENVIII_CPP_WIP_STAGETOOBJ_HPP
