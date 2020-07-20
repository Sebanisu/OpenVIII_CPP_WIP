//
// Created by pcvii on 7/6/2020.
//

#ifndef VIIIARCHIVE_M004_H
#define VIIIARCHIVE_M004_H
#include "MItemID.h"
#include "mEntry.h"
#include "MCardID.h"
#include <array>
namespace OpenVIII::MenuGroup {
struct [[maybe_unused]] m004// Refine cards to items
{
  /* http://wiki.ffrtt.ru/index.php?title=FF8/Menu_m000_m004#Data_5
   * Ability	        # of Entries	mngrp.bin Location	        Offset	Description
   * CardMod	        110 Entries	(mngrp.bin loc: 0x221000)	0x0	Card to Items
   * level
   */

private:
  static constexpr auto cardModCount_ = 110U;
  std::array<mEntry<MCardID, MItemID>, cardModCount_> cardMod_{};

public:
  [[nodiscard]] const auto &CardMod() const noexcept { return cardMod_; }
  [[nodiscard]] const auto *operator->() const { return &cardMod_; }
  [[nodiscard]] static constexpr auto size() { return cardModCount_; }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_M004_H
