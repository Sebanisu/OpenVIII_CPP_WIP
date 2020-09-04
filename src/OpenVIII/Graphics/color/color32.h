//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_COLOR32_H
#define VIIIARCHIVE_COLOR32_H
#include <algorithm>
#include <cstdint>
#include <bitset>
#include "OpenVIII/concepts.h"
namespace open_viii::graphics {
// template <size_t valueT>
// requires (valueT >=0U && valueT <=3U)
// static constexpr bool color32_valid_offset()
//{
//  return true;
//}
template<size_t r_ = 0U, size_t g_ = 1U, size_t b_ = 2U, size_t a_ = 3U> struct Color32
{
private:
  mutable std::array<std::uint8_t, 4> m_parts{};
  template<size_t index, std::floating_point T> std::uint8_t set(T value) const
  {
    return m_parts[index] =
             static_cast<std::uint8_t>(std::clamp(value, static_cast<T>(0.0F), static_cast<T>(1.0F)) * UINT8_MAX);
  }
  template<size_t index, std::integral T> std::uint8_t set(T value) const
  {
    return m_parts[index] = static_cast<std::uint8_t>(std::clamp(value, static_cast<T>(0U), static_cast<T>(UINT8_MAX)));
  }

public:
  Color32() = default;
  template<Color c_t> explicit Color32(c_t input_color)
  {
    r(input_color.r());
    g(input_color.g());
    b(input_color.b());
    a(input_color.a());
  }
  [[nodiscard]] std::uint8_t r() const { return m_parts.at(r_); }
  [[nodiscard]] std::uint8_t g() const { return m_parts.at(g_); }
  [[nodiscard]] std::uint8_t b() const { return m_parts.at(b_); }
  [[nodiscard]] std::uint8_t a() const { return m_parts.at(a_); }

  template<Number T> std::uint8_t r(const T &value) const { return set<r_, T>(value); }
  template<Number T> std::uint8_t g(const T &value) const { return set<g_, T>(value); }
  template<Number T> std::uint8_t b(const T &value) const { return set<b_, T>(value); }
  template<Number T> std::uint8_t a(const T &value) const { return set<a_, T>(value); }


  friend std::ostream &operator<<(std::ostream &os, const Color32<r_, g_, b_, a_> &color)
  {
    return os << std::uppercase << std::hex << '{' << static_cast<std::size_t>(color.R()) << ", "
              << static_cast<std::size_t>(color.G()) << ", " << static_cast<std::size_t>(color.B()) << ", "
              << static_cast<std::size_t>(color.A()) << '}' << std::dec << std::nouppercase;
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_COLOR32_H
