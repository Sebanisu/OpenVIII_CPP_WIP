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
#ifndef VIIIARCHIVE_4BITVALUES_H
#define VIIIARCHIVE_4BITVALUES_H
#include <cstring>
namespace open_viii::graphics {
/**
 * @brief Class to represent two 4-bit values stored in a single byte.
 *
 * This class provides functionality for extracting and manipulating two 4-bit
 * values stored in a single byte. It also allows structured binding of the two
 * 4-bit values. An object of this class can be converted to an 8-bit unsigned
 * integer, which returns the 4-bit values packed into the byte.
 */
struct Bit4Values
{
private:
  static constexpr std::uint8_t MASK_4_BIT        = 0xFU;
  static constexpr std::uint8_t OFFSET_MASK_4_BIT = 0xF0U;
  static constexpr std::uint8_t SHIFT_4_BITS      = 4U;

public:

  std::uint8_t                  first  : 4U {};///< The first 4-bit value.
  std::uint8_t                  second : 4U {};///< The second 4-bit value.
  /**
   * @brief Default constructor.
   */
  constexpr Bit4Values() = default;

  /**
   * @brief Creates a Bit4Values object from a single byte.
   *
   * This function extracts the two 4-bit values from a single byte and returns
   * a Bit4Values object.
   *
   * @param in_raw The byte from which to extract the 4-bit values.
   * @return The Bit4Values object with the extracted 4-bit values.
   */
  constexpr static Bit4Values
    create(const std::uint8_t in_raw)
  {
    return std::bit_cast<Bit4Values>(in_raw);
  }

  /**
   * @brief Creates a Bit4Values object from two 4-bit values.
   *
   * This function creates a Bit4Values object from two 4-bit values and returns
   * the object.
   *
   * @param in_first The first 4-bit value.
   * @param in_second The second 4-bit value.
   * @return The Bit4Values object with the two 4-bit values.
   */
  constexpr static Bit4Values
    create(const std::uint8_t in_first, const std::uint8_t in_second)
  {
    return create(
      static_cast<uint8_t>((in_first & MASK_4_BIT) << SHIFT_4_BITS)
      | (in_second & MASK_4_BIT));
  }

  /**
   * @brief Conversion operator to an 8-bit unsigned integer.
   *
   * This function returns an 8-bit unsigned integer with the two 4-bit values
   * packed into a byte.
   *
   * @note The operator returns a copy of the packed 4-bit values as an 8-bit
   * unsigned integer. Therefore, the return value cannot be used as a
   * reference.
   *
   * @return The 8-bit unsigned integer with the packed 4-bit values.
   */
  constexpr explicit operator std::uint8_t() const noexcept
  {
    return static_cast<std::uint8_t>(
      static_cast<std::uint8_t>(
        static_cast<std::uint8_t>(first << SHIFT_4_BITS) & OFFSET_MASK_4_BIT)
      | (second));
  }
  static constexpr std::size_t EXPECTED_SIZE = 1U;
};

/**
 * @brief Asserts that the size of Bit4Values is equal to its expected size.
 */
static_assert(sizeof(Bit4Values) == Bit4Values::EXPECTED_SIZE);

}// namespace open_viii::graphics
#endif// VIIIARCHIVE_4BITVALUES_H
