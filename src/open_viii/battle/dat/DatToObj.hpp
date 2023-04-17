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
  output << "newmtl " << basename << "_" << texture_id << '\n';
  if (remaster) {
    std::string hd_name = basename + "_" + std::to_string(texture_id) + ".png";
    output << "map_Kd " << hd_name << '\n';
    return;
  }
  std::string name = basename + "_" + std::to_string(texture_id) + "_0_dat.png";
  output << "map_Kd " << name << '\n';
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
  output << "usemtl " << basename << "_" << palette_id << '\n';
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
              << ec.message() << " - " << file_name << '\n';
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
/**
 * @brief Convert the V coordinate for battle models in Final Fantasy VIII
 *        based on the texture's height.
 *
 * The U coordinate ranges between 0 and 127, while the V coordinate is
 * calculated based on the texture's height. There is a noted correlation
 * between V values and texture ID for all heights, with even texture IDs
 * corresponding to V values in the range of 0-127 and odd texture IDs in the
 * range of 128-255. For height 96 or 97, the V range is 128 to 223, and
 * for height 32, the V range is 96 to 127.
 *
 * @param v The V coordinate.
 * @param height The height of the texture.
 * @return The converted V coordinate as a std::uint8_t.
 */
[[nodiscard]] inline auto
  convert_v(const std::uint8_t v, const std::uint32_t height) -> std::uint8_t
{
  // on the psx they are probably stacking these textures in a vtable of
  // 128x256 pixels.
  if (height >= 96) {
    // if height is 128, and texture id is odd (1,3,5), range is 128-255
    // if height is 128, and texture id is even (0,2,4), range is 0-127.
    // if height is 96 or 97, and texture id is odd (probably 1), range is
    // 128-223
    return v % 128;
  }
  // if height is 32 and texture id is even (probably 0), range is 96-127
  return v - 96;
}
/**
 * @brief Convert the UV coordinates for battle models in Final Fantasy VIII
 *        based on the texture's height.
 *
 * The U coordinate ranges between 0 and 127, while the V coordinate is
 * calculated based on the texture's height. There is a noted correlation
 * between V values and texture ID for all heights, with even texture IDs
 * corresponding to V values in the range of 0-127 and odd texture IDs in the
 * range of 128-255. For height 96 or 97, the V range is 128 to 223, and
 * for height 32, the V range is 96 to 127.
 *
 * @param uv The UV coordinates as a graphics::Point<std::uint8_t>.
 * @param height The height of the texture.
 * @return The converted UV coordinates as a graphics::Point<float>.
 */
[[nodiscard]] inline auto
  convert_uv(const graphics::Point<std::uint8_t> uv, const std::uint32_t
  height)
    -> graphics::Point<float>
{
  constexpr auto max_u = 128;
  return { static_cast<float>(uv.x()) / static_cast<float>(max_u),
           1.F
             - static_cast<float>(convert_v(uv.y(), height))
                 / static_cast<float>(height) };
}

graphics::Point<float>
  convert_uv2(
    const graphics::Point<std::uint8_t> &raw_uv,
    const graphics::Tim                  &tim,
    const std::optional<graphics::Png>   &png)
{
  const auto scale_factor = [&]() {
    if (png) {
      return calculate_scaling_factors(
        tim.width(),
        tim.height(),
        png->width(),
        png->height());
    }
    return graphics::Point<float>{ 1.F, 1.F };
  }();

  const auto uv_scaled_before_divide_by_tim_dims
    = graphics::Point<float>(
        static_cast<float>(raw_uv.x()),
        static_cast<float>(convert_v(raw_uv.y(), tim.height())))
    / scale_factor;
  const auto tim_dims = graphics::Point<float>(
    static_cast<float>(tim.width()),
    static_cast<float>(tim.height()));
  const auto new_scaled_uv = uv_scaled_before_divide_by_tim_dims / tim_dims;
  const auto flipped_y
    = graphics::Point<float>(new_scaled_uv.x(), 1.F - new_scaled_uv.y());

  return flipped_y;
}

