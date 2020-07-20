//
// Created by pcvii on 7/6/2020.
//

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
