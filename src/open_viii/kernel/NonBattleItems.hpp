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
#ifndef VIIIARCHIVE_NONBATTLEITEMS_HPP
#define VIIIARCHIVE_NONBATTLEITEMS_HPP
#include "CommonKernel.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
namespace open_viii::kernel {
struct NonBattleItems_impl
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Non-battle-item-name-and-description-offsets
   * 0x0000	2 bytes	Offset to item name
   * 0x0002	2 bytes	Offset to item description
   */
protected:
  EncodedStringOffset          m_name_offset        = {};
  EncodedStringOffset          m_description_offset = {};
  static constexpr std::size_t EXPECTED_SIZE        = 4U;
  constexpr NonBattleItems_impl()                   = default;

public:
  constexpr auto
    operator<=>(const NonBattleItems_impl &right) const noexcept
    = default;
};
using NonBattleItems = CommonKernel<NonBattleItems_impl>;
static_assert(NonBattleItems::EXPECTED_SIZE == sizeof(NonBattleItems));
static_assert(has_name_offset<NonBattleItems>);
static_assert(has_description_offset<NonBattleItems>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_NONBATTLEITEMS_HPP
