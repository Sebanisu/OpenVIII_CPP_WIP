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

#ifndef VIIIARCHIVE_TEXIDBUFFER_H
#define VIIIARCHIVE_TEXIDBUFFER_H
#include <cstdint>
#include "OpenVIII/Graphics/BPPT.hpp"
namespace open_viii::graphics::background {
/**
 * 4 bit paletteID nested inside of 16 bits of data.
 * 2 bits = depth | 2 bits = blend | 1 bit = draw | 4 bits = textID
 * @see https://github.com/myst6re/deling/blob/develop/files/BackgroundFile.h#L49
 */
struct TexIdBuffer
{
private:
  static constexpr std::uint16_t ID_MASK{ 0b0000'0000'0000'1111U };
  static constexpr std::uint16_t DRAW_MASK{ 0b0000'0000'0001'0000U };
  static constexpr std::uint16_t BLEND_MASK{ 0b0000'0000'0110'0000U };
  static constexpr std::uint16_t BLEND_SHIFT{ 6U };
  static constexpr std::uint16_t DEPTH_MASK{ 0b0000'0001'1000'0000U };
  static constexpr std::uint16_t DEPTH_SHIFT{ 7U };
  static constexpr std::uint16_t UNK_MASK{ 0b1111'1110'0000'0000U };
  static constexpr std::uint16_t UNK_SHIFT{ 9U };
  std::uint16_t m_data{};

public:
  TexIdBuffer() = default;
  friend auto operator==(const TexIdBuffer &left, const TexIdBuffer &right) { return left.m_data == right.m_data; }

  [[nodiscard]] std::uint8_t id() const noexcept { return static_cast<std::uint8_t>(m_data & ID_MASK); }
  [[nodiscard]] std::uint8_t blend() const noexcept
  {
    return static_cast<std::uint8_t>(static_cast<std::uint16_t>(m_data & BLEND_MASK) >> BLEND_SHIFT);
  }
  [[nodiscard]] bool draw() const noexcept { return (m_data & DRAW_MASK) != 0; }
  [[nodiscard]] BPPT depth() const noexcept
  {
    auto raw = static_cast<std::uint8_t>(static_cast<std::uint16_t>(m_data & DEPTH_MASK) >> DEPTH_SHIFT);
    BPPT out{};
    if (raw == 0) {
      out.bpp4(true);
    } else if (raw == 1) {
      out.bpp8(true);
    } else if (raw == 2) {
      out.bpp16(true);
    } else if (raw == 3) {
      out.bpp24(true);
    }
    return out;
  }
  [[nodiscard]] std::uint8_t unk() const noexcept
  {
    return static_cast<std::uint8_t>(static_cast<std::uint16_t>(m_data & UNK_MASK) >> UNK_SHIFT);
  }
  [[maybe_unused]] constexpr static auto EXPLICIT_SIZE{ 2U };
};
static_assert(sizeof(TexIdBuffer) == TexIdBuffer::EXPLICIT_SIZE);
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_TEXIDBUFFER_H
