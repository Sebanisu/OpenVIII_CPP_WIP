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
#ifndef VIIIARCHIVE_MIM_HPP
#define VIIIARCHIVE_MIM_HPP
#include "MimType.hpp"
#include "open_viii/graphics/Bit4Values.hpp"
#include "open_viii/graphics/Color.hpp"
#include "open_viii/graphics/Ppm.hpp"
#include "Tile2.hpp"
#include "Tile3.hpp"
#include "TileCommon.hpp"
#include <algorithm>
#include <execution>
#include <version>
namespace open_viii::graphics::background {

/**
 * http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_MIM
 * todo rewrite so you don't have to delete the copy constructors. aka no spans
 * in class.
 */
struct Mim
{
private:
  std::vector<char> m_buffer{};
  MimType           m_mim_type{};
  [[nodiscard]] static const auto &
    clut_width() noexcept
  {
    return open_viii::graphics::background::MimType::COLORS_PER_PALETTE;
  }
  [[nodiscard]] std::uint32_t
    clut_height() const noexcept
  {
    return static_cast<std::uint32_t>(
      ((m_mim_type.palette_section_size() - m_mim_type.bytes_skipped_palettes())
       / sizeof(Color16ABGR))
      / clut_width());
  }
  std::span<const char>
    set_image_span() const
  {
    return std::span<const char>(m_buffer).subspan(
      m_mim_type.palette_section_size());
  }
  std::span<const Color16ABGR>
    set_palette_span() const
  {
    const auto palette_buffer_tmp = std::span<const char>(m_buffer).subspan(
      m_mim_type.bytes_skipped_palettes(),
      m_mim_type.palette_section_size() - m_mim_type.bytes_skipped_palettes());
    return { reinterpret_cast<const Color16ABGR *>(
               std::ranges::data(palette_buffer_tmp)),
             std::ranges::size(palette_buffer_tmp) / sizeof(Color16ABGR) };
  }
  std::span<const Bit4Values>
    set_image_span_bpp4() const
  {
    const auto m_image_buffer = set_image_span();
    return { reinterpret_cast<const Bit4Values *>(
               std::ranges::data(m_image_buffer)),
             std::ranges::size(m_image_buffer) / sizeof(Bit4Values) };
  }
  std::span<const Color16ABGR>
    set_image_span_bpp16() const
  {
    const auto m_image_buffer = set_image_span();
    return { reinterpret_cast<const Color16ABGR *>(
               std::ranges::data(m_image_buffer)),
             std::ranges::size(m_image_buffer) / sizeof(Color16ABGR) };
  }
  [[nodiscard]] Color16ABGR static safe_get_color_from_palette(
    const std::span<const Color16ABGR> palette_buffer,
    size_t                             key)
  {
    if (key < std::ranges::size(palette_buffer)) {
      return palette_buffer[key];
    }
    return {};
  }
  static std::size_t
    get_palette_key(const std::uint8_t &key, const std::size_t &palette)
  {
    return key + (palette * static_cast<std::size_t>(clut_width()));
  }

