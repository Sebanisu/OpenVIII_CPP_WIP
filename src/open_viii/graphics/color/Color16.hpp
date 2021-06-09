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
#ifndef VIIIARCHIVE_COLOR16_HPP
#define VIIIARCHIVE_COLOR16_HPP
#include "open_viii/Concepts.hpp"
#include <compare>
#include <cstdint>
#include <iostream>
#include <limits>
namespace open_viii::graphics {
/**
 *
 * @see https://github.com/myst6re/deling/blob/master/FF8Image.cpp#L30
 */
struct Color16
{
private:
  [[maybe_unused]] static constexpr auto              BITS = 16U;
  mutable std::uint16_t                               m_value{};
  static constexpr std::uint16_t                      BLUE_MASK{ 0x7C00 };
  static constexpr std::uint16_t                      GREEN_MASK{ 0x3E0 };
  static constexpr std::uint16_t                      RED_MASK{ 0x1F };
  [[maybe_unused]] static constexpr std::uint16_t     STP_MASK{ 0x8000U };
  static constexpr std::uint16_t                      ALL_COLOR_MASK{ 0x7FFF };
  static constexpr std::uint_fast8_t                  BLUE_SHIFT{ 10U };
  static constexpr std::uint_fast8_t                  GREEN_SHIFT{ 5U };
  static constexpr std::uint_fast8_t                  RED_SHIFT     = { 0U };
  [[maybe_unused]] static constexpr std::uint_fast8_t STP_SHIFT     = { 15U };
  static constexpr std::uint_fast8_t                  CONVERT_SHIFT = { 3U };
  static constexpr std::uint_fast8_t GET_HIGH_BIT_SHIFT             = { 2U };
  static constexpr std::uint_fast8_t LARGEST_5_BIT_VALUE{ 0b0001'1111 };
  [[nodiscard]] std::uint8_t
    convert(const std::uint16_t &mask, const std::uint_fast8_t &shift) const
  {
    auto temp = static_cast<std::uint16_t>(
      static_cast<std::uint16_t>(m_value & mask) >> shift);
    return static_cast<std::uint8_t>(
      (static_cast<std::uint16_t>(temp << CONVERT_SHIFT)
       | static_cast<std::uint16_t>(temp >> GET_HIGH_BIT_SHIFT)));
  }
  template<std::unsigned_integral T>
  static constexpr T
    flip(const T input)
  {
    return std::numeric_limits<T>::max() - input;
  }
  template<std::floating_point T>
  void
    set(T input, std::uint16_t mask, const std::uint_fast8_t &shift) const
  {
    std::uint16_t val{ static_cast<std::uint_fast8_t>(
      std::clamp(input, static_cast<T>(0.0F), static_cast<T>(1.0F))
      * LARGEST_5_BIT_VALUE) };
    val <<= shift;
    m_value = (static_cast<std::uint16_t>(m_value & flip(mask)) | val);
  }
  template<typename T>
  requires(std::integral<T> && !std::is_same_v<T, std::int8_t>) void set(
    T input, std::uint16_t mask, const std::uint_fast8_t &shift) const
  {
    std::uint16_t val{ static_cast<std::uint_fast8_t>(
      std::clamp(input,
                 static_cast<T>(0),
                 static_cast<T>(std::numeric_limits<std::uint8_t>::max()))) };
    val >>= CONVERT_SHIFT;
    val     = static_cast<decltype(val)>(val << shift);
    m_value = (static_cast<std::uint16_t>(m_value & flip(mask)) | val);
  }

public:
  Color16() = default;
  explicit Color16(std::uint16_t raw_color) : m_value(raw_color) {}
  friend auto operator<=>(const Color16 &left,
                          const Color16 &right) noexcept       = default;
  auto        operator<=>(const Color16 &right) const noexcept = default;
  template<Color cT>
  requires(!std::is_same_v<Color16, cT>) explicit Color16(cT color)
  {
    r(color.r());
    g(color.g());
    b(color.b());
    // pass stp bit? assuming alpha is 100% *shrugs*
    if (m_value == 0) {
      stp(true);
    } else {
      stp(false);
    }
  }
  /**
   * Color Blue stored as 5 bit.
   * @return 8 bit color value.
   */
  [[nodiscard]] std::uint8_t
    b() const
  {
    return convert(BLUE_MASK, BLUE_SHIFT);
  }
  /**
   * Color Green stored as 5 bit.
   * @return 8 bit color value.
   */
  [[nodiscard]] std::uint8_t
    g() const
  {
    return convert(GREEN_MASK, GREEN_SHIFT);
  }
  /**
   * Color Red stored as 5 bit.
   * @return 8 bit color value.
   */
  [[nodiscard]] std::uint8_t
    r() const
  {
    return convert(RED_MASK, RED_SHIFT);
  }
  /**
   * Color Blue stored as 5 bit.
   * @return 8 bit color value.
   * @tparam T is an integral value. Clamped to 0-255.
   * @param b is a new blue value.
   */
  template<Number T>
  [[maybe_unused]] std::uint8_t
    b(const T &b_in) const
  {
    set(b_in, BLUE_MASK, BLUE_SHIFT);
    return b();
  }
  /**
   * Color Green stored as 5 bit.
   * @return 8 bit color value.
   * @tparam T is an integral value. Clamped to 0-255.
   * @param g is a new blue value.
   */
  template<Number T>
  [[maybe_unused]] std::uint8_t
    g(const T &g_in) const
  {
    set(g_in, GREEN_MASK, GREEN_SHIFT);
    return g();
  }
  /**
   * Color Red stored as 5 bit.
   * @return 8 bit color value.
   * @tparam T is an integral value. Clamped to 0-255.
   * @param r is a new blue value.
   */
  template<Number T>
  [[maybe_unused]] std::uint8_t
    r(const T &r_in) const
  {
    set(r_in, RED_MASK, RED_SHIFT);
    return r();
  }
  /**
   * Special transparency bit
   * @return true or false
   */
  [[maybe_unused]] [[nodiscard]] bool
    stp() const
  {
    return (m_value & STP_MASK) != 0;
  }
  /**
   * Special transparency bit
   * @return true or false
   */
  [[maybe_unused]] bool
    stp(bool enabled) const
  {
    if (enabled) {
      m_value |= STP_MASK;
    } else {
      m_value |= ALL_COLOR_MASK;
    }
    return enabled;
  }
  /**
   * @return true if color is black. at least all the color bits are 0.
   */
  [[nodiscard]] bool
    is_black() const
  {
    return (m_value & ALL_COLOR_MASK) == 0;
  }
  /**
   * @return true if color is transparent Black. all bits are 0.
   */
  [[nodiscard]] [[maybe_unused]] bool
    is_transparent_black() const
  {
    return m_value == 0;
  }
  [[nodiscard]] std::uint8_t
    a() const
  {
    return is_black() ? 0U : std::numeric_limits<std::uint8_t>::max();
  }
  friend std::ostream &
    operator<<(std::ostream &os, const Color16 &color)
  {
    return os << std::uppercase << std::hex << '{'
              << static_cast<std::size_t>(color.r()) << ", "
              << static_cast<std::size_t>(color.g()) << ", "
              << static_cast<std::size_t>(color.b()) << ", "
              << static_cast<std::size_t>(color.a()) << '}' << std::dec
              << std::nouppercase;
  }
  [[maybe_unused]] constexpr static auto EXPLICIT_SIZE{ 2U };
};
static_assert(sizeof(Color16) == Color16::EXPLICIT_SIZE);
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_COLOR16_HPP
