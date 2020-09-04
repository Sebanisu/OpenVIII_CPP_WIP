//
// Created by pcvii on 9/2/2020.
//

#ifndef VIIIARCHIVE_PALETTEID_H
#define VIIIARCHIVE_PALETTEID_H
#include <cstdint>
namespace open_viii::graphics::background {
/**
 * 4 bit paletteID nested inside of 16 bits of data.
 * 6 bits = Always 30 | 4 bits = PaletteID | 6 bits = Always 0
 * @see https://github.com/myst6re/deling/blob/develop/files/BackgroundFile.h#L49
 */
struct PaletteID
{
private:
  static constexpr std::uint16_t THIRTY_MASK{ 0b0000'0000'0011'1111U };
  static constexpr std::uint16_t ID_MASK{ 0b0000'0011'1100'0000U };
  static constexpr std::uint16_t ID_SHIFT{ 6U };
  static constexpr std::uint16_t ZERO_MASK{ 0b1111'1100'0000'0000U };
  static constexpr std::uint16_t ZERO_SHIFT{ 12U };
  std::uint16_t m_data{};

public:
  PaletteID() = default;
  friend auto operator==(const PaletteID &left, const PaletteID &right) { return left.m_data == right.m_data; }
  [[nodiscard]] std::uint8_t thirty() const noexcept { return static_cast<std::uint8_t>(m_data & THIRTY_MASK); }
  [[nodiscard]] std::uint8_t id() const noexcept
  {
    return static_cast<std::uint8_t>(static_cast<std::uint16_t>(m_data & ID_MASK) >> ID_SHIFT);
  }

  [[nodiscard]] std::uint8_t zero() const noexcept
  {
    return static_cast<std::uint8_t>(static_cast<std::uint16_t>(m_data & ZERO_MASK) >> ZERO_SHIFT);
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_PALETTEID_H