  template<Color_types T = Color16ABGR>
  [[nodiscard]] std::vector<T>
    get_image_bpp8(const uint8_t &palette) const
  {
    const auto     palette_buffer = set_palette_span();
    const auto     image_buffer   = set_image_span();
    std::vector<T> out{};
    out.resize(std::ranges::size(image_buffer));

    auto get_color_from_palette = [&](const char &color_key) -> T {
      return static_cast<T>(safe_get_color_from_palette(
        palette_buffer,
        get_palette_key(static_cast<uint8_t>(color_key), palette)));
    };
#ifdef __cpp_lib_parallel_algorithm
    // Parallel algorithm available
    out.resize(std::ranges::size(image_buffer));
    std::transform(
      std::execution::par_unseq,
      image_buffer.begin(),
      image_buffer.end(),
      std::ranges::begin(out),
      get_color_from_palette);
#else
    // Parallel algorithm not available
    out.reserve(std::ranges::size(image_buffer));
    std::ranges::transform(
      image_buffer,
      std::back_inserter(out),
      get_color_from_palette);
#endif
    return out;
  }
  template<Color_types T = Color16ABGR>
  [[nodiscard]] std::vector<T>
    get_image_bpp4(const uint8_t &palette) const
  {
    const auto palette_buffer    = set_palette_span();
    const auto image_buffer_bbp4 = set_image_span_bpp4();

    // Compute the size of the output vector.
    const auto output_size       = std::ranges::size(image_buffer_bbp4) * 2U;
    const auto get_color_from_palette = [&](size_t i) {
      const auto    key       = image_buffer_bbp4[i >> 1];
      const uint8_t color_key = i % 2U == 0 ? key.first : key.second;
      return static_cast<T>(safe_get_color_from_palette(
        palette_buffer,
        get_palette_key(color_key, palette)));
    };
#ifdef __cpp_lib_parallel_algorithm
    // Generate indices for the output vector.
    std::vector<size_t> indices(output_size);
    std::iota(indices.begin(), indices.end(), 0);

    // Transform the indices into color values.
    std::vector<T> out(output_size);
    std::transform(
      std::execution::par_unseq,
      indices.begin(),
      indices.end(),
      out.begin(),
      get_color_from_palette);
#else
    // Generate indices for the output vector.
    auto           indices = std::views::iota(size_t{}, output_size);

    // Transform the indices into color values.
    std::vector<T> out{};
    out.reserve(output_size);
    std::ranges::transform(
      indices,
      std::back_inserter(out),
      get_color_from_palette);
#endif
    return out;
  }

public:
  constexpr static auto EXT = std::string_view{ ".mim" };
  Mim()                     = default;
  ~Mim()                    = default;
  /**
   * Load up the raw pixel data, 4bpp, 8bpp or 16bpp Needs at least basename to
   * check or it'll use size of buffer to find type 1 or 2. Type is used to know
   * which map to load.
   * @param buffer is the raw bytes. std::move into object.
   * @param name logo has a unique map type.
   */
  template<typename T>
  Mim(T &&buffer, std::string_view name)
    : m_buffer(std::move(buffer)),
      m_mim_type(get_texture_type(std::ranges::size(m_buffer), name))
  {}
  explicit Mim(const std::filesystem::path &path)
    : Mim(open_viii::tools::read_entire_file(path), path.string())
  {}
  [[nodiscard]] const std::vector<char> &
    buffer() const
  {
    return m_buffer;
  }
  [[nodiscard]] const auto &
    mim_type() const noexcept
  {
    return m_mim_type;
  }
  [[maybe_unused]] [[nodiscard]] static MimType
    get_texture_type(std::size_t mim_filesize, std::string_view name = {})
  {
    for (auto m : MimType::TEXTURE_TYPES()) {
      if (m.file_size() == mim_filesize) {
        if (tools::i_find_any(name, Tile3::FORCE_TYPE_VALUES)) {
          return m.with_type(3);
        }
        if (tools::i_find_any(name, Tile2::FORCE_TYPE_VALUES)) {
          return m.with_type(2);
        }
        return m;
      }
    }
    return {};
  }
  friend std::ostream &
    operator<<(std::ostream &os, const Mim &m)
  {
    return os << m.m_mim_type;
  }
  std::uint32_t
    get_height(bool dump_palette = false) const
  {
    if (std::empty(m_buffer)) {
      return 0U;
    }
    if (dump_palette) {
      return clut_height();
    }
    else {
      return m_mim_type.height();
    }
  }
  std::uint32_t
    get_width(const BPPT &bpp, bool dump_palette = false) const
  {
    if (std::empty(m_buffer)) {
      return 0U;
    }
    if (dump_palette) {
      return clut_width();
    }
    auto width = m_mim_type.width();
    if (bpp.bpp4()) {
      return width *= 2U;
    }
    if (bpp.bpp8()) {
      return width;
    }
    if (bpp.bpp16()) {
      return width /= 2U;
    }
    return 0U;
  }

