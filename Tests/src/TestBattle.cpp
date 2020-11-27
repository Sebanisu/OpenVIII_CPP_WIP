//
// Created by pcvii on 11/17/2020.
//

#include "TestBattle.hpp"
#include "open_viii/battle/stage/Geometries.hpp"
#include <array>
#include <iostream>
int main()
{
  auto v = open_viii::battle::stage::Vertice<std::int16_t>{ -1, 2, -3 };
  auto v2 = v / 2.1F;
  auto v3 = v / 3.1;
  auto v4 = v * 2.1F;
  auto v5 = v * 3.1;
  auto v6 = static_cast<open_viii::battle::stage::Vertice<std::uint16_t>>(v);
  std::cout << v << '\n' << v2 << '\n' << v3 << '\n' << v4 << '\n' << v5 << '\n' << v6 << '\n';

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
    std::cout << static_cast<uint16_t>(open_viii::battle::stage::Triangle::clut(clutid)) << '\n';
  }
  open_viii::graphics::Bit4Values b4v = { 1, 5 };
  auto [a, b] = b4v;
  std::cout << '(' << static_cast<std::uint16_t>(a) << ',' << static_cast<std::uint16_t>(b) << ")\n";

  constexpr open_viii::battle::stage::Triangle triangle{};
  constexpr open_viii::battle::stage::Quad quad{};

  [[maybe_unused]] constexpr auto testmin = open_viii::graphics::min_uv(triangle);

  [[maybe_unused]] constexpr auto testmax = open_viii::graphics::max_uv(quad);

  [[maybe_unused]] constexpr auto rect = open_viii::graphics::rectangle(triangle);

  return 0;
}