//
// Created by pcvii on 9/3/2020.
//

#ifndef VIIIARCHIVE_MAP_H
#define VIIIARCHIVE_MAP_H
#include <cstdint>
#include <filesystem>
#include "Tile1.h"
#include "Tile2.h"
#include "Tile3.h"
#include "Mim.h"
#include <bit>
#include <bitset>
#include "OpenVIII/Tools/Tools.h"
namespace open_viii::graphics::background {
template<size_t typeT = 1> requires(typeT >= 0 && typeT <= 3) struct Map
{
private:
  mutable std::vector<Tile1> m_t1{};
  mutable std::vector<Tile2> m_t2{};
  mutable std::vector<Tile3> m_t3{};

  void remove_end()
  {
    static constexpr auto cmp = [](const auto &x) {
      static constexpr auto END_X{ 0x7FFFU };
      [[maybe_unused]] static constexpr auto limit = 1000U;

      return x.x() == END_X;// || std::abs(x.y()) > limit || std::abs(x.x()) > limit; //|| x.source_x() >limit ||
                            // x.source_y() >limit; //|| !x.draw();
    };

    if constexpr (typeT == 0 || typeT == 1) {
      std::erase_if(m_t1, cmp);
    }
    if constexpr (typeT == 0 || typeT == 2) {
      std::erase_if(m_t2, cmp);
    }
    if constexpr (typeT == 0 || typeT == 3) {
      std::erase_if(m_t3, cmp);
    }
  }
  void remove_duplicates()
  {
    if constexpr (typeT == 0 || typeT == 1) {
      auto last = std::unique(m_t1.begin(), m_t1.end());
      m_t1.erase(last, m_t1.end());
    }
    if constexpr (typeT == 0 || typeT == 2) {
      auto last = std::unique(m_t2.begin(), m_t2.end());
      m_t2.erase(last, m_t2.end());
    }
    if constexpr (typeT == 0 || typeT == 3) {
      auto last = std::unique(m_t3.begin(), m_t3.end());
      m_t3.erase(last, m_t3.end());
    }
  }
  void sort()
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
    if constexpr (typeT == 0 || typeT == 1) {
      std::sort(m_t1.begin(), m_t1.end(), cmp);
    }
    if constexpr (typeT == 0 || typeT == 2) {
      std::sort(m_t2.begin(), m_t2.end(), cmp);
    }
    if constexpr (typeT == 0 || typeT == 3) {
      std::sort(m_t3.begin(), m_t3.end(), cmp);
    }
  }

public:
  Map() = default;
  explicit Map([[maybe_unused]] std::span<const char> buffer)
  {
    if constexpr (typeT == 0 || typeT == 1) {
      auto t1 = std::span(
        reinterpret_cast<const Tile1 *>(std::ranges::data(buffer)), std::ranges::size(buffer) / sizeof(Tile1));
      m_t1 = { t1.begin(), t1.end() };
    }
    if constexpr (typeT == 0 || typeT == 2) {
      auto t2 = std::span(
        reinterpret_cast<const Tile2 *>(std::ranges::data(buffer)), std::ranges::size(buffer) / sizeof(Tile2));
      m_t2 = { t2.begin(), t2.end() };
    }
    if constexpr (typeT == 0 || typeT == 3) {
      auto t3 = std::span(
        reinterpret_cast<const Tile3 *>(std::ranges::data(buffer)), std::ranges::size(buffer) / sizeof(Tile3));
      m_t3 = { t3.begin(), t3.end() };
    }
    remove_end();
    remove_duplicates();
    sort();
  }
  [[nodiscard]] const auto &tiles() const noexcept
  {
    if constexpr (typeT == 2) {
      return m_t2;
    } else if constexpr (typeT == 3) {
      return m_t3;
    } else {
      return m_t1;
    }
  }
  [[maybe_unused]] [[nodiscard]] const auto &min_x() const noexcept
  {
    const auto &t = tiles();
    return (std::min_element(t.cbegin(), t.cend(), [](const auto &a, const auto &b) -> bool { return a.x() < b.x(); }))
      ->x();
  }
  [[nodiscard]] auto minmax_x() const noexcept
  {
    const auto &t = tiles();
    return std::minmax_element(
      t.cbegin(), t.cend(), [](const auto &a, const auto &b) -> bool { return a.x() < b.x(); });
  }

