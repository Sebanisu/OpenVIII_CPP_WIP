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
#include "open_viii/tools/Tools.hpp"
#include "Pupu.hpp"
#include "Tile1.hpp"
#include "Tile2.hpp"
#include "Tile3.hpp"
#include "tl/write.hpp"
#include <limits>
#include <ranges>
#include <utility>
#include <variant>
namespace open_viii::graphics::background {
template<typename T>
concept is_tile = std::is_same_v<Tile1, std::decay_t<T>>
               || std::is_same_v<Tile2, std::decay_t<T>>
               || std::is_same_v<Tile3, std::decay_t<T>>;

template<typename T>
concept is_tiles = is_tile<typename T::value_type>;

struct Map
{
private:
  std::variant<
    std::monostate,
    std::vector<Tile1>,
    std::vector<Tile2>,
    std::vector<Tile3>>
    m_tiles{};

public:
  auto
    visit_tiles(auto &&lambda) const
  {
    return std::visit(
      [&lambda](const auto &tiles) {
        using tiles_type = std::decay_t<decltype(tiles)>;
        if constexpr (!std::is_same_v<tiles_type, std::monostate>) {
          return lambda(tiles);
        }
        else {
          static const std::vector<Tile1> empty{};
          return lambda(empty);// what if default is bad.
        }
      },
      m_tiles);
  }

  auto
    visit_tiles(auto &&lambda)
  {
    return std::visit(
      [&lambda](auto &tiles) {
        using tiles_type = std::decay_t<decltype(tiles)>;
        if constexpr (!std::is_same_v<tiles_type, std::monostate>) {
          return lambda(tiles);
        }
        else {
          std::vector<Tile1> empty{};
          return lambda(empty);// what if default is bad.
        }
      },
      m_tiles);
  }
  static constexpr auto
    filter_invalid() noexcept
  {
    return [](const auto &...args) {
      static constexpr auto end_x{ (std::numeric_limits<std::int16_t>::max)() };
      return (([](const auto &arg) {
               if constexpr (is_tile<std::remove_cvref<decltype(arg)>>) {
                 return std::cmp_not_equal(arg.x(), end_x);
               }
               else
               {
                return true;
               }
             }(args))
          && ...);
    };
  }

  static constexpr auto
    filter_view_invalid() noexcept
  {
    return std::ranges::views::filter(Map::filter_invalid());
  }
  [[nodiscard]] auto
    offset() const noexcept
  {
    return m_offset;
  }

  bool
    operator==(const Map &other) const noexcept
  {
    return (m_tiles.index() == m_tiles.index())
        && other.visit_tiles([this](const auto &other_tiles) -> bool {
             return visit_tiles([&other_tiles](const auto &tiles) -> bool {
               if constexpr (std::is_same_v<
                               std::remove_cvref<decltype(tiles)>,
                               std::remove_cvref<decltype(other_tiles)>>) {
                 return std::ranges::equal(tiles, other_tiles);
               }
               else {
                 return false;
               }
             });
           });
  }

private:
  void
    visit_not_tiles(auto &&lambda) const
  {
    std::visit(
      [&lambda](auto &&tiles) {
        using tiles_type = std::decay_t<decltype(tiles)>;
        if constexpr (std::is_same_v<tiles_type, std::monostate>) {
          lambda();
        }
      },
      m_tiles);
  }
  /**
   * offset holds the original position of canvas.
   */
  Point<std::int16_t> m_offset{};
  //  /**
  //   * remove invalid tiles from buffer.
  //   */
  //  void
  //    remove_invalid()
  //  {
  //    [[maybe_unused]] static constexpr auto cmp = [](const auto &x) {
  //      static constexpr auto end_x{ 0x7FFFU };
  //      // static constexpr auto limit = 1000U;
  //      return x.x() == end_x
  //          || !x.draw();// || std::abs(x.y()) > limit || std::abs(x.x()) >
  //                       // limit; //|| x.source_x() >limit || x.source_y()
  //                       // >limit; //|| !x.draw();
  //    };
  //    visit_tiles([](auto &&tiles) {
  //      std::erase_if(tiles, cmp);
  //    });
  //  }

