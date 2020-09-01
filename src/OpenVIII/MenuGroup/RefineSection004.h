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

#ifndef VIIIARCHIVE_REFINESECTION004_H
#define VIIIARCHIVE_REFINESECTION004_H
#include "OpenVIII/ItemID.h"
#include "RefineEntry.h"
#include "RefineCardID.h"
#include <array>
#include <iostream>
namespace open_viii::menu_group {
struct [[maybe_unused]] RefineSection004// Refine cards to items
{
  /* http://wiki.ffrtt.ru/index.php?title=FF8/Menu_m000_m004#Data_5
   * Ability	        # of Entries	mngrp.bin Location	        Offset	Description
   * CardMod	        110 Entries	(mngrp.bin loc: 0x221000)	0x0	Card to Items
   * level
   */

private:
  static constexpr auto cardModCount_ = 110U;
  std::array<RefineEntry<RefineCardID, ItemID>, cardModCount_> cardMod_{};

public:
  [[nodiscard]] const auto &CardMod() const noexcept { return cardMod_; }
  [[nodiscard]] const auto *operator->() const { return &cardMod_; }
  [[nodiscard]] static constexpr auto size() { return cardModCount_; }
  template<LangT langVal, typename T>
  std::ostream &outArray(const T &arr,
    std::ostream &os,
    const std::string_view &buffer = ""sv,
    const intmax_t offset = 0,
    bool skipFirstNull = false) const

  {
    for (const auto item : arr) { (item.template out<langVal>(os, buffer, offset, skipFirstNull)) << '\n'; }
    return os;
  }
  template<LangT langVal>
  std::ostream &out(std::ostream &os,
    const std::string_view &buffer = ""sv,
    const intmax_t offset = 0,
    bool skipFirstNull = false) const

  {
    os << "cardMod:\n";
    outArray<langVal>(cardMod_, os, buffer, offset, skipFirstNull);
    return os;
  }
};
}// namespace open_viii::menu_group
#endif// VIIIARCHIVE_REFINESECTION004_H
