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
#ifndef VIIIARCHIVE_COLOR24_HPP
#define VIIIARCHIVE_COLOR24_HPP
#include "open_viii/Concepts.hpp"
#include "open_viii/graphics/ColorLayoutT.hpp"
#include "open_viii/tools/Tools.hpp"
#include <bitset>
#include <compare>
#include <cstdint>
#include <limits>
namespace open_viii::graphics {
/**
 * 24 bit colors, Each color is 8 bits, or 1 bytes. You may choose the order of
 * the colors. The indexes must be unique.
 * @tparam r_ red index
 * @tparam g_ green index
 * @tparam b_ blue index
 */

// template<size_t r_ = 2U, size_t g_ = 1U, size_t b_ = 0U>
// requires(r_ < 3U && g_ < 3U && b_ < 3U && r_ != g_ && r_ != b_
//          && g_ != b_) struct Color24
template<ColorLayoutT layoutT>
requires(layoutT == ColorLayoutT::BGR || layoutT == ColorLayoutT::RGB)
struct Color24
{
public:
  [[maybe_unused]] constexpr static auto EXPLICIT_SIZE{ 3U };

private:
  using this_type = Color24<layoutT>;
  constexpr static auto r_ = []() -> std::size_t {
    if constexpr (layoutT == ColorLayoutT::BGR) {
      return 2U;
    }
    else if constexpr (layoutT == ColorLayoutT::RGB) {
      return 0U;
    }
  }();
  constexpr static auto g_ = []() -> std::size_t {
    if constexpr (layoutT == ColorLayoutT::BGR) {
      return 1U;
    }
    else if constexpr (layoutT == ColorLayoutT::RGB) {
      return 1U;
    }
  }();
  constexpr static auto b_ = []() -> std::size_t {
    if constexpr (layoutT == ColorLayoutT::BGR) {
      return 0U;
    }
    else if constexpr (layoutT == ColorLayoutT::RGB) {
      return 2U;
    }
  }();
  mutable std::array<std::uint8_t, EXPLICIT_SIZE> m_parts{};
  template<size_t index, typename T>
  requires(std::integral<T> && !std::is_same_v<T, std::int8_t>) std::uint8_t
    set(T value)
  const
  {
    return m_parts[index] = static_cast<std::uint8_t>(std::clamp(
             value,
             static_cast<T>(0),
             static_cast<T>(std::numeric_limits<std::uint8_t>::max())));
  }
  template<size_t index, std::floating_point T>
  std::uint8_t
    set(T value) const
  {
    return m_parts[index] = static_cast<std::uint8_t>(
             std::clamp(value, static_cast<T>(0.0F), static_cast<T>(1.0F))
             * std::numeric_limits<std::uint8_t>::max());
  }

public:
  [[nodiscard]] std::uint8_t
    r() const
  {
    return m_parts.at(r_);
  }
  [[nodiscard]] std::uint8_t
    g() const
  {
    return m_parts.at(g_);
  }
  [[nodiscard]] std::uint8_t
    b() const
  {
    return m_parts.at(b_);
  }
  [[nodiscard]] std::uint8_t
    a() const
  {
    return std::numeric_limits<std::uint8_t>::max();
  }
  template<Number T>
  std::uint8_t
    r(const T &value) const
  {
    return set<r_, T>(value);
  }
  template<Number T>
  std::uint8_t
    g(const T &value) const
  {
    return set<g_, T>(value);
  }
  template<Number T>
  std::uint8_t
    b(const T &value) const
  {
    return set<b_, T>(value);
  }
  //  template<Number T> [[maybe_unused]] [[nodiscard]] std::uint8_t
  //  a([[maybe_unused]] const T &unused) const
  //  {
  //    return std::numeric_limits<std::uint8_t>::max();
  //  }
  Color24() = default;
  template<Color cT>
  explicit Color24(cT color)
  {
    r(color.r());
    g(color.g());
    b(color.b());
  }
  friend auto
    operator<=>(const this_type &left,
                const this_type &right) noexcept = default;
  auto
    operator<=>(const this_type &right) const noexcept = default;
  friend std::ostream &
    operator<<(std::ostream &os, const this_type &color)
  {
    return os << std::uppercase << std::hex << '{'
              << +color.R() << ", "
              << +color.G() << ", "
              << +color.B() << ", "
              << +color.A() << '}' << std::dec
              << std::nouppercase;
  }
  bool
    is_black() const noexcept
  {
    return std::ranges::all_of(m_parts, [](const auto &i) -> bool {
      return i == 0;
    });
  }
};
static_assert(sizeof(Color24<ColorLayoutT::RGB>) == Color24<ColorLayoutT::RGB>::EXPLICIT_SIZE);
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_COLOR24_HPP
