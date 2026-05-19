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
#ifndef VIIIARCHIVE_SP1HEADER_HPP
#define VIIIARCHIVE_SP1HEADER_HPP
#include "Sp1Offset.hpp"
#include <cstdint>
#include <cstring>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <ranges>
#include <span>

namespace open_viii::graphics {

struct Sp1Header
{
  std::uint32_t          count{};
  std::vector<Sp1Offset> offsets{};

  constexpr Sp1Header() = default;

  explicit Sp1Header(std::span<const char> buffer)
  {
    {
      const std::size_t sz = sizeof(std::uint32_t);
      if (sz > std::ranges::size(buffer)) {
        return;
      }
      std::memcpy(&count, std::ranges::data(buffer), sz);
      buffer = buffer.subspan(sz);
    }
    {
      offsets.resize(count);
      const std::size_t sz
        = static_cast<std::size_t>(count) * sizeof(Sp1Offset);

      if (sz > std::ranges::size(buffer)) {
        count = {};
        offsets.clear();
        return;
      }

      std::memcpy(std::ranges::data(offsets), std::ranges::data(buffer), sz);
    }
  }
};

}// namespace open_viii::graphics

template<>
struct fmt::formatter<open_viii::graphics::Sp1Header>
{
  constexpr auto
    parse(fmt::format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(const open_viii::graphics::Sp1Header &h, FormatContext &ctx) const
  {
    return fmt::format_to(
      ctx.out(),
      "{{count: {}, offsets: [{}]}}",
      h.count,
      fmt::join(h.offsets, ", "));
  }
};
#endif// VIIIARCHIVE_SP1HEADER_HPP
