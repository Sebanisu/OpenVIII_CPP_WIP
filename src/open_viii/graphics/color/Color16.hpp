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
#ifndef VIIIARCHIVE_COLOR16_HPP
#define VIIIARCHIVE_COLOR16_HPP
#include "CommonColor.hpp"
namespace open_viii::graphics {

struct Color16_index_value
{
private:
  std::uint16_t CONVERT_SHIFT      = {};
  std::uint16_t GET_HIGH_BIT_SHIFT = {};
  std::uint16_t mask               = {};
  std::uint16_t shift              = {};
  std::uint16_t inverse_mask       = {};

public:
  constexpr Color16_index_value(std::uint16_t maskT, std::uint16_t shiftT)
    : CONVERT_SHIFT{ static_cast<std::uint16_t>(8 - std::popcount(maskT)) },
      GET_HIGH_BIT_SHIFT{ static_cast<std::uint16_t>(std::popcount(maskT)
                                                     - CONVERT_SHIFT) },
      mask{ maskT }, shift{ shiftT }, inverse_mask{ static_cast<std::uint16_t>(
                                        ~maskT) }
  {
    assert(std::popcount(maskT) < 8 && shiftT < 16U);
  }
  [[nodiscard]] constexpr std::uint8_t
    convert(const std::uint16_t value) const noexcept
  {
    const std::uint16_t temp = (value & mask) >> shift;
    return static_cast<std::uint8_t>((temp << CONVERT_SHIFT)
                                     | (temp >> GET_HIGH_BIT_SHIFT));
  }
  [[nodiscard]] constexpr std::uint16_t
    with(const std::uint16_t value, const std::uint8_t change) const noexcept
  {
    return (value & inverse_mask)
         | static_cast<std::uint16_t>(((change >> CONVERT_SHIFT) << shift) & mask);
  }
};
template<ColorLayoutT layoutT>
requires((has_one_flag<layoutT, ColorLayoutT::BGR, ColorLayoutT::RGB>()
           && has_one_flag<layoutT, ColorLayoutT::PREA, ColorLayoutT::POSTA>())
         || has_one_flag<layoutT,
                         ColorLayoutT::BGR,
                         ColorLayoutT::RGB>()) struct Color16_impl
{
  constexpr static auto current_layout = layoutT;
  static constexpr auto indexes        = get_index<layoutT>(
    std::array{ Color16_index_value{ 0b1000'0000'0000'0000U, 15U },
                Color16_index_value{ 0b0111'1100'0000'0000U, 10U },
                Color16_index_value{ 0b0000'0011'1110'0000U, 5U },
                Color16_index_value{ 0b0000'0000'0001'1111U, 0U } },
    std::array{ Color16_index_value{ 0b1111'1000'0000'0000U, 11U },
                Color16_index_value{ 0b0000'0111'1100'0000U, 6U },
                Color16_index_value{ 0b0000'0000'0011'1110U, 1U },
                Color16_index_value{ 0b0000'0000'0000'0001U, 0U } },
    std::array{ Color16_index_value{ 0b1111'1000'0000'0000U, 11U },
                Color16_index_value{ 0b0000'0111'1110'0000U, 5U },
                Color16_index_value{ 0b0000'0000'0001'1111U, 0U } });

  std::uint16_t value{};
  constexpr Color16_impl() = default;
  template<typename... Ts>
  requires(sizeof...(Ts) == indexes.size()) constexpr Color16_impl(Ts &&...ts)
    : value([&]() {
        auto          i = indexes.begin();
        std::uint16_t v = {};
        ((void)(v = i++->with(v, std::forward<Ts>(ts))), ...);
        return v;
      }())
  {}
  template<typename T>
  constexpr std::uint8_t
    operator[](T &&index) const
  {
    return indexes[std::forward<T>(index)].convert(value);
  }
  template<typename T, typename U>
  constexpr auto
    with(T &&index, U &&change) const
  {
    return indexes[std::forward<T>(index)].with(value, std::forward<U>(change));
  }
  static constexpr bool has_alpha
    = has_one_flag<layoutT, ColorLayoutT::PREA, ColorLayoutT::POSTA>();
  static constexpr std::uint8_t alpha_size    = 1U;
  static constexpr std::size_t  EXPECTED_SIZE = 2U;
};
template<ColorLayoutT layoutT>
using Color16 = CommonColor<Color16_impl<layoutT>>;
static_assert(sizeof(Color16<ColorLayoutT::ABGR>)
              == Color16<ColorLayoutT::ABGR>::EXPECTED_SIZE);
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_COLOR16_HPP