inline void
  write_uvs_to_csv(
    std::ofstream                                   &csv_file,
    const ObjectData                                &self,
    const std::vector<graphics::Tim>                &tims,
    const std::vector<std::optional<graphics::Png>> &pngs)
{

  if (!csv_file.is_open()) {
    std::cerr << "Failed to open CSV file for writing." << '\n';
    return;
  }

  // Write CSV header
  csv_file
    << "Texture ID,Raw U,Raw V,Corrected V,Converted U,Converted V,Scale "
       "Factor U,Scale Factor V,Scaled U before divide,Scaled V before "
       "divide,Scaled U,Scaled V,New Scaled U,New Scaled V,New Flipped Scaled "
       "U,New Flipped Scaled V,TIM Width,TIM Height,PNG Width,PNG "
       "Height,Width Scale, Height Scale\n";

  auto write_uv_data = [&](const auto &primitive) {
    for (const auto uv : primitive.uvs()) {
      std::uint8_t texture_id = primitive.texture_id();
      if (texture_id >= pngs.size()) {
        continue;// todo is this an error?
      }
      const auto &tim          = tims[texture_id];
      const auto  scale_factor = [&]() {
        if (pngs[texture_id]) {
          const auto &png = pngs[texture_id];
          return calculate_scaling_factors(
            tim.width(),
            tim.height(),
            png->width(),
            png->height());
        }
        return graphics::Point<float>{ 1.F, 1.F };
      }();
      const auto new_uv = convert_uv(uv, tim.height());

      const auto uv_scaled_before_divide_by_tim_dims
        = graphics::Point<float>(
            static_cast<float>(uv.x()),
            static_cast<float>(convert_v(uv.y(), tim.height())))
        / scale_factor;
      const auto tim_dims = graphics::Point<float>(
        static_cast<float>(tim.width()),
        static_cast<float>(tim.height()));
      const auto new_scaled_uv = uv_scaled_before_divide_by_tim_dims / tim_dims;
      const auto flipped_y
        = graphics::Point<float>(new_scaled_uv.x(), 1.F - new_scaled_uv.y());
      const auto scaled_uv  = new_uv / scale_factor;
      const auto png_width  = pngs[texture_id] ? pngs[texture_id]->width() : 0;
      const auto png_height = pngs[texture_id] ? pngs[texture_id]->height() : 0;
      csv_file << +texture_id << "," << +uv.x() << "," << +uv.y() << ","
               << +convert_v(uv.y(), tim.height()) << "," << new_uv.x() << ","
               << new_uv.y() << "," << scale_factor.x() << ","
               << scale_factor.y() << ","
               << uv_scaled_before_divide_by_tim_dims.x() << ","
               << uv_scaled_before_divide_by_tim_dims.y() << ","
               << scaled_uv.x() << "," << scaled_uv.y() << ","
               << new_scaled_uv.x() << "," << new_scaled_uv.y() << ","
               << flipped_y.x() << "," << flipped_y.y() << "," << tim.width()
               << "," << tim.height() << "," << png_width << "," << png_height
               << ","
               << static_cast<float>(png_width)
                    / static_cast<float>(tim.width())
               << ","
               << static_cast<float>(png_height)
                    / static_cast<float>(tim.height())
               << "\n";
    }
  };

  for (const auto &triangle : self.triangles) {
    write_uv_data(triangle);
  }

  for (const auto &quad : self.quads) {
    write_uv_data(quad);
  }
}

inline void
  write_uvs_to_csv(
    const std::filesystem::path                     &csv_filename,
    const ObjectData                                &self,
    const std::vector<graphics::Tim>                &tims,
    const std::vector<std::optional<graphics::Png>> &pngs)
{
  std::ofstream csv_file;
  csv_file.open(csv_filename);
  write_uvs_to_csv(csv_file, self, tims, pngs);
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
      const auto &tim    = tims[texture_id];
      const auto &png    = pngs[texture_id];
      const auto  new_uv = convert_uv2(uv, tim, png);
      //        [&]() {
      //        if (pngs[texture_id]) {
      //          return convert_uv(uv, tim.height())
      //               / calculate_scaling_factors(
      //                   tim.width(),
      //                   tim.height(),
      //                   png->width(),
      //                   png->height());
      //        }
      //        else {
      //          return convert_uv(uv, tim.height());
      //        }
      //      }();
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
      const auto &tim    = tims[texture_id];
      const auto &png    = pngs[texture_id];
      const auto  new_uv = convert_uv2(uv, tim, png);
//      const auto  new_uv = [&]() {
//        if (pngs[texture_id]) {
//          const auto &png = pngs[texture_id];
//          return convert_uv(uv, tim.height())
//               / calculate_scaling_factors(
//                   tim.width(),
//                   tim.height(),
//                   png->width(),
//                   png->height());
//        }
//        else {
//          return convert_uv(uv, tim.height());
//        }
//      }();
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
  const auto mtl_name
    = std::filesystem::path(obj_file_name).replace_extension(".mtl");
  std::cout << "saving " << mtl_name.string() << '\n';
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
  const std::filesystem::path csv_name
    = std::filesystem::path(file_name).replace_extension(".csv");
  std::ofstream obj_file(file_name);
  std::cout << "saving " << csv_name.string() << '\n';
  write_uvs_to_csv(csv_name, self, tims, pngs);
  obj_file << "mtllib " << mtl_name.filename().string() << '\n';
  std::cout << "saving " << file_name.string() << '\n';
  write_vertices_to_obj(self, obj_file);
  write_uvs_to_obj(obj_file, self, tims, pngs);
  write_quads_to_obj(
    obj_file,
    self,
    image_base_name,
    write_triangles_to_obj(obj_file, self, image_base_name));
  obj_file.close();
}

[[nodiscard]] std::vector<std::optional<graphics::Png>>
  fetch_pngs(const DatFile &self, const std::optional<archive::ZZZ> &main_zzz)
{
  std::vector<std::optional<graphics::Png>> pngs;
  pngs.resize(self.section_11().m_tims.size());
  const static auto hd_battle_path
    = std::filesystem::path("textures") / "battle.fs" / "hd_new";

  if (main_zzz) {

    const auto dat_path = std::filesystem::path(self.path());
    const auto parent   = dat_path.parent_path();
    const auto stem     = dat_path.stem().string();
    const auto out_path = std::filesystem::path("tmp") / parent;
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
  return pngs;
}

/**
 * @brief Exports battle stage X file geometry data to an OBJ file.
 * @param self The battle stage X object containing the model data.
 */
inline void
  export_dat_to_obj(
    const DatFile                                   &self,
    const std::vector<std::optional<graphics::Png>> &pngs)
{
  const auto dat_path = std::filesystem::path(self.path());
  const auto parent   = dat_path.parent_path();
  const auto stem     = dat_path.stem().string();
  const auto ext      = dat_path.extension().string();
  // todo check for remaster png images here.
  const auto hd_battle_path
    = std::filesystem::path("textures") / "battle.fs" / "hd_new";
  const auto out_path = std::filesystem::path("tmp") / parent;
  create_directory_if_needed(out_path);

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
      stem.size() + 1 + 4 + 10);// Optional: reserve space for string, assuming
                                // max 10 digits for index
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
