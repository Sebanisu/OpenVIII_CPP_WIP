//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_COLOR32_H
#define VIIIARCHIVE_COLOR32_H

#include <cstdint>
#include <bitset>
namespace OpenVIII::Graphics {

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
              << static_cast<std::size_t>(color.A()) << '}' << std::dec << std::nouppercase;
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_COLOR32_H
