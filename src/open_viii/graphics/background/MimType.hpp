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
#ifndef VIIIARCHIVE_MIMTYPE_HPP
#define VIIIARCHIVE_MIMTYPE_HPP
#include "open_viii/graphics/BPPT.hpp"
#include "open_viii/graphics/Rectangle.hpp"
#include <array>
#include <cassert>
namespace open_viii::graphics::background {
struct MimType
{
private:
  std::uint8_t m_palettes{};
  std::uint8_t m_texture_pages{};
  std::uint8_t m_skipped_palettes{};
  std::uint8_t m_type{};

public:
  /**
   * first 8 are junk and are not used.
   */
  static constexpr uint8_t       DEFAULT_SKIPPED_PALETTES{ 8U };
  static constexpr uint8_t       DEFAULT_TYPE{ 1U };
  static constexpr uint8_t       DEFAULT_BPP{ 8U };
  static constexpr std::uint16_t OUT_HEIGHT{ 256U };
  static constexpr std::uint16_t OUT_WIDTH{ 128U };
  static constexpr std::uint16_t COLORS_PER_PALETTE{ 256U };
  static constexpr std::uint16_t BYTES_PER_PALETTE{ 2U * COLORS_PER_PALETTE };
  // I'm not sure without checking file size if we can pick a type this way.
  // really only useful forcing a type.
  explicit constexpr MimType(std::integral auto type)
    : MimType(
      TEXTURE_TYPES().front().with_type(static_cast<std::uint8_t>(type)))
  {}
  constexpr MimType() = default;
  constexpr MimType(
    std::uint8_t palettes,
    std::uint8_t texture_pages,
    std::uint8_t skipped_palettes = DEFAULT_SKIPPED_PALETTES,
    std::uint8_t type             = DEFAULT_TYPE)
    : m_palettes(palettes), m_texture_pages(texture_pages),
      m_skipped_palettes(skipped_palettes), m_type(type)
  {}
  [[nodiscard]] const auto &
    type() const noexcept
  {
    assert(m_type == 0 || m_type == 1 || m_type == 2 || m_type == 3);
    return m_type;
  }
  [[nodiscard]] constexpr MimType
    with_type(std::uint8_t new_type) &&noexcept
  {
    m_type = new_type;
    return *this;
  }
  [[nodiscard]] constexpr MimType
    with_type(std::uint8_t new_type) const &noexcept
  {
    return MimType(*this).with_type(new_type);
  }
  [[nodiscard]] std::uint32_t
    bytes_skipped_palettes() const noexcept
  {
    return static_cast<std::uint32_t>(BYTES_PER_PALETTE)
         * static_cast<std::uint32_t>(m_skipped_palettes);
  }
  [[nodiscard]] std::uint32_t
    palette_section_size() const noexcept
  {
    return static_cast<std::uint32_t>(BYTES_PER_PALETTE)
         * static_cast<std::uint32_t>(m_palettes);
  }
  [[nodiscard]] constexpr std::uint32_t
    width() const noexcept
  {
    return static_cast<std::uint32_t>(OUT_WIDTH)
         * static_cast<std::uint32_t>(m_texture_pages);
  }
  [[nodiscard]] constexpr std::uint32_t
    width(const BPPT &bbp) const noexcept
  {
    return (width() * DEFAULT_BPP) / static_cast<std::uint32_t>(bbp);
  }
  [[nodiscard]] constexpr std::uint32_t
    width(const unsigned int &bbp) const noexcept
  {
    return (width() * DEFAULT_BPP) / bbp;
  }
  [[nodiscard]] std::uint32_t
    file_size() const noexcept
  {
    return palette_section_size()
         + (width() * static_cast<std::uint32_t>(OUT_HEIGHT));
  }
  [[nodiscard]] constexpr static auto
    height() noexcept
  {
    return OUT_HEIGHT;
  }
  [[nodiscard]] constexpr auto
    canvas(unsigned int bpp = DEFAULT_BPP, std::uint8_t scale = 1) const noexcept
  {
    return Rectangle<std::uint32_t>(0, 0, width(bpp) * scale, height() * scale);
  }
  static constexpr std::array<MimType, 2U>
    TEXTURE_TYPES()
  {
    return { MimType{ 24U, 13U, DEFAULT_SKIPPED_PALETTES, DEFAULT_TYPE },
             MimType{ 16U, 12U, 0U, 2U } };
  }
  friend std::ostream &
    operator<<(std::ostream &os, const MimType &m)
  {
    return os << '{' << static_cast<std::uint32_t>(m.m_palettes) << ", "
              << static_cast<std::uint32_t>(m.m_texture_pages) << ", "
              << static_cast<std::uint32_t>(m.m_skipped_palettes) << ", "
              << static_cast<std::uint32_t>(m.m_type) << '}';
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_MIMTYPE_HPP
