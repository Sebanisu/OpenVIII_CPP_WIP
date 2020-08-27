//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_COLOR24_H
#define VIIIARCHIVE_COLOR24_H

#include <cstdint>
#include <bitset>
#include "OpenVIII/concepts.h"
#include "OpenVIII/Tools/Tools.h"
namespace OpenVIII::Graphics {

template<size_t r_ = 2U, size_t g_ = 1U, size_t b_ = 0U> struct color24
{
private:
  mutable std::array<std::uint8_t, 3> parts{};
  template<size_t index, std::integral T> std::uint8_t set(T value) const
  {
    return parts[index] = static_cast<std::uint8_t>(Tools::clamp(value, 0, UINT8_MAX));
  }

  template<size_t index, std::floating_point T> std::uint8_t set(T value) const
  {
    return parts[index] = static_cast<std::uint8_t>(Tools::clamp(value, 0.0F, 1.0F) * UINT8_MAX);
  }

public:
  [[nodiscard]] std::uint8_t R() const { return parts.at(r_); }
  [[nodiscard]] std::uint8_t G() const { return parts.at(g_); }
  [[nodiscard]] std::uint8_t B() const { return parts.at(b_); }
  [[nodiscard]] std::uint8_t A() const { return UINT8_MAX; }
  template<Number T> [[nodiscard]] std::uint8_t R(const T &value) const { return set<r_, T>(value); }
  template<Number T> [[nodiscard]] std::uint8_t G(const T &value) const { return set<g_, T>(value); }
  template<Number T> [[nodiscard]] std::uint8_t B(const T &value) const { return set<b_, T>(value); }
  template<Number T> [[maybe_unused]] [[nodiscard]] std::uint8_t A([[maybe_unused]] const T &unused) const
  {
    return UINT8_MAX;
  }

  color24() = default;
  template<Color cT>
  explicit color24(cT color)
  {
    R(color.R());
    G(color.G());
    B(color.B());
  }
  friend std::ostream &operator<<(std::ostream &os, const color24<r_, g_, b_> &color)
  {
    return os << std::uppercase << std::hex << '{' << static_cast<std::size_t>(color.R()) << ", "
              << static_cast<std::size_t>(color.G()) << ", " << static_cast<std::size_t>(color.B()) << ", "
              << static_cast<std::size_t>(color.A()) << '}' << std::dec << std::nouppercase;
  }
};

}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_COLOR24_H
