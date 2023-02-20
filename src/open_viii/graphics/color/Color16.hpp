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
#include "Color16_index_value.hpp"
#include "CommonColor.hpp"
namespace open_viii::graphics {
template<ColorLayoutT layoutT>
requires(
  (has_one_flag<layoutT, ColorLayoutT::BGR, ColorLayoutT::RGB>()
   && has_one_flag<layoutT, ColorLayoutT::PREA, ColorLayoutT::POSTA>())
  || has_one_flag<
    layoutT,
    ColorLayoutT::BGR,
    ColorLayoutT::RGB>()) struct Color16_impl
{

private:
  static constexpr auto indexes = get_index<layoutT>(
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

protected:
  constexpr static auto current_layout = layoutT;
  std::uint16_t         value{};
  constexpr Color16_impl() = default;
  template<typename... Ts>
  requires(
    sizeof...(Ts) == indexes.size()
    && (std::integral<std::decay_t<Ts>> && ...)) constexpr Color16_impl(Ts
                                                                          &&...ts)
    : value([&]() {
        auto          i = indexes.begin();
        std::uint16_t v = {};
        ((void)(v = i++->with(v, std::forward<Ts>(ts))), ...);
        return v;
      }())
  {}
  constexpr const std::uint16_t * data() const
  {
    return &value;
  }
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
using Color16     = CommonColor<Color16_impl<layoutT>>;
using Color16ABGR = Color16<ColorLayoutT::ABGR>;
using Color16ARGB = Color16<ColorLayoutT::ARGB>;
using Color16BGRA = Color16<ColorLayoutT::BGRA>;
using Color16RGBA = Color16<ColorLayoutT::RGBA>;
using Color16BGR  = Color16<ColorLayoutT::BGR>;
using Color16RGB  = Color16<ColorLayoutT::RGB>;
static_assert(Color<Color16ABGR>);
static_assert(Color<Color16ARGB>);
static_assert(Color<Color16BGRA>);
static_assert(Color<Color16RGBA>);
static_assert(Color<Color16BGR>);
static_assert(Color<Color16RGB>);
static_assert(
  sizeof(Color16<ColorLayoutT::ABGR>)
  == Color16<ColorLayoutT::ABGR>::EXPECTED_SIZE);
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_COLOR16_HPP
