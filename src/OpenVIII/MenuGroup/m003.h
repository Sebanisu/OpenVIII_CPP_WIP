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

#ifndef VIIIARCHIVE_M003_H
#define VIIIARCHIVE_M003_H
#include "MItemID.h"
#include "mEntry.h"
#include <array>
namespace OpenVIII::MenuGroup {
struct [[maybe_unused]] m003// Refine items to items
{
  /* http://wiki.ffrtt.ru/index.php?title=FF8/Menu_m000_m004#Data_3
   * Ability	        # of Entries	mngrp.bin Location	        Offset	Description
   * Med LV Up	        12 Entries	(mngrp.bin loc: 0x220800)	0x0	Level up low level recovery items to
   * higher items level
   */

private:
  static constexpr auto medLVUPCount_ = 12U;
  std::array<mEntry<MItemID, MItemID>, medLVUPCount_> medLVUP_{};

public:
  [[nodiscard]] const auto &MedLVUP() const noexcept { return medLVUP_; }
  [[nodiscard]] const auto *operator->() const { return &medLVUP_; }
  [[nodiscard]] static constexpr auto size() { return medLVUPCount_; }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_M003_H
