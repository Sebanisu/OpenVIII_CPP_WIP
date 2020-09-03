//
// Created by pcvii on 9/2/2020.
//

#ifndef VIIIARCHIVE_MIM_H
#define VIIIARCHIVE_MIM_H
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

public:
  static constexpr std::array<MimType, 2> TEXTURE_TYPES{ MimType(24, 13), MimType(16, 12, 0, 2) };
  Mim() = default;
  explicit Mim(std::span<const char> buffer)
    : m_buffer(buffer.begin(), buffer.end()), m_mim_type(get_texture_type(buffer))
  {}
  [[maybe_unused]] [[nodiscard]] static MimType get_texture_type(std::span<const char> buffer)
  {
    for (const auto &m : TEXTURE_TYPES) {
      if (m.file_size() == std::ranges::size(buffer)) {
        return m;
      }
    }
    return {};
  }
  friend std::ostream &operator<<(std::ostream &os, const Mim &m) { return os << m.m_mim_type; }
  [[maybe_unused]] void save([[maybe_unused]] std::string_view filename) const
  {
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
    const auto s = ((path.parent_path() / path.stem()).string()) + "_Clut.mim";
    const uint16_t clut_width = open_viii::graphics::background::MimType::COLORS_PER_PALETTE;
    const std::size_t clut_height = std::ranges::size(palette_buffer_bbp16) / clut_width;
    Ppm::save(palette_buffer_bbp16, clut_width, clut_height, s);
    {
      for (std::size_t i{}; i < clut_height; i++) {
        std::vector<Color16> out{};
        out.reserve(std::ranges::size(image_buffer_bbp8));
        for (std::size_t key : image_buffer_bbp8) {
          key += i * clut_width;
          if (key < std::ranges::size(palette_buffer_bbp16)) {
            out.emplace_back(palette_buffer_bbp16[key]);
          } else {
            out.emplace_back();
          }
        }

        const auto s2 = ((path.parent_path() / path.stem()).string()) + "_8bpp_" + std::to_string(i) + ".mim";
        Ppm::save(out, m_mim_type.width(), std::ranges::size(out) / m_mim_type.width(), s2);
      }
    }
    {
      for (std::size_t i{}; i < clut_height; i++) {
        std::vector<Color16> out{};
        out.reserve(std::ranges::size(image_buffer_bbp4) * 2);
        for (const Bit4Values &key : image_buffer_bbp4) {
          auto k1 = key.first + (i * clut_width);
          auto k2 = key.second + (i * clut_width);
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
        }

        const auto s2 = ((path.parent_path() / path.stem()).string()) + "_4bpp_" + std::to_string(i) + ".mim";
        Ppm::save(out, m_mim_type.width() * 2, std::ranges::size(out) / (m_mim_type.width() * 2), s2);
      }
    }
    {
      const auto s2 = ((path.parent_path() / path.stem()).string()) + "_16bpp.mim";
      std::size_t width_local{ m_mim_type.width() / 2U };
      Ppm::save(image_buffer_bbp16, width_local, std::ranges::size(image_buffer_bbp16) / width_local, s2);
    }
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_MIM_H
