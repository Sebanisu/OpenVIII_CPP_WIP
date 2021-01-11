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
#include "Tile2.hpp"
#include "Tile3.hpp"
#include "open_viii/graphics/Bit4Values.hpp"
#include "open_viii/graphics/Color.hpp"
#include "open_viii/graphics/Ppm.hpp"
#include <span>
#include <vector>
namespace open_viii::graphics::background {
/**
 * http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_MIM
 */
struct Mim
{
private:
  const std::vector<char> m_buffer{};
  const MimType m_mim_type{};
  const std::span<const char> m_image_buffer{};
  const std::span<const Color16> m_palette_buffer{};
  const std::span<const Bit4Values> m_image_buffer_bbp4{};
  const std::span<const Color16> m_image_buffer_bbp16{};

  [[nodiscard]] static const auto &clut_width() noexcept
  {
    return open_viii::graphics::background::MimType::COLORS_PER_PALETTE;
  }
  [[nodiscard]] std::size_t clut_height() const noexcept
  {
    return ((m_mim_type.palette_section_size() - m_mim_type.bytes_skipped_palettes()) / sizeof(Color16)) / clut_width();
  }
  std::span<const char> set_image_span() const
  {
    return std::span<const char>(m_buffer).subspan(m_mim_type.palette_section_size());
  }
  std::span<const Color16> set_palette_span() const
  {
    const auto palette_buffer_tmp = std::span<const char>(m_buffer).subspan(
      m_mim_type.bytes_skipped_palettes(), m_mim_type.palette_section_size() - m_mim_type.bytes_skipped_palettes());
    return { reinterpret_cast<const Color16 *>(std::ranges::data(palette_buffer_tmp)),
      std::ranges::size(palette_buffer_tmp) / sizeof(Color16) };
  }
  std::span<const Bit4Values> set_image_span_bpp4() const
  {
    return { reinterpret_cast<const Bit4Values *>(std::ranges::data(m_image_buffer)),
      std::ranges::size(m_image_buffer) / sizeof(Bit4Values) };
  }
  std::span<const Color16> set_image_span_bpp16() const
  {
    return { reinterpret_cast<const Color16 *>(std::ranges::data(m_image_buffer)),
      std::ranges::size(m_image_buffer) / sizeof(Color16) };
  }
  [[nodiscard]] Color16 safe_get_color_from_palette(size_t key) const
  {
    auto size1 = std::ranges::size(m_palette_buffer);
    if (key < size1) {
      return m_palette_buffer[key];
    }
    return {};
  }
  static std::size_t get_palette_key(const std::uint8_t &key, const std::size_t &palette)
  {
    return key + (palette * static_cast<std::size_t>(clut_width()));
  }
  [[nodiscard]] std::vector<Color16> get_image_bpp8(const uint8_t &palette) const
  {
    std::vector<Color16> out{};
    out.reserve(std::ranges::size(m_image_buffer));
    std::ranges::transform(m_image_buffer, std::back_inserter(out), [&palette, this](const char &c) -> Color16 {
      return safe_get_color_from_palette(get_palette_key(static_cast<uint8_t>(c), palette));
    });
    return out;
  }
  [[nodiscard]] std::vector<Color16> get_image_bpp4(const uint8_t &palette) const
  {
    std::vector<Color16> out{};
    out.reserve(std::ranges::size(m_image_buffer_bbp4) * 2);
    std::ranges::for_each(m_image_buffer_bbp4, [&palette, this, &out](const Bit4Values &key) {
      const auto output = [&out, &palette, this](const uint8_t &input) {
        out.emplace_back(safe_get_color_from_palette(get_palette_key(input, palette)));
      };
      output(key.first());
      output(key.second());
    });
    return out;
  }
  static constexpr std::array<MimType, 2> TEXTURE_TYPES{ MimType(24, 13), MimType(16, 12, 0, 2) };

public:
  constexpr static auto EXT = std::string_view{ ".mim" };
  Mim() = default;
  ~Mim() = default;
  Mim(Mim &&) = delete;
  Mim(const Mim &) = delete;
  Mim &operator=(const Mim &) = delete;
  Mim &operator=(Mim &&) = delete;
  /**
   * Load up the raw pixel data, 4bpp, 8bpp or 16bpp Needs at least basename to check or it'll use size of buffer to
   * find type 1 or 2. Type is used to know which map to load.
   * @param buffer is the raw bytes. std::move into object.
   * @param name logo has a unique map type.
   */
  explicit Mim(std::vector<char> &&buffer, std::string_view name = {})
    : m_buffer(std::move(buffer)),
      m_mim_type(get_texture_type(std::ranges::size(m_buffer), name)),
      m_image_buffer(set_image_span()),
      m_palette_buffer(set_palette_span()),
      m_image_buffer_bbp4(set_image_span_bpp4()),
      m_image_buffer_bbp16(set_image_span_bpp16())
  {}
  [[nodiscard]] const auto &mim_type() const noexcept
  {
    return m_mim_type;
  }
  [[maybe_unused]] [[nodiscard]] static MimType get_texture_type(std::size_t mim_filesize, std::string_view name = {})
  {
    for (auto m : TEXTURE_TYPES) {
      if (m.file_size() == mim_filesize) {
        if (tools::i_find_any(name, Tile3::FORCE_TYPE_VALUES)) {
          m.type(3);
        } else if (tools::i_find_any(name, Tile2::FORCE_TYPE_VALUES)) {
          m.type(2);
        }
        return m;
      }
    }
    return {};
  }
  friend std::ostream &operator<<(std::ostream &os, const Mim &m)
  {
    return os << m.m_mim_type;
  }