  template<Color_types T>
  std::vector<T>
    get_colors(
      const BPPT    &bpp,
      const uint8_t &palette,
      bool           dump_palette = false) const
  {

    std::vector<T> colors{};
    if (std::empty(m_buffer)) {
      return colors;
    }

    const auto convert_color = [&colors](const auto &raw_color) {
      const auto cast_color = [](const auto &color) {
        return static_cast<T>(color);
      };
#ifdef __cpp_lib_parallel_algorithm
      colors.resize(raw_color.size());
      std::transform(
        std::execution::par_unseq,
        std::ranges::begin(raw_color),
        std::ranges::end(raw_color),
        std::ranges::begin(colors),
        cast_color);
#else
      colors.reserve(raw_color.size());
      std::ranges::transform(raw_color, std::back_inserter(colors), cast_color);
#endif
    };
    if (dump_palette) {
      const auto m_palette_buffer = set_palette_span();
      convert_color(m_palette_buffer);
    }
    else if (bpp.bpp4()) {
      colors = get_image_bpp4<T>(palette);
    }
    else if (bpp.bpp8()) {
      colors = get_image_bpp8<T>(palette);
    }
    else if (bpp.bpp16()) {
      const auto m_image_buffer_bbp16 = set_image_span_bpp16();
      convert_color(m_image_buffer_bbp16);
    }
    return colors;
  }

