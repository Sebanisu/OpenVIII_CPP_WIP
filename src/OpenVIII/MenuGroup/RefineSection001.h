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
#include "OpenVIII/ItemID.h"
#include "RefineEntry.h"
#include <array>
#include <iostream>
namespace open_viii::menu_group {
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
  std::array<RefineEntry<ItemID, ItemID>, recovMedRFCount_> recovMedRF_{};
  std::array<RefineEntry<ItemID, ItemID>, stMedRFCount_> stMedRF_{};
  std::array<RefineEntry<ItemID, ItemID>, ammoRFCount_> ammoRF_{};
  std::array<RefineEntry<ItemID, ItemID>, forbidMedRFCount_> forbidMedRF_{};
  std::array<RefineEntry<ItemID, ItemID>, gfRecovRFCount_> gfRecovRF_{};
  std::array<RefineEntry<ItemID, ItemID>, gfAblMedRFCount_> gfAblMedRF_{};
  std::array<RefineEntry<ItemID, ItemID>, toolRFCount_> toolRF_{};

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
    os << "recovMedRF:\n";
    outArray<langVal>(recovMedRF_, os, buffer, offset, skipFirstNull);
    os << "stMedRF:\n";
    outArray<langVal>(stMedRF_, os, buffer, offset, skipFirstNull);
    os << "ammoRF:\n";
    outArray<langVal>(ammoRF_, os, buffer, offset, skipFirstNull);
    os << "forbidMedRF:\n";
    outArray<langVal>(forbidMedRF_, os, buffer, offset, skipFirstNull);
    os << "gfRecovRF:\n";
    outArray<langVal>(gfRecovRF_, os, buffer, offset, skipFirstNull);
    os << "gfAblMedRF:\n";
    outArray<langVal>(gfAblMedRF_, os, buffer, offset, skipFirstNull);
    os << "toolRF:\n";
    outArray<langVal>(toolRF_, os, buffer, offset, skipFirstNull);
    return os;
  }
};
}// namespace open_viii::menu_group
#endif// VIIIARCHIVE_REFINESECTION001_H
