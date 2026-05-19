/**
 * @file TestBattle.cpp
 * @brief Battle archive extraction and OBJ export test utilities.
 */
#include "TestBattle.hpp"
#include "BitReader.hpp"
#include "DatToObj.hpp"
#include "open_viii/archive/Archives.hpp"
#include "open_viii/battle/dat/DatFile.hpp"
#include "open_viii/battle/stage/FlattenBattleTim.hpp"
#include "open_viii/battle/stage/Geometries.hpp"
#include "open_viii/battle/stage/X.hpp"
#include "open_viii/graphics/Vertice.hpp"
#include "open_viii/paths/Paths.hpp"
#include "StageToObj.hpp"
#include <array>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <spdlog/spdlog.h>

/**
 * @brief Retrieves remastered PNG textures associated with a DAT model.
 *
 * Loads external remastered textures from the main ZZZ archive and returns
 * PNG images corresponding to texture entries referenced by the DAT file.
 *
 * @param main_zzz Optional main ZZZ archive containing remastered assets.
 * @param dat Battle DAT model file.
 *
 * @return PNG textures associated with the DAT texture entries.
 */
inline std::vector<std::optional<open_viii::graphics::Png>>
  get_pngs(
    const std::optional<open_viii::archive::ZZZ> &main_zzz,
    const open_viii::battle::dat::DatFile        &dat)
{// dump pngs from remaster
  return open_viii::battle::dat::DatToObj::fetch_pngs(dat, main_zzz);
}

/**
 * @brief Exports all battle stage archives as OBJ geometry.
 *
 * Iterates through battle stage X archives contained in the battle archive
 * and exports stage geometry and texture data to Wavefront OBJ format.
 *
 * @param battle_archive Battle archive containing stage X files.
 */
inline void
  all_stage_to_obj(const open_viii::archive::FIFLFS<false> &battle_archive)
{
  for (const auto &battle_fetch : battle_archive) {
    if (!battle_fetch.file_name().ends_with(open_viii::battle::stage::X::EXT)) {
      continue;
    }
    fmt::println("Begin Processing: {}", battle_fetch.file_name());
    const auto x_file = open_viii::battle::stage::X(
      battle_fetch.get(),
      static_cast<std::string>(battle_fetch.file_name()));

    open_viii::battle::stage::StageToObj::export_x_to_obj(x_file);
    open_viii::battle::stage::FlattenBattleTim::extract_used_colors(x_file);

    fmt::println("End Processing");
  }
}

/**
 * @brief Writes UV export data for all DAT objects to a CSV stream.
 *
 * Iterates through all object geometry entries contained in the DAT file and
 * exports UV mapping information to a consolidated CSV file.
 *
 * @param dat Battle DAT model file.
 * @param all_csv_out Output CSV file stream.
 * @param pngs Optional remastered PNG textures used for UV scaling.
 */
inline void
  dump_dat_to_all_csv(
    const open_viii::battle::dat::DatFile                      &dat,
    std::ofstream                                              &all_csv_out,
    const std::vector<std::optional<open_viii::graphics::Png>> &pngs)
{// dump all csv data to one large csv file.
  if (all_csv_out) {
    for (const auto &object_data : dat.section_2().object_data) {
      open_viii::battle::dat::DatToObj::write_uvs_to_csv(
        all_csv_out,
        object_data,
        dat.section_11().m_tims,
        pngs);
    }
  }
}

/**
 * @brief Processes and exports battle DAT model files.
 *
 * Iterates through battle DAT files contained in the archive, prints model and
 * animation metadata, and optionally exports geometry, textures, and UV data.
 *
 * Current debug output includes:
 * - model names
 * - skeleton bone counts
 * - animation counts
 * - frame statistics
 * - raw animation buffer contents
 *
 * @param battle_archive Battle archive containing DAT model files.
 * @param main_zzz Optional remastered asset archive.
 */
