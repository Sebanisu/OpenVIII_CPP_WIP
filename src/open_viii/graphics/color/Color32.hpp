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

#ifndef VIIIARCHIVE_COLOR32_HPP
#define VIIIARCHIVE_COLOR32_HPP
#include <algorithm>
#include <cstdint>
#include <bitset>
#include <limits>
#include "OpenVIII/concepts.h"
namespace open_viii::graphics {
// template <size_t valueT>
// requires (valueT >=0U && valueT <=3U)
// static constexpr bool color32_valid_offset()
//{
//  return true;
//}
/**
 * 32 bit colors, Each color is 8 bits, or 1 bytes. You may choose the order of the colors. The indexes must be unique.
 * @tparam r_ red index
 * @tparam g_ green index
 * @tparam b_ blue index
 * @tparam a_ alpha index
 */
template<size_t r_ = 0U, size_t g_ = 1U, size_t b_ = 2U, size_t a_ = 3U>
requires(
  r_ < 4U && g_ < 4U && b_ < 4U && r_ != g_ && r_ != b_ && g_ != b_ && a_ != g_ && a_ != b_ && a_ != r_) struct Color32
{
public:
  [[maybe_unused]] constexpr static auto EXPLICIT_SIZE{ 4U };

private:
  mutable std::array<std::uint8_t, EXPLICIT_SIZE> m_parts{};
  template<size_t index, std::floating_point T> std::uint8_t set(T value) const
  {
    return m_parts[index] = static_cast<std::uint8_t>(
             std::clamp(value, static_cast<T>(0.0F), static_cast<T>(1.0F)) * std::numeric_limits<std::uint8_t>::max());
  }
  template<size_t index, typename T>
  requires(std::integral<T> && !std::is_same_v<T, std::int8_t>) std::uint8_t set(T value) const
  {
    return m_parts[index] = static_cast<std::uint8_t>(
             std::clamp(value, static_cast<T>(0U), static_cast<T>(std::numeric_limits<std::uint8_t>::max())));
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
static_assert(sizeof(Color32<>) == Color32<>::EXPLICIT_SIZE);
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_COLOR32_HPP
