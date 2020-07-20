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
#include "RefineItemID.h"
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
  std::array<RefineEntry<RefineItemID, RefineItemID>, medLVUPCount_> medLVUP_{};

public:
  [[nodiscard]] const auto &MedLVUP() const noexcept { return medLVUP_; }
  [[nodiscard]] const auto *operator->() const { return &medLVUP_; }
  [[nodiscard]] static constexpr auto size() { return medLVUPCount_; }

  friend std::ostream & operator <<(std::ostream & os,const RefineSection003 & input)
  {
    const auto outArray = [&os](const auto & arr){
           for(const auto item : arr)
           {
             os << item << '\n';
           }
    };
    os << "medLVUP:\n";
    outArray(input.medLVUP_);
    return os;
  }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_REFINESECTION003_H