  //  /**
  //   * Sort the data and then remove duplicates.
  //   */
  //  void
  //    sort_remove_duplicates()
  //  {
  //    // unique requires sorted data to work.
  //    sort();
  //    visit_tiles([](auto &&tiles) {
  //      auto last = std::unique(tiles.begin(), tiles.end());
  //      tiles.erase(last, tiles.end());
  //    });
  //  }
  //  /**
  //   * Sort in draw order.
  //   */
  //  void
  //    sort()
  //  {
  //    [[maybe_unused]] static constexpr auto cmp
  //      = [](const auto &l, const auto &r) -> bool {
  //      if (l.z() > r.z()) {
  //        return true;
  //      }
  //      //      if (l.z() < r.z()) {
  //      //        return false;
  //      //      }
  //      //      if (l.layer_id() > r.layer_id()) {
  //      //        return true;
  //      //      }
  //      //      if (l.layer_id() < r.layer_id()) {
  //      //        return false;
  //      //      }
  //      //      if (l.animation_id() < r.animation_id()) {
  //      //        return true;
  //      //      }
  //      //      if (l.animation_id() > r.animation_id()) {
  //      //        return false;
  //      //      }
  //      //      if (l.animation_state() < r.animation_state()) {
  //      //        return true;
  //      //      }
  //      //      if (l.animation_state() > r.animation_state()) {
  //      //        return false;
  //      //      }
  //      //      if (l.x() < r.x()) {
  //      //        return true;
  //      //      }
  //      //      if (l.x() > r.x()) {
  //      //        return false;
  //      //      }
  //      //      if (l.y() < r.y()) {
  //      //        return true;
  //      //      }
  //      return false;
  //    };
  //    visit_tiles([](auto &&tiles) {
  //      std::ranges::stable_sort(tiles, cmp);
  //    });
  //  }
  [[nodiscard]] constexpr auto
    minmax_generic(auto compare, auto get, auto fail) const noexcept
  {
    return std::visit(
      [&compare, &get, &fail](auto &&tiles) {
        using tiles_type = std::decay_t<decltype(tiles)>;
        if constexpr (!std::is_same_v<tiles_type, std::monostate>) {
          auto filtered = tiles | std::views::filter(filter_invalid());
          auto tmp      = std::ranges::minmax_element(filtered, compare);
          return get(tmp);
        }
        else {
          return fail();
        }
      },
      m_tiles);
  }
  [[nodiscard]] constexpr auto
    min_generic(auto compare, auto get, auto fail) const noexcept
  {
    return std::visit(
      [&compare, &get, &fail](auto &&tiles) {
        using tiles_type = std::decay_t<decltype(tiles)>;
        if constexpr (!std::is_same_v<tiles_type, std::monostate>) {
          auto filtered = tiles | std::views::filter(filter_invalid());
          auto tmp      = std::ranges::min_element(filtered, compare);
          return get(tmp);
        }
        else {
          return fail();
        }
      },
      m_tiles);
  }
  [[nodiscard]] constexpr auto
    max_generic(auto compare, auto get, auto fail) const noexcept
  {
    return std::visit(
      [&compare, &get, &fail](auto &&tiles) {
        using tiles_type = std::decay_t<decltype(tiles)>;
        if constexpr (!std::is_same_v<tiles_type, std::monostate>) {
          auto filtered = tiles | std::views::filter(filter_invalid());
          auto tmp      = std::ranges::max_element(filtered, compare);
          return get(tmp);
        }
        else {
          return fail();
        }
      },
      m_tiles);
  }
  [[nodiscard]] constexpr auto
    minmax_x() const noexcept
  {
    return minmax_generic(
      [](const auto &a, const auto &b) -> bool {
        return a.x() < b.x();
      },
      [](auto &&tmp) {
        return std::pair<const std::int16_t, const std::int16_t>{
          tmp.min->x(),
          tmp.max->x()
        };
      },
      []() {
        return std::pair<const std::int16_t, const std::int16_t>{};
      });
  }
  [[nodiscard]] constexpr auto
    min_x() const noexcept
  {
    return min_generic(
      [](const auto &a, const auto &b) -> bool {
        return a.x() < b.x();
      },
      [](auto &&tmp) {
        return tmp->x();
      },
      []() {
        return std::int16_t{};
      });
  }
  [[nodiscard]] constexpr auto
    max_x() const noexcept
  {
    return max_generic(
      [](const auto &a, const auto &b) -> bool {
        return a.x() < b.x();
      },
      [](auto &&tmp) {
        return tmp->x();
      },
      []() {
        return std::int16_t{};
      });
  }
  [[nodiscard]] constexpr auto
    minmax_y() const noexcept
  {
    return minmax_generic(
      [](const auto &a, const auto &b) -> bool {
        return a.y() < b.y();
      },
      [](auto &&tmp) {
        return std::pair<const std::int16_t, const std::int16_t>{
          tmp.min->y(),
          tmp.max->y()
        };
      },
      []() {
        return std::pair<const std::int16_t, const std::int16_t>{};
      });
  }
  [[nodiscard]] constexpr auto
    min_y() const noexcept
  {
    return min_generic(
      [](const auto &a, const auto &b) -> bool {
        return a.y() < b.y();
      },
      [](auto &&tmp) {
        return tmp->y();
      },
      []() {
        return std::int16_t{};
      });
  }
  [[nodiscard]] constexpr auto
    max_y() const noexcept
  {
    return max_generic(
      [](const auto &a, const auto &b) -> bool {
        return a.y() < b.y();
      },
      [](auto &&tmp) {
        return tmp->y();
      },
      []() {
        return std::int16_t{};
      });
  }

