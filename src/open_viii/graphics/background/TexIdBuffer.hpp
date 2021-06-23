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
#ifndef VIIIARCHIVE_TEXIDBUFFER_HPP
#define VIIIARCHIVE_TEXIDBUFFER_HPP
#include "open_viii/graphics/BPPT.hpp"
#include <cassert>
#include <compare>
#include <cstdint>
namespace open_viii::graphics::background {
/**
 * 4 bit paletteID nested inside of 16 bits of data.
 * 2 bits = depth | 2 bits = blend | 1 bit = draw | 4 bits = textID
 * @see
 * https://github.com/myst6re/deling/blob/develop/files/BackgroundFile.h#L49
 */
struct TexIdBuffer
{
private:
  enum : std::uint16_t
  {
    ID_MASK            = 0b0000'0000'0000'1111U,
    DRAW_MASK          = 0b0000'0000'0001'0000U,
    BLEND_MASK         = 0b0000'0000'0110'0000U,
    DEPTH_MASK         = 0b0000'0001'1000'0000U,
    UNK_MASK           = 0b1111'1110'0000'0000U,

    ID_INVERSE_MASK    = 0b1111'1111'1111'0000U,
    DRAW_INVERSE_MASK  = 0b1111'1111'1110'1111U,
    BLEND_INVERSE_MASK = 0b1111'1111'1001'1111U,
    DEPTH_INVERSE_MASK = 0b1111'1110'0111'1111U,
    UNK_INVERSE_MASK   = 0b0000'0001'1111'1111U,

    DRAW_SHIFT         = 4U,
    BLEND_SHIFT        = 5U,
    DEPTH_SHIFT        = 7U,
    UNK_SHIFT          = 9U,
  };
  std::uint16_t m_data{};

public:
  [[maybe_unused]] constexpr static auto EXPLICIT_SIZE = 2U;
  constexpr TexIdBuffer()                              = default;
  constexpr auto
    operator<=>(const TexIdBuffer &) const = default;
  [[nodiscard]] constexpr TexIdBuffer
    with_id(std::uint8_t in_id) const noexcept
  {
    TexIdBuffer out{};
    out.m_data = static_cast<std::uint16_t>((m_data & ID_INVERSE_MASK))
               | static_cast<std::uint16_t>((in_id & ID_MASK));
    return out;
  }
  [[nodiscard]] constexpr std::uint8_t
    id() const noexcept
  {
    return static_cast<std::uint8_t>(m_data & ID_MASK);
  }
  [[nodiscard]] constexpr TexIdBuffer
    with_blend(std::uint8_t in_blend) const noexcept
  {
    TexIdBuffer out{};
    out.m_data
      = (m_data & BLEND_INVERSE_MASK)
      | static_cast<std::uint16_t>(((in_blend << BLEND_SHIFT) & BLEND_MASK));
    return out;
  }
  [[nodiscard]] std::uint8_t
    blend() const noexcept
  {
    return static_cast<std::uint8_t>(
      static_cast<std::uint16_t>(m_data & BLEND_MASK) >> BLEND_SHIFT);
  }
  [[nodiscard]] constexpr TexIdBuffer
    with_draw(bool in_draw) const noexcept
  {
    TexIdBuffer out{};
    out.m_data = (m_data & DRAW_INVERSE_MASK)
               | (in_draw ? std::uint16_t{ DRAW_MASK } : std::uint16_t{ 0U });
    return out;
  }
  [[nodiscard]] bool
    draw() const noexcept
  {
    return (m_data & DRAW_MASK) != 0U;
  }
  [[nodiscard]] constexpr TexIdBuffer
    with_depth(BPPT in_depth) const noexcept
  {
    TexIdBuffer out{};
    out.m_data = (m_data & DEPTH_INVERSE_MASK)
               | static_cast<std::uint16_t>((in_depth.raw() << DEPTH_SHIFT)
                                            & DEPTH_MASK);
    return out;
  }
  [[nodiscard]] BPPT
    depth() const noexcept
  {
    auto raw = static_cast<std::uint8_t>(
      static_cast<std::uint16_t>(m_data & DEPTH_MASK) >> DEPTH_SHIFT);
    using namespace graphics::literals;
    switch (raw) {
    case 0b00U:
    default:
      return 4_bpp;
    case 0b01U:
      return 8_bpp;
    case 0b10U:
      return 16_bpp;
    case 0b11U:
      return 24_bpp;
    }
  }
  [[nodiscard]] constexpr TexIdBuffer
    with_unk(BPPT in_unk) const noexcept
  {
    TexIdBuffer out{};
    out.m_data
      = (m_data & UNK_INVERSE_MASK)
      | static_cast<std::uint16_t>(((in_unk.raw() << UNK_SHIFT) & UNK_MASK));
    return out;
  }
  [[nodiscard]] std::uint8_t
    unk() const noexcept
  {
    return static_cast<std::uint8_t>(
      static_cast<std::uint16_t>(m_data & UNK_MASK) >> UNK_SHIFT);
  }
  [[nodiscard]] explicit operator std::uint16_t() const noexcept
  {
    return m_data;
  }
};
static_assert(sizeof(TexIdBuffer) == TexIdBuffer::EXPLICIT_SIZE);
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_TEXIDBUFFER_HPP
