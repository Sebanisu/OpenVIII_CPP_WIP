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
#include "open_viii/graphics/Png.hpp"
#include "PupuID.hpp"
#include "UniquifyPupu.hpp"
#include <fmt/format.h>
#include <spdlog/spdlog.h>
namespace open_viii::graphics::background {
/**
 * Deswizzle is a temporary struct that is used to take a map and mim and
 * descramble the tiles to something modders can work with.
 * @tparam map_type there are 3 types of maps
 */
struct Deswizzle
{
private:
  using outColorT = Color32<ColorLayoutT::RGBA>;
  std::variant<Mim, MimFromPath, std::monostate> m_mim             = {};
  const Map                                     &m_map             = {};
  std::string                                    m_path            = {};
  std::vector<std::uint8_t>                      m_unique_palettes = {};
  Rectangle<std::int32_t>                        m_canvas          = {};
  std::vector<PupuID>                            m_pupus           = {};
  std::vector<PupuID>                            m_unique_pupus    = {};
  auto
    find_unique_palettes() const
  {
    auto out = std::vector<uint8_t>{};
    m_map.visit_tiles([&out](auto &&tiles) {
      auto pupu_view = tiles | std::views::transform([](const auto &tile) {
                         return tile.palette_id();
                       });
      out            = std::vector<uint8_t>(
        std::ranges::begin(pupu_view),
        std::ranges::end(pupu_view));
      std::sort(out.begin(), out.end());
      auto last = std::unique(std::ranges::begin(out), std::ranges::end(out));
      out.erase(last, std::ranges::end(out));
    });
    return out;
  }

  /**
   * @brief Generates a uniquified PupuID for every tile in traversal order.
   *
   * Each tile is processed through UniquifyPupu so tiles that would
   * otherwise generate identical PupuIDs receive stable unique offsets
   * based on their occurrence and position.
   *
   * The returned vector preserves:
   * - original tile traversal order
   * - duplicate entries
   * - one entry per tile
   *
   * This result acts as the canonical tile-to-Pupu mapping for the
   * current deswizzle session and should be reused rather than
   * regenerated.
   *
   * @return Vector of uniquified PupuIDs matching tile traversal order.
   */
  auto
    find_pupu() const
  {
    return m_map.visit_tiles([](auto &&tiles) {
      UniquifyPupu uniquify_pupu = {};
      auto         pupu_view
        = tiles | std::views::transform([&uniquify_pupu](const auto &tile) {
            return uniquify_pupu(tile);
          });
      return std::vector<PupuID>(
        std::ranges::begin(pupu_view),
        std::ranges::end(pupu_view));
    });
  }

  /**
   * @brief Generates a sorted list of unique uniquified PupuIDs.
   *
   * This function derives its result from find_pupu() and removes
   * duplicate entries while preserving the uniquified identifiers
   * generated during the original traversal pass.
   *
   * The returned vector is intended for grouped iteration and
   * deswizzle output generation where only distinct PupuIDs are
   * required.
   *
   * @return Sorted vector containing unique uniquified PupuIDs.
   */
  auto
    find_unique_pupu() const
  {
    auto out = find_pupu();
    std::sort(out.begin(), out.end());
    auto last = std::unique(std::ranges::begin(out), std::ranges::end(out));
    out.erase(last, std::ranges::end(out));
    return out;
  }

