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
#ifndef VIIIARCHIVE_4BITVALUES_H
#define VIIIARCHIVE_4BITVALUES_H
#include <cstdint>
#include <type_traits>
#include <utility>
namespace open_viii::graphics {
struct Bit4Values
{
private:
  static constexpr std::uint8_t MASK_4_BIT        = 0xFU;
  static constexpr std::uint8_t OFFSET_MASK_4_BIT = 0xF0U;
  static constexpr std::uint8_t SHIFT_8_BITS      = 8U;
  std::uint8_t                  m_first  : 4U {};
  std::uint8_t                  m_second : 4U {};

public:
  constexpr Bit4Values() = default;
  constexpr Bit4Values(const std::uint8_t &in_first,
                       const std::uint8_t &in_second)
    : m_first(in_first & MASK_4_BIT), m_second(in_second & MASK_4_BIT)
  {}
  constexpr explicit Bit4Values(const std::uint8_t &in_raw)
    : m_first(static_cast<std::uint8_t>((in_raw >> SHIFT_8_BITS)) & MASK_4_BIT),
      m_second(in_raw & MASK_4_BIT)
  {}
  constexpr explicit operator std::uint8_t() const noexcept
  {
    return static_cast<std::uint8_t>(
      static_cast<std::uint8_t>(
        static_cast<std::uint8_t>(m_first << SHIFT_8_BITS) & OFFSET_MASK_4_BIT)
      | (m_second));
  }
  /**
   * Get Value
   *@note required to structured binding support
   * @note can't be reference because it's a copy of 4 bits to 8 bits.
   */
  template<std::size_t I>
  requires(I < 2U) [[nodiscard]] constexpr auto get() const noexcept
  {
    if constexpr (I == 0U) {
      return m_first;
    } else if constexpr (I == 1U) {
      return m_second;
    }
  }
  //  template<int I>
  //  [[nodiscard]] std::tuple_element_t<I, std::uint8_t> get(){
  //    static_assert(I < 2,"only 2 items");
  //    if      constexpr(I == 0) return first;
  //    else if constexpr(I == 1) return second;
  //  }
  [[nodiscard]] constexpr std::uint8_t first() const noexcept
  {
    return m_first;
  }
  [[nodiscard]] constexpr std::uint8_t second() const noexcept
  {
    return m_second;
  }
  std::uint8_t first(const std::uint8_t &value) noexcept
  {
    return m_first = value & MASK_4_BIT;
  }
  std::uint8_t second(const std::uint8_t &value) noexcept
  {
    return m_second = value & MASK_4_BIT;
  }
  static constexpr std::size_t EXPECTED_SIZE = 1U;
};
static_assert(sizeof(Bit4Values) == Bit4Values::EXPECTED_SIZE);
}// namespace open_viii::graphics
namespace std {
/**
 * number of arguments
 * @note required to structured binding support
 */
template<>
struct tuple_size<open_viii::graphics::Bit4Values>
  : std::integral_constant<size_t, 2>
{
};
/**
 * type of arguments
 * @note required to structured binding support
 */
template<size_t I> struct tuple_element<I, open_viii::graphics::Bit4Values>
{
  using type = uint8_t;
};
}// namespace std
#endif// VIIIARCHIVE_4BITVALUES_H
