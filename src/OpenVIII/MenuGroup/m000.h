//
// Created by pcvii on 7/6/2020.
//

#ifndef VIIIARCHIVE_M000_H
#define VIIIARCHIVE_M000_H
#include <cstdint>
#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "MItemID.h"
#include "MSpellID.h"
#include "mEntry.h"
namespace OpenVIII::MenuGroup {
struct [[maybe_unused]] m000// Refine Item to Magic
{
  /* http://wiki.ffrtt.ru/index.php?title=FF8/Menu_m000_m004#Data
   * Ability	    # of Entries  mngrp.bin Location	        Offset	Description
   * T Mag-RF	    7 entries	  (mngrp.bin loc: 0x21F000)	0x0	Item to Thunder/Wind Magic
   * I Mag-RF	    7 entries	  (mngrp.bin loc: 0x21F038)	0x38	Item to Ice/Water Magic
   * F Mag-RF	    10 entries	  (mngrp.bin loc: 0x21F070)	0x70	Item to Fire/Flare Magic
   * L Mag-RF	    21 entries	  (mngrp.bin loc: 0x21F0C0)	0xC0	Item to Life Magic
   * Time Mag-RF    14 entries	  (mngrp.bin loc: 0x21F168)	0x168	Item to Time Magic
   * ST Mag-RF	    17 entries	  (mngrp.bin loc: 0x21F1D8)	0x1D8	Item to Status Magic
   * Supt Mag-RF    20 entries	  (mngrp.bin loc: 0x21F260)	0x260	Items to Support Magic
   * Forbid Mag-RF  6 entries	  (mngrp.bin loc: 0x21F300)	0x300	Items to Forbidden Magic
   */
private:
  constexpr auto tMagRFCount_ = 7U;
  constexpr auto iMagRFCount_ = 7U;
  constexpr auto fMagRFCount_ = 10U;
  constexpr auto lMagRFCount_ = 21U;
  constexpr auto timeMagRFCount_ = 14U;
  constexpr auto stMagRFCount_ = 17U;
  constexpr auto suptRFCount_ = 20U;
  constexpr auto forbidMagRFCount_ = 6U;
  std::array<mEntry<MItemID, MSpellID>, tMagRFCount_> tMagRF_{};
  std::array<mEntry<MItemID, MSpellID>, iMagRFCount_> iMagRF_{};
  std::array<mEntry<MItemID, MSpellID>, fMagRFCount_> fMagRF_{};
  std::array<mEntry<MItemID, MSpellID>, lMagRFCount_> lMagRF_{};
  std::array<mEntry<MItemID, MSpellID>, timeMagRFCount_> timeMagRF_{};
  std::array<mEntry<MItemID, MSpellID>, stMagRFCount_> stMagRF_{};
  std::array<mEntry<MItemID, MSpellID>, suptRFCount_> suptRF_{};
  std::array<mEntry<MItemID, MSpellID>, forbidMagRFCount_> forbidMagRF_{};

public:
  [[nodiscard]] const auto &tMagRF() const noexcept { return tMagRF_; }
  [[nodiscard]] const auto &iMagRF() const noexcept { return iMagRF_; }
  [[nodiscard]] const auto &fMagRF() const noexcept { return fMagRF_; }
  [[nodiscard]] const auto &lMagRF() const noexcept { return lMagRF_; }
  [[nodiscard]] const auto &timeMagRF() const noexcept { return timeMagRF_; }
  [[nodiscard]] const auto &stMagRF() const noexcept { return stMagRF_; }
  [[nodiscard]] const auto &suptRF() const noexcept { return suptRF_; }
  [[nodiscard]] const auto &forbidMagRF() const noexcept { return forbidMagRF_; }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_M000_H
