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
#ifndef VIIIARCHIVE_BLENDMODET_HPP
#define VIIIARCHIVE_BLENDMODET_HPP
#include <cstdint>
#include <fmt/format.h>
#include <string_view>
namespace open_viii::graphics::background {
enum struct BlendModeT : std::uint8_t
{
  half_add [[maybe_unused]],
  add [[maybe_unused]],
  subtract [[maybe_unused]],
  quarter_add [[maybe_unused]],
  none [[maybe_unused]],
};
}// namespace open_viii::graphics::background
template<>
struct fmt::formatter<open_viii::graphics::background::BlendModeT>
  : fmt::formatter<std::string_view>
{
  // parse is inherited from formatter<string_view>.
  template<typename FormatContext>
  constexpr auto
    format(
      open_viii::graphics::background::BlendModeT blend_mode_t,
      FormatContext                              &ctx) const
  {
    using namespace open_viii::graphics::background;
    using namespace std::string_view_literals;
    std::string_view name = {};
    switch (blend_mode_t) {
    case BlendModeT::add:
      name = "add"sv;
      break;
    case BlendModeT::half_add:
      name = "half add"sv;
      break;
    case BlendModeT::none:
      name = "none"sv;
      break;
    case BlendModeT::quarter_add:
      name = "quarter add"sv;
      break;
    case BlendModeT::subtract:
      name = "subtract"sv;
      break;
    }
    return fmt::formatter<std::string_view>::format(name, ctx);
  }
};
#endif// VIIIARCHIVE_BLENDMODET_HPP
