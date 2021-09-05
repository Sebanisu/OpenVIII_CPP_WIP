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
#ifndef VIIIARCHIVE_LANGT_HPP
#define VIIIARCHIVE_LANGT_HPP
#include "open_viii/tools/Tools.hpp"
#include <cstdint>
#include <string_view>
namespace open_viii {
enum class LangT : std::uint8_t
{
  en,
  fr,
  de,
  it,
  es,
  jp,
  end,
  begin = en,
};
}// namespace open_viii
#endif// VIIIARCHIVE_LANGT_HPP
