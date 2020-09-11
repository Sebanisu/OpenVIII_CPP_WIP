//
// Created by pcvii on 9/10/2020.
//

#ifndef VIIIARCHIVE_DESWIZZLE_H
#define VIIIARCHIVE_DESWIZZLE_H
#include "Map.h"
#include "Mim.h"
namespace open_viii::graphics::background
{
struct Deswizzle
{
    template<typename tileT>
    requires( std::is_convertible_v<tileT, Tile1> || std::is_convertible_v<tileT, Tile2> || std::is_convertible_v<tileT, Tile3>)
    static auto unique_pupu(const std::vector<tileT> &t)
    {
      auto pupu_view = t | std::views::transform([](const auto &tile) { return Pupu(tile); });
      std::vector<Pupu> out(std::ranges::begin(pupu_view), std::ranges::end(pupu_view));
      std::sort(out.begin(), out.end());
      auto last = std::unique(std::ranges::begin(out), std::ranges::end(out));
      out.erase(last, std::ranges::end(out));
      return out;
    }
    template <size_t map_type>
  static void save([[maybe_unused]] const Mim &in_mim, [[maybe_unused]] const Map<map_type> &in_map, [[maybe_unused]] const std::string_view &in_path)
  {
    const auto &ts = in_map.tiles();
    Rectangle<std::int32_t> rect = in_map.canvas();
    const auto depth_and_palettes = in_map.used_depth_and_palette();
    std::ranges::for_each(unique_pupu(ts), [&in_mim,&rect, &depth_and_palettes, &ts, &in_path](const auto &pupu) {
           std::vector<Color16> out{};
           out.resize(rect.area());
           bool drawn = false;
           std::ranges::for_each(
             depth_and_palettes | std::views::filter([&](const auto &pair) -> bool { return pupu == pair.first; }),
             [&drawn, &rect, &out, &pupu, &in_mim, &ts, &in_path](const auto &depth_and_palette) {
                    const auto &[depth, palette] = depth_and_palette;
                    in_mim.get_colors(
                      in_path,
                      depth,
                      palette,
                      [&drawn, &rect, &depth, &palette, &ts, &pupu, &out](const std::span<const Color16> &data,
                                                                          [[maybe_unused]] const std::size_t &raw_width,
                                                                          [[maybe_unused]] const std::size_t &raw_height,
                                                                          std::string local_filename) {
                             std::ranges::for_each(ts | std::views::filter([&pupu, &palette](const auto &local_t) -> bool {
                                                          return local_t.draw() && palette == local_t.palette_id() && pupu == local_t;
                                                   }),
                                                   [&pupu, &raw_width, &rect, &data, &out, &drawn](const auto &t) {
                                                          for (std::uint32_t y = {}; y < t.HEIGHT; y++) {
                                                            for (std::uint32_t x = {}; x < t.WIDTH; x++) {
                                                              if (t.source_x() < 0 || t.source_y() < 0)
                                                                continue;
                                                              auto pixel_in = (static_cast<std::uint32_t>(t.source_x()) + x)
                                                                              + ((static_cast<std::uint32_t>(t.source_y()) + y) * raw_width);
                                                              if (t.depth().bpp4()) {
                                                                pixel_in += t.TEXTURE_PAGE_WIDTH * t.texture_id() * 2U;
                                                              } else if (t.depth().bpp8()) {
                                                                pixel_in += t.TEXTURE_PAGE_WIDTH * t.texture_id();
                                                              }

                                                              else if (t.depth().bpp16()) {
                                                                pixel_in += (t.TEXTURE_PAGE_WIDTH * t.texture_id()) / 2U;
                                                              }
                                                              auto pixel_out =
                                                                (static_cast<std::uint32_t>(t.x()) + x)
                                                                + ((static_cast<std::uint32_t>(t.y()) + y) * static_cast<std::uint32_t>(rect.width()));
                                                              Color16 color = data[pixel_in];
                                                              if (!color.is_black()) {
                                                                out.at(pixel_out) = data[pixel_in];
                                                                drawn = true;
                                                              }
                                                            }
                                                          }
                                                   });
                             auto p = std::filesystem::path(local_filename);
                      },
                      false);
             });
           if (drawn) {
             auto path_v = std::filesystem::path(in_path);
             std::stringstream ss{};
             ss << (path_v.parent_path() / path_v.stem()).string() << '_' << std::uppercase << std::hex << std::setfill('0')
                << std::setw(16U) << pupu << ".mimmap";
             Ppm::save(
               out, static_cast<std::uint32_t>(rect.width()), static_cast<std::uint32_t>(rect.height()), ss.str(), true);
           }
    });
  }
};
}
#endif //VIIIARCHIVE_DESWIZZLE_H
