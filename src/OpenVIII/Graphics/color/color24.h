//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_COLOR24_H
#define VIIIARCHIVE_COLOR24_H

#include <cstdint>
#include <bitset>
#include <limits>
#include "OpenVIII/concepts.h"
#include "OpenVIII/Tools/Tools.h"
namespace open_viii::graphics {

/**
 * 24 bit colors, Each color is 8 bits, or 1 bytes. You may choose the order of the colors. The indexes must be unique.
 * @tparam r_ red index
 * @tparam g_ green index
 * @tparam b_ blue index
 */
template<size_t r_ = 2U, size_t g_ = 1U, size_t b_ = 0U> requires (
  r_<3U && g_<3U && b_<3U && r_!=g_ && r_!=b_ && g_!=b_
  )struct Color24
{
public:
  [[maybe_unused]] constexpr static auto EXPLICIT_SIZE{3U};
private:
  mutable std::array<std::uint8_t, EXPLICIT_SIZE> m_parts{};
  template<size_t index, typename T> requires( std::integral<T> && !std::is_same_v<T,std::int8_t>) std::uint8_t set(T value) const
  {
    return m_parts[index] = static_cast<std::uint8_t>(std::clamp(value, static_cast<T>(0), static_cast<T>(std::numeric_limits<std::uint8_t>::max())));
  }

  template<size_t index, std::floating_point T> std::uint8_t set(T value) const
  {
    return m_parts[index] =
             static_cast<std::uint8_t>(std::clamp(value, static_cast<T>(0.0F), static_cast<T>(1.0F)) * std::numeric_limits<std::uint8_t>::max());
  }

public:
  [[nodiscard]] std::uint8_t r() const { return m_parts.at(r_); }
  [[nodiscard]] std::uint8_t g() const { return m_parts.at(g_); }
  [[nodiscard]] std::uint8_t b() const { return m_parts.at(b_); }
  [[nodiscard]] std::uint8_t a() const { return std::numeric_limits<std::uint8_t>::max(); }
  template<Number T> std::uint8_t r(const T &value) const { return set<r_, T>(value); }
  template<Number T> std::uint8_t g(const T &value) const { return set<g_, T>(value); }
  template<Number T> std::uint8_t b(const T &value) const { return set<b_, T>(value); }
//  template<Number T> [[maybe_unused]] [[nodiscard]] std::uint8_t a([[maybe_unused]] const T &unused) const
//  {
//    return std::numeric_limits<std::uint8_t>::max();
//  }

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
static_assert(sizeof(Color24<>)==Color24<>::EXPLICIT_SIZE);

}// namespace open_viii::graphics
#endif// VIIIARCHIVE_COLOR24_H
