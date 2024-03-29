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
#include "ColorByteArray.hpp"
#include "CommonColor.hpp"
namespace open_viii::graphics {
template<ColorLayoutT layoutT>
requires(
  layoutT == ColorLayoutT::BGR
  || layoutT == ColorLayoutT::RGB) struct Color24_impl : ColorByteArray<3U>
{
protected:
  constexpr static auto current_layout = layoutT;
  using ColorByteArray<3U>::ColorByteArray;
};
template<ColorLayoutT layoutT>
using Color24    = CommonColor<Color24_impl<layoutT>>;
using Color24BGR = Color24<ColorLayoutT::BGR>;
using Color24RGB = Color24<ColorLayoutT::RGB>;
static_assert(Color<Color24BGR>);
static_assert(Color<Color24RGB>);
static_assert(sizeof(Color24RGB) == Color24RGB::EXPECTED_SIZE);
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_COLOR24_HPP
