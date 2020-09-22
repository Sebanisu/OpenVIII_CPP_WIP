//
// Created by pcvii on 9/21/2020.
//

#ifndef VIIIARCHIVE_RESWIZZLE_HPP
#define VIIIARCHIVE_RESWIZZLE_HPP

#include <numeric>
#include <string>
#include <string_view>
#include <filesystem>
#include <concepts>
#include <ranges>
#include "Map.hpp"
#include "Pupu.hpp"
#include "open_viii/graphics/Ppm.hpp"
struct PupuPath
{
  open_viii::graphics::background::Pupu pupu{};
  std::filesystem::path path{};
  auto read_file() const { return open_viii::Tools::read_file<std::string>(path); }
};
namespace open_viii::graphics::background {
template<typename map_type>
requires(std::is_same_v<map_type,
           Tile1> || std::is_same_v<map_type, Tile2> || std::is_same_v<map_type, Tile3>) struct Reswizzle
{
private:
  const Map<map_type> m_map{};
  const std::filesystem::path &m_dir_path{};
  const std::string_view m_dir_name{};
  const std::string &m_output_prefix{};
  const std::array<std::vector<PupuPath>, 3> m_path_grouped_by_bppt{};
  const std::vector<std::uint16_t> m_valid_texture_ids{};

  const std::uint8_t m_scale{};// when I can detect the scale this will probably be mutable or I can detect on init
  const std::uint32_t m_width{};
  const std::uint32_t m_height{};
  const std::uint32_t m_area{};
  mutable std::vector<open_viii::graphics::Color24<0, 1, 2>> m_out{};
  mutable bool m_drawn{ false };
  std::array<std::vector<PupuPath>, 3> find_files()
  {
    std::array<std::vector<PupuPath>, 3> path_grouped_by_bppt{};
    open_viii::Tools::execute_on_directory(
      m_dir_path, { m_dir_name }, { ".ppm" }, [&path_grouped_by_bppt, this](const std::filesystem::path &file_path) {
        // if(!file_path.has_stem()) {return;}
        auto basename = file_path.stem().string();
        static constexpr auto minsize{ 24 };
        if (std::ranges::size(basename) < minsize) {
          return;
        }
        constexpr static auto suffix_offset = 7;
        auto suffix = std::string_view(basename).substr(std::ranges::size(basename) - suffix_offset);
        if (suffix != "_mimmap") {
          return;
        }
        constexpr static auto hex_begin_offset = 23;
        constexpr static auto hex_length = 16;
        constexpr static auto hex_begin_offset_and_dash = hex_begin_offset + 1;
        auto hex = std::string_view(basename).substr(std::ranges::size(basename) - hex_begin_offset, hex_length);
        auto prefix = std::string_view(basename).substr(0, std::ranges::size(basename) - hex_begin_offset_and_dash);
        if (open_viii::Tools::i_equals(prefix, m_dir_name)) {
          // std::cout << prefix << '\n';
          auto pp = PupuPath{ open_viii::graphics::background::Pupu(hex), file_path };

          if (pp.pupu.depth().bpp4()) {
            path_grouped_by_bppt.at(0).emplace_back(std::move(pp));
          } else if (pp.pupu.depth().bpp8()) {
            path_grouped_by_bppt.at(1).emplace_back(std::move(pp));
          } else if (pp.pupu.depth().bpp16()) {
            path_grouped_by_bppt.at(2).emplace_back(std::move(pp));
          }
        }
      });
    return path_grouped_by_bppt;
  }
  std::vector<std::uint16_t> get_valid_texture_ids()
  {
    std::vector<std::uint16_t> valid_texture_ids{};
    const auto tiles = m_map.tiles();
    valid_texture_ids.reserve(std::ranges::size(tiles));
    std::ranges::transform(
      tiles, std::back_insert_iterator(valid_texture_ids), [](const map_type &tile) { return tile.texture_id(); });
    std::ranges::sort(valid_texture_ids);
    auto last = std::unique(std::ranges::begin(valid_texture_ids), std::ranges::end(valid_texture_ids));
    valid_texture_ids.erase(last, std::ranges::end(valid_texture_ids));
    return valid_texture_ids;
  }
  template<std::invocable<std::uint16_t> lambdaT> void for_each_texture_id(const lambdaT &lambda) const
  {
    std::ranges::for_each(m_valid_texture_ids, lambda);
  }

  template<std::invocable<int, std::vector<PupuPath>> lambdaT>
  void for_each_pupu_path_vector(const lambdaT &lambda) const
  {
    constexpr static std::array<std::size_t, 3> index_values = { 0U, 1U, 2U };
    constexpr static std::array<int, 3> bpp_values = { 4, 8, 16 };
    std::ranges::for_each(index_values,
      [&lambda, this](const std::size_t &index) { lambda(bpp_values.at(index), m_path_grouped_by_bppt.at(index)); });
  }
  template<std::invocable<map_type> lambdaT1, std::invocable<map_type> lambdaT2>
  void for_each_tile(const lambdaT1 &exec, const lambdaT2 &filter) const
  {
    std::ranges::for_each(m_map.tiles() | std::views::filter(filter), exec);
  }

  void save_and_clear_out_buffer(const int &bpp, const std::uint16_t &texture_id) const
  {
    if (m_drawn) {
      std::string output_name = m_output_prefix + "_" + std::to_string(bpp) + "_" + std::to_string(texture_id);
      Ppm::save(m_out, m_width, m_height, output_name, true);
      std::fill(std::ranges::begin(m_out), std::ranges::end(m_out), Color24<0, 1, 2>{});
      m_drawn = false;
    }
  }

public:
  Reswizzle(const std::vector<char> &buffer,
    const std::filesystem::path &dir_path,
    const std::string_view &dir_name,
    const std::string &output_prefix,
    const uint8_t scale = 1U)
    : m_map(buffer), m_dir_path(dir_path), m_dir_name(dir_name), m_output_prefix(output_prefix),
      m_path_grouped_by_bppt(find_files()), m_valid_texture_ids(get_valid_texture_ids()), m_scale(scale),
      m_width(open_viii::graphics::background::MimType::height() * m_scale),
      m_height(open_viii::graphics::background::MimType::height() * m_scale), m_area(m_width * m_height), m_out(m_area)
  {
    if (empty()) {
      return;
    }
    std::cout << size() << std::endl;
  }

  [[nodiscard]] std::size_t size() const
  {
    const auto array = m_path_grouped_by_bppt | std::views::transform([](const std::vector<PupuPath> &vector) {
      return std::ranges::size(vector);
    });

    return std::reduce(std::ranges::begin(array),
      std::ranges::end(array),
      static_cast<std::size_t>(0U),
      [](const std::size_t &total, const std::size_t &vector_size) { return total + vector_size; });
  }
  [[nodiscard]] bool empty() const
  {
    return std::ranges::all_of(
      m_path_grouped_by_bppt, [](const std::vector<PupuPath> &vector) { return std::ranges::empty(vector); });
  }
  void process() const
  {
    for_each_texture_id([this](const std::uint16_t texture_id) {
      for_each_pupu_path_vector([this, &texture_id](const int bpp, const std::vector<PupuPath> &pupu_path_vector) {
        std::ranges::for_each(pupu_path_vector, [this, &bpp, &texture_id](const PupuPath &pupu_path) {
          const auto ppm = Ppm(pupu_path.read_file());
          for_each_tile(
            [this, &ppm](const map_type &tile) {
              static constexpr auto tile_size = 16U;
              open_viii::Tools::for_each_xy(tile_size * m_scale, [this, &tile, &ppm](const auto &x, const auto &y) {
                const auto scaled_tile_y = static_cast<std::size_t>(tile.y()) * m_scale;
                const auto scaled_tile_x = static_cast<std::size_t>(tile.x()) * m_scale;
                auto color = ppm.color(x + scaled_tile_x, y + scaled_tile_y);
                if (!color.is_black()) {
                  const std::size_t scaled_tile_source_x = tile.source_x() * m_scale;
                  const std::size_t scaled_tile_source_y = tile.source_y() * m_scale;
                  m_out.at(x + scaled_tile_source_x + ((y + scaled_tile_source_y) * m_width)) = color;
                  m_drawn = true;
                }
              });
            },
            [&pupu_path, &texture_id](
              const map_type &tile) { return pupu_path.pupu == tile && tile.texture_id() == texture_id; });
        });

        save_and_clear_out_buffer(bpp, texture_id);
      });
    });
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_RESWIZZLE_HPP
