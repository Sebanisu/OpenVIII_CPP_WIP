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
#include "OpenVIII/Strings/EncodedStringOffset.hpp"

namespace open_viii::kernel {
template<LangT langVal> struct NonBattleItems
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Non-battle-item-name-and-description-offsets
   * 0x0000	2 bytes	Offset to item name
   * 0x0002	2 bytes	Offset to item description
   */
private:
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};

public:
  [[nodiscard]] auto &name_offset() const noexcept { return m_name_offset; }
  [[nodiscard]] auto &description_offset() const noexcept { return m_description_offset; }
  std::ostream &out(std::ostream &os, const std::span<const char> &buffer) const
  {
    auto name = m_name_offset.decoded_string<langVal>(buffer);
    auto description = m_description_offset.decoded_string<langVal>(buffer);
    if (!std::empty(name)) {
      os << Tools::u8_to_sv(name);
    }
    if (!std::empty(description)) {
      os << ", " << Tools::u8_to_sv(description);
    }
    return os;
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_NONBATTLEITEMS_HPP