  template<typename lambdaT>
  requires(std::invocable<lambdaT,
             std::span<const Color16>,
             std::size_t,
             std::size_t,
             std::string> || std::invocable<lambdaT, std::span<const Color16>, std::size_t>)
    [[maybe_unused]] void get_colors(const std::string_view &filename,
      const BPPT &bpp,
      const uint8_t &palette,
      const lambdaT lambda,
      bool dump_palette = false) const
  {// going to get all the colors for image and run the supplied lambda passing the colors.
    const auto path = std::filesystem::path(filename);
    if (dump_palette) {
      if constexpr (std::invocable<lambdaT, std::span<const Color16>, std::size_t, std::size_t, std::string>) {
        lambda(m_palette_buffer,
          static_cast<std::size_t>(clut_width()),
          clut_height(),
          ((path.parent_path() / path.stem()).string()) + "_Clut.mim");
      } else if constexpr (std::invocable<lambdaT, std::span<const Color16>, std::size_t>) {
        lambda(m_palette_buffer, static_cast<std::size_t>(clut_width()));
      }
      return;
    }
    auto width = m_mim_type.width();
    auto out_path = [&palette, &path](const int &bpp_num, bool has_palette = true) {
      return (path.parent_path() / path.stem()).string() + "_" + std::to_string(bpp_num) + "bpp"
             + (has_palette ? "_" + std::to_string(palette) : "") + open_viii::graphics::background::Mim::EXT.data();
    };
    if (bpp.bpp8()) {
      auto out = get_image_bpp8(palette);
      if constexpr (std::invocable<lambdaT, std::span<const Color16>, std::size_t, std::size_t, std::string>) {
        lambda(out, width, std::ranges::size(out) / width, out_path(BPPT::BPP8));
      } else if (std::invocable<lambdaT, std::span<const Color16>, std::size_t>) {
        lambda(out, width);
      }

    } else if (bpp.bpp4()) {
      std::vector<Color16> out = get_image_bpp4(palette);
      width *= 2U;
      if constexpr (std::invocable<lambdaT, std::span<const Color16>, std::size_t, std::size_t, std::string>) {
        lambda(out, width, std::ranges::size(out) / width, out_path(BPPT::BPP4));
      } else if (std::invocable<lambdaT, std::span<const Color16>, std::size_t>) {
        lambda(out, width);
      }

    } else if (bpp.bpp16()) {
      width /= 2U;
      if constexpr (std::invocable<lambdaT, std::span<const Color16>, std::size_t, std::size_t, std::string>) {
        lambda(
          m_image_buffer_bbp16, (width), std::ranges::size(m_image_buffer_bbp16) / width, out_path(BPPT::BPP16, false));
      } else if (std::invocable<lambdaT, std::span<const Color16>, std::size_t>) {
        lambda(m_image_buffer_bbp16, width);
      }
    }
  }
  std::size_t get_raw_width(const BPPT &depth) const
  {
    return get_raw_width(depth, m_mim_type.width());
  }
  static std::size_t get_raw_width(const BPPT &depth, std::size_t width)
  {
    if (depth.bpp4()) {
      return width * 2U;
    }
    if (depth.bpp16()) {
      return width / 2U;
    }
    return width;
  }
  [[nodiscard]] Color16 get_color(const std::unsigned_integral auto &x,
    const std::unsigned_integral auto &y,
    const BPPT &depth,
    const std::unsigned_integral auto &palette = 0U,
    const std::unsigned_integral auto &texture_id = 0U) const
  {
    auto width = m_mim_type.width();
    constexpr static auto offset_interval = 128U;
    auto texture_page_offset = offset_interval;
    if (depth.bpp8()) {
      texture_page_offset *= texture_id;
      return safe_get_color_from_palette(
        get_palette_key(static_cast<uint8_t>(m_image_buffer[x + texture_page_offset + (y * width)]), palette));
    }
    if (depth.bpp4()) {
      texture_page_offset *= 2U * texture_id;
      const Bit4Values pair = m_image_buffer_bbp4[(x + texture_page_offset) / 2U + (y * width)];
      if (x % 2U == 0) {
        return safe_get_color_from_palette(get_palette_key(pair.first(), palette));
      }
      return safe_get_color_from_palette(get_palette_key(pair.second(), palette));
    }
    if (depth.bpp16()) {
      width /= 2U;
      texture_page_offset /= 2U;
      texture_page_offset *= texture_id;
      return m_image_buffer_bbp16[x + texture_page_offset + (y * width)];
    }
    return {};
  }
  [[maybe_unused]] void save([[maybe_unused]] std::string_view filename) const
  {
    static constexpr auto ppm_save = [](const std::span<const Color16> &data,
                                       const std::size_t &width,
                                       const std::size_t &height,
                                       const std::string &local_filename) {
      Ppm::save(data, width, height, local_filename);
    };
    get_colors(filename, {}, {}, ppm_save, true);
    BPPT bpp{};
    {
      bpp.bpp4(true);
      for (std::uint8_t i{}; i < static_cast<std::uint8_t>(clut_height()); i++) {
        get_colors(filename, bpp, i, ppm_save, false);
      }
    }
    {
      bpp.bpp8(true);
      for (std::uint8_t i{}; i < static_cast<std::uint8_t>(clut_height()); i++) {
        get_colors(filename, bpp, i, ppm_save, false);
      }
    }
    bpp.bpp16(true);
    get_colors(filename, bpp, {}, ppm_save, false);
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_MIM_HPP