  [[maybe_unused]] [[nodiscard]] const auto &max_x() const noexcept
  {
    const auto &t = tiles();
    return (std::max_element(t.cbegin(), t.cend(), [](const auto &a, const auto &b) -> bool { return a.x() < b.x(); }))
      ->x();
  }
  [[nodiscard]] auto minmax_y() const noexcept
  {
    const auto &t = tiles();
    return std::minmax_element(
      t.cbegin(), t.cend(), [](const auto &a, const auto &b) -> bool { return a.y() < b.y(); });
  }
  [[maybe_unused]] [[nodiscard]] const auto &min_y() const noexcept
  {
    const auto &t = tiles();
    return (std::min_element(t.cbegin(), t.cend(), [](const auto &a, const auto &b) -> bool { return a.y() < b.y(); }))
      ->y();
  }

  [[maybe_unused]] [[nodiscard]] const auto &max_y() const noexcept
  {
    const auto &t = tiles();
    return (std::max_element(t.cbegin(), t.cend(), [](const auto &a, const auto &b) -> bool { return a.y() < b.y(); }))
      ->y();
  }

  [[nodiscard]] auto unique_pupu() const
  {

    const auto &t = tiles();
    std::vector<std::uint64_t> out{};
    out.reserve(std::ranges::size(t));
    std::transform(std::ranges::cbegin(t), std::ranges::cend(t), std::back_inserter(out), [this](const auto &tile) {
      return generate_pupu(tile);
    });
    std::sort(std::ranges::begin(out), std::ranges::end(out));
    auto last = std::unique(std::ranges::begin(out), std::ranges::end(out));
    out.erase(last, std::ranges::end(out));
    return out;
  }
  [[nodiscard]] auto used_depth_and_palette() const
  {
    const auto &t = tiles();
    using T = typename std::decay<decltype(*t.begin())>::type;
    std::vector<T> out_s1{};
    static constexpr auto default_size = 16U;
    out_s1.reserve(default_size);
    std::unique_copy(t.begin(), t.end(), std::back_inserter(out_s1), [](const auto &left, const auto &right) {
      return left.depth() == right.depth() && left.palette_id() == right.palette_id();
    });
    std::vector<std::pair<BPPT, std::uint8_t>> out{};
    out.reserve(std::ranges::size(out_s1));
    std::transform(out_s1.begin(), out_s1.end(), std::back_inserter(out), [](const auto &tile) {
      return std::make_pair(tile.depth(), tile.palette_id());
    });
    std::sort(out.begin(), out.end(), [](const auto &left, const auto &right) {
      if (left.first < right.first) {
        return true;
      }
      if (left.first > right.first) {
        return false;
      }
      if (left.second < right.second) {
        return true;
      }
      return false;
    });
    auto last = std::unique(out.begin(), out.end());
    out.erase(last, out.end());
    return out;
  }
  [[nodiscard]] Rectangle<std::int32_t> canvas() const noexcept
  {
    const auto l_minmax_x = minmax_x();
    const auto l_minmax_y = minmax_y();

    const auto &[l_min_x, l_max_x] = l_minmax_x;
    const auto &[l_min_y, l_max_y] = l_minmax_y;
    constexpr static auto tile_size = 16;

    open_viii::graphics::Rectangle<std::int32_t> l_canvas{};
    l_canvas.x(l_min_x->x());
    l_canvas.y(l_min_y->y());
    l_canvas.height(static_cast<std::int32_t>(std::abs(l_max_y->y()) + std::abs(l_min_y->y()) + tile_size));
    l_canvas.width(static_cast<std::int32_t>(std::abs(l_max_x->x()) + std::abs(l_min_x->x()) + tile_size));
    return l_canvas;
  }
  void shift_to_origin() const noexcept
  {
    const auto l_minmax_x = minmax_x();
    const auto l_minmax_y = minmax_y();

    const auto &[l_min_x, l_max_x] = l_minmax_x;
    const auto &[l_min_y, l_max_y] = l_minmax_y;
    const auto abs_x = std::abs(l_min_x->x());
    const auto abs_y = std::abs(l_min_y->y());

    if constexpr (typeT == 1 || typeT == 0) {
      std::transform(std::execution::seq, m_t1.cbegin(), m_t1.cend(), m_t1.begin(), [&abs_x, &abs_y](Tile1 t) {
        t.x(static_cast<std::int16_t>(t.x() + abs_x));
        t.y(static_cast<std::int16_t>(t.y() + abs_y));
        return t;
      });
    }
    if constexpr (typeT == 2 || typeT == 0) {
      std::transform(std::execution::seq, m_t2.cbegin(), m_t2.cend(), m_t2.begin(), [&abs_x, &abs_y](Tile2 t) {
        t.x(static_cast<std::int16_t>(t.x() + abs_x));
        t.y(static_cast<std::int16_t>(t.y() + abs_y));
        return t;
      });
    }
    if constexpr (typeT == 3 || typeT == 0) {
      std::transform(std::execution::seq, m_t3.cbegin(), m_t3.cend(), m_t3.begin(), [&abs_x, &abs_y](Tile3 t) {
        t.x(static_cast<std::int16_t>(t.x() + abs_x));
        t.y(static_cast<std::int16_t>(t.y() + abs_y));
        return t;
      });
    }
  }
  [[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const Map &m)
  {
    return os << std::ranges::size(m.m_t1) << ", " << std::ranges::size(m.m_t2) << ", " << std::ranges::size(m.m_t3)
              << '\n';
  }
  template<typename tileT>
  requires(
    std::is_convertible_v<tileT, Tile1> || std::is_convertible_v<tileT, Tile2> || std::is_convertible_v<tileT, Tile3>)
    [[nodiscard]] std::uint64_t generate_pupu(tileT tile) const noexcept
  {
    static constexpr auto bits_per_long = static_cast<int>(sizeof(std::uint64_t) * 8);
    static constexpr auto bits_per_short = static_cast<int>(sizeof(std::uint16_t) * 8);
    static constexpr auto bits_per_byte = static_cast<int>(sizeof(std::uint8_t) * 8);// 0-255
    static constexpr auto bits_per_nibble = bits_per_byte / 2;// 0-15
    static constexpr auto bits_per_crumb = bits_per_nibble / 2;// 0-3
    [[maybe_unused]] static constexpr auto short_mask = 0xFFFFU;
    [[maybe_unused]] static constexpr auto byte_mask = 0xFFU;
    [[maybe_unused]] static constexpr auto nibble_mask = 0xFU;
    [[maybe_unused]] static constexpr auto crumb_mask = 0x3U;
    auto bits = bits_per_long;
    std::uint64_t pupu_id{};
    [[maybe_unused]] const auto add_common = [&bits, &pupu_id](
                                               std::uint64_t value, const int &shift, const uint64_t &mask) {
      bits -= shift;
      pupu_id |= std::rotl((value & mask), bits);
    };
    [[maybe_unused]] const auto add_crumb = [&add_common](
                                              const auto &value) { add_common(value, bits_per_crumb, crumb_mask); };
    [[maybe_unused]] const auto add_nibble = [&add_common](const auto &value) {
      add_common(static_cast<std::uint64_t>(value), bits_per_nibble, nibble_mask);
    };
    [[maybe_unused]] const auto add_byte = [&add_common](
                                             const auto &value) { add_common(value, bits_per_byte, byte_mask); };
    [[maybe_unused]] const auto add_short = [&add_common](
                                              const auto &value) { add_common(value, bits_per_short, short_mask); };
    add_crumb(tile.depth().raw());
    add_nibble(tile.layer_id());
    add_nibble(tile.blend_mode());
    add_byte(tile.animation_id());
    add_byte(tile.animation_state());
    add_short(tile.z());
    return pupu_id;
  }

  [[nodiscard]] auto extract_pupu(std::uint64_t pupu_id) const noexcept
  {
    static constexpr auto bits_per_long = static_cast<int>(sizeof(std::uint64_t) * 8);
    static constexpr auto bits_per_short = static_cast<int>(sizeof(std::uint16_t) * 8);
    static constexpr auto bits_per_byte = static_cast<int>(sizeof(std::uint8_t) * 8);// 0-255
    static constexpr auto bits_per_nibble = bits_per_byte / 2;// 0-15
    static constexpr auto bits_per_crumb = bits_per_nibble / 2;// 0-3
    [[maybe_unused]] static constexpr auto short_mask = 0xFFFFU;
    [[maybe_unused]] static constexpr auto byte_mask = 0xFFU;
    [[maybe_unused]] static constexpr auto nibble_mask = 0xFU;
    [[maybe_unused]] static constexpr auto crumb_mask = 0x3U;
    int bits = bits_per_long;

    [[maybe_unused]] const auto extract_common = [&bits, &pupu_id](const int &shift, const uint64_t &mask) {
      bits -= shift;
      return std::rotr(pupu_id, bits) & mask;
    };
    [[maybe_unused]] const auto extract_crumb = [&extract_common]() -> std::uint8_t {
      return static_cast<std::uint8_t>(extract_common(bits_per_crumb, crumb_mask));
    };
    [[maybe_unused]] const auto extract_nibble = [&extract_common]() -> std::uint8_t {
      return static_cast<std::uint8_t>(extract_common(bits_per_nibble, nibble_mask));
    };

    [[maybe_unused]] const auto extract_byte = [&extract_common]() -> std::uint8_t {
      return static_cast<std::uint8_t>(extract_common(bits_per_byte, byte_mask));
    };
    [[maybe_unused]] const auto extract_short = [&extract_common]() -> std::uint16_t {
      return static_cast<std::uint16_t>(extract_common(bits_per_short, short_mask));
    };
    auto depth = extract_crumb();
    auto layer_id = extract_nibble();
    auto blend_mode = extract_nibble();
    auto animation_id = extract_byte();
    auto animation_state = extract_byte();
    auto z = extract_short();
    return std::make_tuple(depth, layer_id, blend_mode, animation_id, animation_state, z);
  }
  void save([[maybe_unused]] const Mim &in_mim, [[maybe_unused]] const std::string_view &in_path) const
  {

    const auto &ts = tiles();
    Rectangle<std::int32_t> rect = canvas();
    const auto dnps = used_depth_and_palette();
    const auto upupu = unique_pupu();

    for (auto pupu : upupu) {
      const auto [p_depth, p_layer_id, p_blend_mode, p_animation_id, p_animation_state, p_z] = extract_pupu(pupu);

      std::vector<Color16> out{};
      out.resize(rect.area());
      bool drawn = false;
      for (const auto &[depth, palette] : dnps) {
        const auto depth_raw = static_cast<std::uint8_t>(depth.raw() & 0x3U);
        if (depth_raw != p_depth) {
          continue;
        }
        const auto ppm_save = [this,
                                &drawn,
                                &rect,
                                &depth,
                                &palette,
                                &ts,
                                &p_animation_id,
                                &p_animation_state,
                                &p_blend_mode,
                                &p_layer_id,
                                &p_z,

                                &out](const std::span<const Color16> &data,
                                [[maybe_unused]] const std::size_t &raw_width,
                                [[maybe_unused]] const std::size_t &raw_height,
                                std::string local_filename) {
          // Ppm::save(data, width, height, local_filename);

          for (const auto &t : ts) {

            // auto x = t.x();
            // auto y = t.y();

            if (depth != t.depth() || palette != t.palette_id() || !t.draw() || p_animation_id != t.animation_id()
                || p_animation_state != t.animation_state()
                || p_blend_mode != static_cast<decltype(p_blend_mode)>(t.blend_mode()) || p_layer_id != t.layer_id()
                || p_z != t.z()) {
              continue;
            }
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
                auto pixel_out = (static_cast<std::uint32_t>(t.x()) + x)
                                 + ((static_cast<std::uint32_t>(t.y()) + y) * static_cast<std::uint32_t>(rect.width()));
                Color16 color = data[pixel_in];
                if (!color.is_black()) {
                  out.at(pixel_out) = data[pixel_in];
                  drawn = true;
                }
              }
            }
          }
          auto p = std::filesystem::path(local_filename);
          // local_filename = (p.parent_path() / p.stem()).string() + "_tiled_mim.map";
        };

        in_mim.get_colors(in_path, depth, palette, ppm_save, false);
      }
      if (drawn) {
        auto path_v = std::filesystem::path(in_path);
        std::stringstream ss{};
        ss << (path_v.parent_path() / path_v.stem()).string() << '_' << std::uppercase << std::hex << std::setfill('0')
           << std::setw(16U) << pupu << ".mimmap";
        Ppm::save(out, static_cast<std::uint32_t>(rect.width()), static_cast<std::uint32_t>(rect.height()), ss.str());
      }
    }
  }

  [[maybe_unused]] void save_csv([[maybe_unused]] const Mim &in_mim,
    [[maybe_unused]] const std::string_view &in_path) const
  {

    const auto &ts = tiles();
    auto path = std::filesystem::path(in_path);
    Tools::write_buffer(
      [this, &ts](std::ostream &os) {
        os
          << R"("Depth","Blend Mode","Blend Other","Layer ID","Texture ID","Palette ID","Animation ID","Animation State","Source X","Source Y","X","Y","Z","PUPU ID")"
          << '\n';
        std::for_each(std::ranges::cbegin(ts), std::ranges::cend(ts), [this, &os](const auto &t) {
          os << '"' << t.depth() << "\"," << static_cast<uint16_t>(t.blend_mode()) << ','
             << static_cast<uint16_t>(t.blend()) << ',' << static_cast<uint16_t>(t.layer_id()) << ','
             << static_cast<uint16_t>(t.texture_id()) << ',' << static_cast<uint16_t>(t.palette_id()) << ','
             << static_cast<uint16_t>(t.animation_id()) << ',' << static_cast<uint16_t>(t.animation_state()) << ','
             << static_cast<uint16_t>(t.source_x()) << ',' << static_cast<uint16_t>(t.source_y()) << ','
             << static_cast<uint16_t>(t.x()) << ',' << static_cast<uint16_t>(t.y()) << ','
             << static_cast<uint16_t>(t.z()) << ',' << std::uppercase << std::hex << std::setfill('0') << std::setw(16U)
             << generate_pupu(t) << std::nouppercase << std::dec << std::setw(0U) << '\n';
        });
      },
      (path.parent_path() / path.stem()).string() + "_map.csv");
  }
  [[maybe_unused]] void save_v1([[maybe_unused]] const Mim &in_mim,
    [[maybe_unused]] const std::string_view &in_path) const
  {

    const auto &ts = tiles();
    Rectangle<std::int32_t> rect = canvas();
    const auto dnps = used_depth_and_palette();
    const auto upupu = unique_pupu();
    for (const auto &[depth, palette] : dnps) {
      const auto ppm_save = [this, &rect, &depth, &palette, &ts](const std::span<const Color16> &data,
                              const std::size_t &width,
                              const std::size_t &height,
                              std::string local_filename) {
        // Ppm::save(data, width, height, local_filename);
        std::vector<Color16> out{};
        out.resize(width * height);
        bool drawn = false;
        for (const auto &t : ts) {
          // auto x = t.x();
          // auto y = t.y();

          if (depth != t.depth() || palette != t.palette_id() || !t.draw())
            continue;
          for (std::uint32_t y = {}; y < t.HEIGHT; y++) {
            for (std::uint32_t x = {}; x < t.WIDTH; x++) {
              if (t.source_y() < 0 || t.source_x() < 0)
                continue;
              auto pixel_in = (static_cast<std::uint32_t>(t.source_x()) + x)
                              + ((static_cast<std::uint32_t>(t.source_y()) + y) * width);
              if (t.depth().bpp4()) {
                pixel_in += t.TEXTURE_PAGE_WIDTH * t.texture_id() * 2U;
              } else if (t.depth().bpp8()) {
                pixel_in += t.TEXTURE_PAGE_WIDTH * t.texture_id();
              }

              else if (t.depth().bpp16()) {
                pixel_in += (t.TEXTURE_PAGE_WIDTH * t.texture_id()) / 2U;
              }
              auto pixel_out = (static_cast<std::uint32_t>(t.x()) + x)
                               + ((static_cast<std::uint32_t>(t.y()) + y) * static_cast<std::uint32_t>(rect.width()));
              Color16 color = data[pixel_in];
              if (!color.is_black()) {
                out.at(pixel_out) = data[pixel_in];
                drawn = true;
              }
            }
          }
        }
        if (drawn) {
          auto p = std::filesystem::path(local_filename);
          local_filename = (p.parent_path() / p.stem()).string() + "_tiled_mim.map";
          Ppm::save(
            out, static_cast<std::uint32_t>(rect.width()), static_cast<std::uint32_t>(rect.height()), local_filename);
        }
      };
      in_mim.get_colors(in_path, depth, palette, ppm_save, false);
    }
  }
};
}// namespace open_viii::graphics::background

#endif// VIIIARCHIVE_MAP_H