  template<typename lambdaT>
    requires(
      std::invocable<
        lambdaT,
        std::span<const Color16ABGR>,
        std::size_t,
        std::size_t,
        std::string>
      || std::invocable<lambdaT, std::span<const Color16ABGR>, std::size_t>)
  [[maybe_unused]] void
    get_colors_for_saving(
      const std::string_view &filename,
      const BPPT             &bpp,
      const uint8_t          &palette,
      const lambdaT           lambda,
      bool                    dump_palette = false) const
  {// going to get all the colors for image and run the supplied lambda passing
   // the colors.
    const auto path = std::filesystem::path(filename);
    if (dump_palette) {
      const auto m_palette_buffer = set_palette_span();
      if constexpr (std::invocable<
                      lambdaT,
                      std::span<const Color16ABGR>,
                      std::size_t,
                      std::size_t,
                      std::string>) {
        lambda(
          m_palette_buffer,
          static_cast<std::size_t>(clut_width()),
          clut_height(),
          ((path.parent_path() / path.stem()).string()) + "_Clut.mim");
      }
      else if constexpr (std::invocable<
                           lambdaT,
                           std::span<const Color16ABGR>,
                           std::size_t>) {
        lambda(m_palette_buffer, static_cast<std::size_t>(clut_width()));
      }
      return;
    }
    auto width = m_mim_type.width();
    auto out_path
      = [&palette, &path](const int &bpp_num, bool has_palette = true) {
          return (path.parent_path() / path.stem()).string() + "_"
               + std::to_string(bpp_num) + "bpp"
               + (has_palette ? "_" + std::to_string(palette) : "")
               + open_viii::graphics::background::Mim::EXT.data();
        };
    if (bpp.bpp8()) {
      auto out = get_image_bpp8(palette);
      if constexpr (std::invocable<
                      lambdaT,
                      std::span<const Color16ABGR>,
                      std::size_t,
                      std::size_t,
                      std::string>) {
        lambda(
          out,
          width,
          std::ranges::size(out) / width,
          out_path(BPPT::BPP8));
      }
      else if (std::invocable<
                 lambdaT,
                 std::span<const Color16ABGR>,
                 std::size_t>) {
        lambda(out, width);
      }
    }
    else if (bpp.bpp4()) {
      std::vector<Color16ABGR> out = get_image_bpp4(palette);
      width *= 2U;
      if constexpr (std::invocable<
                      lambdaT,
                      std::span<const Color16ABGR>,
                      std::size_t,
                      std::size_t,
                      std::string>) {
        lambda(
          out,
          width,
          std::ranges::size(out) / width,
          out_path(BPPT::BPP4));
      }
      else if (std::invocable<
                 lambdaT,
                 std::span<const Color16ABGR>,
                 std::size_t>) {
        lambda(out, width);
      }
    }
    else if (bpp.bpp16()) {
      width /= 2U;
      if constexpr (std::invocable<
                      lambdaT,
                      std::span<const Color16ABGR>,
                      std::size_t,
                      std::size_t,
                      std::string>) {
        const auto m_image_buffer_bbp16 = set_image_span_bpp16();
        lambda(
          m_image_buffer_bbp16,
          (width),
          std::ranges::size(m_image_buffer_bbp16) / width,
          out_path(BPPT::BPP16, false));
      }
      else if (std::invocable<
                 lambdaT,
                 std::span<const Color16ABGR>,
                 std::size_t>) {
        const auto m_image_buffer_bbp16 = set_image_span_bpp16();
        lambda(m_image_buffer_bbp16, width);
      }
    }
  }
  std::size_t
    get_raw_width(const BPPT &depth) const
  {
    return get_raw_width(depth, m_mim_type.width());
  }
  static std::size_t
    get_raw_width(const BPPT &depth, std::size_t width)
  {
    if (depth.bpp4()) {
      return width * 2U;
    }
    if (depth.bpp16()) {
      return width / 2U;
    }
    return width;
  }
  [[nodiscard]] Color16ABGR
    get_color(
      const std::uint32_t x,
      const std::uint32_t y,
      const BPPT          depth,
      const std::uint8_t  palette    = 0U,
      const std::uint8_t  texture_id = 0U) const
  {
    if (depth.bpp16()) {
      return get_color_16bit(x, y, texture_id);
    }
    else if (depth.bpp8()) {
      return get_color_8bit(x, y, palette, texture_id);
    }
    else if (depth.bpp4()) {
      return get_color_4bit(x, y, palette, texture_id);
    }
    return {};
  }
  enum struct texture_page_width_t : std::uint16_t
  {
    bit_4  = 256U,
    bit_8  = bit_4 >> 1,
    bit_16 = bit_4 >> 2,
  };
  friend std::underlying_type_t<texture_page_width_t>
    operator+(texture_page_width_t tpw)
  {
    return std::underlying_type_t<texture_page_width_t>(tpw);
  }
  [[nodiscard]] Color16ABGR
    get_color_16bit(
      const std::uint32_t x,
      const std::uint32_t y,
      const std::uint8_t  texture_id) const
  {
    const auto width = m_mim_type.width(BPPT::BPP16_CONST());
    const auto texture_page_offset
      = std::uint32_t{ +texture_page_width_t::bit_16 }
      * std::uint32_t{ texture_id };
    const auto m_image_buffer_bbp16 = set_image_span_bpp16();
    return m_image_buffer_bbp16
      [std::uint32_t{ x } + texture_page_offset + (std::uint32_t{ y } * width)];
  }

  [[nodiscard]] Color16ABGR
    get_color_4bit(
      const std::uint32_t x,
      const std::uint32_t y,
      const std::uint8_t  palette,
      const std::uint8_t  texture_id) const
  {
    const auto palette_buffer = set_palette_span();
    const auto color          = [&](std::uint8_t key) -> Color16ABGR {
      return safe_get_color_from_palette(
        palette_buffer,
        get_palette_key(key, palette));
    };
    return color(get_color_key_4bit(x, y, texture_id));
  }

  std::uint8_t
    get_color_key_4bit(
      const std::uint32_t x,
      const std::uint32_t y,
      const std::uint8_t  texture_id) const
  {
    const auto width = m_mim_type.width(BPPT::BPP4_CONST());
    const auto texture_page_offset
      = std::uint32_t{ +texture_page_width_t::bit_4 }
      * std::uint32_t{ texture_id };
    const auto       m_image_buffer_bbp4 = set_image_span_bpp4();
    const Bit4Values pair                = m_image_buffer_bbp4
      [(std::uint32_t{ x } + texture_page_offset) / 2U
       + (std::uint32_t{ y } * width)];
    return x % 2U == 0 ? pair.first : pair.second;
  }

