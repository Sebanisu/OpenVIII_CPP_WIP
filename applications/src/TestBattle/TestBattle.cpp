//
// Created by pcvii on 11/17/2020.
//
#include "TestBattle.hpp"
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
inline void
  all_stage_to_obj(const open_viii::archive::FIFLFS<false> &battle_archive)
{
  for (const auto &battle_fetch : battle_archive) {
    if (!battle_fetch.file_name().ends_with(open_viii::battle::stage::X::EXT)) {
      continue;
    }
    std::cout << "Begin Processing: " << battle_fetch.file_name() << "\n";
    const auto x = open_viii::battle::stage::X(
      battle_fetch.get(),
      static_cast<std::string>(std::move(battle_fetch.file_name())));

    open_viii::battle::stage::StageToObj::export_x_to_obj(x);
    open_viii::battle::stage::FlattenBattleTim::extract_used_colors(x);
    std::cout << "End Processing\n";
  }
}
inline void
  all_dat_to_obj(
    const open_viii::archive::FIFLFS<false>      &battle_archive,
    const std::optional<open_viii::archive::ZZZ> &main_zzz)
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
    const auto pngs
      = open_viii::battle::dat::DatToObj::fetch_pngs(dat, main_zzz);
    if (all_csv_out) {
      for (const auto &object_data : dat.section_2().object_data) {
        open_viii::battle::dat::DatToObj::write_uvs_to_csv(
          all_csv_out,
          object_data,
          dat.section_11().m_tims,
          pngs);
      }
    }
    std::cout << dat.section_7().name() << std::endl;

    open_viii::battle::dat::DatToObj::export_dat_to_obj(dat, pngs);
  }
}

int
  main()
{
  const auto start = std::chrono::steady_clock::now();
  open_viii::Paths::for_each_path([&](const std::filesystem::path &path) {
    std::cout << path << std::endl;
    static constexpr auto coo      = open_viii::LangT::en;
    const auto            archives = open_viii::archive::Archives(
      path,
      open_viii::LangCommon::to_string<coo>());
    if (!static_cast<bool>(archives)) {
      std::cerr << "Failed to load path: " << path.string() << '\n';
      return;
    }
    const auto &battle_archive
      = archives.get<open_viii::archive::ArchiveTypeT::battle>();
    const auto &main_zzz
      = archives.get<open_viii::archive::ArchiveTypeT::zzz_main>();

    all_stage_to_obj(battle_archive);
    all_dat_to_obj(battle_archive, main_zzz);

    const auto end  = std::chrono::steady_clock::now();
    const auto diff = end - start;
    std::cout << std::chrono::duration<double, std::milli>(diff).count()
              << " ms" << '\n';
    std::exit(0);// because we don't want to extract from two or more archives
                 // at the same time.
  });

  return 0;
}