//
// Created by pcvii on 7/6/2020.
//

#ifndef VIIIARCHIVE_M002_H
#define VIIIARCHIVE_M002_H
#include "MSpellID.h"
#include "mEntry.h"
namespace OpenVIII::MenuGroup {
struct [[maybe_unused]] m002// Refine Magic to Magic
{
  /* http://wiki.ffrtt.ru/index.php?title=FF8/Menu_m000_m004#Data_3
   * Ability	        # of Entries	mngrp.bin Location	        Offset	Description
   * Mid-Mag-RF	        4 entries	(mngrp.bin loc: 0x2200000)	0x0	Upgrade Magic from low level to mid
   * level High-Mag-RF	6 entries	(mngrp.bin loc: 0x2200020)	0x20	Upgrade Magic from mid level to high
   * level
   */

private:
  static constexpr auto midMagRFCount_ = 4U;
  static constexpr auto highMagRFCount_ = 6U;
  std::array<mEntry<MSpellID, MSpellID>, midMagRFCount_> midMagRF_{};
  std::array<mEntry<MSpellID, MSpellID>, highMagRFCount_> highMagRF_{};


public:
  [[nodiscard]] const auto &MidMagRF() const noexcept { return midMagRF_; }
  [[nodiscard]] const auto &HighMagRF() const noexcept { return highMagRF_; }
  [[nodiscard]] static constexpr auto size() { return midMagRFCount_ + highMagRFCount_; }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_M002_H
