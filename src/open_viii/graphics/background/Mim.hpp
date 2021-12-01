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
#include <span>
#include <vector>
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
  [[nodiscard]] Color16ABGR
    safe_get_color_from_palette(size_t key) const
  {
    const auto m_palette_buffer = set_palette_span();
    auto       size1            = std::ranges::size(m_palette_buffer);
    if (key < size1) {
      return m_palette_buffer[key];
    }
    return {};
  }
  static std::size_t
    get_palette_key(const std::uint8_t &key, const std::size_t &palette)
  {
    return key + (palette * static_cast<std::size_t>(clut_width()));
  }

  template<Color T = Color16ABGR>
  [[nodiscard]] std::vector<T>
    get_image_bpp8(const uint8_t &palette) const
  {
    const auto     m_image_buffer = set_image_span();
    std::vector<T> out{};
    out.reserve(std::ranges::size(m_image_buffer));
    std::ranges::transform(
      m_image_buffer,
      std::back_inserter(out),
      [&palette, this](const char &c) {
        return static_cast<T>(safe_get_color_from_palette(
          get_palette_key(static_cast<uint8_t>(c), palette)));
      });
    return out;
  }
  template<Color T = Color16ABGR>
  [[nodiscard]] std::vector<T>
    get_image_bpp4(const uint8_t &palette) const
  {
    const auto     m_image_buffer_bbp4 = set_image_span_bpp4();
    std::vector<T> out{};
    out.reserve(std::ranges::size(m_image_buffer_bbp4) * 2);
    std::ranges::for_each(
      m_image_buffer_bbp4,
      [&palette, this, &out](const Bit4Values &key) {
        const auto output = [&out, &palette, this](const uint8_t &input) {
          out.emplace_back(
            safe_get_color_from_palette(get_palette_key(input, palette)));
        };
        output(key.first());
        output(key.second());
      });
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

  template<Color T>
  std::vector<T>
    get_colors(
      const BPPT    &bpp,
      const uint8_t &palette,
      bool           dump_palette = false) const
  {

    std::vector<T> colors{};
    if(std::empty(m_buffer)) {
      return colors;
    }

    const auto     convert_color = [&colors](const auto &raw_color) {
      colors.reserve(raw_color.size());
      std::ranges::transform(
            raw_color,
            std::back_inserter(colors),
            [](const auto &color) {
          return static_cast<T>(color);
            });
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
      std::
        string> || std::invocable<lambdaT, std::span<const Color16ABGR>, std::size_t>)
    [[maybe_unused]] void get_colors_for_saving(
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
    auto                  width = m_mim_type.width();
    constexpr static auto offset_interval
      = background::TileCommonConstants::texture_page_width;
    auto texture_page_offset = static_cast<std::uint32_t>(offset_interval);
    if (depth.bpp8()) {
      texture_page_offset *= texture_id;
      const auto m_image_buffer = set_image_span();
      return safe_get_color_from_palette(get_palette_key(
        static_cast<std::uint8_t>(
          m_image_buffer[std::size_t{x} + texture_page_offset + (std::size_t{y} * width)]),
        palette));
    }
    if (depth.bpp4()) {
      texture_page_offset *= 2U * texture_id;
      const auto       m_image_buffer_bbp4 = set_image_span_bpp4();
      const Bit4Values pair
        = m_image_buffer_bbp4[(std::size_t{x} + texture_page_offset) / 2U + (std::size_t{y} * width)];
      if (x % 2U == 0) {
        return safe_get_color_from_palette(
          get_palette_key(pair.first(), palette));
      }
      return safe_get_color_from_palette(
        get_palette_key(pair.second(), palette));
    }
    if (depth.bpp16()) {
      width /= 2U;
      texture_page_offset /= 2U;
      texture_page_offset *= texture_id;
      const auto m_image_buffer_bbp16 = set_image_span_bpp16();
      return m_image_buffer_bbp16
        [std::size_t{ x } + texture_page_offset + (std::size_t{ y } * width)];
    }
    return {};
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
    get_colors_for_saving(filename, {}, {}, ppm_save, true);
    BPPT bpp{};
    {
      bpp.bpp4(true);
      for (std::uint8_t i{}; i < static_cast<std::uint8_t>(clut_height());
           i++) {
        get_colors_for_saving(filename, bpp, i, ppm_save, false);
      }
    }
    {
      bpp.bpp8(true);
      for (std::uint8_t i{}; i < static_cast<std::uint8_t>(clut_height());
           i++) {
        get_colors_for_saving(filename, bpp, i, ppm_save, false);
      }
    }
    bpp.bpp16(true);
    get_colors_for_saving(filename, bpp, {}, ppm_save, false);
  }
  /**
   * bpp selections
   */
  static constexpr std::array<open_viii::graphics::BPPT, 3U>
    bpp_selections() noexcept
  {
    using namespace open_viii::graphics::literals;
    return { 4_bpp, 8_bpp, 16_bpp };
  }
  /**
   * bpp selections strings
   */
  static constexpr std::array<const char *, 3U>
    bpp_selections_c_str() noexcept
  {
    return { "4", "8", "16" };
  }
  /**
   * palettes selections
   */
  static constexpr std::array<int, 16U>
    palette_selections() noexcept
  {
    return { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  }
  /**
   * palettes selections strings
   */
  static constexpr std::array<const char *, 16U>
    palette_selections_c_str() noexcept
  {
    return { "0", "1", "2",  "3",  "4",  "5",  "6",  "7",
             "8", "9", "10", "11", "12", "13", "14", "15" };
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_MIM_HPP
