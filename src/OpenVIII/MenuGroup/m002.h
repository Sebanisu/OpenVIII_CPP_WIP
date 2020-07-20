//
// Created by pcvii on 7/6/2020.
//

#ifndef VIIIARCHIVE_M000_H
#define VIIIARCHIVE_M000_H
#include <cstdint>
#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "MSpellID.h"
#include "mEntry.h"
namespace OpenVIII::MenuGroup {
struct [[maybe_unused]] m000 //Refine Magic to Magic
{
  /* 
   * Mid-Mag-RF	        4 entries	(mngrp.bin loc: 0x2200000)	0x0	Upgrade Magic from low level to mid level
   * High-Mag-RF	6 entries	(mngrp.bin loc: 0x2200020)	0x20	Upgrade Magic from mid level to high level
   */

  constexpr auto midMagRFCount_ = 4U;
  constexpr auto highMagRFCount_ =6U;
  std::array<mEntry<MSpellID,MSpellID>, midMagRFCount_> midMagRF_{};
  std::array<mEntry<MSpellID,MSpellID>, highMagRFCount_> highMagRF_{};
private:

[[nodiscard]] auto MidMagRF() const noexcept {return midMagRF_;}
[[nodiscard]] auto HighMagRF() const noexcept {return highMagRF_;}

public:

};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_M000_H
