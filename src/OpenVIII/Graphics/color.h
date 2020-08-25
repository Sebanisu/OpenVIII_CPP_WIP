//
// Created by pcvii on 8/24/2020.
//

#ifndef VIIIARCHIVE_COLOR_H
#define VIIIARCHIVE_COLOR_H
#include <cstdint>
#include <bitset>
namespace OpenVIII::Graphics {

struct color16
{
private:
  static constexpr auto bits = 16U;
  mutable std::bitset<bits> value{};
  static constexpr std::bitset<bits> BlueMask{ 0x7C00 };
  static constexpr std::bitset<bits> GreenMask{ 0x3E0 };
  static constexpr std::bitset<bits> RedMask{ 0x1F };
  [[maybe_unused]] static constexpr std::bitset<bits> STPMask{ 0x8000U };
  static constexpr std::bitset<bits> AllColorMask{ 0x7FFF };
  static constexpr std::uint_fast8_t BlueShift{ 10U };
  static constexpr std::uint_fast8_t GreenShift{ 5U };
  static constexpr std::uint_fast8_t RedShift = { 0U };
  [[maybe_unused]] static constexpr std::uint_fast8_t STPShift = { 15U };
  static constexpr std::uint_fast8_t ConvertShift = { 3U };
  static constexpr std::uint_fast8_t GetHighBitShift = { 2U };
  static constexpr std::uint_fast8_t largest5BitValue{ 0b0001'1111 };
  [[nodiscard]] std::uint8_t convert(const std::bitset<bits> &mask, const std::uint_fast8_t &shift) const
  {
    auto temp = ((value & mask) >> shift);
    return static_cast<std::uint8_t>(((temp << ConvertShift) | temp >> GetHighBitShift).to_ulong());
  }

  template<typename number> void set(number input, std::bitset<bits> &mask, const std::uint_fast8_t &shift) const
  {
    static_assert(std::is_integral_v<number> || std::is_floating_point_v<number>);
    if constexpr (std::is_integral_v<number>) {
      if (input > UINT8_MAX) {
        input = UINT8_MAX;
      }
      if (input < 0) {
        input = 0;
      }
      std::bitset<bits> val{ static_cast<std::uint_fast8_t>(input) };
      val >>= ConvertShift;
      val <<= shift;
      value = (value & mask.flip()) | val;
    } else if constexpr (std::is_floating_point_v<number>) {
      if (input > 1.0F) {
        input = 1.0F;
      }
      if (input < 0.0F) {
        input = 0.0F;
      }

      std::bitset<bits> val{ static_cast<std::uint_fast8_t>(input * largest5BitValue) };
      val <<= shift;
      value = (value & mask.flip()) | val;
    }
  }

public:
  /**
   * Color Blue stored as 5 bit.
   * @return 8 bit color value.
   */
  [[nodiscard]] std::uint8_t B() const { return convert(BlueMask, BlueShift); }
  /**
   * Color Green stored as 5 bit.
   * @return 8 bit color value.
   */
  [[nodiscard]] std::uint8_t G() const { return convert(GreenMask, GreenShift); }
  /**
   * Color Red stored as 5 bit.
   * @return 8 bit color value.
   */
  [[nodiscard]] std::uint8_t R() const { return convert(RedMask, RedShift); }
  /**
   * Special transparency bit
   * @return true or false
   */


  /**
   * Color Blue stored as 5 bit.
   * @return 8 bit color value.
   * @tparam number is an integral value. Clamped to 0-255.
   * @param b is a new blue value.
   */
  template<typename number> std::uint8_t B(const number &b) const
  {
    set(b, BlueMask, BlueShift);
    return B();
  }
  /**
   * Color Green stored as 5 bit.
   * @return 8 bit color value.
   * @tparam number is an integral value. Clamped to 0-255.
   * @param g is a new blue value.
   */
  template<typename number> std::uint8_t G(const number &g) const
  {
    set(g, GreenMask, GreenShift);
    return G();
  }
  /**
   * Color Red stored as 5 bit.
   * @return 8 bit color value.
   * @tparam number is an integral value. Clamped to 0-255.
   * @param r is a new blue value.
   */
  template<typename number> std::uint8_t R(const number &r) const
  {
    set(r, RedMask, RedShift);
    return R();
  }
  /**
   * Special transparency bit
   * @return true or false
   */
  [[maybe_unused]] [[nodiscard]] bool stp() const { return (value & BlueMask).any(); }
  /**
   * @return true if color is black. at least all the color bits are 0.
   */

  [[nodiscard]] bool isBlack() const { return (value & AllColorMask).none(); }

  /**
   * @return true if color is transparent Black. all bits are 0.
   */
  [[nodiscard]] bool isTransparentBlack() const { return value.none(); }
  [[nodiscard]] std::uint8_t A() const
  {
    if (isBlack()) {
      return 0;
    }
    return UINT8_MAX;
  }
  friend std::ostream &operator<<(std::ostream &os, const color16 &color)
  {
    return os << std::uppercase << std::hex << '{' << static_cast<std::size_t>(color.R()) << ", "
              << static_cast<std::size_t>(color.G()) << ", " << static_cast<std::size_t>(color.B()) << ", "
              << static_cast<std::size_t>(color.A()) << '}'
              << std::dec << std::nouppercase;
  }
};


template<size_t r_ = 2U, size_t g_ = 1U, size_t b_ = 0U> struct color24
{
private:
  mutable std::array<std::uint8_t, 3> parts;
  template<size_t index, typename number> std::uint8_t set(number value) const
  {
    if constexpr (std::is_integral_v<number>) {
      if (value > UINT8_MAX) {
        value = UINT8_MAX;
      } else if (value < 0) {
        value = 0;
      }
      return parts[index] = static_cast<std::uint8_t>(value);
    } else if constexpr (std::is_floating_point_v<number>) {
      if (value > 1.0F) {
        value = 1.0F;
      } else if (value < 0.0F) {
        value = 0.0F;
      }
      return parts[index] = static_cast<std::uint8_t>(value * UINT8_MAX);
    }
  }

public:
  [[nodiscard]] std::uint8_t R() const { return parts.at(r_); }
  [[nodiscard]] std::uint8_t G() const { return parts.at(g_); }
  [[nodiscard]] std::uint8_t B() const { return parts.at(b_); }
  [[nodiscard]] std::uint8_t A() const { return UINT8_MAX; }

  template<typename number> [[nodiscard]] std::uint8_t R(const number &value) const { return set<r_, number>(value); }
  template<typename number> [[nodiscard]] std::uint8_t G(const number &value) const { return set<g_, number>(value); }
  template<typename number> [[nodiscard]] std::uint8_t B(const number &value) const { return set<b_, number>(value); }
  template<typename number> [[nodiscard]] std::uint8_t A(number) const { return UINT8_MAX; }

  friend std::ostream &operator<<(std::ostream &os, const color24<r_, g_, b_> &color)
  {
    return os << std::uppercase << std::hex << '{' << static_cast<std::size_t>(color.R()) << ", "
              << static_cast<std::size_t>(color.G()) << ", " << static_cast<std::size_t>(color.B()) << ", "
              << static_cast<std::size_t>(color.A()) << '}'
              << std::dec << std::nouppercase;
  }
};


template<size_t r_ = 0U, size_t g_ = 1U, size_t b_ = 2U, size_t a_ = 3U> struct color32
{
private:
  union {
    mutable std::uint32_t raw{};
    mutable std::array<std::uint8_t, 4> parts;
  };
  template<size_t index, typename number> std::uint8_t set(number value) const
  {
    if constexpr (std::is_integral_v<number>) {
      if (value > UINT8_MAX) {
        value = UINT8_MAX;
      } else if (value < 0) {
        value = 0;
      }
      return parts[index] = static_cast<std::uint8_t>(value);
    } else if constexpr (std::is_floating_point_v<number>) {
      if (value > 1.0F) {
        value = 1.0F;
      } else if (value < 0.0F) {
        value = 0.0F;
      }
      return parts[index] = static_cast<std::uint8_t>(value * UINT8_MAX);
    }
  }

public:
  color32() = default;
  template<typename c_t> explicit color32(c_t color)
  {
    R(color.R());
    G(color.G());
    B(color.B());
    A(color.A());
  }
  [[nodiscard]] std::uint8_t R() const { return parts.at(r_); }
  [[nodiscard]] std::uint8_t G() const { return parts.at(g_); }
  [[nodiscard]] std::uint8_t B() const { return parts.at(b_); }
  [[nodiscard]] std::uint8_t A() const { return parts.at(a_); }

  template<typename number> std::uint8_t R(const number &value) const { return set<r_, number>(value); }
  template<typename number> std::uint8_t G(const number &value) const { return set<g_, number>(value); }
  template<typename number> std::uint8_t B(const number &value) const { return set<b_, number>(value); }
  template<typename number> std::uint8_t A(const number &value) const { return set<a_, number>(value); }


  [[nodiscard]] std::uint32_t RAW() const { return raw; }
  friend std::ostream &operator<<(std::ostream &os, const color32<r_, g_, b_, a_> &color)
  {
    return os << std::uppercase << std::hex << '{' << static_cast<std::size_t>(color.R()) << ", "
              << static_cast<std::size_t>(color.G()) << ", " << static_cast<std::size_t>(color.B()) << ", "
              << static_cast<std::size_t>(color.A()) << '}'
              << std::dec << std::nouppercase;
  }
};

}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_COLOR_H