inline void
  all_dat_to_obj(
    const open_viii::archive::FIFLFS<false> &battle_archive,
    [[maybe_unused]] const std::optional<open_viii::archive::ZZZ> &main_zzz)
{
  static const auto all_csv     = std::filesystem::path("tmp") / "ff8" / "data"
                                / "eng" / "battle" / "_all_.csv";
  auto              all_csv_out = std::ofstream(all_csv);

  for (const auto &battle_fetch : battle_archive) {
    const std::filesystem::path &as_path
      = std::filesystem::path(battle_fetch.file_name());
    if (
      !battle_fetch.file_name().ends_with(".dat") || !as_path.has_stem()
      || !(
        as_path.stem().string().starts_with("c0m")
        || as_path.stem().string().starts_with("d"))) {
      continue;
    }
    fmt::println("{}", battle_fetch.file_name());
    const auto dat = [&]() -> std::optional<open_viii::battle::dat::DatFile> {
      try {
        auto datfile = open_viii::battle::dat::DatFile(
          battle_fetch.get(),
          battle_fetch.file_name());

        return datfile;
        // processing...
      }
      catch (const std::exception &e) {
        spdlog::error(
          "Failed processing {}: {}",
          battle_fetch.file_name(),
          e.what());
        return std::nullopt;
      }
    }();
    if (!dat)
      continue;
    // print name.
    fmt::println("section 7 name: {}", dat->section_7().name());
    // goal I want to dump the animation section.
    const open_viii::battle::dat::Section1_Skeleton &skeleton
      = dat->section_1();

    const open_viii::battle::dat::Section3_Model_Animation &model_animations
      = dat->section3();
    fmt::println("number_of_bones: {}", skeleton.m_header.numBones);
    fmt::println(
      "number_of_animations: {}",
      model_animations.m_section_header.m_count);

    for (const auto &animation : model_animations.m_animations) {
      fmt::println("number_of_frames: {}", +animation.frames_count);
      fmt::println("bytes_in_animation: {}", animation.buffer.size());
      fmt::println(
        "bytes_per_frame: {}",
        animation.buffer.size() / animation.frames_count);

      fmt::println(
        "bytes_per_frame_per_bone: {}",
        animation.buffer.size() / animation.frames_count
          / skeleton.m_header.numBones);

      for (const std::uint8_t byte : animation.buffer) {
        fmt::print("{:02X}", byte);
      }
      fmt::print("\n\n");
    }

    //    std::vector<std::optional<open_viii::graphics::Png>> pngs
    //      = get_pngs(main_zzz, dat);
    //    dump_dat_to_all_csv(dat, all_csv_out, pngs);
    //    open_viii::battle::dat::DatToObj::export_dat_to_obj(dat, pngs);
  }
}

/**
 * @brief Entry point for battle extraction and export tests.
 *
 * Iterates through configured FFVIII installation paths, loads battle-related
 * archives, and executes geometry or animation extraction test routines.
 *
 * @return Exit status code.
 */
int
  main()
{
  const auto start = std::chrono::steady_clock::now();
  open_viii::Paths::for_each_path(
    [&](const std::filesystem::path &path) -> open_viii::Paths::Ops {
      fmt::println("Path: {}", path);
      static constexpr auto coo      = open_viii::LangT::en;
      const auto            archives = open_viii::archive::Archives(
        path,
        open_viii::LangCommon::to_string<coo>());
      if (!static_cast<bool>(archives)) {
        spdlog::error("Failed to load path: {}", path.string());
        return open_viii::Paths::Ops::Continue;
      }
      [[maybe_unused]] const auto &battle_archive
        = archives.get<open_viii::archive::ArchiveTypeT::battle>();
      [[maybe_unused]] const auto &main_zzz
        = archives.get<open_viii::archive::ArchiveTypeT::zzz_main>();

      // all_stage_to_obj(battle_archive);
      all_dat_to_obj(battle_archive, main_zzz);

      const auto end  = std::chrono::steady_clock::now();
      const auto diff = end - start;
      fmt::println(
        "{} ms",
        std::chrono::duration<double, std::milli>(diff).count());
      // because we don't want to extract from two or more archives at the same
      // time.
      return open_viii::Paths::Ops::Stop;
    });

  return 0;
}