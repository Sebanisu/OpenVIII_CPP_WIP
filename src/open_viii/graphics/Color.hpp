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

#ifndef VIIIARCHIVE_COLOR_HPP
#define VIIIARCHIVE_COLOR_HPP
#include "open_viii/Concepts.hpp"
#include "color/Color16.hpp"
#include "color/Color24.hpp"
#include "color/Color32.hpp"
namespace open_viii::graphics {
/**
 * Compare two colors that aren't the same type. Because the color types declare their own compare methods.
 * @tparam cT1 color type 1
 * @tparam cT2 color type 2
 * @param left left side color
 * @param right right side color
 * @return true if both the same.
 */
template<Color cT1, Color cT2> requires(!std::is_same_v<cT1, cT2>) auto operator==(const cT1 &left, const cT2 &right)
{
  return left.r() == right.r() && left.g() == right.g() && left.b() == right.b() && left.a() == right.a();
}
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_COLOR_HPP
