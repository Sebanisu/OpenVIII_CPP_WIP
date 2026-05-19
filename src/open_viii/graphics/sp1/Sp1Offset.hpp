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
#ifndef VIIIARCHIVE_SP1OFFSET_HPP
#define VIIIARCHIVE_SP1OFFSET_HPP
#include <cstdint>
#include <fmt/format.h>
namespace open_viii::graphics {

struct Sp1Offset
{
  std::uint16_t offset{};
  std::uint16_t count{};

  constexpr Sp1Offset() = default;
};

}// namespace open_viii::graphics

template<>
struct fmt::formatter<open_viii::graphics::Sp1Offset>
{
  constexpr auto
    parse(fmt::format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(const open_viii::graphics::Sp1Offset &o, FormatContext &ctx) const
  {
    return fmt::format_to(
      ctx.out(),
      "{{offset: {}, count: {}}}",
      o.offset,
      o.count);
  }
};
#endif// VIIIARCHIVE_SP1OFFSET_HPP
