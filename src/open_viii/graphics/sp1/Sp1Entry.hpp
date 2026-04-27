// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#ifndef VIIIARCHIVE_SP1ENTRY_HPP
#define VIIIARCHIVE_SP1ENTRY_HPP
namespace open_viii::graphics {
struct Sp1Entry
{
  std::uint8_t x{};
  std::uint8_t y{};
  std::uint8_t unknown1{};
  std::uint8_t unknown2{};
  std::uint8_t width{};
  std::int8_t  offset_x{};
  std::uint8_t height{};
  std::int8_t  offset_y{};

  constexpr Sp1Entry() = default;
};
}// namespace open_viii::graphics

#include <fmt/format.h>

template<>
struct fmt::formatter<open_viii::graphics::Sp1Entry>
{
  constexpr auto
    parse(fmt::format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(const open_viii::graphics::Sp1Entry &e, FormatContext &ctx) const
  {
    return fmt::format_to(
      ctx.out(),
      "{{ (X, Y) = ({}, {}), (W, H) = ({}, {}), "
      "(Offset X, Offset Y) = ({}, {}), Unk({}, {}) }}",
      +e.x,
      +e.y,
      +e.width,
      +e.height,
      +e.offset_x,
      +e.offset_y,
      +e.unknown1,
      +e.unknown2);
  }
};
#endif// VIIIARCHIVE_SP1ENTRY_HPP
