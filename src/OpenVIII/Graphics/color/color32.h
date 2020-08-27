//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_COLOR32_H
#define VIIIARCHIVE_COLOR32_H

#include <cstdint>
#include <bitset>
#include "OpenVIII/concepts.h"
namespace OpenVIII::Graphics {

template<size_t r_ = 0U, size_t g_ = 1U, size_t b_ = 2U, size_t a_ = 3U> struct color32
{
private:
  mutable std::array<std::uint8_t, 4> parts{};
  template<size_t index, std::floating_point T> std::uint8_t set(T value) const
  {
    return parts[index] = static_cast<std::uint8_t>(Tools::clamp(value, 0.0F, 1.0F) * UINT8_MAX);
  }
  template<size_t index, std::integral T> std::uint8_t set(T value) const
  {
    return parts[index] = static_cast<std::uint8_t>(Tools::clamp(value, 0, UINT8_MAX));
  }

public:
  color32() = default;
  template<Color c_t> explicit color32(c_t input_color)
  {
    R(input_color.R());
    G(input_color.G());
    B(input_color.B());
    A(input_color.A());
  }
  [[nodiscard]] std::uint8_t R() const { return parts.at(r_); }
  [[nodiscard]] std::uint8_t G() const { return parts.at(g_); }
  [[nodiscard]] std::uint8_t B() const { return parts.at(b_); }
  [[nodiscard]] std::uint8_t A() const { return parts.at(a_); }

  template<Number T> std::uint8_t R(const T &value) const { return set<r_, T>(value); }
  template<Number T> std::uint8_t G(const T &value) const { return set<g_, T>(value); }
  template<Number T> std::uint8_t B(const T &value) const { return set<b_, T>(value); }
  template<Number T> std::uint8_t A(const T &value) const { return set<a_, T>(value); }


  friend std::ostream &operator<<(std::ostream &os, const color32<r_, g_, b_, a_> &color)
  {
    return os << std::uppercase << std::hex << '{' << static_cast<std::size_t>(color.R()) << ", "
              << static_cast<std::size_t>(color.G()) << ", " << static_cast<std::size_t>(color.B()) << ", "
              << static_cast<std::size_t>(color.A()) << '}' << std::dec << std::nouppercase;
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_COLOR32_H
