//
// Created by pcvii on 9/2/2020.
//

#ifndef VIIIARCHIVE_MIMTYPE_H
#define VIIIARCHIVE_MIMTYPE_H
#include <cstdint>
#include <array>
//#include <ranges>
#include <algorithm>
namespace open_viii::graphics::background {
struct MimType
{
private:
  std::uint8_t m_palettes{};
  std::uint8_t m_texture_pages{};
  std::uint8_t m_skipped_palettes{};
  mutable std::uint8_t m_type{};


public:
  /**
   * first 8 are junk and are not used.
   */
  static constexpr uint8_t DEFAULT_SKIPPED_PALETTES{ 8U };
  static constexpr uint8_t DEFAULT_TYPE{ 1U };
  static constexpr std::uint16_t OUT_HEIGHT{ 256U };

  static constexpr std::uint16_t OUT_WIDTH{ 128U };

  static constexpr std::uint16_t COLORS_PER_PALETTE{ 256U };
  static constexpr std::uint16_t BYTES_PER_PALETTE{ 2U * COLORS_PER_PALETTE };
  constexpr MimType() = default;
  constexpr MimType(std::uint8_t palettes,
    std::uint8_t texture_pages,
    std::uint8_t skipped_palettes = DEFAULT_SKIPPED_PALETTES,
    std::uint8_t type = DEFAULT_TYPE)
    : m_palettes(palettes), m_texture_pages(texture_pages), m_skipped_palettes(skipped_palettes), m_type(type)
  {}

  [[nodiscard]] const auto &type() const noexcept { return m_type; }
  void type(std::uint8_t new_type) const noexcept { m_type = new_type; }
  [[nodiscard]] std::size_t bytes_skipped_palettes() const noexcept
  {
    return static_cast<std::size_t>(BYTES_PER_PALETTE) * static_cast<std::size_t>(m_skipped_palettes);
  }
  [[nodiscard]] std::size_t palette_section_size() const noexcept
  {
    return static_cast<std::size_t>(BYTES_PER_PALETTE) * static_cast<std::size_t>(m_palettes);
  }
  [[nodiscard]] std::size_t width() const noexcept
  {
    return static_cast<std::size_t>(OUT_WIDTH) * static_cast<std::size_t>(m_texture_pages);
  }
  [[nodiscard]] std::size_t file_size() const noexcept
  {
    return palette_section_size() + (width() * static_cast<std::size_t>(OUT_HEIGHT));
  }
  friend std::ostream &operator<<(std::ostream &os, const MimType &m)
  {
    return os << '{' << static_cast<std::uint32_t>(m.m_palettes) << ", "
              << static_cast<std::uint32_t>(m.m_texture_pages) << ", "
              << static_cast<std::uint32_t>(m.m_skipped_palettes) << ", " << static_cast<std::uint32_t>(m.m_type)
              << '}';
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_MIMTYPE_H
