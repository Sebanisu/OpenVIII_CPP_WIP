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
  std::uint8_t                   palettes{};
  std::uint8_t                   texture_pages{};
  std::uint8_t                   skipped_palettes{};
  std::uint8_t                   type{};

  static constexpr uint8_t       DEFAULT_SKIPPED_PALETTES{ 8U };
  static constexpr uint8_t       DEFAULT_TYPE{ 1U };
  static constexpr uint8_t       DEFAULT_BPP{ 8U };
  static constexpr std::uint16_t OUT_HEIGHT{ 256U };
  static constexpr std::uint16_t OUT_WIDTH{ 128U };
  static constexpr std::uint16_t COLORS_PER_PALETTE{ 256U };
  static constexpr std::uint16_t BYTES_PER_PALETTE{ 2U * COLORS_PER_PALETTE };

  constexpr MimType() = default;

  explicit constexpr MimType(std::integral auto t)
    : MimType(TEXTURE_TYPES().front().with_type(static_cast<std::uint8_t>(t)))
  {}

  constexpr MimType(
    std::uint8_t palettes_,
    std::uint8_t texture_pages_,
    std::uint8_t skipped_palettes_ = DEFAULT_SKIPPED_PALETTES,
    std::uint8_t type_             = DEFAULT_TYPE)
    : palettes(palettes_), texture_pages(texture_pages_),
      skipped_palettes(skipped_palettes_), type(type_)
  {}

  // Keep invariant check
  [[nodiscard]] std::uint8_t
    checked_type() const noexcept
  {
    assert(type == 0 || type == 1 || type == 2 || type == 3);
    return type;
  }

  [[nodiscard]] constexpr MimType
    with_type(std::uint8_t new_type) const noexcept
  {
    auto copy = *this;
    copy.type = new_type;
    return copy;
  }

  [[nodiscard]] std::uint32_t
    bytes_skipped_palettes() const noexcept
  {
    return BYTES_PER_PALETTE * static_cast<std::uint32_t>(skipped_palettes);
  }

  [[nodiscard]] std::uint32_t
    palette_section_size() const noexcept
  {
    return BYTES_PER_PALETTE * static_cast<std::uint32_t>(palettes);
  }

  [[nodiscard]] constexpr std::uint32_t
    width() const noexcept
  {
    return OUT_WIDTH * static_cast<std::uint32_t>(texture_pages);
  }

  [[nodiscard]] constexpr std::uint32_t
    width(const BPPT &bpp) const noexcept
  {
    return (width() * DEFAULT_BPP) / static_cast<std::uint32_t>(bpp);
  }

  [[nodiscard]] constexpr std::uint32_t
    width(unsigned int bpp) const noexcept
  {
    return (width() * DEFAULT_BPP) / bpp;
  }

  [[nodiscard]] std::uint32_t
    file_size() const noexcept
  {
    return palette_section_size()
         + (width() * static_cast<std::uint32_t>(OUT_HEIGHT));
  }

  static constexpr std::uint32_t height = OUT_HEIGHT;

  [[nodiscard]] constexpr auto
    canvas(unsigned int bpp = DEFAULT_BPP, std::uint8_t scale = 1)
      const noexcept
  {
    return Rectangle<std::uint32_t>(0, 0, width(bpp) * scale, height * scale);
  }

  static constexpr std::array<MimType, 2U>
    TEXTURE_TYPES()
  {
    return { MimType{ 24U, 13U, DEFAULT_SKIPPED_PALETTES, DEFAULT_TYPE },
             MimType{ 16U, 12U, 0U, 2U } };
  }
};

}// namespace open_viii::graphics::background

template<>
struct fmt::formatter<open_viii::graphics::background::MimType>
{
  constexpr auto
    parse(fmt::format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(
      const open_viii::graphics::background::MimType &m,
      FormatContext                                  &ctx) const
  {
    return fmt::format_to(
      ctx.out(),
      "{{{}, {}, {}, {}, dims: ({}x{})}}",
      +m.palettes,
      +m.texture_pages,
      +m.skipped_palettes,
      +m.type,
      m.width(),
      open_viii::graphics::background::MimType::height);
  }
};
#endif// VIIIARCHIVE_MIMTYPE_HPP
