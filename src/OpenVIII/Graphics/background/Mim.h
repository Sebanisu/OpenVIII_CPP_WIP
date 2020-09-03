//
// Created by pcvii on 9/2/2020.
//

#ifndef VIIIARCHIVE_MIM_H
#define VIIIARCHIVE_MIM_H
#include "MimType.h"
#include <span>
namespace open_viii::graphics::background {
/**
 * http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_MIM
 */
struct Mim
{
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
    const auto palette_buffer_tmp = buffer.subspan(0U, m_mim_type.palette_section_size());
    const auto palette_buffer_bbp16 =
      std::span(reinterpret_cast<const Color16 *>(std::ranges::data(palette_buffer_tmp)),
        std::ranges::size(palette_buffer_tmp) / sizeof(Color16));
    const auto image_buffer = buffer.subspan(m_mim_type.palette_section_size());
    const auto image_buffer_bbp16 =
      std::span<const std::uint16_t>(reinterpret_cast<const std::uint16_t *>(std::ranges::data(image_buffer)),
        std::ranges::size(image_buffer) / sizeof(std::uint16_t));
    const auto image_buffer_bbp8 =
      std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t *>(std::ranges::data(image_buffer)),
        std::ranges::size(image_buffer) / sizeof(std::uint8_t));
    const auto image_buffer_bbp4 =
      std::span<const Bit4Values>(reinterpret_cast<const Bit4Values *>(std::ranges::data(image_buffer)),
        std::ranges::size(image_buffer) / sizeof(Bit4Values));
    const auto path = std::filesystem::path(filename);
    const auto s = ((path.parent_path() / path.stem()).string()) + "_Clut.t";
    Ppm::save(palette_buffer_bbp16,
      m_mim_type.COLORS_PER_PALETTE,
      m_mim_type.palette_section_size() / m_mim_type.COLORS_PER_PALETTE,
      s);
    //    if (clut_rows() == 0) {
    //      Ppm::save(get_colors<Color16>(), width(), height(), filename);
    //    } else {
    //      auto path = std::filesystem::path(filename);
    //      for (std::uint16_t i = 0; i < clut_rows(); i++) {
    //        auto ss = std::stringstream{};
    //        ss << (path.parent_path() / path.stem()).string() << '_' << i << path.extension().string();
    //        Ppm::save(get_colors<Color16>(i), width(), height(), ss.str());
    //      }
    //    }
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_MIM_H
