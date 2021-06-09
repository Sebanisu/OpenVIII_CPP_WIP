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
#ifndef VIIIARCHIVE_MAP_HPP
#define VIIIARCHIVE_MAP_HPP
#include "Mim.hpp"
#include "Pupu.hpp"
#include "Tile1.hpp"
#include "Tile2.hpp"
#include "Tile3.hpp"
#include "open_viii/tools/Tools.hpp"
#include <bit>
#include <bitset>
#include <cstdint>
#include <filesystem>
#include <variant>
namespace open_viii::graphics::background {
template<typename map_type = Tile1>
requires(
  std::is_same_v<
    map_type,
    Tile1> || std::is_same_v<map_type, Tile2> || std::is_same_v<map_type, Tile3>) struct
  Map
{
private:
  mutable std::vector<map_type> m_tiles{};
  /**
   * offset holds the original position of canvas.
   */
  mutable Point<std::int16_t> m_offset{};
  /**
   * remove invalid tiles from buffer.
   */
  void
    remove_invalid()
  {
    static constexpr auto cmp = [](const auto &x) {
      static constexpr auto end_x{ 0x7FFFU };
      // static constexpr auto limit = 1000U;
      return x.x() == end_x;// || std::abs(x.y()) > limit || std::abs(x.x()) >
                            // limit; //|| x.source_x() >limit || x.source_y()
                            // >limit; //|| !x.draw();
    };
    std::erase_if(m_tiles, cmp);
  }
  /**
   * Sort the data and then remove duplicates.
   */
  void
    sort_remove_duplicates()
  {
    // unique requires sorted data to work.
    sort();
    auto last = std::unique(m_tiles.begin(), m_tiles.end());
    m_tiles.erase(last, m_tiles.end());
  }
  /**
   * Sort in draw order.
   */
  void
    sort()
  {
    static constexpr auto cmp = [](const auto &l, const auto &r) -> bool {
      if (l.z() > r.z()) {
        return true;
      }
      if (l.z() < r.z()) {
        return false;
      }
      if (l.layer_id() < r.layer_id()) {
        return true;
      }
      if (l.layer_id() > r.layer_id()) {
        return false;
      }
      if (l.animation_id() < r.animation_id()) {
        return true;
      }
      if (l.animation_id() > r.animation_id()) {
        return false;
      }
      if (l.animation_state() < r.animation_state()) {
        return true;
      }
      if (l.animation_state() > r.animation_state()) {
        return false;
      }
      if (l.x() < r.x()) {
        return true;
      }
      if (l.x() > r.x()) {
        return false;
      }
      if (l.y() < r.y()) {
        return true;
      }
      return false;
    };
    std::ranges::sort(m_tiles, cmp);
  }
  /**
   * Some tiles are drawn off screen to create a texture we are going to shift
   * everything to at least (0,0)
   */
  void
    shift_to_origin() const noexcept
  {
    m_offset = Point(min_x(), min_y());
    if (m_offset.x() < 0 || m_offset.y() < 0) {
      shift(m_offset.abs());
    }
  }
  [[nodiscard]] constexpr auto
    min_x() const noexcept
  {
    return (std::min_element(m_tiles.cbegin(),
                             m_tiles.cend(),
                             [](const auto &a, const auto &b) -> bool {
                               return a.x() < b.x();
                             }))
      ->x();
  }
  [[nodiscard]] auto
    minmax_x() const noexcept
  {
    return std::minmax_element(m_tiles.cbegin(),
                               m_tiles.cend(),
                               [](const auto &a, const auto &b) -> bool {
                                 return a.x() < b.x();
                               });
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    max_x() const noexcept
  {
    return (std::max_element(m_tiles.cbegin(),
                             m_tiles.cend(),
                             [](const auto &a, const auto &b) -> bool {
                               return a.x() < b.x();
                             }))
      ->x();
  }
  [[nodiscard]] auto
    minmax_y() const noexcept
  {
    return std::minmax_element(m_tiles.cbegin(),
                               m_tiles.cend(),
                               [](const auto &a, const auto &b) -> bool {
                                 return a.y() < b.y();
                               });
  }
  [[nodiscard]] constexpr auto
    min_y() const noexcept
  {
    return (std::min_element(m_tiles.cbegin(),
                             m_tiles.cend(),
                             [](const auto &a, const auto &b) -> bool {
                               return a.y() < b.y();
                             }))
      ->y();
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto
    max_y() const noexcept
  {
    return (std::max_element(m_tiles.cbegin(),
                             m_tiles.cend(),
                             [](const auto &a, const auto &b) -> bool {
                               return a.y() < b.y();
                             }))
      ->y();
  }

public:
  constexpr static auto EXT = std::string_view(".map");
  Map()                     = default;
  /**
   * Import tiles from a raw buffer.
   * @param buffer is a raw char buffer.
   */
  explicit Map(const std::vector<char> &buffer,
               bool                     sort_remove = true,
               bool                     shift       = true)
  {
    const auto count         = std::ranges::size(buffer) / sizeof(map_type);
    const auto size_in_bytes = count * sizeof(map_type);
    m_tiles.resize(count);
    std::memcpy(
      std::ranges::data(m_tiles), std::ranges::data(buffer), size_in_bytes);
    if (sort_remove) {
      remove_invalid();
      sort_remove_duplicates();
    }
    if (shift) {
      shift_to_origin();
    }
  }
  explicit Map(const std::filesystem::path &path,
               bool                         sort_remove = true,
               bool                         shift       = true)
    : Map(tools::read_entire_file(path), sort_remove, shift)
  {}
  template<std::invocable tile_funcT> explicit Map(tile_funcT tile_func)
  {
    bool       on   = true;
    const auto push = [this, &on](auto item) {
      if constexpr (std::is_same_v<decltype(item), map_type>) {
        m_tiles.push_back(item);
      } else {
        on = false;
      }
    };
    while (on) {
      std::visit(push, tile_func());
    }
  }
  [[nodiscard]] const auto &
    tiles() const noexcept
  {
    return m_tiles;
  }
  [[nodiscard]] const auto *
    operator->() const noexcept
  {
    return &m_tiles;
  }
  /**
   * Get a rectangle large enough to hold all the tiles.
   * @return rectangle
   */
  [[nodiscard]] Rectangle<std::int32_t>
    canvas() const noexcept
  {
    const auto l_minmax_x                                  = minmax_x();
    const auto l_minmax_y                                  = minmax_y();
    const auto &[l_min_x, l_max_x]                         = l_minmax_x;
    const auto &[l_min_y, l_max_y]                         = l_minmax_y;
    constexpr static auto                        tile_size = 16;
    open_viii::graphics::Rectangle<std::int32_t> l_canvas{
      l_min_x->x(),
      l_min_y->y(),
      static_cast<std::int32_t>(std::abs(l_max_x->x()) + std::abs(l_min_x->x())
                                + tile_size),
      static_cast<std::int32_t>(std::abs(l_max_y->y()) + std::abs(l_min_y->y())
                                + tile_size)
    };
    return l_canvas;
  }
  /**
   * Shift all the positions of the tiles.
   * @param x horizontal shift
   * @param y vertical shift
   */
  void
    shift(const std::int16_t &x, const std::int16_t &y) const noexcept
  {
    const auto xy = Point(x, y);
    std::ranges::transform(m_tiles, std::ranges::begin(m_tiles), [&xy](auto t) {
      return t.with_xy(t.xy() + xy);
    });
  }
  void
    shift(const Point<std::int16_t> &point) const noexcept
  {
    shift(point.x(), point.y());
  }
  [[nodiscard]] friend std::ostream &
    operator<<(std::ostream &os, const Map &m)
  {
    return os << std::ranges::size(m.m_tiles) << ", "
              << std::ranges::size(m.m_t2) << ", " << std::ranges::size(m.m_t3)
              << '\n';
  }
  void
    save_csv(const std::string_view &in_path) const
  {
    auto path = std::filesystem::path(in_path);
    tools::write_buffer(
      [this](std::ostream &os) {
        os
          << R"("Index","Raw bytes","Draw","BPP","Blend Mode","Blend Other","Layer","Texture Page","Palette","Animation","Animation Frame","Source X","Source Y","X","Y","Z")"
          << '\n';
        std::size_t i{};
        std::for_each(
          std::ranges::cbegin(m_tiles),
          std::ranges::cend(m_tiles),
          [&os, &i](const auto &t) {
            std::array<char, sizeof(t)> raw{};
            std::memcpy(raw.data(), &t, sizeof(t));
            os << i++ << ',' << '"' << "0x";
            for (char c : raw)
              os << std::hex << std::setfill('0') << std::setw(2)
                 << std::uppercase << (static_cast<unsigned short>(c) & 0xFFU);
            os << std::dec << std::setfill(' ') << std::setw(1)
               << std::nouppercase << "\"," << t.draw() << ','
               << int{ t.depth() } << ",\"" <<
              [&t]() {
                switch (t.blend_mode()) {
                case open_viii::graphics::background::BlendModeT::half_add:
                  return "Half Add";
                case open_viii::graphics::background::BlendModeT::add:
                  return "Add";
                case open_viii::graphics::background::BlendModeT::subtract:
                  return "Subtract";
                case open_viii::graphics::background::BlendModeT::quarter_add:
                  return "Quarter Add";
                case open_viii::graphics::background::BlendModeT::none:
                default:
                  return "None";
                }
              }()
               << "\"," << static_cast<uint16_t>(t.blend()) << ','
               << static_cast<uint16_t>(t.layer_id()) << ','
               << static_cast<uint16_t>(t.texture_id()) << ','
               << static_cast<uint16_t>(t.palette_id()) << ','
               << static_cast<uint16_t>(t.animation_id()) << ','
               << static_cast<uint16_t>(t.animation_state()) << ','
               << static_cast<uint16_t>(t.source_x()) << ','
               << static_cast<uint16_t>(t.source_y()) << ','
               << static_cast<int16_t>(t.x()) << ','
               << static_cast<int16_t>(t.y()) << ','
               << static_cast<int16_t>(t.z()) << ','//<< Pupu(t)
               << '\n';// std::nouppercase << std::dec << std::setw(0U)
          });
      },
      (path.parent_path() / path.stem()).string() + ".csv");
  }
  //  [[nodiscard]] auto used_depth_and_palette() const
  //  {
  //    using T = typename std::decay<decltype(*m_tiles.begin())>::type;
  //    std::vector<T> out_s1{};
  //    static constexpr auto default_size = 16U;
  //    out_s1.reserve(default_size);
  //    std::unique_copy(
  //      m_tiles.begin(), m_tiles.end(), std::back_inserter(out_s1), [](const
  //      auto &left, const auto &right) {
  //        return left.depth() == right.depth() && left.palette_id() ==
  //        right.palette_id();
  //      });
  //    std::vector<std::pair<BPPT, std::uint8_t>> out{};
  //    out.reserve(std::ranges::size(out_s1));
  //    std::transform(out_s1.begin(), out_s1.end(), std::back_inserter(out),
  //    [](const auto &tile) {
  //      return std::make_pair(tile.depth(), tile.palette_id());
  //    });
  //    std::sort(out.begin(), out.end(), [](const auto &left, const auto
  //    &right) {
  //      if (left.first < right.first) {
  //        return true;
  //      }
  //      if (left.first > right.first) {
  //        return false;
  //      }
  //      if (left.second < right.second) {
  //        return true;
  //      }
  //      return false;
  //    });
  //    auto last = std::unique(out.begin(), out.end());
  //    out.erase(last, out.end());
  //    return out;
  //  }
  //  void save_v1(const Mim &in_mim, const std::string_view &in_path) const
  //  {
  //
  //    const auto &ts = tiles();
  //    Rectangle<std::int32_t> rect = canvas();
  //    const auto dnps = used_depth_and_palette();
  //    for (const auto &[depth, palette] : dnps) {
  //      const auto ppm_save = [this, &rect, &depth, &palette, &ts](const
  //      std::span<const Color16> &data,
  //                              const std::size_t &width,
  //                              const std::size_t &height,
  //                              std::string local_filename) {
  //        // Ppm::save(data, width, height, local_filename);
  //        std::vector<Color16> out{};
  //        out.resize(width * height);
  //        bool drawn = false;
  //        for (const auto &t : ts) {
  //          // auto x = t.x();
  //          // auto y = t.y();
  //
  //          if (depth != t.depth() || palette != t.palette_id() || !t.draw())
  //          {
  //            continue;
  //          }
  //          for (std::uint32_t y = {}; y < t.HEIGHT; y++) {
  //            for (std::uint32_t x = {}; x < t.WIDTH; x++) {
  //              if (t.source_y() < 0 || t.source_x() < 0) {
  //                continue;
  //              }
  //              auto pixel_in = (static_cast<std::uint32_t>(t.source_x()) + x)
  //                              + ((static_cast<std::uint32_t>(t.source_y()) +
  //                              y) * width);
  //              if (t.depth().bpp4()) {
  //                pixel_in += t.TEXTURE_PAGE_WIDTH * t.texture_id() * 2U;
  //              } else if (t.depth().bpp8()) {
  //                pixel_in += t.TEXTURE_PAGE_WIDTH * t.texture_id();
  //              }
  //
  //              else if (t.depth().bpp16()) {
  //                pixel_in += (t.TEXTURE_PAGE_WIDTH * t.texture_id()) / 2U;
  //              }
  //              auto pixel_out = (static_cast<std::uint32_t>(t.x()) + x)
  //                               + ((static_cast<std::uint32_t>(t.y()) + y) *
  //                               static_cast<std::uint32_t>(rect.width()));
  //              Color16 color = data[pixel_in];
  //              if (!color.is_black()) {
  //                out.at(pixel_out) = data[pixel_in];
  //                drawn = true;
  //              }
  //            }
  //          }
  //        }
  //        if (drawn) {
  //          auto p = std::filesystem::path(local_filename);
  //          local_filename = (p.parent_path() / p.stem()).string() +
  //          "_tiled_mim.map"; Ppm::save(
  //            out, static_cast<std::uint32_t>(rect.width()),
  //            static_cast<std::uint32_t>(rect.height()), local_filename);
  //        }
  //      };
  //      in_mim.get_colors(in_path, depth, palette, ppm_save, false);
  //    }
  //  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_MAP_HPP
