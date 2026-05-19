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
#ifndef VIIIARCHIVE_SP2ENTRY_HPP
#define VIIIARCHIVE_SP2ENTRY_HPP
#include <cstdint>
#include <fmt/format.h>
namespace open_viii::graphics {

struct Sp2Entry
{
  std::uint32_t count{};
  std::uint8_t  x{};
  std::uint8_t  y{};
  std::uint8_t  unknown1{};
  std::uint8_t  unknown2{};
  std::uint16_t width{};
  std::uint16_t height{};
  std::uint8_t  unknown3{};
  std::uint8_t  unknown4{};
  std::uint8_t  unknown5{};
  std::uint8_t  unknown6{};

  constexpr Sp2Entry() = default;
};

}// namespace open_viii::graphics

template<>
struct fmt::formatter<open_viii::graphics::Sp2Entry>
{
  constexpr auto
    parse(fmt::format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(const open_viii::graphics::Sp2Entry &e, FormatContext &ctx) const
  {
    return fmt::format_to(
      ctx.out(),
      "{{ {} (X, Y) = ({}, {}), (W, H) = ({}, {}), "
      "Unk({}, {}, {}, {}, {}, {}) }}",
      e.count,
      +e.x,
      +e.y,
      e.width,
      e.height,
      +e.unknown1,
      +e.unknown2,
      +e.unknown3,
      +e.unknown4,
      +e.unknown5,
      +e.unknown6);
  }
};
#endif// VIIIARCHIVE_SP2ENTRY_HPP
