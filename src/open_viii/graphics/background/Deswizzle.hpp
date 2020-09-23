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

#ifndef VIIIARCHIVE_DESWIZZLE_HPP
#define VIIIARCHIVE_DESWIZZLE_HPP
#include "Map.hpp"
#include "Mim.hpp"
namespace open_viii::graphics::background {
/**
 * Deswizzle is a temporary struct that is used to take a map and mim and descramble the tiles to something modders can
 * work with.
 * @tparam map_type there are 3 types of maps
 */
template<typename map_type>
requires(std::is_same_v<map_type,
           Tile1> || std::is_same_v<map_type, Tile2> || std::is_same_v<map_type, Tile3>) struct Deswizzle
{
private:
  const Mim &m_mim{};
  const Map<map_type> &m_map{};
  const std::string_view &m_path{};
  const std::vector<std::uint8_t> m_unique_palettes{};
  const Rectangle<std::int32_t> m_canvas{};
  const std::vector<Pupu> m_unique_pupus{};
  auto find_unique_palettes()
  {
    const auto &tiles = m_map.tiles();
    auto pupu_view = tiles | std::views::transform([](const auto &tile) {
      return tile.palette_id();
    });
    auto out = std::vector<uint8_t>(std::ranges::begin(pupu_view), std::ranges::end(pupu_view));
    std::sort(out.begin(), out.end());
    auto last = std::unique(std::ranges::begin(out), std::ranges::end(out));
    out.erase(last, std::ranges::end(out));
    return out;
  }
  auto find_unique_pupu()
  {
    const auto &tiles = m_map.tiles();
    auto pupu_view = tiles | std::views::transform([](const auto &tile) {
      return Pupu(tile);
    });
    auto out = std::vector<Pupu>(std::ranges::begin(pupu_view), std::ranges::end(pupu_view));
    std::sort(out.begin(), out.end());
    auto last = std::unique(std::ranges::begin(out), std::ranges::end(out));
    out.erase(last, std::ranges::end(out));
    return out;
  }
  template<typename lambdaT> void for_each_pupu(const lambdaT &lambda)
  {
    std::ranges::for_each(m_unique_pupus, lambda);
  }
  template<typename lambdaT> void for_each_palette(const lambdaT &lambda)
  {
    std::ranges::for_each(m_unique_palettes, lambda);
  }
  void save_out_buffer_and_clear(std::vector<Color16> &out, const Pupu &pupu) const
  {
    auto path_v = std::filesystem::path(m_path);
    std::stringstream ss{};
    constexpr static auto hex_width = 16U;
    ss << (path_v.parent_path() / path_v.stem()).string() << '_' << std::uppercase << std::hex << std::setfill('0')
       << std::setw(hex_width) << pupu << ".mimmap";
    Ppm::save(out, static_cast<uint32_t>(m_canvas.width()), static_cast<uint32_t>(m_canvas.height()), ss.str(), true);
    static constexpr auto blank = Color16{};
    std::ranges::fill(out, blank);
  }
  bool set_color(std::vector<Color16> &out, const std::integral auto &index_out, const Color16 &color) const
  {
    if (!color.is_black()) {
      assert(out.at(index_out).is_black());
      out.at(index_out) = color;
      return true;
    }
    return false;
  }
  //  bool set_color(std::vector<Color16> &out,
  //    const std::integral auto &index_out,
  //    const std::span<const Color16> &in,
  //    const std::integral auto &index_in) const
  //  {
  //    Color16 color = in[index_in];
  //    if (!color.is_black()) {
  //      out.at(index_out) = color;
  //      return true;
  //    }
  //    return false;
  //  }
  auto get_output_index(auto &x, auto &y, auto &tile) const
  {
    return (static_cast<uint32_t>(tile.x()) + x)
           + ((static_cast<uint32_t>(tile.y()) + y) * static_cast<uint32_t>(m_canvas.width()));
  }
  //  auto get_input_index(auto &x, auto &y, const size_t &width, auto &tile) const
  //  {
  //    auto pixel_in =
  //      (static_cast<uint32_t>(tile.source_x()) + x) + ((static_cast<uint32_t>(tile.source_y()) + y) * width);
  //    unsigned int texture_page_offset = tile.TEXTURE_PAGE_WIDTH * tile.texture_id();
  //    if (tile.depth().bpp4()) {
  //      texture_page_offset *= 2U;
  //    } else if (tile.depth().bpp16()) {
  //      texture_page_offset /= 2U;
  //    }
  //    return pixel_in + texture_page_offset;
  //  }


public:
  Deswizzle(const Mim &in_mim, const Map<map_type> &in_map, const std::string_view &in_path)
    : m_mim(in_mim),
      m_map(in_map),
      m_path(in_path),
      m_unique_palettes(find_unique_palettes()),
      m_canvas(in_map.canvas()),
      m_unique_pupus(find_unique_pupu())
  {}

  void save()
  {
    std::vector<Color16> out(m_canvas.area());
    for_each_pupu([this, &out](const Pupu &pupu) {
      bool drawn = false;
      auto raw_width = m_mim.get_raw_width(pupu.depth());
      for_each_palette([&pupu, &drawn, &raw_width, &out, this](const std::uint8_t &palette) {
        std::ranges::for_each(m_map.tiles() | std::views::filter([&pupu, &palette](const auto &local_t) -> bool {
          return local_t.draw() && palette == local_t.palette_id() && pupu == local_t;
        }),
          [&pupu, &raw_width, this, &out, &drawn, &palette](const auto &t) {
            open_viii::Tools::for_each_xy(
              t.HEIGHT, [&pupu, &raw_width, this, &out, &drawn, &t, &palette](const auto &x, const auto &y) {
                auto pixel_in = m_mim.get_color(x + t.source_x(),
                  y + t.source_y(),
                  pupu.depth(),
                  palette,
                  t.texture_id());// get_input_index(x, y, raw_width, t);
                auto pixel_out = get_output_index(x, y, t);
                drawn |= set_color(out, pixel_out, pixel_in);
              });
          });
      });
      if (drawn) {
        save_out_buffer_and_clear(out, pupu);
      }
    });
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_DESWIZZLE_HPP
