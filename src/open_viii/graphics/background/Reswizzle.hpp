//
// Created by pcvii on 9/21/2020.
//
#ifndef VIIIARCHIVE_RESWIZZLE_HPP
#define VIIIARCHIVE_RESWIZZLE_HPP
#include "Map.hpp"
#include "Pupu.hpp"
#include "open_viii/graphics/Ppm.hpp"
#include <concepts>
#include <filesystem>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
namespace open_viii::graphics::background {
struct PupuPath
{
  open_viii::graphics::background::Pupu pupu = {};
  std::filesystem::path                 path = {};
  //  auto read_entire_file() const
  //  {
  //    return open_viii::tools::read_entire_file<std::string>(path);
  //  }
};
template<typename map_type>
requires(
  std::is_same_v<
    map_type,
    Tile1> || std::is_same_v<map_type, Tile2> || std::is_same_v<map_type, Tile3>) struct
  Reswizzle
{
private:
  const Map<map_type>              m_map                = {};
  const std::filesystem::path &    m_dir_path           = {};
  const std::string_view           m_dir_name           = {};
  const std::string &              m_output_prefix      = {};
  static constexpr std::size_t     PUPUPATH_GROUP_COUNT = 3U;
  const std::vector<PupuPath>      m_pupu_paths         = {};
  const std::vector<std::uint16_t> m_valid_texture_ids  = {};
  mutable std::uint8_t             m_scale
    = {};// when I can detect the scale this will
         // probably be mutable or I can detect on init
  const Point<uint16_t>        m_map_width_height                 = {};
  mutable std::uint32_t        m_width                            = {};
  mutable std::uint32_t        m_height                           = {};
  mutable std::uint32_t        m_area                             = {};
  static constexpr std::size_t PALETTE_COUNT                      = 16U;
  mutable std::array<std::vector<map_type>, PALETTE_COUNT> m_skip = {};
  mutable std::vector<
    open_viii::graphics::Color24<open_viii::graphics::ColorLayoutT::RGB>>
               m_out   = {};
  mutable bool m_drawn = { false };
  uint32_t
    get_area() const noexcept
  {
    return m_width * m_height;
  }
  uint16_t
    get_scaled_dim() const noexcept
  {
    return MimType::height() * m_scale;
  }
  void
    update_dims(std::uint8_t scale = 1U) const noexcept
  {
    if (scale < 1U || scale == m_scale) {
      return;
    }
    m_scale  = scale;
    m_height = m_width = get_scaled_dim();
    m_area             = get_area();
    m_out.resize(m_area);
  }
  Ppm
    get_ppm(const PupuPath &pupu_path) const
  {
    return Ppm(pupu_path.path);
  }
  void
    update_dims(const Ppm &ppm) const
  {
    auto scale_point = (ppm.width_height() / m_map_width_height);
    assert(scale_point.x() == scale_point.y());
    update_dims(static_cast<uint8_t>(scale_point.x()));
  }
  void
    empty_skip() const
  {
    std::ranges::for_each(m_skip, [](std::vector<map_type> &vector) {
      vector.clear();
    });
  }
  auto
    find_pupu_image_file_paths() const
  {
    std::vector<PupuPath> pupu_paths{};
    open_viii::tools::execute_on_directory(
      m_dir_path,
      { m_dir_name },
      { ".ppm" },
      [&pupu_paths, this](const std::filesystem::path &file_path) {
        // if(!file_path.has_stem()) {return;}
        auto                  basename = file_path.stem().string();
        static constexpr auto minsize{ 24 };
        if (std::ranges::size(basename) < minsize) {
          return;
        }
        constexpr static auto suffix_offset = 7;
        auto                  suffix        = std::string_view(basename).substr(
          std::ranges::size(basename) - suffix_offset);
        if (suffix != "_mimmap") {
          return;
        }
        constexpr static auto hex_begin_offset          = 23;
        constexpr static auto hex_length                = 16;
        constexpr static auto hex_begin_offset_and_dash = hex_begin_offset + 1;
        auto hex = std::string_view(basename).substr(std::ranges::size(basename)
                                                       - hex_begin_offset,
                                                     hex_length);
        auto prefix = std::string_view(basename).substr(
          0,
          std::ranges::size(basename) - hex_begin_offset_and_dash);
        if (open_viii::tools::i_equals(prefix, m_dir_name)) {
          // std::cout << prefix << '\n';
          pupu_paths.emplace_back(
            PupuPath{ open_viii::graphics::background::Pupu(hex), file_path });
        }
      });
    return pupu_paths;
  }
  std::vector<std::uint16_t>
    get_valid_texture_ids() const
  {
    std::vector<std::uint16_t> valid_texture_ids{};
    const auto                 tiles = m_map.tiles();
    valid_texture_ids.reserve(std::ranges::size(tiles));
    std::ranges::transform(tiles,
                           std::back_insert_iterator(valid_texture_ids),
                           [](const map_type &tile) {
                             return tile.texture_id();
                           });
    std::ranges::sort(valid_texture_ids);
    auto last = std::unique(std::ranges::begin(valid_texture_ids),
                            std::ranges::end(valid_texture_ids));
    valid_texture_ids.erase(last, std::ranges::end(valid_texture_ids));
    return valid_texture_ids;
  }
  template<std::invocable<std::uint16_t> lambdaT>
  void
    for_each_texture_id(const lambdaT &lambda) const
  {
    std::ranges::for_each(m_valid_texture_ids, lambda);
  }
  template<std::invocable<map_type> lambdaT1, std::invocable<map_type> lambdaT2>
  void
    for_each_tile(const std::vector<map_type> &vector,
                  const lambdaT1 &             exec,
                  const lambdaT2 &             filter) const
  {
    std::ranges::for_each(vector | std::views::filter(filter), exec);
  }
  template<std::invocable<map_type> lambdaT1, std::invocable<map_type> lambdaT2>
  void
    for_each_tile(const lambdaT1 &exec, const lambdaT2 &filter) const
  {
    for_each_tile(m_map.tiles(), exec, filter);
  }
  void
    save_and_clear_out_buffer(const std::uint16_t &texture_id) const
  {
    if (m_drawn) {
      std::string output_name
        = m_output_prefix + "(" + std::to_string(texture_id) + ")";
      Ppm::save(m_out, m_width, m_height, output_name, true);
      std::fill(std::ranges::begin(m_out),
                std::ranges::end(m_out),
                Color24<open_viii::graphics::ColorLayoutT::RGB>{});
      m_drawn = false;
    }
  }
  void
    save_and_clear_out_buffer(const std::uint16_t &texture_id,
                              const std::uint8_t & palette) const
  {
    if (m_drawn) {
      std::string output_name = m_output_prefix + "("
                              + std::to_string(texture_id) + "_"
                              + std::to_string(palette) + ")";
      Ppm::save(m_out, m_width, m_height, output_name, true);
      std::fill(std::ranges::begin(m_out),
                std::ranges::end(m_out),
                Color24<open_viii::graphics::ColorLayoutT::RGB>{});
      m_drawn = false;
    }
  }
  void
    process_main_tiles(const uint16_t &texture_id) const
  {
    std::ranges::for_each(
      m_pupu_paths,
      [this, &texture_id](const PupuPath &pupu_path) {
        const auto ppm = get_ppm(pupu_path);
        update_dims(ppm);
        for_each_tile(
          [this, &ppm](const map_type &tile) {
            static constexpr auto tile_size = 16U;
            open_viii::tools::for_each_xy(tile_size * m_scale,
                                          get_set_color_lambda(ppm, tile));
          },
          [&pupu_path, &texture_id](const map_type &tile) {
            return pupu_path.pupu == tile && tile.texture_id() == texture_id;
          });
      });
    save_and_clear_out_buffer(texture_id);
  }
  void
    process_skipped_tiles(const uint16_t &texture_id) const
  {
    std::array<std::uint8_t, PALETTE_COUNT> indexes{};
    std::iota(indexes.begin(), indexes.end(), std::uint8_t(0U));
    std::ranges::for_each(
      indexes,
      [this, &texture_id](const uint8_t &palette_id) {
        if (std::ranges::empty(m_skip.at(palette_id))) {
          return;
        }
        std::ranges::for_each(
          m_pupu_paths,
          [this, &texture_id, &palette_id](const PupuPath &pupu_path) {
            const auto ppm = get_ppm(pupu_path);
            update_dims(ppm);
            for_each_tile(
              m_skip.at(palette_id),
              [this, &ppm](const map_type &tile) {
                static constexpr auto tile_size = 16U;
                open_viii::tools::for_each_xy(
                  tile_size * m_scale,
                  get_set_color_lambda(ppm, tile, true));
              },
              [&pupu_path, &texture_id, &palette_id](const map_type &tile) {
                return pupu_path.pupu == tile && tile.texture_id() == texture_id
                    && tile.palette_id() == palette_id;
              });
          });
        save_and_clear_out_buffer(texture_id, palette_id);
      });
  }
  auto
    get_clear_color_lambda(const Ppm &ppm, const map_type &tile) const
  {
    return [this, &tile, &ppm](const auto &x, const auto &y) -> bool {
      const auto scaled_tile_y
        = static_cast<std::size_t>(tile.y()) * this->m_scale;
      const auto scaled_tile_x
        = static_cast<std::size_t>(tile.x()) * this->m_scale;
      auto color = ppm.color(x + scaled_tile_x, y + scaled_tile_y);
      if (!color.is_black()) {
        const std::size_t scaled_tile_source_x
          = tile.source_x() * this->m_scale;
        const std::size_t scaled_tile_source_y
          = tile.source_y() * this->m_scale;
        this->m_out.at(x + scaled_tile_source_x
                       + ((y + scaled_tile_source_y) * this->m_width))
          = {};// put the values back to black.
      }
      return false;
    };
  }
  auto
    get_set_color_lambda(const Ppm &     ppm,
                         const map_type &tile,
                         const bool      skipped = false) const
  {
    return [this, &tile, &ppm, skipped](const auto &x, const auto &y) -> bool {
      const auto scaled_tile_y = scale_dim(tile.y());
      const auto scaled_tile_x = scale_dim(tile.x());
      auto       color = ppm.color(x + scaled_tile_x, y + scaled_tile_y);
      if (!color.is_black()) {
        const auto scaled_tile_source_x = scale_dim(tile.source_x());
        const auto scaled_tile_source_y = scale_dim(tile.source_y());
        const auto dst_index
          = x + scaled_tile_source_x + ((y + scaled_tile_source_y) * m_width);
        const auto &current = m_out.at(dst_index);
        if (!skipped && !current.is_black()) {
          tools::for_each_xy(x + 1, y + 1, get_clear_color_lambda(ppm, tile));
          m_skip.at(tile.palette_id()).push_back(tile);
          return true;// break out.
        }
        m_out.at(dst_index) = color;
        m_drawn             = true;
      }
      return false;
    };
  }
  template<std::integral intT>
  size_t
    scale_dim(const intT value) const
  {
    return static_cast<std::size_t>(value) * m_scale;
  }
  [[nodiscard]] std::size_t
    skipped_size() const
  {
    const auto array
      = m_skip | std::views::transform([](const std::vector<map_type> &vector) {
          return std::ranges::size(vector);
        });
    return std::reduce(
      std::ranges::begin(array),
      std::ranges::end(array),
      static_cast<std::size_t>(0U),
      [](const std::size_t &total, const std::size_t &vector_size) {
        return total + vector_size;
      });
  }
  [[nodiscard]] bool
    skipped_empty() const
  {
    return std::ranges::all_of(m_skip, [](const std::vector<map_type> &vector) {
      return std::ranges::empty(vector);
    });
  }

public:
  Reswizzle(const std::vector<char> &    buffer,
            const std::filesystem::path &dir_path,
            const std::string_view &     dir_name,
            const std::string &          output_prefix,
            const uint8_t                scale = 1U)
    : m_map(buffer), m_dir_path(dir_path), m_dir_name(dir_name),
      m_output_prefix(output_prefix),
      m_pupu_paths(find_pupu_image_file_paths()),
      m_valid_texture_ids(get_valid_texture_ids()), m_scale(scale),
      m_map_width_height(m_map.canvas().width_height()),
      m_width(get_scaled_dim()), m_height(get_scaled_dim()), m_area(get_area()),
      m_out(static_cast<std::size_t>(m_area))
  {}
  [[nodiscard]] std::size_t
    size() const noexcept
  {
    return std::ranges::size(m_pupu_paths);
  }
  [[nodiscard]] bool
    empty() const noexcept
  {
    return std::ranges::empty(m_pupu_paths);
  }
  void
    process() const
  {
    for_each_texture_id([this](const std::uint16_t texture_id) {
      process_main_tiles(texture_id);
      process_skipped_tiles(texture_id);
      empty_skip();
    });
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_RESWIZZLE_HPP
