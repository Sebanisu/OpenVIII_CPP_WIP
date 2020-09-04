//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_COLOR24_H
#define VIIIARCHIVE_COLOR24_H

#include <cstdint>
#include <bitset>
#include "OpenVIII/concepts.h"
#include "OpenVIII/Tools/Tools.h"
namespace open_viii::graphics {

template<size_t r_ = 2U, size_t g_ = 1U, size_t b_ = 0U> struct Color24
{
private:
  mutable std::array<std::uint8_t, 3> m_parts{};
  template<size_t index, std::integral T> std::uint8_t set(T value) const
  {
    return m_parts[index] = static_cast<std::uint8_t>(std::clamp(value, static_cast<T>(0), static_cast<T>(UINT8_MAX)));
  }

  template<size_t index, std::floating_point T> std::uint8_t set(T value) const
  {
    return m_parts[index] =
             static_cast<std::uint8_t>(std::clamp(value, static_cast<T>(0.0F), static_cast<T>(1.0F)) * UINT8_MAX);
  }

public:
  [[nodiscard]] std::uint8_t r() const { return m_parts.at(r_); }
  [[nodiscard]] std::uint8_t g() const { return m_parts.at(g_); }
  [[nodiscard]] std::uint8_t b() const { return m_parts.at(b_); }
  [[nodiscard]] std::uint8_t a() const { return UINT8_MAX; }
  template<Number T> [[nodiscard]] std::uint8_t r(const T &value) const { return set<r_, T>(value); }
  template<Number T> [[nodiscard]] std::uint8_t g(const T &value) const { return set<g_, T>(value); }
  template<Number T> [[nodiscard]] std::uint8_t b(const T &value) const { return set<b_, T>(value); }
  template<Number T> [[maybe_unused]] [[nodiscard]] std::uint8_t a([[maybe_unused]] const T &unused) const
  {
    return UINT8_MAX;
  }

  Color24() = default;
  template<Color cT> explicit Color24(cT color)
  {
    R(color.R());
    G(color.G());
    B(color.B());
  }
  friend std::ostream &operator<<(std::ostream &os, const Color24<r_, g_, b_> &color)
  {
    return os << std::uppercase << std::hex << '{' << static_cast<std::size_t>(color.R()) << ", "
              << static_cast<std::size_t>(color.G()) << ", " << static_cast<std::size_t>(color.B()) << ", "
              << static_cast<std::size_t>(color.A()) << '}' << std::dec << std::nouppercase;
  }
};

}// namespace open_viii::graphics
#endif// VIIIARCHIVE_COLOR24_H
