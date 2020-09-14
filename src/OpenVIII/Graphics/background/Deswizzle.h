//
// Created by pcvii on 9/10/2020.
//

#ifndef VIIIARCHIVE_DESWIZZLE_H
#define VIIIARCHIVE_DESWIZZLE_H
#include "Map.h"
#include "Mim.h"
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
  std::vector<std::uint8_t> m_unique_palettes{};
  Rectangle<std::int32_t> m_canvas{};
  std::vector<Pupu> m_unique_pupus{};
  auto find_unique_palettes()
  {
    const auto &tiles = m_map.tiles();
    auto pupu_view = tiles | std::views::transform([](const auto &tile) { return tile.palette_id(); });
    auto out = std::vector<uint8_t>(std::ranges::begin(pupu_view), std::ranges::end(pupu_view));
    std::sort(out.begin(), out.end());
    auto last = std::unique(std::ranges::begin(out), std::ranges::end(out));
    out.erase(last, std::ranges::end(out));
    return out;
  }
  auto find_unique_pupu()
  {
    const auto &tiles = m_map.tiles();
    auto pupu_view = tiles | std::views::transform([](const auto &tile) { return Pupu(tile); });
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
  void save()
  {
    std::vector<Color16> out(m_canvas.area());
    for_each_pupu([this, &out](const Pupu &pupu) {
      bool drawn = false;
      for_each_palette([&pupu, &drawn, &out, this](const std::uint8_t &palette) {
        m_mim.get_colors(
          m_path,
          pupu.depth(),
          palette,
          [&](const std::span<const Color16> &data,
            [[maybe_unused]] const std::size_t &raw_width,
            [[maybe_unused]] const std::size_t &raw_height,
            const std::string &local_filename) {
            std::ranges::for_each(m_map.tiles() | std::views::filter([&pupu, &palette](const auto &local_t) -> bool {
              return local_t.draw() && palette == local_t.palette_id() && pupu == local_t;
            }),
              [&pupu, &raw_width, this, &data, &out, &drawn](const auto &t) {
                for (std::uint32_t y = {}; y < t.HEIGHT; y++) {
                  for (std::uint32_t x = {}; x < t.WIDTH; x++) {
                    if (t.source_x() < 0 || t.source_y() < 0) {
                      continue;
                    }
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
                      + ((static_cast<std::uint32_t>(t.y()) + y) * static_cast<std::uint32_t>(m_canvas.width()));
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
        auto path_v = std::filesystem::path(m_path);
        std::stringstream ss{};
        constexpr static auto hex_width = 16U;
        ss << (path_v.parent_path() / path_v.stem()).string() << '_' << std::uppercase << std::hex << std::setfill('0')
           << std::setw(hex_width) << pupu << ".mimmap";
        Ppm::save(out,
          static_cast<std::uint32_t>(m_canvas.width()),
          static_cast<std::uint32_t>(m_canvas.height()),
          ss.str(),
          true);
        static constexpr auto blank = Color16{};
        std::ranges::fill(out, blank);
      }
    });
  }

public:
  Deswizzle(const Mim &in_mim, const Map<map_type> &in_map, const std::string_view &in_path)
    : m_mim(in_mim), m_map(in_map), m_path(in_path), m_unique_palettes(find_unique_palettes()),
      m_canvas(in_map.canvas()), m_unique_pupus(find_unique_pupu())
  {
    save();
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_DESWIZZLE_H
