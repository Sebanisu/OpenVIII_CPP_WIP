//
// Created by pcvii on 11/17/2020.
//
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
#include <iostream>

inline std::vector<std::optional<open_viii::graphics::Png>>
  get_pngs(
    const std::optional<open_viii::archive::ZZZ> &main_zzz,
    const open_viii::battle::dat::DatFile        &dat)
{// dump pngs from remaster
  const auto pngs = open_viii::battle::dat::DatToObj::fetch_pngs(dat, main_zzz);
  return pngs;
}
inline void
  all_stage_to_obj(const open_viii::archive::FIFLFS<false> &battle_archive)
{
  for (const auto &battle_fetch : battle_archive) {
    if (!battle_fetch.file_name().ends_with(open_viii::battle::stage::X::EXT)) {
      continue;
    }
    std::cout << "Begin Processing: " << battle_fetch.file_name() << "\n";
    const auto x_file = open_viii::battle::stage::X(
      battle_fetch.get(),
      static_cast<std::string>(battle_fetch.file_name()));

    open_viii::battle::stage::StageToObj::export_x_to_obj(x_file);
    open_viii::battle::stage::FlattenBattleTim::extract_used_colors(x_file);
    std::cout << "End Processing\n";
  }
}
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
inline void
  all_dat_to_obj(
    const open_viii::archive::FIFLFS<false> &battle_archive,
    [[maybe_unused]] const std::optional<open_viii::archive::ZZZ> &main_zzz)
{
  const static auto all_csv = std::filesystem::path("tmp") / "ff8" / "data"
                            / "eng" / "battle" / "_all_.csv";
  auto all_csv_out = std::ofstream(all_csv);

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
    std::cout << battle_fetch.file_name() << std::endl;
    const auto dat = open_viii::battle::dat::DatFile(
      battle_fetch.get(),
      battle_fetch.file_name());
    // print name.
    std::cout << dat.section_7().name() << std::endl;
    // goal I want to dump the animation section.
    const open_viii::battle::dat::Section1_Skeleton &skeleton = dat.section_1();

    const open_viii::battle::dat::Section3_Model_Animation &model_animations
      = dat.section3();
    std::cout << "number_of_bones: " << skeleton.m_header.numBones << '\n';
    std::cout << "number_of_animations: "
              << model_animations.m_section_header.m_count << '\n';

    for (const auto &animation : model_animations.m_animations) {
      std::cout << "number_of_frames: " << +animation.frames_count << '\n';
      std::cout << "bytes_in_animation: " << animation.buffer.size() << '\n';
      std::cout << "bytes_per_frame: "
                << animation.buffer.size() / animation.frames_count << '\n';
      std::cout << "bytes_per_frame_per_bone: "
                << animation.buffer.size() / animation.frames_count
                     / skeleton.m_header.numBones
                << '\n';
      // set hex formatting flags
      std::cout << std::uppercase << std::hex << std::setw(2)
                << std::setfill('0');
      for (const std::uint8_t byte : animation.buffer) {
        // print hex here.
        std::cout << +byte;
      }
      // Reset the formatting flags and attributes.
      std::cout << std::nouppercase << std::dec << std::setw(0)
                << std::setfill(' ');
      std::cout << '\n' << std::endl;
    }

    //    std::vector<std::optional<open_viii::graphics::Png>> pngs
    //      = get_pngs(main_zzz, dat);
    //    dump_dat_to_all_csv(dat, all_csv_out, pngs);
    //    open_viii::battle::dat::DatToObj::export_dat_to_obj(dat, pngs);
  }
}

int
  main()
{
  const auto start = std::chrono::steady_clock::now();
  open_viii::Paths::for_each_path(
    [&](const std::filesystem::path &path) -> bool {
      std::cout << path << std::endl;
      static constexpr auto coo      = open_viii::LangT::en;
      const auto            archives = open_viii::archive::Archives(
        path,
        open_viii::LangCommon::to_string<coo>());
      if (!static_cast<bool>(archives)) {
        std::cerr << "Failed to load path: " << path.string() << '\n';
        return true;// true to continue;
      }
      [[maybe_unused]] const auto &battle_archive
        = archives.get<open_viii::archive::ArchiveTypeT::battle>();
      [[maybe_unused]] const auto &main_zzz
        = archives.get<open_viii::archive::ArchiveTypeT::zzz_main>();

      // all_stage_to_obj(battle_archive);
      all_dat_to_obj(battle_archive, main_zzz);

      const auto end  = std::chrono::steady_clock::now();
      const auto diff = end - start;
      std::cout << std::chrono::duration<double, std::milli>(diff).count()
                << " ms" << '\n';
      return false;// because we don't want to extract from two or more archives
                   // at the same time.
    });

  return 0;
}