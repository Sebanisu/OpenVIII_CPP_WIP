#include "open_viii/battle/stage/RawClut.hpp"
#include <array>
#include <boost/ut.hpp>
#include <cstdint>
#include <iostream>
void
  test_clut_ids()
{
  static constexpr std::array<std::uint16_t, 16U> clutids = {
    0x3C00U, 0x3C40U, 0x3C80U, 0x3CC0U, 0x3D00U, 0x3D40U, 0x3D80U, 0x3DC0U,
    0x3E00U, 0x3E40U, 0x3E80U, 0x3EC0U, 0x3F00U, 0x3F40U, 0x3F80U, 0x3FC0U,
  };
  for (std::uint8_t i = 0; const auto &clutid : clutids) {
    boost::ut::expect(
    boost::ut::eq(
      +static_cast<std::uint8_t>(open_viii::battle::stage::RawClut(clutid)),
      i++));
  }
}
int main()
{
  using namespace boost::ut::literals;
    "test_clut_ids"_test = []()
    {
        test_clut_ids();
    };
}