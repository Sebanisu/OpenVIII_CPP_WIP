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

#ifndef VIIIARCHIVE_REFINESECTION003_H
#define VIIIARCHIVE_REFINESECTION003_H
#include "OpenVIII/ItemID.h"
#include "RefineEntry.h"
#include <array>
#include <iostream>
namespace OpenVIII::MenuGroup {
struct [[maybe_unused]] RefineSection003// Refine items to items
{
  /* http://wiki.ffrtt.ru/index.php?title=FF8/Menu_m000_m004#Data_3
   * Ability	        # of Entries	mngrp.bin Location	        Offset	Description
   * Med LV Up	        12 Entries	(mngrp.bin loc: 0x220800)	0x0	Level up low level recovery items to
   * higher items level
   */

private:
  static constexpr auto medLVUPCount_ = 12U;
  std::array<RefineEntry<ItemID, ItemID>, medLVUPCount_> medLVUP_{};

public:
  [[nodiscard]] const auto &MedLVUP() const noexcept { return medLVUP_; }
  [[nodiscard]] const auto *operator->() const { return &medLVUP_; }
  [[nodiscard]] static constexpr auto size() { return medLVUPCount_; }
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
    os << "MedLVUP:\n";
    outArray<langVal>(medLVUP_, os, buffer, offset, skipFirstNull);
    return os;
  }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_REFINESECTION003_H
