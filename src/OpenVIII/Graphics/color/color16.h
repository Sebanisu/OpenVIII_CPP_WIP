//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_COLOR16_H
#define VIIIARCHIVE_COLOR16_H

#include <cstdint>
#include <bitset>
#include <limits>
#include "OpenVIII/concepts.h"
#include "OpenVIII/Tools/Tools.h"
namespace open_viii::graphics {
/**
 *
 * @see https://github.com/myst6re/deling/blob/master/FF8Image.cpp#L30
 */
struct Color16
{
private:
  static constexpr auto BITS = 16U;
  mutable uint16_t m_value{};
  static constexpr std::bitset<BITS> BLUE_MASK{ 0x7C00 };
  static constexpr std::bitset<BITS> GREEN_MASK{ 0x3E0 };
  static constexpr std::bitset<BITS> RED_MASK{ 0x1F };
  [[maybe_unused]] static constexpr std::bitset<BITS> STP_MASK{ 0x8000U };
  static constexpr std::bitset<BITS> ALL_COLOR_MASK{ 0x7FFF };
  static constexpr std::uint_fast8_t BLUE_SHIFT{ 10U };
  static constexpr std::uint_fast8_t GREEN_SHIFT{ 5U };
  static constexpr std::uint_fast8_t RED_SHIFT = { 0U };
  [[maybe_unused]] static constexpr std::uint_fast8_t STP_SHIFT = { 15U };
  static constexpr std::uint_fast8_t CONVERT_SHIFT = { 3U };
  static constexpr std::uint_fast8_t GET_HIGH_BIT_SHIFT = { 2U };
  static constexpr std::uint_fast8_t LARGEST_5_BIT_VALUE{ 0b0001'1111 };
  [[nodiscard]] std::bitset<BITS> value_bit() const { return std::bitset<BITS>{ m_value }; }
  void value_bit(const std::bitset<BITS> &new_value) const
  {
    m_value = static_cast<std::uint16_t>(new_value.to_ulong());
  }
  [[nodiscard]] std::uint8_t convert(const std::bitset<BITS> &mask, const std::uint_fast8_t &shift) const
  {
    auto temp = ((value_bit() & mask) >> shift);
    return static_cast<std::uint8_t>(((temp << CONVERT_SHIFT) | temp >> GET_HIGH_BIT_SHIFT).to_ulong());
  }

  template<std::floating_point T> void set(T input, std::bitset<BITS> mask, const std::uint_fast8_t &shift) const
  {
    std::bitset<BITS> val{ static_cast<std::uint_fast8_t>(
      std::clamp(input, static_cast<T>(0.0F), static_cast<T>(1.0F)) * LARGEST_5_BIT_VALUE) };
    val <<= shift;
    value_bit((value_bit() & mask.flip()) | val);
  }
  template<typename T> requires(std::integral<T> && !std::is_same_v<T,std::int8_t>) void set(T input, std::bitset<BITS> mask, const std::uint_fast8_t &shift) const
  {
    std::bitset<BITS> val{ static_cast<std::uint_fast8_t>(
      std::clamp(input, static_cast<T>(0), static_cast<T>(std::numeric_limits<std::uint8_t>::max()))) };
    val >>= CONVERT_SHIFT;
    val <<= shift;
    value_bit((value_bit() & mask.flip()) | val);
  }

public:
  Color16() = default;
  explicit Color16(std::uint16_t raw_color) : m_value(raw_color) {}
  template<Color cT> explicit Color16(cT color)
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
  [[nodiscard]] std::uint8_t b() const { return convert(BLUE_MASK, BLUE_SHIFT); }
  /**
   * Color Green stored as 5 bit.
   * @return 8 bit color value.
   */
  [[nodiscard]] std::uint8_t g() const { return convert(GREEN_MASK, GREEN_SHIFT); }
  /**
   * Color Red stored as 5 bit.
   * @return 8 bit color value.
   */
  [[nodiscard]] std::uint8_t r() const { return convert(RED_MASK, RED_SHIFT); }
  /**
   * Color Blue stored as 5 bit.
   * @return 8 bit color value.
   * @tparam T is an integral value. Clamped to 0-255.
   * @param b is a new blue value.
   */
  template<Number T> [[maybe_unused]] std::uint8_t b(const T &b_in) const
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
  template<Number T> [[maybe_unused]] std::uint8_t g(const T &g_in) const
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
  template<Number T> [[maybe_unused]] std::uint8_t r(const T &r_in) const
  {
    set(r_in, RED_MASK, RED_SHIFT);
    return r();
  }
  /**
   * Special transparency bit
   * @return true or false
   */
  [[maybe_unused]] [[nodiscard]] bool stp() const { return (value_bit() & STP_MASK).any(); }

  /**
   * Special transparency bit
   * @return true or false
   */
  [[maybe_unused]] bool stp(bool enabled) const
  {
    if (enabled) {
      value_bit(value_bit() | STP_MASK);
    } else {
      value_bit(value_bit() & ALL_COLOR_MASK);
    }
    return enabled;
  }
  /**
   * @return true if color is black. at least all the color bits are 0.
   */

  [[nodiscard]] bool is_black() const { return (value_bit() & ALL_COLOR_MASK).none(); }

  /**
   * @return true if color is transparent Black. all bits are 0.
   */
  [[nodiscard]] [[maybe_unused]] bool is_transparent_black() const { return m_value == 0; }
  [[nodiscard]] std::uint8_t a() const
  {
    if (is_black()) {
      return 0;
    }
    return std::numeric_limits<std::uint8_t>::max();
  }
  friend std::ostream &operator<<(std::ostream &os, const Color16 &color)
  {
    return os << std::uppercase << std::hex << '{' << static_cast<std::size_t>(color.r()) << ", "
              << static_cast<std::size_t>(color.g()) << ", " << static_cast<std::size_t>(color.b()) << ", "
              << static_cast<std::size_t>(color.a()) << '}' << std::dec << std::nouppercase;
  }
  [[maybe_unused]] constexpr static auto EXPLICIT_SIZE{2U};
};
static_assert(sizeof(Color16)==Color16::EXPLICIT_SIZE);
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_COLOR16_H
