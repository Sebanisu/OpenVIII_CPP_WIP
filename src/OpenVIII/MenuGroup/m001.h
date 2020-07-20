//
// Created by pcvii on 7/6/2020.
//

#ifndef VIIIARCHIVE_M001_H
#define VIIIARCHIVE_M001_H
#include <cstdint>
#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "MItemID.h"
#include "mEntry.h"
#include <array>
namespace OpenVIII::MenuGroup {
struct [[maybe_unused]] m001// Item to Item
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
  constexpr auto recovMedRFCount_ = 9U;
  constexpr auto stMedRFCount_ = 12U;
  constexpr auto ammoRFCount_ = 16U;
  constexpr auto forbidMedRFCount_ = 20U;
  constexpr auto gfRecovRFCount_ = 12U;
  constexpr auto gfAblMedRFCount_ = 42U;
  constexpr auto toolRFCount_ = 32U;
  std::array<mEntry<MItemID, MItemID>, recovMedRFCount_> recovMedRF_{};
  std::array<mEntry<MItemID, MItemID>, stMedRFCount_> stMedRF_{};
  std::array<mEntry<MItemID, MItemID>, ammoRFCount_> ammoRF_{};
  std::array<mEntry<MItemID, MItemID>, forbidMedRFCount_> forbidMedRF_{};
  std::array<mEntry<MItemID, MItemID>, gfRecovRFCount_> gfRecovRF_{};
  std::array<mEntry<MItemID, MItemID>, gfAblMedRFCount_> gfAblMedRF_{};
  std::array<mEntry<MItemID, MItemID>, toolRFCount_> toolRF_{};

public:
  [[nodiscard]] const auto &RecovMedRF() const noexcept { return recovMedRF_; }
  [[nodiscard]] const auto &STMedRF() const noexcept { return stMedRF_; }
  [[nodiscard]] const auto &AmmoRF() const noexcept { return ammoRF_; }
  [[nodiscard]] const auto &ForbidMedRF() const noexcept { return forbidMedRF_; }
  [[nodiscard]] const auto &GFRecovRF() const noexcept { return gfRecovRF_; }
  [[nodiscard]] const auto &GFAblMedRF() const noexcept { return gfAblMedRF_; }
  [[nodiscard]] const auto &ToolRF() const noexcept { return toolRF_; }
  [[nodiscard]] constexpr auto size()
  {
    return recovMedRFCount_ + stMedRFCount_ + ammoRFCount_ + forbidMedRFCount_ + gfRecovRFCount_ + gfAblMedRFCount_
           + toolRFCount_;
  }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_M001_H
