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
#include "MimFromPath.hpp"
#include <utility>
namespace open_viii::graphics::background {
/**
 * Deswizzle is a temporary struct that is used to take a map and mim and
 * descramble the tiles to something modders can work with.
 * @tparam map_type there are 3 types of maps
 */
template<typename map_type, typename mim_type>
requires(
  std::is_same_v<
    map_type,
    Map<
      Tile1>> || std::is_same_v<map_type, Map<Tile2>> || std::is_same_v<map_type, Map<Tile3>>) struct
  Deswizzle
{
private:
  using outColorT = Color24<0, 1, 2>;
  const mim_type *          m_mim;
  const map_type *          m_map;
  std::string               m_path{};
  std::vector<std::uint8_t> m_unique_palettes{};
  Rectangle<std::int32_t>   m_canvas{};
  std::vector<Pupu>         m_unique_pupus{};
  auto
    find_unique_palettes() const
  {
    const auto &tiles     = m_map->tiles();
    auto        pupu_view = tiles | std::views::transform([](const auto &tile) {
                       return tile.palette_id();
                     });
    auto        out       = std::vector<uint8_t>(std::ranges::begin(pupu_view),
                                    std::ranges::end(pupu_view));
    std::sort(out.begin(), out.end());
    auto last = std::unique(std::ranges::begin(out), std::ranges::end(out));
    out.erase(last, std::ranges::end(out));
    return out;
  }
  auto
    find_unique_pupu() const
  {
    const auto &tiles     = m_map->tiles();
    auto        pupu_view = tiles | std::views::transform([](const auto &tile) {
                       return Pupu(tile);
                     });
    auto        out       = std::vector<Pupu>(std::ranges::begin(pupu_view),
                                 std::ranges::end(pupu_view));
    std::sort(out.begin(), out.end());
    auto last = std::unique(std::ranges::begin(out), std::ranges::end(out));
    out.erase(last, std::ranges::end(out));
    return out;
  }
  template<typename lambdaT>
  void
    for_each_pupu(const lambdaT &lambda) const
  {
    std::ranges::for_each(m_unique_pupus, lambda);
  }
  template<typename lambdaT>
  void
    for_each_palette(const lambdaT &lambda) const
  {
    std::ranges::for_each(m_unique_palettes, lambda);
  }
  void
    save_out_buffer_and_clear(std::vector<outColorT> &out,
                              const Pupu &            pupu) const
  {
    auto                  path_v = std::filesystem::path(m_path);
    std::stringstream     ss{};
    constexpr static auto hex_width = 16U;
    ss << (path_v.parent_path() / path_v.stem()).string() << '_'
       << std::uppercase << std::hex << std::setfill('0')
       << std::setw(hex_width) << pupu << ".mimmap";
    Ppm::save(out,
              static_cast<uint32_t>(m_canvas.width()),
              static_cast<uint32_t>(m_canvas.height()),
              ss.str(),
              true);
    static constexpr auto blank = outColorT{};
    std::ranges::fill(out, blank);
  }
  template<Color cT, std::integral indT>
  bool
    set_color(std::vector<outColorT> &out,
              const indT &            index_out,
              const cT &              color) const
  {
    if (!color.is_black()) {
      // assert(out.at(index_out).is_black() || color==out.at(index_out));
      // so there is possible overlapping tiles here. does this matter? because
      // pixels behind cannot be seen in game anyway?
      if constexpr (std::is_same_v<cT, outColorT>) {
        out.at(index_out) = color;
      } else {
        out.at(index_out) = static_cast<outColorT>(color);
      }
      return true;
    }
    return false;
  }
  template<typename tile_type>
  std::uint32_t
    get_output_index(const std::uint32_t x,
                     const std::uint32_t y,
                     const tile_type &   tile) const
  {
    return (static_cast<uint32_t>(tile.x()) + x)
         + ((static_cast<uint32_t>(tile.y()) + y)
            * static_cast<uint32_t>(m_canvas.width()));
  }

public:
  Deswizzle(const mim_type &in_mim, const map_type &in_map, std::string in_path)
    : m_mim(&in_mim),
      m_map(&in_map),
      m_path(std::move(in_path)),
      m_unique_palettes(find_unique_palettes()),
      m_canvas(in_map.canvas()),
      m_unique_pupus(find_unique_pupu())
  {}
  void
    save() const
  {
    std::vector<outColorT> out(static_cast<std::size_t>(m_canvas.area()));
    for_each_pupu([this, &out](const Pupu &pupu) {
      bool        drawn     = false;
      auto        raw_width = m_mim->get_raw_width(pupu.depth());
      const auto &tiles     = m_map->tiles();

      for_each_palette([this, &pupu, &tiles, &drawn, &raw_width, &out](
                         const std::uint8_t &palette) {
        auto filtered_tiles =
          tiles
          | std::views::filter([&pupu, &palette](const auto &local_t) -> bool {
              return local_t.draw() && palette == local_t.palette_id()
                  && pupu == local_t;
            });
        std::ranges::for_each(
          filtered_tiles,
          [this, &pupu, &raw_width, &out, &drawn, &palette](const auto &t) {
            open_viii::tools::for_each_xy(
              t.HEIGHT,
              [this, &pupu, &raw_width, &out, &drawn, &t, &palette](
                const auto &x, const auto &y) {
                const auto pixel_in = m_mim->get_color(x + t.source_x(),
                                                       y + t.source_y(),
                                                       pupu.depth(),
                                                       palette,
                                                       t.texture_id());
                const std::uint32_t pixel_out = get_output_index(x, y, t);
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
