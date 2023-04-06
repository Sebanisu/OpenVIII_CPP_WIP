//
// Created by pcvii on 11/17/2020.
//
#include "TestBattle.hpp"
#include "open_viii/archive/Archives.hpp"
#include "open_viii/battle/dat/DatFile.hpp"
#include "open_viii/battle/stage/FlattenBattleTim.hpp"
#include "open_viii/battle/stage/Geometries.hpp"
#include "open_viii/battle/dat/DatToObj.hpp"
#include "open_viii/battle/stage/StageToObj.hpp"
#include "open_viii/battle/stage/X.hpp"
#include "open_viii/graphics/Vertice.hpp"
#include "open_viii/paths/Paths.hpp"
#include <array>
#include <iostream>
[[maybe_unused]] void
  test_vertice()
{
  auto v  = open_viii::graphics::Vertice<std::int16_t>{ -1, 2, -3 };
  auto v2 = v / 2.1F;
  auto v3 = v / 3.1;
  auto v4 = v * 2.1F;
  auto v5 = v * 3.1;
  auto v6 = static_cast<open_viii::graphics::Vertice<std::uint16_t>>(v);
  std::cout << v << '\n'
            << v2 << '\n'
            << v3 << '\n'
            << v4 << '\n'
            << v5 << '\n'
            << v6 << '\n';
}
[[maybe_unused]] void
  test_clut_ids()
{
  static constexpr std::array<std::uint16_t, 16U> clutids = {
    0x3C00U, 0x3C40U, 0x3C80U, 0x3CC0U, 0x3D00U, 0x3D40U, 0x3D80U, 0x3DC0U,
    0x3E00U, 0x3E40U, 0x3E80U, 0x3EC0U, 0x3F00U, 0x3F40U, 0x3F80U, 0x3FC0U,
  };
  for (int i = 0; const auto &clutid : clutids) {
    std::cout << +static_cast<std::uint8_t>(
      open_viii::battle::stage::RawClut(clutid))
              << " == " << i++ << '\n';
  }
}
[[maybe_unused]] void
  test_bit4values()
{
  const auto b4v = open_viii::graphics::Bit4Values::create(1, 5);
  auto [a, b]    = b4v;
  std::cout << '(' << static_cast<std::uint16_t>(a) << ','
            << static_cast<std::uint16_t>(b) << ")\n";
}
[[maybe_unused]] void
  test_shapes()
{
  constexpr open_viii::battle::stage::Triangle triangle{};
  constexpr open_viii::battle::stage::Quad     quad{};
  [[maybe_unused]] constexpr auto              testmin
    = open_viii::graphics::min_uv(triangle);
  [[maybe_unused]] constexpr auto testmax = open_viii::graphics::max_uv(quad);
  [[maybe_unused]] constexpr auto rect
    = open_viii::graphics::rectangle(triangle);
}
int
  main()
{
  test_clut_ids();
  const auto start = std::chrono::steady_clock::now();
  open_viii::Paths::for_each_path([](const std::filesystem::path &path) {
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
    //    for (const auto &battle_fetch : battle_archive) {
    //      if (!battle_fetch.file_name().ends_with(
    //            open_viii::battle::stage::X::EXT)) {
    //        continue;
    //      }
    //      std::cout << "Begin Processing: " << battle_fetch.file_name() <<
    //      "\n"; const auto x = open_viii::battle::stage::X(
    //        battle_fetch.get(),
    //        static_cast<std::string>(std::move(battle_fetch.file_name())));
    //
    //      open_viii::battle::stage::StageToObj::export_x_to_obj(x);
    //      open_viii::battle::stage::FlattenBattleTim::extract_used_colors(x);
    //      std::cout << "End Processing\n";
    //    }
    for (const auto &battle_fetch : battle_archive) {
      const std::filesystem::path &as_path
        = std::filesystem::path(battle_fetch.file_name());
      if (
        !battle_fetch.file_name().ends_with(".dat") || !as_path.has_stem()
        || !(
          //as_path.stem().string().starts_with("c0m")
          //||
            as_path.stem().string().starts_with("d"))) {
        continue;
      }
      std::cout << battle_fetch.file_name() << std::endl;
      const auto dat = open_viii::battle::dat::DatFile(
        battle_fetch.get(),
        battle_fetch.file_name());
      std::cout << dat.section_7().name() << std::endl;
      open_viii::battle::dat::DatToObj::export_dat_to_obj(dat,main_zzz);
    }

    std::exit(0);
  });
  const auto end  = std::chrono::steady_clock::now();
  const auto diff = end - start;
  std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms"
            << '\n';
  return 0;
}