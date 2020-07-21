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

#ifndef VIIIARCHIVE_REFINESECTION001_H
#define VIIIARCHIVE_REFINESECTION001_H
#include "RefineItemID.h"
#include "RefineEntry.h"
#include <array>
#include <iostream>
namespace OpenVIII::MenuGroup {
struct [[maybe_unused]] RefineSection001// Item to Item
{
  /* http://wiki.ffrtt.ru/index.php?title=FF8/Menu_m000_m004#Data_2
   * Ability	    # of Entries  mngrp.bin Location	        Offset  Description
   * Recov Med-RF   9 Entries	  (mngrp.bin loc: 0x21F800)	0x0	Item to Recovery Items
   * ST Med-RF      12 Entries	  (mngrp.bin loc: 0x21F848)	0x48	Item to Status Removal Items
   * Ammo-RF	    16 Entries	  (mngrp.bin loc: 0x21F8A8)	0xA8	Item to Ammo Item
   * Forbid Med-RF  20 Entries	  (mngrp.bin loc: 0x21F928)	0x128	Item to Forbidden Medicine
   * GFRecov Med-RF 12 Entries	  (mngrp.bin loc: 0x21F9C8)	0x1C8	Item to GF Recovery Items
   * GFAbl Med-RF   42 Entries	  (mngrp.bin loc: 0x21FA28)	0x228	Item to GF Ability Medicine Items
   * Tool-RF	    32 Entries	  (mngrp.bin loc: 0x21FB78)	0x378	Item to Tool Items
   * */
private:
  static constexpr auto recovMedRFCount_ = 9U;
  static constexpr auto stMedRFCount_ = 12U;
  static constexpr auto ammoRFCount_ = 16U;
  static constexpr auto forbidMedRFCount_ = 20U;
  static constexpr auto gfRecovRFCount_ = 12U;
  static constexpr auto gfAblMedRFCount_ = 42U;
  static constexpr auto toolRFCount_ = 32U;
  std::array<RefineEntry<RefineItemID, RefineItemID>, recovMedRFCount_> recovMedRF_{};
  std::array<RefineEntry<RefineItemID, RefineItemID>, stMedRFCount_> stMedRF_{};
  std::array<RefineEntry<RefineItemID, RefineItemID>, ammoRFCount_> ammoRF_{};
  std::array<RefineEntry<RefineItemID, RefineItemID>, forbidMedRFCount_> forbidMedRF_{};
  std::array<RefineEntry<RefineItemID, RefineItemID>, gfRecovRFCount_> gfRecovRF_{};
  std::array<RefineEntry<RefineItemID, RefineItemID>, gfAblMedRFCount_> gfAblMedRF_{};
  std::array<RefineEntry<RefineItemID, RefineItemID>, toolRFCount_> toolRF_{};

public:
  [[nodiscard]] const auto &RecovMedRF() const noexcept { return recovMedRF_; }
  [[nodiscard]] const auto &STMedRF() const noexcept { return stMedRF_; }
  [[nodiscard]] const auto &AmmoRF() const noexcept { return ammoRF_; }
  [[nodiscard]] const auto &ForbidMedRF() const noexcept { return forbidMedRF_; }
  [[nodiscard]] const auto &GFRecovRF() const noexcept { return gfRecovRF_; }
  [[nodiscard]] const auto &GFAblMedRF() const noexcept { return gfAblMedRF_; }
  [[nodiscard]] const auto &ToolRF() const noexcept { return toolRF_; }
  [[nodiscard]] static constexpr auto size()
  {
    return recovMedRFCount_ + stMedRFCount_ + ammoRFCount_ + forbidMedRFCount_ + gfRecovRFCount_ + gfAblMedRFCount_
           + toolRFCount_;
  }
  friend std::ostream &operator<<(std::ostream &os, const RefineSection001 &input)
  {
    const auto outArray = [&os](const auto &arr) {
      for (const auto item : arr) { os << item << '\n'; }
    };
    os << "recovMedRF:\n";
    outArray(input.recovMedRF_);
    os << "stMedRF:\n";
    outArray(input.stMedRF_);
    os << "ammoRF:\n";
    outArray(input.ammoRF_);
    os << "forbidMedRF:\n";
    outArray(input.forbidMedRF_);
    os << "gfRecovRF:\n";
    outArray(input.gfRecovRF_);
    os << "gfAblMedRF:\n";
    outArray(input.gfAblMedRF_);
    os << "toolRF:\n";
    outArray(input.toolRF_);
    return os;
  }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_REFINESECTION001_H