  /**
   * Some tiles are drawn off screen to create a texture we are going to shift
   * everything to at least (0,0)
   */
  void
    shift_to_origin() noexcept
  {
    m_offset = Point(min_x(), min_y());
    if (m_offset.x() < 0 || m_offset.y() < 0) {
      shift(m_offset.abs());
    }
  }
  static auto
    init_tiles(const MimType &mim_type, const std::vector<char> &buffer)
  {
    decltype(m_tiles) tiles{};
    if (mim_type.type() == 1) {
      tiles = std::vector<Tile1>{};
    }
    else if (mim_type.type() == 2) {
      tiles = std::vector<Tile2>{};
    }
    else if (mim_type.type() == 3) {
      tiles = std::vector<Tile3>{};
    }
    else {
      tiles = std::monostate{};
    }
    std::visit(
      [&buffer](auto &&local_tiles) {
        using tiles_type = std::decay_t<decltype(local_tiles)>;
        if constexpr (!std::is_same_v<tiles_type, std::monostate>) {
          using map_type   = typename tiles_type::value_type;
          const auto count = std::ranges::size(buffer) / sizeof(map_type);
          const auto size_in_bytes = count * sizeof(map_type);
          local_tiles.resize(count);
          std::memcpy(
            std::ranges::data(local_tiles),
            std::ranges::data(buffer),
            size_in_bytes);
        }
      },
      tiles);
    return tiles;
  }

public:
  constexpr static auto EXT = std::string_view(".map");
  Map()                     = default;
  /**
   * Import tiles from a raw buffer.
   * @param buffer is a raw char buffer.
   */
  explicit Map(
    const MimType           &mim_type,
    const std::vector<char> &buffer,
    // bool                     sort_remove = true,
    bool                     shift = true)
    : m_tiles(init_tiles(mim_type, buffer))
  {
    //    if (sort_remove) {
    //      remove_invalid();
    //      // sort_remove_duplicates();
    //    }
    if (shift) {
      shift_to_origin();
    }
  }
  Map(
    const MimType               &mim_type,
    const std::filesystem::path &path,
    // bool                         sort_remove = true,
    bool                         shift = true)
    : Map(mim_type, tools::read_entire_file(path), shift)
  {}
  Map(
    std::integral auto &&mim_type,
    auto               &&buffer,
    bool                 sort_remove = true,
    bool                 shift       = true)
    : Map(
        static_cast<MimType>(mim_type),
        std::forward<decltype(buffer)>(buffer),
        sort_remove,
        shift)
  {}
  template<typename tile_funcT>
    requires(std::is_invocable_r_v<
             std::variant<
               open_viii::graphics::background::Tile1,
               open_viii::graphics::background::Tile2,
               open_viii::graphics::background::Tile3,
               std::monostate>,
             tile_funcT>)
  explicit Map(tile_funcT tile_func) : m_tiles(std::monostate())
  {

    bool       on   = true;
    const auto push = [this, &on](const auto &item) {
      using map_type = std::decay_t<decltype(item)>;
      if constexpr (std::is_same_v<map_type, std::monostate>) {
        on = false;
      }
      else {
        visit_not_tiles([this]() {
          m_tiles = std::vector<map_type>{};
        });
        visit_tiles([&on, &item](auto &&tiles) {
          using tiles_type     = std::decay_t<decltype(tiles)>;
          using local_map_type = typename tiles_type::value_type;
          if constexpr (std::is_same_v<map_type, local_map_type>) {
            tiles.push_back(item);
          }
          else {
            on = false;
          }
        });
      }
    };
    while (on) {
      std::visit(push, tile_func());
    }
  }
  std::uint8_t
    get_type() const noexcept
  {
    //    std::uint8_t ret = {};
    //    visit_tiles([&on, &item](auto &&tiles) {
    //      using tiles_type     = std::decay_t<decltype(tiles)>;
    //      using local_map_type = typename tiles_type::value_type;
    //      if constexpr ()
    //    });
    if (m_tiles.index() == 3U) {
      return static_cast<std::uint8_t>(0U);
    }
    return static_cast<std::uint8_t>(m_tiles.index() + 1U);
  }
  //  template<std::invocable tile_funcT, typename map_type =
  //  decltype(tile_func())> requires(std::is_same_v<map_type, std::monostate>)
  //  explicit Map(
  //    tile_funcT tile_func) : m_tiles(std::monostate)
  //  {}
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
      l_min_x,
      l_min_y,
      std::abs(l_max_x) + std::abs(l_min_x) + tile_size,
      std::abs(l_max_y) + std::abs(l_min_y) + tile_size
    };
    return l_canvas;
  }
  /**
   * Shift all the positions of the tiles.
   * @param x horizontal shift
   * @param y vertical shift
   */
  void
    shift(const std::int16_t &x, const std::int16_t &y) noexcept
  {
    const auto xy = Point(x, y);
    visit_tiles([&xy](auto &tiles) {
      auto filtered = tiles | std::views::filter(filter_invalid());
      std::ranges::transform(
        filtered,
        std::ranges::begin(tiles),
        [&xy](const auto &t) {
          return t.shift_xy(xy);
        });
    });
  }
  void
    shift(const Point<std::int16_t> &point) noexcept
  {
    shift(point.x(), point.y());
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
        visit_tiles([&i, &os](auto &&tiles) {
          std::for_each(
            std::ranges::cbegin(tiles),
            std::ranges::cend(tiles),
            [&os, &i](const auto &t) {
              os << i++ << ',' << '"';
              t.to_hex(os);
              os << "\"," << t.draw() << ',' << int{ t.depth() } << ",\"" <<
                [&t]() {
                  switch (t.blend_mode()) {
                  case BlendModeT::half_add:
                    return "Half Add";
                  case BlendModeT::add:
                    return "Add";
                  case BlendModeT::subtract:
                    return "Subtract";
                  case BlendModeT::quarter_add:
                    return "Quarter Add";
                  case BlendModeT::none:
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
        });
      },
      (path.parent_path() / path.stem()).string() + ".csv");
  }
  void
    save_map(const std::string_view &in_path) const
  {
    auto path = std::filesystem::path(in_path);
    tools::write_buffer(
      [this](std::ostream &os) {
        visit_tiles([&os](auto &&tiles) {
          std::ranges::for_each(tiles, [&os](auto &&tile) {
            tl::write::append(os, tile);
          });
        });
      },
      (path.parent_path() / path.stem()).string() + ".map");
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
  //          if (depth != t.depth() || palette != t.palette_id() ||
  //          !t.draw())
  //          {
  //            continue;
  //          }
  //          for (std::uint32_t y = {}; y < t.HEIGHT; y++) {
  //            for (std::uint32_t x = {}; x < t.WIDTH; x++) {
  //              if (t.source_y() < 0 || t.source_x() < 0) {
  //                continue;
  //              }
  //              auto pixel_in = (static_cast<std::uint32_t>(t.source_x()) +
  //              x)
  //                              + ((static_cast<std::uint32_t>(t.source_y())
  //                              + y) * width);
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
  //                               + ((static_cast<std::uint32_t>(t.y()) + y)
  //                               *
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
// template<typename T>
// void
//   save_map(const T &map, const std::string_view &path)
//{
//   std::visit(
//     [&path](const auto &m) {
//       if constexpr (!is_monostate<decltype(m)>)
//         m.save_map(path);
//     },
//     map);
// }
auto
  get_tile(char tile_type, std::ranges::range auto &&data, auto &&...misc)
    -> std::variant<Tile1, Tile2, Tile3, std::monostate>
{
  switch (tile_type) {
  case 1:
  case '1':
    return Tile1(
      std::forward<decltype(data)>(data),
      std::forward<decltype(misc)>(misc)...);
    break;
  case 2:
  case '2':
    return Tile2(
      std::forward<decltype(data)>(data),
      std::forward<decltype(misc)>(misc)...);
    break;
  case 3:
  case '3':
    return Tile3(
      std::forward<decltype(data)>(data),
      std::forward<decltype(misc)>(misc)...);
    break;
  }
  return std::monostate{};
}
inline std::ostream &
  operator<<(std::ostream &os, const Map &m)
{
  m.visit_tiles([&os](auto &&tiles) {
    os << "map contains " << tiles.size() << " tile entries. \n";
  });
  return os;
}

}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_MAP_HPP