  [[nodiscard]] Color16ABGR
    get_color_8bit(
      const std::uint32_t x,
      const std::uint32_t y,
      const std::uint8_t  palette,
      const std::uint8_t  texture_id) const
  {
    const auto palette_buffer = set_palette_span();
    const auto color_key      = get_color_key_8bit(x, y, texture_id);
    return safe_get_color_from_palette(
      palette_buffer,
      get_palette_key(color_key, palette));
  }

  std::uint8_t
    get_color_key_8bit(
      const std::uint32_t x,
      const std::uint32_t y,
      const std::uint8_t  texture_id) const
  {
    return get_color_key_8bit(
      set_image_span(),
      m_mim_type.width(BPPT::BPP8_CONST()),
      x,
      y,
      texture_id);
  }
  static std::uint8_t
    get_color_key_8bit(
      const std::span<const char> image_buffer,
      const std::uint32_t         width,
      const std::uint32_t         x,
      const std::uint32_t         y,
      const std::uint8_t          texture_id)
  {
    const auto texture_page_offset
      = std::uint32_t{ +texture_page_width_t::bit_8 }
      * std::uint32_t{ texture_id };
    const auto color_key
      = std::uint32_t{ x } + texture_page_offset + (std::uint32_t{ y } * width);
    return static_cast<std::uint8_t>(image_buffer[color_key]);
  }

  [[maybe_unused]] void
    save([[maybe_unused]] std::string_view filename) const
  {
    static constexpr auto ppm_save = [](
                                       const std::span<const Color16ABGR> &data,
                                       const std::size_t &width,
                                       const std::size_t &height,
                                       const std::string &local_filename) {
      Ppm::save(data, width, height, local_filename);
    };
    using namespace open_viii::graphics::literals;
    get_colors_for_saving(filename, {}, {}, ppm_save, true);
    {
      for (std::uint8_t i{}; i < static_cast<std::uint8_t>(clut_height());
           i++) {
        get_colors_for_saving(filename, 4_bpp, i, ppm_save, false);
      }
    }
    {
      for (std::uint8_t i{}; i < static_cast<std::uint8_t>(clut_height());
           i++) {
        get_colors_for_saving(filename, 8_bpp, i, ppm_save, false);
      }
    }
    get_colors_for_saving(filename, 16_bpp, {}, ppm_save, false);
  }
  /**
   * bpp selections
   */
  static consteval const std::array<open_viii::graphics::BPPT, 3U> &
    bpp_selections() noexcept
  {
    using namespace open_viii::graphics::literals;
    static constexpr std::array values{ 4_bpp, 8_bpp, 16_bpp };
    return values;
  }
  /**
   * bpp selections strings
   */
  static consteval const std::array<std::string_view, 3U> &
    bpp_selections_c_str() noexcept
  {
    using namespace std::string_view_literals;
    static constexpr std::array values{ "4"sv, "8"sv, "16"sv };
    return values;
  }
  /**
   * palettes selections
   */
  static constexpr const std::array<std::uint8_t, 16U> &
    palette_selections() noexcept
  {
    static constexpr auto values
      = std::array<std::uint8_t, 16U>{ 0u, 1u, 2u,  3u,  4u,  5u,  6u,  7u,
                                       8u, 9u, 10u, 11u, 12u, 13u, 14u, 15u };
    return values;
  }
  /**
   * palettes selections strings
   */
  static constexpr std::array<std::string_view, 16U> &
    palette_selections_c_str() noexcept
  {
    using namespace std::string_view_literals;
    static constexpr const std::array<std::string_view, 16U> values{
      "0"sv, "1"sv, "2"sv,  "3"sv,  "4"sv,  "5"sv,  "6"sv,  "7"sv,
      "8"sv, "9"sv, "10"sv, "11"sv, "12"sv, "13"sv, "14"sv, "15"sv
    };
    return values;
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_MIM_HPP
