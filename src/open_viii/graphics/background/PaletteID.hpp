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
#ifndef VIIIARCHIVE_PALETTEID_HPP
#define VIIIARCHIVE_PALETTEID_HPP
namespace open_viii::graphics::background {
/**
 * 4 bit paletteID nested inside of 16 bits of data.
 * 6 bits = Always 30 | 4 bits = PaletteID | 6 bits = Always 0
 * @see
 * https://github.com/myst6re/deling/blob/develop/files/BackgroundFile.h#L49
 */
struct PaletteID
{
private:
  enum : std::uint16_t
  {
    THIRTY_MASK         = 0b0000'0000'0011'1111U,
    ID_MASK             = 0b0000'0011'1100'0000U,
    ZERO_MASK           = 0b1111'1100'0000'0000U,

    THIRTY_INVERSE_MASK = 0b1111'1111'1100'0000U,
    ID_INVERSE_MASK     = 0b1111'1100'0011'1111U,
    ZERO_INVERSE_MASK   = 0b0000'0011'1111'1111U,

    ID_SHIFT            = 6U,
    ZERO_SHIFT          = 10U,
  };
  std::uint16_t m_data{};

public:
  [[maybe_unused]] constexpr static auto EXPLICIT_SIZE = 2U;
  PaletteID()                                          = default;
  constexpr auto
    operator<=>(const PaletteID &) const
    = default;
  [[nodiscard]] auto
    with_thirty(std::uint16_t in_thirty) const noexcept
  {
    PaletteID out{};
    out.m_data = static_cast<std::uint16_t>(
      (m_data & THIRTY_INVERSE_MASK) | (in_thirty & THIRTY_MASK));
    return out;
  }
  [[nodiscard]] std::uint8_t
    thirty() const noexcept
  {
    return static_cast<std::uint8_t>(m_data & THIRTY_MASK);
  }

  [[nodiscard]] auto
    with_id(std::uint8_t in_id) const noexcept
  {
    PaletteID out{};
    out.m_data = static_cast<std::uint16_t>(
      (m_data & ID_INVERSE_MASK) | ((in_id << ID_SHIFT) & ID_MASK));
    return out;
  }
  [[nodiscard]] constexpr std::uint8_t
    id() const noexcept
  {
    return static_cast<std::uint8_t>((m_data & ID_MASK) >> ID_SHIFT);
  }
  [[nodiscard]] auto
    with_zero(std::uint16_t in_zero) const noexcept
  {
    PaletteID out{};
    out.m_data = static_cast<std::uint16_t>(
      (m_data & ZERO_INVERSE_MASK) | ((in_zero << ZERO_SHIFT) & ZERO_MASK));
    return out;
  }
  [[nodiscard]] std::uint8_t
    zero() const noexcept
  {
    return static_cast<std::uint8_t>((m_data & ZERO_MASK) >> ZERO_SHIFT);
  }
  [[nodiscard]] explicit
    operator std::uint16_t() const noexcept
  {
    return m_data;
  }
};
static_assert(sizeof(PaletteID) == PaletteID::EXPLICIT_SIZE);
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_PALETTEID_HPP
