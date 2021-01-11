//
// Created by pcvii on 11/17/2020.
//

#include "TestBattle.hpp"
#include "open_viii/archive/Archives.hpp"
#include "open_viii/battle/stage/Geometries.hpp"
#include "open_viii/battle/stage/X.hpp"
#include "open_viii/paths/Paths.hpp"
#include <array>
#include <iostream>
[[maybe_unused]] static void test_vertice()
{
  auto v = open_viii::graphics::Vertice<std::int16_t>{ -1, 2, -3 };
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
[[maybe_unused]] static void test_clut_ids()
{
  static constexpr std::array<std::uint16_t, 16U> clutids = {
    0x003CU,
    0x403CU,
    0x803CU,
    0xC03CU,
    0x003DU,
    0x403DU,
    0x803DU,
    0xC03DU,
    0x003EU,
    0x403EU,
    0x803EU,
    0xC03EU,
    0x003FU,
    0x403FU,
    0x803FU,
    0xC03FU,
  };
  for (const auto &clutid : clutids) {
    std::cout << static_cast<uint16_t>(open_viii::battle::stage::Triangle::clut(
      clutid)) << '\n';
  }
}
[[maybe_unused]] static void test_bit4values()
{
  open_viii::graphics::Bit4Values b4v = { 1, 5 };
  auto [a, b] = b4v;
  std::cout << '(' << static_cast<std::uint16_t>(a) << ','
            << static_cast<std::uint16_t>(b) << ")\n";
}
[[maybe_unused]] static void test_shapes()
{
  constexpr open_viii::battle::stage::Triangle triangle{};
  constexpr open_viii::battle::stage::Quad quad{};

  [[maybe_unused]] constexpr auto testmin =
    open_viii::graphics::min_uv(triangle);

  [[maybe_unused]] constexpr auto testmax = open_viii::graphics::max_uv(quad);

  [[maybe_unused]] constexpr auto rect =
    open_viii::graphics::rectangle(triangle);
}
int main()
{
  const auto start = std::chrono::steady_clock::now();
  open_viii::Paths::for_each_path([](const std::filesystem::path &path) {
    std::cout << path << std::endl;
    const auto archives =
      open_viii::archive::Archives<open_viii::LangT::en>(path);
    const auto battle =
      archives.get<open_viii::archive::ArchiveTypeT::battle>();
    battle.execute_on({ open_viii::battle::stage::X::EXT },
      []([[maybe_unused]] std::vector<char> &&rvalue_buffer,
        std::string &&rvalue_path) {
        open_viii::battle::stage::X(
          std::move(rvalue_buffer), std::move(rvalue_path));
      });
  });


  const auto end = std::chrono::steady_clock::now();
  const auto diff = end - start;
  std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms"
            << '\n';
  return 0;
}