//
// Created by pcvii on 3/28/2023.
//

#ifndef OPENVIII_CPP_WIP_DATTOOBJ_HPP
#define OPENVIII_CPP_WIP_DATTOOBJ_HPP
#include "open_viii/archive/ZZZ.hpp"
#include "open_viii/battle/dat/DatFile.hpp"
#include <filesystem>
#include <iostream>
#include <optional>
namespace open_viii::battle::dat::DatToObj {
/**
 * @brief Calculates the scaling factors for texture coordinates to fix specific
 * issues in the remastered version of Final Fantasy VIII characters, such as
 * Rinoa and Ward.
 *
 * This function calculates the scaling factors for the U and V coordinates of
 * the texture based on the classic texture dimensions and the new texture
 * dimensions. It adjusts the aspect ratio to handle specific animations like
 * the eye-closing frames. It is designed to work with up to 3X upscale and may
 * require modifications for higher upscales.
 *
 * @param classicWidth The width of the classic texture.
 * @param classicHeight The height of the classic texture.
 * @param width The width of the new texture.
 * @param height The height of the new texture.
 * @return A graphics::Point<float> representing the U and V scaling factors.
 */
graphics::Point<float>
  calculate_scaling_factors(
    const std::uint32_t classicWidth,
    const std::uint32_t classicHeight,
    const std::uint32_t width,
    const std::uint32_t height)
{
  const auto x_scale
    = static_cast<float>(width) / static_cast<float>(classicWidth);
  const auto y_scale
    = static_cast<float>(height) / static_cast<float>(classicHeight);

  const float scaledHeight = [=]() {
    if (x_scale > y_scale) {
      return static_cast<float>(height) / std::floor(y_scale);
    }
    else {
      return static_cast<float>(height) / std::floor(x_scale);
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

/**
 * @brief Extracts an optional number from the stem of a given file name.
 *
 * This function searches for the last underscore in the file stem and attempts
 * to extract a number following the underscore. If the substring following the
 * last underscore consists only of digits, it converts the substring to a
 * std::uint8_t. If no number is found or the conversion is not possible, the
 * function returns an empty std::optional.
 *
 * @param fileStem The file stem to extract the number from.
 * @return An std::optional<std::uint8_t> containing the extracted number, if
 * present and valid.
 */
std::optional<std::uint8_t>
  extractNumberFromStem(const std::string &fileStem)
{
  // Find the last underscore in the string
  std::size_t const pos = fileStem.rfind('_');

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
 * @brief Writes material information to an output MTL file, specifying the
 * material name and texture map.
 *
 * This function writes a new material entry to the provided MTL file stream. It
 * generates the material name using the given basename and texture ID, and
 * creates a texture map entry depending on whether the input material is from
 * the remastered version or not.
 *
 * @param output An output file stream for the MTL file to which the material
 * information will be written.
 * @param basename The base name of the material, used to generate the material
 * name and texture map entry.
 * @param texture_id The texture ID for the material, used to generate the
 * material name and texture map entry.
 * @param remaster A boolean flag indicating if the material is from the
 * remastered version of the game.
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
    std::string const hd_name
      = basename + "_" + std::to_string(texture_id) + ".png";
    output << "map_Kd " << hd_name << '\n';
    return;
  }
  std::string const name
    = basename + "_" + std::to_string(texture_id) + "_0_dat.png";
  output << "map_Kd " << name << '\n';
}

/**
 * @brief Writes material usage information to an output OBJ file, specifying
 * the material to be used.
 *
 * This function writes a material usage entry to the provided OBJ file stream.
 * It generates the material usage command using the given basename and palette
 * ID, which helps assign the corresponding material to the subsequent geometry
 * in the OBJ file.
 *
 * @param output An output file stream for the OBJ file to which the material
 * usage information will be written.
 * @param basename The base name of the material, used to generate the material
 * usage command.
 * @param palette_id The palette ID for the material, used to generate the
 * material usage command.
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
 * @brief Creates the necessary directory structure for a given file path, if it
 * does not already exist.
 *
 * This function checks if the directories for the specified file path exist,
 * and creates them if needed. It also handles error checking and outputs error
 * messages to the standard error stream in case of issues.
 *
 * @param file_name The output file path for which the directory structure needs
 * to be created.
 */
inline void
  create_directory_if_needed(const std::filesystem::path &file_name)
{
  std::error_code error_code{};
  std::filesystem::create_directories(file_name.parent_path(), error_code);
  if (error_code) {
    std::cout << std::flush;
    std::cerr << __FILE__ << ":" << __LINE__ << " - " << error_code.value()
              << ": " << error_code.message() << " - " << file_name << '\n';
    error_code.clear();
  }
}

/**
 * @brief Writes vertex data from a Geometry object to an output OBJ file.
 *
 * This function iterates through the vertices in a given ObjectData instance
 * and writes the vertex data to the provided output OBJ file stream. The vertex
 * data is scaled and transformed as required before being written to the file.
 *
 * @param self The ObjectData object containing the model data.
 * @param obj_file The output OBJ file stream to which the vertex data will be
 * written.
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
 * @param height The height of the tim texture.
 * @return The converted V coordinate as a std::uint8_t.
 */
[[nodiscard]] inline auto
  convert_v(const std::uint8_t v_cord, const std::uint32_t tim_height)
    -> std::uint8_t
{
  // on the psx they are probably stacking these textures in a vtable of
  // 128x256 pixels.
  constexpr std::uint8_t ninty_six        = 96;
  constexpr std::uint8_t one_twenty_eight = 128;
  if (tim_height >= ninty_six) {
    // if height is 128, and texture id is odd (1,3,5), range is 128-255
    // if height is 128, and texture id is even (0,2,4), range is 0-127.
    // if height is 96 or 97, and texture id is odd (probably 1), range is
    // 128-223
    return v_cord % one_twenty_eight;
  }
  // if height is 32 and texture id is even (probably 0), range is 96-127
  return v_cord - ninty_six;
}

/**
 * @brief Converts the UV coordinates for battle models in Final Fantasy VIII
 *        based on the texture's dimensions and format (TIM or PNG).
 *
 * The U coordinate ranges between 0 and 127, while the V coordinate is
 * calculated based on the texture's height. There is a noted correlation
 * between V values and texture ID for all heights, with even texture IDs
 * corresponding to V values in the range of 0-127 and odd texture IDs in the
 * range of 128-255. For height 96 or 97, the V range is 128 to 223, and
 * for height 32, the V range is 96 to 127.
 *
 * @param raw_uv The raw UV coordinates as a graphics::Point<std::uint8_t>.
 * @param tim The TIM texture to consider for the conversion.
 * @param png The optional PNG texture to consider for the conversion (if
 * available).
 * @return The converted UV coordinates as a graphics::Point<float>.
 */
graphics::Point<float>
  convert_uv(
    const graphics::Point<std::uint8_t> &raw_uv,
    const graphics::Tim                 &tim,
    const std::optional<graphics::Png>  &png)
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

/**
 * @brief Writes UV data from an ObjectData instance and associated textures to
 * an output CSV file stream.
 *
 * This function iterates through the UV data in a given ObjectData instance and
 * writes the associated data, including texture ID, raw and converted UV
 * coordinates, scaling factors, TIM and PNG dimensions, and width and height
 * scale ratios, to the provided output CSV file stream.
 *
 * @param csv_file The output CSV file stream to which the UV data will be
 * written.
 * @param self The ObjectData object containing the model data.
 * @param tims A vector of graphics::Tim objects representing the original
 * textures.
 * @param pngs A vector of std::optional<graphics::Png> objects representing the
 * remastered textures.
 */
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
       "Factor U,Scale Factor V,TIM Width,TIM Height,PNG Width,PNG "
       "Height,Width Scale, Height Scale\n";

  auto write_uv_data = [&](const auto &primitive) {
    for (const auto uv_cord : primitive.uvs()) {
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
      const auto &png        = pngs[texture_id];
      const auto  new_uv     = convert_uv(uv_cord, tim, png);
      const auto  png_width  = png ? png->width() : 0;
      const auto  png_height = png ? png->height() : 0;
      csv_file << +texture_id << "," << +uv_cord.x() << "," << +uv_cord.y()
               << "," << +convert_v(uv_cord.y(), tim.height()) << ","
               << new_uv.x() << "," << new_uv.y() << "," << scale_factor.x()
               << "," << scale_factor.y() << "," << tim.width() << ","
               << tim.height() << "," << png_width << "," << png_height << ","
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

/**
 * @brief Writes UV data from an ObjectData instance and associated textures to
 * an output CSV file.
 *
 * This function creates and opens a CSV file with the specified file name, and
 * then calls the write_uvs_to_csv function to write the UV data from the given
 * ObjectData instance and associated textures to the CSV file.
 *
 * @param csv_filename The file name of the output CSV file.
 * @param self The ObjectData object containing the model data.
 * @param tims A vector of graphics::Tim objects representing the original
 * textures.
 * @param pngs A vector of std::optional<graphics::Png> objects representing the
 * remastered textures.
 */
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

/**
 * @brief Writes the UV data from an ObjectData instance to an output OBJ file
 * stream.
 *
 * This function iterates through the UV data in the given ObjectData instance
 * and writes the associated converted UV coordinates to the provided output OBJ
 * file stream.
 *
 * @param obj_file The output OBJ file stream to which the UV data will be
 * written.
 * @param self The ObjectData object containing the model data.
 * @param tims A vector of graphics::Tim objects representing the original
 * textures.
 * @param pngs A vector of std::optional<graphics::Png> objects representing the
 * remastered textures.
 */
inline void
  write_uvs_to_obj(
    std::ofstream                                   &obj_file,
    const ObjectData                                &self,
    const std::vector<graphics::Tim>                &tims,
    const std::vector<std::optional<graphics::Png>> &pngs)
{// Write UVs
  for (const auto &triangle : self.triangles) {
    for (const auto uv_cord : triangle.uvs()) {
      std::uint8_t const texture_id = triangle.texture_id();
      if (texture_id >= pngs.size()) {
        continue;// todo is this an error?
      }
      const auto &tim    = tims[texture_id];
      const auto &png    = pngs[texture_id];
      const auto  new_uv = convert_uv(uv_cord, tim, png);
      obj_file << "vt " << new_uv.x() << " " << new_uv.y() << "\n";
    }
  }
  for (const auto &quad : self.quads) {
    for (const auto uv_cord : quad.uvs()) {

      std::uint8_t const texture_id = quad.texture_id();
      if (texture_id >= pngs.size()) {
        continue;// todo is this an error?
      }
      const auto &tim         = tims[texture_id];
      const auto &png         = pngs[texture_id];
      const auto  new_uv_cord = convert_uv(uv_cord, tim, png);
      obj_file << "vt " << new_uv_cord.x() << " " << new_uv_cord.y() << "\n";
    }
  }
}

/**
 * @brief Writes texture IDs to an MTL file stream.
 *
 * This function writes the material information for each texture in the pngs
 * vector to the provided output MTL file stream. It calls the
 * write_material_mtl function to write the material data for each texture.
 *
 * @param mtl_file The output MTL file stream to which the texture IDs will be
 * written.
 * @param image_base_name The base name of the image files.
 * @param pngs A vector of std::optional<graphics::Png> objects representing the
 * remastered textures.
 */
inline void
  write_texture_ids_to_mtl(
    std::ofstream                                   &mtl_file,
    const std::string                               &image_base_name,
    const std::vector<std::optional<graphics::Png>> &pngs)
{
  for (std::uint8_t index{}; const auto &png : pngs) {
    write_material_mtl(mtl_file, image_base_name, index++, png.has_value());
  }
}

/**
 * @brief Writes a single triangle face to an OBJ file stream.
 *
 * This function writes the vertex indices and associated UV indices of the
 * input triangle face to the output OBJ file stream.
 *
 * @param obj_file The output OBJ file stream to which the triangle face will be
 * written.
 * @param triangle The DatTriangle object representing the input triangle face.
 * @param uv_index An array containing the UV indices associated with the
 * triangle face.
 */
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

/**
 * @brief Writes all triangle faces from an ObjectData instance to an OBJ file
 * stream.
 *
 * This function iterates through the triangles in the given ObjectData instance
 * and writes the associated triangle faces and material information to the
 * provided output OBJ file stream.
 *
 * @param obj_file The output OBJ file stream to which the triangle faces will
 * be written.
 * @param self The ObjectData object containing the model data.
 * @param image_base_name The base name of the image files.
 * @return The index of the last written UV data.
 */
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

/**
 * @brief Writes all quad faces from an ObjectData instance to an OBJ file
 * stream.
 *
 * This function iterates through the quads in the given ObjectData instance and
 * writes the associated quad faces (as triangles) and material information to
 * the provided output OBJ file stream.
 *
 * @param obj_file The output OBJ file stream to which the quad faces will be
 * written.
 * @param self The ObjectData object containing the model data.
 * @param image_base_name The base name of the image files.
 * @param uv_index The starting index for the UV data.
 */
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

/**
 * @brief Creates an MTL file and writes texture ID information to it.
 *
 * This function creates an MTL file using the provided OBJ file name, and
 * writes texture ID information to it using the given image base name and
 * vector of remastered textures.
 *
 * @param obj_file_name The name of the OBJ file associated with the MTL file to
 * be created.
 * @param image_base_name The base name of the image files.
 * @param pngs A vector of std::optional<graphics::Png> objects representing the
 * remastered textures.
 * @return The filesystem path of the created MTL file.
 */
inline auto
  create_mtl_file(
    const std::filesystem::path                     &obj_file_name,
    const std::string                               &image_base_name,
    const std::vector<std::optional<graphics::Png>> &pngs)
    -> std::filesystem::path
{
  auto mtl_name
    = std::filesystem::path(obj_file_name).replace_extension(".mtl");
  std::cout << "saving " << mtl_name.string() << '\n';
  std::ofstream mtl_file(mtl_name);
  write_texture_ids_to_mtl(mtl_file, image_base_name, pngs);
  return mtl_name;
}

/**
 * @brief Exports geometry data from an ObjectData instance to an OBJ file.
 *
 * This function exports the model data stored in the input ObjectData instance
 * to an OBJ file. It writes vertices, UVs, and face information to the output
 * file.
 *
 * @param self The ObjectData object containing the model data.
 * @param file_name The output file path for the OBJ file.
 * @param image_base_name The base name of the image files.
 * @param tims The vector of graphics::Tim objects containing texture
 * information.
 * @param pngs The vector of std::optional<graphics::Png> objects representing
 * the remastered textures.
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

/**
 * @brief Fetches PNG textures associated with a DatFile object.
 *
 * This function retrieves the PNG textures associated with the given DatFile
 * object and stores them in a vector of std::optional<graphics::Png> objects.
 *
 * @param self The DatFile object containing the model data.
 * @param main_zzz An optional archive::ZZZ object containing information about
 * the main texture archive.
 * @return A vector of std::optional<graphics::Png> objects representing the
 * retrieved PNG textures.
 */
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
 * @brief Exports battle stage DAT file geometry data to an OBJ file.
 *
 * This function exports the model data stored in the input battle stage DAT
 * file to an OBJ file. It writes vertices, UVs, and face information to the
 * output file.
 *
 * @param self The DatFile object containing the model data.
 * @param pngs The vector of std::optional<graphics::Png> objects representing
 * the remastered textures.
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
  const auto hd_battle_path
    = std::filesystem::path("textures") / "battle.fs" / "hd_new";
  const auto out_path = std::filesystem::path("tmp") / parent;
  create_directory_if_needed(out_path);
  // Optional: reserve space for string, assuming max 10 digits for index
  constexpr static std::uint8_t digits          = 10;
  std::string                   output_filename = {};
  output_filename.reserve(stem.size() + 1 + ext.size() + digits);
  for (int                             index = 0;
       const open_viii::graphics::Tim &tim : self.section_11().m_tims) {
    if (tim.check()) {
      output_filename.append(stem)
        .append(1, '_')
        .append(std::to_string(index))
        .append(ext);
      std::filesystem::path const output_path = parent / output_filename;
      tim.save(output_path.string());
    }
    ++index;
    output_filename.clear();
  }
  constexpr static std::string_view obj_ext = ".obj";
  for (std::size_t index{};
       const auto &object_data : self.section_2().object_data) {
    output_filename.append(stem)
      .append(1, '_')
      .append(std::to_string(index))
      .append(obj_ext);
    export_geometry_to_obj(
      object_data,
      out_path / output_filename,
      stem,
      self.section_11().m_tims,
      pngs);
    ++index;
    output_filename.clear();
  }
}
}// namespace open_viii::battle::dat::DatToObj
#endif// OPENVIII_CPP_WIP_DATTOOBJ_HPP
