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
#ifndef VIIIARCHIVE_TIMHEADER_HPP
#define VIIIARCHIVE_TIMHEADER_HPP
#include "open_viii/graphics/BPPT.hpp"
namespace open_viii::graphics {
/**
 * @struct open_viii::graphics::timHeader
 * @ingroup graphics
 * @brief Color Lookup Table Header
 */
struct TimHeader
{
private:
  /**
   * @brief The expected value of TAG_VAL is 0x10.
   */
  static constexpr auto TAG_VAL     = 0x10;

  /**
   * @brief The expected value of VERSION_VAL is 0x0.
   */
  static constexpr auto VERSION_VAL = 0x0;

  /**
   * @brief The first byte of a TIM file, representing the tag.
   */
  std::uint8_t          m_tag       = {};

  /**
   * @brief The second byte of a TIM file, representing the version.
   */
  std::uint8_t          m_version   = {};

  /**
   * @brief Reserved byte, expected value is 0.
   */
  std::uint8_t          m_none0     = {};

  /**
   * @brief Reserved byte, expected value is 0.
   */
  std::uint8_t          m_none1     = {};

  /**
   * @brief Bits per pixel flags and CLP flag.
   */
  BPPT                  m_bpp       = {};

  /**
   * @brief Reserved byte, expected value is 0.
   */
  std::uint8_t          m_none2     = {};

  /**
   * @brief Reserved byte, expected value is 0.
   */
  std::uint8_t          m_none3     = {};

  /**
   * @brief Reserved byte, expected value is either 0 or 22.
   */
  std::uint8_t          m_none4     = {};

public:
  /**
   * @brief The size of the `TimHeader` struct in bytes.
   *
   * This constant stores the expected size of the `TimHeader` struct, in bytes.
   * It is used in a static assertion to ensure that the size of the `TimHeader`
   * struct matches the expected size at compile time.
   */
  static constexpr auto SIZE = std::size_t{ 8U };

  /**
   * @brief Default constructor.
   */
  constexpr TimHeader()      = default;

  /**
   * @brief Default three-way comparison.
   */
  constexpr auto
    operator<=>(const TimHeader &) const
    = default;

  /**
   * @brief Get the bits per pixel flags and CLP flag.
   * @return The value of m_bpp.
   */
  [[nodiscard]] constexpr auto
    bpp() const
  {
    return m_bpp;
  }

  /**
   * @brief Test member variables for valid values.
   * All none values should be 0. version_ should be VERSION_VAL.
   * tag_ should be TAG_VAL. Only valid values for bpp are 4bpp with CLP,
   * 8bpp with CLP, 16bpp, or 24 bpp.
   * @return true if Check test passes.
   */
  [[nodiscard]] constexpr bool
    check() const
  {
    return m_none0 == 0 && m_none1 == 0 && m_none2 == 0 && m_none3 == 0
        && /*(none4_ == 0 || none4_ == 22) &&*/ m_tag == TAG_VAL
        && m_version == VERSION_VAL && m_bpp.check();
  }

  /**
   * @brief Implicit conversion to bool, calls check() method.
   * @return true if Check test passes.
   */
  [[nodiscard]] explicit constexpr
    operator bool() const
  {
    return check();
  }

  /**
   * @brief Overloaded ostream operator for TimHeader.
   * @param os Output stream.
   * @param input TimHeader instance to output.
   * @return Modified output stream.
   */
  friend std::ostream &
    operator<<(std::ostream &os, const TimHeader &input)
  {
    //[[maybe_unused]]static constexpr auto size_ = sizeof(input);
    return os << "{ Tag: " << static_cast<uint32_t>(input.m_tag)
              << ", Version: " << static_cast<uint32_t>(input.m_version) << ", "
              << input.m_bpp << '}';
  }
};
/**
 * Ensures that the size of the TimHeader object matches its expected size.
 */
static_assert(sizeof(TimHeader) == TimHeader::SIZE);
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TIMHEADER_HPP
