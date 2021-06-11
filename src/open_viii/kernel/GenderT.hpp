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
#ifndef VIIIARCHIVE_GENDERT_HPP
#define VIIIARCHIVE_GENDERT_HPP
#include <cstdint>
namespace open_viii::kernel {
enum class GenderT : std::uint8_t
{
  male   = 0,
  female = 1,
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_GENDERT_HPP