  /**
   * @brief Iterates over each tile and its corresponding uniquified PupuID.
   *
   * Visits tiles in traversal order and pairs each tile with the
   * corresponding entry from m_pupus. The provided lambda receives:
   *
   * - the uniquified PupuID
   * - the tile
   *
   * This preserves the stable mapping generated during the original
   * uniquification pass.
   *
   * @tparam lambdaT Callable type.
   * @param lambda Function invoked for each PupuID/tile pair.
   */
  template<typename lambdaT>
  void
    for_each_pupu_and_tile(const lambdaT &lambda) const
  {
    m_map.visit_tiles([this, &lambda](auto &&tiles) {
      auto zipped = std::views::zip(m_pupus, tiles);

      std::ranges::for_each(zipped, [&lambda](const auto &pair) {
        lambda(std::get<0>(pair), std::get<1>(pair));
      });
    });
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
    save_out_buffer_and_clear(std::vector<outColorT> &out, const PupuID &pupu)
      const
  {
    const auto width     = static_cast<uint32_t>(m_canvas.width());
    const auto height    = static_cast<uint32_t>(m_canvas.height());

    const auto base_path = std::filesystem::path(m_path).parent_path()
                         / fmt::format(
                             "{}_{}.mimmap",
                             std::filesystem::path(m_path).stem().string(),
                             pupu);
    if (!Png::save(out, width, height, { .filename = base_path })) {
      spdlog::error("Failed to save mimmap image");
    }

    std::ranges::fill(out, outColorT{});
  }

  template<Color cT, std::integral indT>
  bool
    set_color(
      std::vector<outColorT> &out,
      const indT             &index_out,
      const cT               &color) const
  {
    if (!color.is_black()) {
      // assert(out.at(index_out).is_black() || color==out.at(index_out));
      // so there is possible overlapping tiles here. does this matter? because
      // pixels behind cannot be seen in game anyway?
      if constexpr (std::is_same_v<cT, outColorT>) {
        out.at(index_out) = color;
      }
      else {
        out.at(index_out) = static_cast<outColorT>(color);
      }
      return true;
    }
    return false;
  }
  template<typename tile_type>
  std::uint32_t
    get_output_index(
      const std::uint32_t x,
      const std::uint32_t y,
      const tile_type    &tile) const
  {
    return (static_cast<uint32_t>(tile.x()) + x)
         + ((static_cast<uint32_t>(tile.y()) + y)
            * static_cast<uint32_t>(m_canvas.width()));
  }
  void
    visit_mim(auto &&lambda) const
  {
    std::visit(
      [&lambda](auto &&mim) {
        using mim_type = std::decay_t<decltype(mim)>;
        if constexpr (!std::is_same_v<mim_type, std::monostate>) {
          lambda(std::forward<decltype(mim)>(mim));
        }
      },
      m_mim);
  }
  void
    visit_not_mim(auto &&lambda) const
  {
    std::visit(
      [&lambda](auto &&mim) {
        using mim_type = std::decay_t<decltype(mim)>;
        if constexpr (std::is_same_v<mim_type, std::monostate>) {
          lambda();
        }
      },
      m_mim);
  }

public:
  Deswizzle(
    const decltype(m_mim) &in_mim,
    const Map             &in_map,
    std::string            in_path)
    : m_mim(in_mim), m_map(in_map), m_path(std::move(in_path)),
      m_unique_palettes(find_unique_palettes()), m_canvas(in_map.canvas()),
      m_pupus(find_pupu()), m_unique_pupus(find_unique_pupu())
  {}
  void
    save() const
  {
    std::vector<outColorT> out(static_cast<std::size_t>(m_canvas.area()));

    for_each_pupu_and_tile(
      [this, &out](const PupuID &pupu, const is_tile auto &tile) {
        if (!tile.draw()) {
          return;
        }

        bool drawn = false;

        open_viii::tools::for_each_xy(
          tile.height(),
          [this, &pupu, &out, &drawn, &tile](
            const std::integral auto &x,
            const std::integral auto &y) {
            Color32RGBA pixel_in{};

            visit_mim([&tile, &y, &x, &pixel_in](auto &&mim) {
              pixel_in = Color32RGBA{ mim.get_color(
                static_cast<std::uint32_t>(x + tile.source_x()),
                static_cast<std::uint32_t>(y + tile.source_y()),
                tile.depth(),
                tile.palette_id(),
                tile.texture_id()) };
            });

            const std::uint32_t pixel_out = get_output_index(x, y, tile);

            drawn |= set_color(out, pixel_out, pixel_in);
          });

        if (drawn) {
          save_out_buffer_and_clear(out, pupu);
        }
      });
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_DESWIZZLE_HPP
