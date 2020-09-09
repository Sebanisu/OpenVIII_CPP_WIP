//
// Created by pcvii on 9/2/2020.
//

#ifndef VIIIARCHIVE_MIM_H
#define VIIIARCHIVE_MIM_H
#include "Tile2.h"
#include "Tile3.h"
#include "MimType.h"
#include <span>
#include <vector>
namespace open_viii::graphics::background {
/**
 * http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_MIM
 */
struct Mim
{
private:
  std::vector<char> m_buffer{};
  MimType m_mim_type{};

  [[nodiscard]] static const auto &clut_width() noexcept
  {
    return open_viii::graphics::background::MimType::COLORS_PER_PALETTE;
  }
  [[nodiscard]] std::size_t clut_height() const noexcept
  {
    return ((m_mim_type.palette_section_size() - m_mim_type.bytes_skipped_palettes()) / sizeof(Color16)) / clut_width();
  }

public:
  static constexpr std::array<MimType, 2> TEXTURE_TYPES{ MimType(24, 13), MimType(16, 12, 0, 2) };
  Mim() = default;
  /**
   * Load up the raw pixel data, 4bpp, 8bpp or 16bpp Needs at least basename to check or it'll use size of buffer to
   * find type 1 or 2. Type is used to know which map to load.
   * @param buffer is the raw bytes. std::move into object.
   * @param name logo has a unique map type.
   */
  explicit Mim(std::vector<char> &&buffer, std::string_view name = {}) : m_buffer(std::move(buffer))
  {
    m_mim_type = get_texture_type(std::ranges::size(m_buffer), name);
  }
  [[nodiscard]] const auto &mim_type() const noexcept { return m_mim_type; }
  [[maybe_unused]] [[nodiscard]] static MimType get_texture_type(std::size_t mim_filesize, std::string_view name = {})
  {
    for (auto m : TEXTURE_TYPES) {
      if (m.file_size() == mim_filesize) {
        if (Tools::i_find_any(name, Tile3::FORCE_TYPE_VALUES)) {
          m.type(3);
        } else if (Tools::i_find_any(name, Tile2::FORCE_TYPE_VALUES)) {
          m.type(2);
        }
        return m;
      }
    }
    return {};
  }
  friend std::ostream &operator<<(std::ostream &os, const Mim &m) { return os << m.m_mim_type; }
  template<typename lambdaT>
  requires(std::invocable<lambdaT, std::span<const Color16>, std::size_t, std::size_t, std::string>)
    [[maybe_unused]] void get_colors(const std::string_view &filename,
      const BPPT &bpp,
      const uint8_t &palette,
      const lambdaT lambda,
      bool dump_palette = false) const
  {// going to get all the colors for image and run the supplied lambda passing the colors.
    const auto buffer = std::span<const char>(m_buffer);
    const auto palette_buffer_tmp = buffer.subspan(
      m_mim_type.bytes_skipped_palettes(), m_mim_type.palette_section_size() - m_mim_type.bytes_skipped_palettes());
    const auto palette_buffer_bbp16 =
      std::span(reinterpret_cast<const Color16 *>(std::ranges::data(palette_buffer_tmp)),
        std::ranges::size(palette_buffer_tmp) / sizeof(Color16));
    const auto image_buffer = buffer.subspan(m_mim_type.palette_section_size());
    const auto image_buffer_bbp16 =
      std::span<const Color16>(reinterpret_cast<const Color16 *>(std::ranges::data(image_buffer)),
        std::ranges::size(image_buffer) / sizeof(Color16));
    const auto image_buffer_bbp8 =
      std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t *>(std::ranges::data(image_buffer)),
        std::ranges::size(image_buffer) / sizeof(std::uint8_t));
    const auto image_buffer_bbp4 =
      std::span<const Bit4Values>(reinterpret_cast<const Bit4Values *>(std::ranges::data(image_buffer)),
        std::ranges::size(image_buffer) / sizeof(Bit4Values));
    const auto path = std::filesystem::path(filename);

    if (dump_palette) {
      lambda(palette_buffer_bbp16,
        static_cast<std::size_t>(clut_width()),
        clut_height(),
        ((path.parent_path() / path.stem()).string()) + "_Clut.mim");
      return;
    }
    if (bpp.bpp8()) {

      std::vector<Color16> out{};
      out.resize(std::ranges::size(image_buffer_bbp8));
      std::transform(image_buffer_bbp8.begin(),
        image_buffer_bbp8.end(),
        out.begin(),
        [&palette, &palette_buffer_bbp16](std::size_t key) -> Color16 {
          key += static_cast<std::size_t>(palette) * static_cast<std::size_t>(clut_width());
          if (key < std::ranges::size(palette_buffer_bbp16)) {
            return palette_buffer_bbp16[key];
          } else {
            return {};
          }
        });

      lambda(out,
        m_mim_type.width(),
        std::ranges::size(out) / m_mim_type.width(),
        ((path.parent_path() / path.stem()).string()) + "_8bpp_" + std::to_string(palette) + ".mim");

    } else if (bpp.bpp4()) {
      std::vector<Color16> out{};
      out.reserve(std::ranges::size(image_buffer_bbp4) * 2);
      std::for_each(image_buffer_bbp4.begin(),
        image_buffer_bbp4.end(),
        [&palette, &palette_buffer_bbp16, &out](const Bit4Values &key) {
          auto k1 = static_cast<std::size_t>(key.first + (palette * clut_width()));
          auto k2 = static_cast<std::size_t>(key.second + (palette * clut_width()));
          if (k1 < std::ranges::size(palette_buffer_bbp16)) {
            out.emplace_back(palette_buffer_bbp16[k1]);
          } else {
            out.emplace_back();
          }
          if (k2 < std::ranges::size(palette_buffer_bbp16)) {
            out.emplace_back(palette_buffer_bbp16[k1]);
          } else {
            out.emplace_back();
          }
        });
      lambda(out,
        m_mim_type.width() * 2,
        std::ranges::size(out) / (m_mim_type.width() * 2),
        ((path.parent_path() / path.stem()).string()) + "_4bpp_" + std::to_string(palette) + ".mim");

    } else if (bpp.bpp16()) {
      lambda(image_buffer_bbp16,
        (m_mim_type.width() / 2U),
        std::ranges::size(image_buffer_bbp16) / (m_mim_type.width() / 2U),
        ((path.parent_path() / path.stem()).string()) + "_16bpp.mim");
    }
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
#endif// VIIIARCHIVE_MIM_H
