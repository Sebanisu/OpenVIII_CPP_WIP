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
#include "CommonColor.hpp"
#include "ColorByteArray.hpp"
namespace open_viii::graphics {
template<ColorLayoutT layoutT>
requires(has_one_flag<layoutT, ColorLayoutT::BGR, ColorLayoutT::RGB>() &&
         has_one_flag<layoutT,
           ColorLayoutT::PREA,
           ColorLayoutT::POSTA>()) struct Color32_impl
  : ColorByteArray<4U>
{
protected:
  constexpr static auto current_layout = layoutT;
  using ColorByteArray<4U>::ColorByteArray;
};
template<ColorLayoutT layoutT>
using Color32 = CommonColor<Color32_impl<layoutT>>;
using Color32ABGR = Color32<ColorLayoutT::ABGR>;
using Color32ARGB = Color32<ColorLayoutT::ARGB>;
using Color32BGRA = Color32<ColorLayoutT::BGRA>;
using Color32RGBA = Color32<ColorLayoutT::RGBA>;
static_assert(Color<Color32ABGR>);
static_assert(Color<Color32ARGB>);
static_assert(Color<Color32BGRA>);
static_assert(Color<Color32RGBA>);
static_assert(sizeof(Color32RGBA)
              == Color32RGBA::EXPECTED_SIZE);
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_COLOR32_HPP
